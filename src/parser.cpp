#include "parser.h"
#include <iomanip>

Parser::Parser(bool sinStopWords) {
	bag = new BagOfWords();
//	cargarDiccionarioStopWords(NOMBRE_ARCHIVO_STOPWORDS1);
//	cargarDiccionarioStopWords(NOMBRE_ARCHIVO_STOPWORDS2);
	if (sinStopWords) cargarDiccionarioStopWords(NOMBRE_ARCHIVO_STOPWORDS_PYHTON);
}

Parser::~Parser() {
	delete this->bag;
}

void Parser::cargarDiccionarioStopWords(string nombreArchivo) {
	ifstream archivo(nombreArchivo.c_str());
	if ( archivo.is_open() ){
		string stopWord;
		while ( getline(archivo,stopWord) ){
			stopWords.insert(pair<string, int>(stopWord, 1));
		}
	}
	archivo.close();
}

bool Parser::esStopWord(string word) {
	return ( stopWords.count(word) > 0 );
}

vector<string> Parser::limpiarReview(string review, int sentiment, bool agregar) {
	vector<string> palabrasReview;
	review.erase(0,2); // Elimino comillas y tab del comienzo
	review.erase(review.length()-1, 1); // Elimino comillas del final
	transform(review.begin(), review.end(), review.begin(), ::tolower);
	istringstream iss(review);
	string word;
	bool anteriorEsSaltoDeLinea = false;
	while(iss >> word) {
		//Elimino los <br />
		if (anteriorEsSaltoDeLinea){
			if (word == "/>") {
				anteriorEsSaltoDeLinea = false;
				continue;
			} else {
				anteriorEsSaltoDeLinea = false;
				word.erase (word.begin(), word.begin()+2);
			}
		}
		if (not anteriorEsSaltoDeLinea) {
			if (word == "<br"){
				anteriorEsSaltoDeLinea = true;
				continue;
			}
			if (word.length() > 3) {
				string br = word.substr(word.length()-3,3);
				if (br == "<br"){
					anteriorEsSaltoDeLinea = true;
					word.erase (word.end()-3, word.end());
				}
			}
		}

		//Si empieza con http:// o https:// o www.
		bool continuar = false;
		string wordSinURL = eliminarURL(word, continuar);
		if (continuar) continue;

		//Separo en todas las palabras que estan incluidas en la word y las dejo solo con letras
		vector<string> words = soloLetras(wordSinURL);

		vector<string>::iterator iterador = words.begin();
		for ( ; iterador != words.end() ; iterador++){
			string wordAAgregar = (*iterador);
			if ( esStopWord(wordAAgregar) ) continue;
			if ( (wordAAgregar.length() == 1) or (wordAAgregar.length() == 0) ) continue;
			if ( agregar ) bag->agregar(wordAAgregar, sentiment);
			palabrasReview.push_back(wordAAgregar);
		}
	}
	unsigned int size = palabrasReview.size();
	if (biWord){
		for (unsigned int i = 0; i < size-1; i++){
			string word1 = palabrasReview[i];
			string word2 = palabrasReview[i+1];
			string concat = word1 + " " + word2;
			if (agregar) bag->agregar(concat, sentiment);
			palabrasReview.push_back(concat);
		}
	}
	if (triWord){
		for (unsigned int i = 0; i < size-2; i++){
			string word1 = palabrasReview[i];
			string word2 = palabrasReview[i+1];
			string word3 = palabrasReview[i+2];
			string concat = word1 + " " + word2 + " " + word3;
			if (agregar) bag->agregar(concat, sentiment);
			palabrasReview.push_back(concat);
		}
	}
	return palabrasReview;
}

BagOfWords* Parser::parsearReviews(string nombreArchivo, bool biWord, bool triWord) {
	ifstream archivo(nombreArchivo.c_str());
	this->biWord = biWord;
	this->triWord = triWord;
	if ( archivo.is_open() ){
		string id, header;
		int sentimiento;
		int i = 0;
		getline(archivo,header); // Leo el header
		while ( (archivo >> id >> sentimiento) and (i < CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO) ){ //Leo id y sentimiento
			if( (i+1) % 1000 == 0 )cout <<  "Se parsearon " << (i+1) << " reviews de " << CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << " para el entrenamiento."<< endl;
			string review;
			getline(archivo,review); //Leo review
			limpiarReview(review, sentimiento, true);
			i++;
		}
	}
	cout << "Se terminaron de parsear las palabras para el entrenamiento." << endl << endl;
	archivo.close();
	bag->crearVectorConProbabilidades();
	return bag;
}

