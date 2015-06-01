#include "network.h"

Network::Network(){
	this->listaNodos = new list<Nodo* >();
}

Network::~Network(){
	delete listaNodos;
}

void Network::agregarNodo(Nodo* nodo) {
	listaNodos->push_back(nodo);
}

Nodo * Network::hayNodoConPalabra(string palabra) {
	list<Nodo* >::iterator iteradorNodos = listaNodos->begin();
	for( ; iteradorNodos != listaNodos->end(); iteradorNodos++){
		Nodo * nodito = (*iteradorNodos);
		if(palabra.compare(nodito->getPalabra()) == 0){

			return nodito;	// En la red hay un nodo que contiene la misma palabra
		}
	}
	return NULL;
}

void Network::agregarPalabra(string palabra, string palabrasAnteriores) {
	Nodo * nodoConPalabra = hayNodoConPalabra(palabra);
	if(nodoConPalabra == NULL){
		Nodo * nodo = new Nodo(palabra);
		agregarNodo(nodo);
		Nodo * nodoConPalabraAnterior = hayNodoConPalabra(palabrasAnteriores);
		if( nodoConPalabraAnterior != NULL){
			nodo->agregarNodoQueMeApunta(nodoConPalabraAnterior);
		}
	} else {
		nodoConPalabra->agregarNodoQueMeApunta(hayNodoConPalabra(palabrasAnteriores));
	}
}

list<Nodo* >* Network::getListaNodos() {
	return listaNodos;
}
