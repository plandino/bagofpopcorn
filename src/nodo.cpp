#include "nodo.h"

Nodo::Nodo(string palabra){
	this->palabra = palabra;
	this->nodosQueApunto 	= new list<Nodo* >();
	this->nodosQueMeApuntan = new list<Nodo* >();
	this->frecuenciasNodosQueApunto = new vector<int>();
	this->frecuenciasNodosQueMeApuntan = new vector<int>();
}

Nodo::~Nodo(){
	delete nodosQueApunto;
	delete nodosQueMeApuntan;
	delete frecuenciasNodosQueApunto;
	delete frecuenciasNodosQueMeApuntan;
}

list<Nodo*>* Nodo::getNodosQueApunto() {
	return nodosQueApunto;
}

list<Nodo*>* Nodo::getNodosQueMeApuntan() {
	return nodosQueMeApuntan;
}

vector<int>* Nodo::getFrecuenciasNodosQueApunto() {
	return frecuenciasNodosQueApunto;
}

vector<int>* Nodo::getFrecuenciasNodosQueMeApuntan() {
	return frecuenciasNodosQueMeApuntan;
}

void Nodo::agregarNodoQueMeApunta(Nodo *nodo) {
	bool agregarNodo = true;
	list<Nodo* >::iterator iteradorNodos = nodosQueMeApuntan->begin();
	int i = 0;
	for(; iteradorNodos != nodosQueMeApuntan->end(); iteradorNodos++, i++){
		Nodo * nodito = (*iteradorNodos);
		if( nodo->getPalabra().compare(nodito->getPalabra()) == 0){
			agregarNodo = false;
			frecuenciasNodosQueMeApuntan->at(i)++;
		}
	}
	if(agregarNodo){
		nodosQueMeApuntan->push_back(nodo);
		frecuenciasNodosQueMeApuntan->push_back(1);
	}
}

void Nodo::apuntarANodo(Nodo *nodo) {
	bool agregarNodo = true;
	list<Nodo* >::iterator iteradorNodos = nodosQueApunto->begin();
	int i = 0;
	for(; iteradorNodos != nodosQueApunto->end(); iteradorNodos++, i++){
		Nodo * nodito = (*iteradorNodos);
		if( nodo->getPalabra().compare(nodito->getPalabra()) == 0){
			agregarNodo = false;
			frecuenciasNodosQueApunto->at(i)++;
		}
	}
	if(agregarNodo){
		nodosQueApunto->push_back(nodo);
		frecuenciasNodosQueApunto->push_back(1);
	}
}

string Nodo::getPalabra() {
//	cout << "Voy a devolver palabra " << palabra.c_str() << endl;
	return this->palabra;
}
