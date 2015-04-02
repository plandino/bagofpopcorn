#include "parser.h"

const int CANTIDAD_REVIEWS_A_CONSIDERAR = 15000;

Parser::Parser() {
	bag = new BagOfWords();
	cargarDiccionarioStopWords();
}

Parser::~Parser() {
}

void Parser::cargarDiccionarioStopWords() {
	stopWords["with"] = 1;
}

bool Parser::esStopWord(string word) {
	return ( stopWords.count(word) > 0 );
}

void Parser::limpiarReview(string review, int sentiment) {
	review.erase(0,2); //Elimino comillas y tab del comienzo
	review.erase(review.length()-1, 1); //Elimino comillas del final
	transform(review.begin(), review.end(), review.begin(), ::tolower);
	istringstream iss(review);
	string word;
	while(iss >> word) {
		//TODO: Limpiar palabra por palabra e ir agregándolas a la lista del bag
		if ( esStopWord(word) ) continue;
		bag->agregar(word, sentiment);
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
		cout << "ID de la ultima review leida: " << id;
	}
	archivo.close();
	return bag;
}

void Parser::generarTSV() {
	ofstream archivo("data/frecuencias.tsv");
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
