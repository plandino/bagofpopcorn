#include "network.h"

Network::Network(){
	networkPropia = true;
	this->listaNodos = new list<Nodo* >();
}

Network::Network(list<Nodo*>* listaDeNodos) {
	networkPropia = false;
	this->listaNodos = listaDeNodos;
}

Network::~Network(){
	if(networkPropia){
		delete listaNodos;
	}

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
	// Encuentro los nodos de la palabra actual y de la palabra anterior
	Nodo * nodoConPalabra = hayNodoConPalabra(palabra);
	Nodo * nodoConPalabraAnterior = hayNodoConPalabra(palabrasAnteriores);

	// Si no hay un nodo con la palabra anterior, agrego un nodo con esa palabra
	if(nodoConPalabra == NULL){
		nodoConPalabra = new Nodo(palabra);
		agregarNodo(nodoConPalabra);
//		if( nodoConPalabraAnterior != NULL){
//			nodoConPalabra->agregarNodoQueMeApunta(nodoConPalabraAnterior);
//		}
//	} else {
//		if(nodoConPalabraAnterior != NULL){
//
//		}
	}

	// Si hay un nodo con la palabra anterior hago las apuntaciones
	if(nodoConPalabraAnterior != NULL){
		nodoConPalabra->agregarNodoQueMeApunta(nodoConPalabraAnterior);
		nodoConPalabraAnterior->apuntarANodo(nodoConPalabra);
	}
}

list<Nodo* >* Network::getListaNodos() {
	return listaNodos;
}


