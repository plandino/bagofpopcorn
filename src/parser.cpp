#include "parser.h"

Parser::Parser() {
	bag = new BagOfWords();
	cargarDiccionarioStopWords(NOMBRE_ARCHIVO_STOPWORDS1);
	cargarDiccionarioStopWords(NOMBRE_ARCHIVO_STOPWORDS2);
}

Parser::~Parser() {
	//TODO: Delete bag?
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

vector<string> Parser::limpiarReview(string review, int sentiment) {
	vector<string> palabrasReview;
	review.erase(0,2); // Elimino comillas y tab del comienzo
	review.erase(review.length()-1, 1); // Elimino comillas del final
//	replace(review.begin(), review.end(), '.', ' '); // Reemplazo los . por espacios TODO: Esto se puede sacar para evitar las urls en las reviews, pero asi en caso de que se haya olvidado el espacio despues del punto se separa como 2 palabras, que me parece mas importante
//	replace(review.begin(), review.end(), '-', ' ');
//	replace(review.begin(), review.end(), ',', ' ');
//	replace(review.begin(), review.end(), '?', ' ');
//	replace(review.begin(), review.end(), '!', ' ');
	transform(review.begin(), review.end(), review.begin(), ::tolower);
	istringstream iss(review);
	string word;
	bool anteriorEsSaltoDeLinea = false;
//	char chars[] = "ºª!|\"@·#$%&¬/()='?¡¿[]^*ç`'\\{},;.:-_<>";
	while(iss >> word) {
		//TODO: Limpiar palabra por palabra e ir agregándolas a la lista del bag

		//Elimino los <br />
		if (anteriorEsSaltoDeLinea){
			if (word == "/>") {
				anteriorEsSaltoDeLinea = false;
				continue;
			} else {
				anteriorEsSaltoDeLinea = false;
				word.erase (word.begin(), word.begin()+2);
//				char saltoDeLinea[] = "/>";
//				for (unsigned int i = 0; i < strlen(saltoDeLinea); ++i){
//					word.erase ( remove(word.begin(), word.end(), saltoDeLinea[i]), word.end() );
//				}
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
//		string wordSoloLetras = "";
//		unsigned int length = wordSinURL.length();
//		for (unsigned int i = 0; i < length; i++){
//			if ( (wordSinURL[i] >= 'a') and (wordSinURL[i] <= 'z') ) wordSoloLetras = wordSoloLetras + wordSinURL[i];
//				else {
//					words.push_back(wordSoloLetras);
//					wordSoloLetras = "";
//				}
//		}


//		//Elimino caracteres especiales
//		for (int i = 0; i < strlen(chars); ++i){
//			word.erase ( remove(word.begin(), word.end(), chars[i]), word.end() );
//		}

		//TODO: Estas garompas  -> Con el cambio que hice ya se sacan.

		//TODO: Algun algoritmo para que, por ejemplo, considere palabras terminadas en s, ed, ing, ly sean lo mismo. Ejemplo: terrible, terribled, terribly, terribles (?
		vector<string>::iterator iterador = words.begin();
		for ( ; iterador != words.end() ; iterador++){
			string wordAAgregar = (*iterador);
			if ( esStopWord(wordAAgregar) ) continue;
			if ( (wordAAgregar.length() == 1) or (wordAAgregar.length() == 0) ) continue;
			bag->agregar(wordAAgregar, sentiment);
			palabrasReview.push_back(wordAAgregar);
		}
	}
	return palabrasReview;
}

BagOfWords* Parser::parsearReviews(string nombreArchivo) {
	ifstream archivo(nombreArchivo.c_str());
	if ( archivo.is_open() ){
		string id, header;
		int sentimiento;
		int i = 0;
		getline(archivo,header); // Leo el header
		while ( (archivo >> id >> sentimiento) and (i < CANTIDAD_REVIEWS_A_CONSIDERAR) ){ //Leo id y sentimiento
			if( (i+1) % 1000 == 0 )cout <<  "Review " << (i+1) << " de " << CANTIDAD_REVIEWS_A_CONSIDERAR << endl;
			string review;
			getline(archivo,review); //Leo review
			limpiarReview(review, sentimiento);
			i++;
		}
	}
	cout << "Se terminaron de parsear las palabras." << endl << endl;
	archivo.close();
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

BagOfWords* Parser::leerPalabrasYFrecuenciasDesdeTSV(string nombreArchivo) {
	ifstream archivo(nombreArchivo.c_str());
	if ( archivo.is_open() ){
		string word, header;
		int frecPos, frecNeg;
		getline(archivo,header); // Leo el header
		while ( archivo >> word >> frecPos >> frecNeg ){ //Leo palabra y frecuencias
			bag->agregar(word, frecPos, frecNeg);
		}
	}
	cout << "Se cargo el archivo de palabras y frecuencias." << endl;
	archivo.close();
	return bag;
}

BagOfWords* Parser::leerPalabrasYFrecuenciasDesdeCSVPython(string nombreArchivo) {
	ifstream archivo(nombreArchivo.c_str());
	if ( archivo.is_open() ){
		string word, header, linea;
		int frecPos, frecNeg;
		getline(archivo,header); // Leo el header
		while ( getline(archivo,linea) ){ //Leo palabra y frecuencias
			replace(linea.begin(), linea.end(), ',', ' ');
			istringstream iss(linea);
			iss >> frecNeg >> frecPos >> word;
			bag->agregar(word, frecPos, frecNeg);
		}
	}
	cout << "Se cargo el archivo de palabras y frecuencias generado en python." << endl;
	archivo.close();
	return bag;
}

string Parser::eliminarURL(string word, bool &continuar) {
	unsigned int length = word.length();
	string wordSinURL = "";
	for (unsigned int i = 0; i < length; i++){
		if ( (word[i] != '<') and (word[i] != '(') and (word[i] != ')') ) wordSinURL = wordSinURL + word[i];
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
		if ( (word[i] >= 'a') and (word[i] <= 'z') ) wordSoloLetras = wordSoloLetras + word[i];
			else {
				words.push_back(wordSoloLetras);
				wordSoloLetras = "";
			}
	}
	return words;
}

vector< Review >* Parser::parsearReviewsAPredecir(string nombreArchivo, int desde) {
	ifstream archivo(nombreArchivo.c_str());
	vector< Review >* reviews = new vector< Review >();
	if ( archivo.is_open() ){
		string id, header;
		int sentimiento;
		int i = 0;
		getline(archivo,header); // Leo el header
		while ( archivo >> id >> sentimiento ){ //Leo id y sentimiento
			if ( i < desde ) { //Saco las primeras que no me interesan
				string review_str;
				getline(archivo,review_str); //Leo review
				i++;
				continue;
			}
			if( (i+1-CANTIDAD_REVIEWS_A_CONSIDERAR) % 1000 == 0 )cout <<  "Review " << (i+1-CANTIDAD_REVIEWS_A_CONSIDERAR) << " de " << 25000-desde << endl;
			string review_str;
			getline(archivo,review_str); //Leo review
			vector<string> palabrasReview = limpiarReview(review_str, sentimiento);
			Review review(id, palabrasReview, sentimiento);
			reviews->push_back(review);
			i++;
		}
	}
	archivo.close();
	return reviews;
}