void Parser::generarTSV() {
	ofstream archivo(NOMBRE_ARCHIVO_FRECUENCIAS.c_str());
	if ( archivo.is_open() ){
		vector<string>* words = bag->getWords();
		vector<int>* frecPos = bag->getFrecuencias(1);
		vector<int>* frecNeg = bag->getFrecuencias(0);
		int length = words->size();
		archivo << "palabra" << "\t" << "frecPos" << "\t" << "frecNeg" << "\n";
		for (int i = 0; i < length; i++){
			archivo << words->at(i) << "\t" << frecPos->at(i) << "\t" << frecNeg->at(i) << "\n";
		}
	}
	archivo.close();
}

void Parser::agregarAlCSV(vector<string>& id, vector<numeroReal>& probabilidad, string nombreArchivoSalida){
	ofstream archivo(nombreArchivoSalida.c_str());
	if ( archivo.is_open() ){
		archivo << "\"id\",\"sentiment\"" << "\n";
		for (unsigned int i = 0; i < probabilidad.size(); i++){
			archivo << id[i].c_str() << ","<< std::fixed << std::setprecision(10) << probabilidad[i] << "\n";
		}
	}
	archivo.close();
}

BagOfWords* Parser::leerPalabrasYFrecuenciasDesdeTSV(string nombreArchivo) {
	// CAMBIAR PARA QUE LEA BIWORDS!!
	ifstream archivo(nombreArchivo.c_str());
	int i = 0;
	if ( archivo.is_open() ){
		string word, header;
		int frecPos, frecNeg;
		getline(archivo,header); // Leo el header
		while ( archivo >> word >> frecPos >> frecNeg ){ //Leo palabra y frecuencias
			bag->agregar(word, frecPos, frecNeg);
			i++;
		}
	}
	cout << "Se cargo el archivo de palabras y frecuencias, con un total de " << i << " palabras." << endl;
	archivo.close();
	bag->crearVectorConProbabilidades();
	return bag;
}

string Parser::eliminarURL(string word, bool &continuar) {
	unsigned int length = word.length();
	string wordSinURL = "";
	for (unsigned int i = 0; i < length; i++){
		if (not ( (i == 0) and ( (word[i] == '<') or (word[i] == '(') or (word[i] == ')') or (word[i] == ':') ))) wordSinURL = wordSinURL + word[i]; //Solo si es la primera letra
	}

	string http = wordSinURL.substr(0,7);
	string https = wordSinURL.substr(0,8);
	string www = wordSinURL.substr(0,4);
	if (http == "http://") continuar = true;
	if (https == "https://") continuar = true;
	if (www == "www.") continuar = true;

	return wordSinURL;
}

vector<string> Parser::soloLetras(string word) {
	string wordSoloLetras = "";
	unsigned int length = word.length();
	vector<string> words;
	for (unsigned int i = 0; i < length; i++){
		//TODO: Ver lo de la ñ en algun momento
		if ( (word[i] >= 'a') and (word[i] <= 'z') ) wordSoloLetras = wordSoloLetras + word[i];
			else {
				words.push_back(wordSoloLetras);
				wordSoloLetras = "";
			}
	}
	//Con esto agrego la ultima (o unica) palabra
	words.push_back(wordSoloLetras);
	return words;
}

vector< Review >* Parser::parsearReviewsAPredecir(string nombreArchivo, int desde, bool tieneSentimiento) {
	ifstream archivo(nombreArchivo.c_str());
	vector< Review >* reviews = new vector< Review >();
	if ( archivo.is_open() ){
		string id, header;
		int sentimiento = -1;
		int i = 0;
		getline(archivo,header); // Leo el header
		while ( archivo >> id ){ //Leo id y sentimiento
			if (tieneSentimiento) archivo >> sentimiento;
			if ( i < desde ) { //Saco las primeras que no me interesan
				string review_str;
				getline(archivo,review_str); //Leo review
				i++;
				continue;
			}
			if( (i+1-desde) % 1000 == 0 ) cout <<  "Se parsearon " << (i+1-desde) << " reviews a predecir de " << 25000-desde << endl;
			string review_str;
			getline(archivo,review_str); //Leo review
			vector<string> palabrasReview = limpiarReview(review_str, sentimiento, false);
			Review review(id, palabrasReview, sentimiento);
			reviews->push_back(review);
			i++;
		}
	}
	cout << "Se terminaron de parsear las reviews para predecir." << endl << endl;
	archivo.close();
	return reviews;
}

void Parser::leerCsvProbas(string nombreArchivo, vector<numeroReal>& vectorProbabilidades, vector<string>& vectorIds) {
	ifstream archivo(nombreArchivo.c_str());
	if ( archivo.is_open() ){
		string header, linea, id;
		double probabilidad;
		getline(archivo,header); // Leo el header
		while ( getline(archivo,linea) ){ //Leo id y probabilidad
			replace(linea.begin(), linea.end(), ',', ' ');
			istringstream iss(linea);
			iss >> id >> probabilidad;
			vectorProbabilidades.push_back(probabilidad);
			vectorIds.push_back(id);
		}
	}
	archivo.close();
}
