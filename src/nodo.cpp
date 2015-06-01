#include "nodo.h"

Nodo::Nodo(string palabra){
	this->palabra = palabra;
	this->nodosQueApunto 	= new list<Nodo* >();
	this->nodosQueMeApuntan = new list<Nodo* >();
}

Nodo::~Nodo(){
	delete nodosQueApunto;
	delete nodosQueMeApuntan;
}

list<Nodo*>* Nodo::getNodosQueApunto() {
	return nodosQueApunto;
}

list<Nodo*>* Nodo::getNodosQueMeApuntan() {
//	list<Nodo* >::iterator iterador = nodosQueMeApuntan->begin();
//	cout << "mi word" << palabra.c_str() << (*iterador)->getPalabra().c_str() << endl;
	return nodosQueMeApuntan;
}

void Nodo::agregarNodoQueMeApunta(Nodo *nodo) {
	nodosQueMeApuntan->push_back(nodo);
}

void Nodo::apuntarANodo(Nodo *nodo) {
	nodosQueApunto->push_back(nodo);
}


bool Nodo::nodoNoEstaEnLista(int numeroDeLista, Nodo* nodo) {
	list<Nodo* >::iterator iteradorNodos;
	list<Nodo* >::iterator nodoEnd;

	if(numeroDeLista == 0){
		iteradorNodos 	= nodosQueMeApuntan->begin();
		nodoEnd 		= nodosQueMeApuntan->end();
	} else if(numeroDeLista == 1){
		iteradorNodos 	= nodosQueApunto->begin();
		nodoEnd 		= nodosQueApunto->end();
	} else {
		/* numeroLista tiene que ser igual a 0 para revisar en la lista de nodos
		 * que me apuntan o igual a 1 para revisar en la lista de nodos a los que apunto.
		 */
		cout << "SOS UN CONCHUDO -- revisa nodo.cpp." << endl;
	}


	// Checkeo contra todos los nodos a ver si es igual
	for( ; iteradorNodos != nodoEnd; iteradorNodos++){
		Nodo * nodito = (*iteradorNodos);
		if(nodito == nodo)	return false;
	}
	return true;
}

string Nodo::getPalabra() {
//	cout << "Trato de sacar palabra" << endl;
//	cout << "Returneo esto: " << palabra << endl;
	return this->palabra;
}
