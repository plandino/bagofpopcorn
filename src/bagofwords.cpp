#include "bagofwords.h"

BagOfWords::BagOfWords() {
	this->bag = new unordered_map<string,int>();
	this->frecuenciasPositivas = new vector<int>();
	this->frecuenciasNegativas = new vector<int>();
	this->pesosPositivos = new vector<int>();
	this->pesosNegativos = new vector<int>();
	this->probabilidadesPositivas = new vector<numeroReal>();
	this->probabilidadesNegativas = new vector<numeroReal>();
	this->probabilidadesNegativasTrad = new vector<numeroReal>();
	this->probabilidadesPositivasTrad = new vector<numeroReal>();
	this->words = new vector<string>();
	this->contador = 0;
}

BagOfWords::~BagOfWords() {
	delete this->bag;
	delete this->frecuenciasNegativas;
	delete this->frecuenciasPositivas;
	delete this->pesosPositivos;
	delete this->pesosNegativos;
	delete this->probabilidadesPositivas;
	delete this->probabilidadesNegativas;
	delete this->probabilidadesPositivasTrad;
	delete this->probabilidadesNegativasTrad;
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

bool pred(const int a, const int b) {
	return a < b;
}

void BagOfWords::crearVectorConProbabilidades() {
	// FALTA HACER QUE USE LAS PONDERACIONES DE LOS PESOS
	// Con esto calculo la probabilidad de que una palabra sea pos o neg
	int length = this->bag->size();
	numeroReal frecPositiva = 0;
	numeroReal frecNegativa = 0;
	numeroReal frecTotal = 0;
	for(int i = 0; i < length; i++){
		frecPositiva = (*frecuenciasPositivas)[i] + 1;
		frecNegativa = (*frecuenciasNegativas)[i] + 1;
		frecTotal = frecPositiva + frecNegativa;
		probabilidadesPositivas->push_back(frecPositiva / frecTotal);
		probabilidadesNegativas->push_back(frecNegativa / frecTotal);
	}

	// Con esto calculo la probabilidad de que una palabra este adentro de un determinado bag
	numeroReal frecuenciaTodaLaBagPos = 0;
	numeroReal frecuenciaTodaLaBagNeg = 0;
	for(int i = 0; i < length; i++){
		frecuenciaTodaLaBagPos += (*frecuenciasPositivas)[i];
		frecuenciaTodaLaBagNeg += (*frecuenciasNegativas)[i];
	}
	frecuenciaTodaLaBagPos += length;
	frecuenciaTodaLaBagNeg += length;
	for(int i = 0; i < length; i++){
		frecPositiva = (*frecuenciasPositivas)[i] + 1;
		frecNegativa = (*frecuenciasNegativas)[i] + 1;
		probabilidadesPositivasTrad->push_back(log(frecPositiva / frecuenciaTodaLaBagPos)); // / frecPositiva);	//frecPositiva / frecuenciaTodaLaBagPos));
		probabilidadesNegativasTrad->push_back(log(frecNegativa / frecuenciaTodaLaBagNeg)); // / frecNegativa);	//frecNegativa / frecuenciaTodaLaBagNeg));
	}

}

vector<numeroReal>* BagOfWords::getProbabilidadesPositivasTradicionales() {
	return probabilidadesPositivasTrad;
}

vector<numeroReal>* BagOfWords::getProbabilidadesNegativasTradicionales() {
	return probabilidadesNegativasTrad;
}

void BagOfWords::pesarBag(double paso, double potencia, bool exponencial) {
	int maxPos = (*max_element(frecuenciasPositivas->begin(), frecuenciasPositivas->end(), pred));
	int maxNeg = (*max_element(frecuenciasNegativas->begin(), frecuenciasNegativas->end(), pred));

	vector<int>::iterator iteradorPos = frecuenciasPositivas->begin();
	int i = 0;
	for ( ; iteradorPos != frecuenciasPositivas->end() ; iteradorPos++, i++){
		int frecuencia = (*iteradorPos);
		for (double j = 0; j < 1; j+=paso){
			if (exponencial) {
				if (((exp(pow(j, potencia))-1)*maxPos/(exp(1)-1) < frecuencia) and ((exp(pow(j+paso, potencia))-1)*maxPos/(exp(1)-1) >= frecuencia)) {
					pesosPositivos->at(i) = ((j+paso)/paso);
					break;
				}
			} else {
				if ((pow(j, potencia)*maxPos < frecuencia) and (pow((j+paso), potencia)*maxPos >= frecuencia)) {
					pesosPositivos->at(i) = ((j+paso)/paso);
					break;
				}
			}
		}
		if ( (i+1) % 100000 == 0 ) cout << "Pesados " << i+1 << " elementos positivos de la bag." << endl;
	}

	vector<int>::iterator iteradorNeg = frecuenciasNegativas->begin();
	i = 0;
	for ( ; iteradorNeg != frecuenciasNegativas->end() ; iteradorNeg++, i++){
		int frecuencia = (*iteradorNeg);
		for (double j = 0; j < 1; j+=paso){
			if (exponencial) {
				if (((exp(pow(j, potencia))-1)*maxNeg/(exp(1)-1) < frecuencia) and ((exp(pow(j+paso, potencia))-1)*maxNeg/(exp(1)-1) >= frecuencia)) {
					pesosNegativos->at(i) = ((j+paso)/paso);
					break;
				}
			} else {
				if ((pow(j, potencia)*maxNeg < frecuencia) and (pow((j+paso), potencia)*maxNeg >= frecuencia)) {
					pesosNegativos->at(i) = ((j+paso)/paso);
					break;
				}
			}
		}
		if ( (i+1) % 100000 == 0 ) cout << "Pesados " << i+1 << " elementos negativos de la bag." << endl;
	}
}
