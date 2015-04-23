#include "bagofwords.h"

BagOfWords::BagOfWords() {
	this->bag = new map<string,int>();
	this->frecuenciasPositivas = new vector<int>();
	this->frecuenciasNegativas = new vector<int>();
	this->pesosPositivos = new vector<int>();
	this->pesosNegativos = new vector<int>();
	this->words = new vector<string>();
	this->contador = 0;
}

BagOfWords::~BagOfWords() {
	//TODO: Delete de vectores y map?
	delete this->pesosPositivos;
	delete this->pesosNegativos;
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
		this->pesosPositivos->push_back(1);
		this->pesosNegativos->push_back(1);
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
	this->pesosPositivos->push_back(1);
	this->pesosNegativos->push_back(1);
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

vector<int>* BagOfWords::getPesosPositivos() {
	return this->pesosPositivos;
}

vector<int>* BagOfWords::getPesosNegativos() {
	return this->pesosNegativos;
}

vector<string>* BagOfWords::getWords() {
	return this->words;
}

int BagOfWords::cantidadDePalabrasNegativas() {

	return (bag->size() - this->cantidadDePalabrasPositivas() - this->cantidadDePalabrasConFrecuenciaIgualPosyNeg());
}

int BagOfWords::cantidadDePalabrasPositivas() {

	int contadorPositivas = 0;
	for( unsigned int i = 0; i < frecuenciasNegativas->size() ; i++)
	{
		if((*frecuenciasNegativas)[i] < (*frecuenciasPositivas)[i])
		{
					contadorPositivas++;
		}
	}
	return contadorPositivas;
}

int BagOfWords::cantidadDePalabrasConFrecuenciaIgualPosyNeg() {

	int contadorIguales = 0;
	for( unsigned int i = 0; i < frecuenciasNegativas->size() ; i++)
	{
		if((*frecuenciasNegativas)[i] == (*frecuenciasPositivas)[i])
		{
			contadorIguales++;
		}
	}
	return contadorIguales;
}

int BagOfWords::cantidadDePalabrasTotales() {
	return bag->size();
}

bool pred(const int a, const int b) {
	return a < b;
}

void BagOfWords::pesarBag() {
	int maxPos = (*max_element(frecuenciasPositivas->begin(), frecuenciasPositivas->end(), pred));
	int maxNeg = (*max_element(frecuenciasNegativas->begin(), frecuenciasNegativas->end(), pred));

	vector<int>::iterator iteradorPos = frecuenciasPositivas->begin();
	int i = 0;
	for ( ; iteradorPos != frecuenciasPositivas->end() ; iteradorPos++, i++){
		int frecuencia = (*iteradorPos);
		float paso = 0.1;
		for (float j = 0; j < 1; j+=paso){
			if ((j*maxPos < frecuencia) and ((j+paso)*maxPos >= frecuencia)) {
				pesosPositivos->at(i) = ((j+paso)/paso);
				break;
			}
		}
	}

	vector<int>::iterator iteradorNeg = frecuenciasNegativas->begin();
	i = 0;
	for ( ; iteradorNeg != frecuenciasNegativas->end() ; iteradorNeg++, i++){
		int frecuencia = (*iteradorNeg);
		float paso = 0.1;
		for (float j = 0; j < 1; j+=paso){
			if ((j*maxNeg < frecuencia) and ((j+paso)*maxNeg >= frecuencia)) {
				pesosNegativos->at(i) = ((j+paso)/paso);
				break;
			}
		}
	}
}
