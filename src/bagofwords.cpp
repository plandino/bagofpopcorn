#include "bagofwords.h"

BagOfWords::BagOfWords() {
	this->bag = new map<string,int>();
	this->frecuenciasPositivas = new vector<int>();
	this->frecuenciasNegativas = new vector<int>();
	this->words = new vector<string>();
	this->contador = 0;
}

BagOfWords::~BagOfWords() {
	//TODO: Delete de vectores y map?
	delete this->bag;
	delete this->frecuenciasNegativas;
	delete this->frecuenciasPositivas;
	delete this->words;
}

// Agrega una nueva key al bag, mismo caso que bow simple
void BagOfWords::agregar(string key, int sentiment) {
	if ( this->estaEnBag(key) ){
		if (sentiment == 0) this->frecuenciasNegativas->at( this->bag->at(key) ) += 1;
			else this->frecuenciasPositivas->at( this->bag->at(key) ) += 1;
	} else {
		bag->insert(pair<string, int>(key, contador));
		if (sentiment == 0){
			this->frecuenciasPositivas->push_back(0);
			this->frecuenciasNegativas->push_back(1);
		} else {
			this->frecuenciasNegativas->push_back(0);
			this->frecuenciasPositivas->push_back(1);
		}
		this->words->push_back(key);
		contador++;
	}
}

// Agrega una nueva key al bag, con una frecuencia positiva y otra negativa.
// Sirve pàra cuando se levantan las palabras directamente desde el TSV.
void BagOfWords::agregar(string key, int frecPos, int frecNeg) {
	bag->insert(pair<string, int>(key, contador));
	this->frecuenciasNegativas->push_back(frecNeg);
	this->frecuenciasPositivas->push_back(frecPos);
	this->words->push_back(key);
	contador++;
}

// Devuelve la frecuencia con la que aparece una key
int BagOfWords::frecuencia(string key, int sentiment) {
	if ( this->estaEnBag(key) ){
		if (sentiment == 0) return this->frecuenciasNegativas->at( this->bag->at(key) );
			else return this->frecuenciasPositivas->at( this->bag->at(key) );
	} else return 0;
}

// Devuelve true si la key esta en el bag
bool BagOfWords::estaEnBag(string key) {
	return (this->bag->count(key) > 0);
}

// Devuelve la posicion del vector de frecuencias en la que se encuentra la key
int BagOfWords::posicionEnBag(string key) {
	if ( estaEnBag(key) ) return bag->at(key);
		else return -1;
}

// Devuelve el vector con las frecuencias
vector<int>* BagOfWords::getFrecuencias(int sentiment) {
	if (sentiment == 0) return this->frecuenciasNegativas;
		else return this->frecuenciasPositivas;
}

vector<string>* BagOfWords::getWords() {
	return this->words;
}
