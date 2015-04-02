#include "bagofwords.h"

BagOfWords::BagOfWords() {
	this->bag = new map<string,int>();
	this->frecuenciasPositivas = new vector<int>();
	this->frecuenciasNegativas = new vector<int>();
	this->words = new vector<string>();
	contador = 0;
}

BagOfWords::~BagOfWords() {
	//TODO: Delete de vectores y map?
}

// Agrega una nueva key al bag, mismo caso que bow simple
void BagOfWords::agregar(string key, int sentiment) {
	if ( this->estaEnBag(key) ){
		if (sentiment == 0) this->frecuenciasNegativas->at( this->bag->at(key) ) += 1;
			else this->frecuenciasPositivas->at( this->bag->at(key) ) += 1;
	} else {
		this->bag->at(key) = this->contador;
		if (sentiment == 0){
			this->frecuenciasPositivas->push_back(0);
			this->frecuenciasNegativas->push_back(1);
		} else {
			this->frecuenciasNegativas->push_back(0);
			this->frecuenciasPositivas->push_back(1);
		}
		this->contador++;
		this->words->push_back(key);
	}
}

int BagOfWords::frecuencia(string key, int sentiment) {
	if ( this->estaEnBag(key) ){
		if (sentiment == 0) return this->frecuenciasNegativas->at( this->bag->at(key) );
			else return this->frecuenciasPositivas->at( this->bag->at(key) );
	} else return 0;
}

bool BagOfWords::estaEnBag(string key) {
	return (this->bag->count(key) > 0);
}

int BagOfWords::posicionEnBag(string key) {
	if ( estaEnBag(key) ) return bag->at(key);
		else return -1;
}

int BagOfWords::getContador() {
	return this->contador;
}

vector<int>* BagOfWords::getFrecuencias(int sentiment) {
	if (sentiment == 0) return this->frecuenciasNegativas;
		else return this->frecuenciasPositivas;
}


vector<string>* BagOfWords::getWords() {
	return this->words;
}
