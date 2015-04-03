#include "parser.h"

const int CANTIDAD_REVIEWS_A_CONSIDERAR = 15000;

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

void Parser::limpiarReview(string review, int sentiment) {
	review.erase(0,2); // Elimino comillas y tab del comienzo
	review.erase(review.length()-1, 1); // Elimino comillas del final
	replace(review.begin(), review.end(), '.', ' '); // Reemplazo los . por espacios TODO: Esto se puede sacar para evitar las urls en las reviews, pero asi en caso de que se haya olvidado el espacio despues del punto se separa como 2 palabras, que me parece mas importante
	replace(review.begin(), review.end(), '-', ' ');
	replace(review.begin(), review.end(), ',', ' ');
	replace(review.begin(), review.end(), '?', ' ');
	replace(review.begin(), review.end(), '!', ' ');
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
				char saltoDeLinea[] = "\>";
				for (unsigned int i = 0; i < strlen(saltoDeLinea); ++i){
					word.erase ( remove(word.begin(), word.end(), saltoDeLinea[i]), word.end() );
				}
			}
		} else {
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
		string http = word.substr(0,7);
		string https = word.substr(0,8);
		string www = word.substr(0,4);
		if (http == "http://") continue;
		if (https == "https://") continue;
		if (www == "www.") continue;

		string word1 = "";
		unsigned int length = word.length();
		for (unsigned int i = 0; i < length; i++){
			if ( (word[i] >= 'a') and (word[i] <= 'z') ) word1 = word1 + word[i];
		}
//		//Elimino caracteres especiales
//		for (int i = 0; i < strlen(chars); ++i){
//			word.erase ( remove(word.begin(), word.end(), chars[i]), word.end() );
//		}

		//TODO: Estas garompas  -> Con el cambio que hice ya se sacan.

		//TODO: Algun algoritmo para que, por ejemplo, considere palabras terminadas en s, ed, ing, ly sean lo mismo. Ejemplo: terrible, terribled, terribly, terribles (?

		if ( esStopWord(word1) ) continue;
		bag->agregar(word1, sentiment);
	}
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
	archivo.close();
	return bag;
}
