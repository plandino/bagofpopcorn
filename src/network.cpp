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

Nodo * Network::hayNodoConPalabra(string palabra, unsigned int *posicion) {
	*posicion = -1;
	list<Nodo* >::iterator iteradorNodos = listaNodos->begin();
	for(unsigned int i = 0 ; iteradorNodos != listaNodos->end(); iteradorNodos++, i++){
		Nodo * nodito = (*iteradorNodos);
//		cout << "Palabra: " << palabra.c_str() << endl;
		if(palabra.compare(nodito->getPalabra()) == 0){
			*posicion = i;
			return nodito;	// En la red hay un nodo que contiene la misma palabra
		}
	}
	return NULL;
}

Nodo * Network::hayNodoConPalabra(string palabra) {
	list<Nodo* >::iterator iteradorNodos = listaNodos->begin();
	for(unsigned int i = 0 ; iteradorNodos != listaNodos->end(); iteradorNodos++, i++){
		Nodo * nodito = (*iteradorNodos);
//		cout << "Palabra: " << palabra.c_str() << endl;
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
	}

	if(nodoConPalabraAnterior != NULL){
		nodoConPalabra->agregarNodoQueMeApunta(nodoConPalabraAnterior);
		nodoConPalabraAnterior->apuntarANodo(nodoConPalabra);
	}
}

list<Nodo* >* Network::getListaNodos() {
	return listaNodos;
}

// Me pasan un NODO y una lista
// Devuelvo, en la lista que me pasaron por parametro, todos los que nodos que apuntan a NODO

void Network::obtenerNodosQueApuntanA(Nodo* nodo, list<Nodo* >* lista) {

	// Empiezo a iterar por mi lista de nodos
	list<Nodo* >::iterator iteradorNodos = listaNodos->begin();
	for( ; iteradorNodos != listaNodos->end(); iteradorNodos++){

		// Obtengo un nodo
		Nodo * nodito = (*iteradorNodos);

		// Empiezo a iterar por la lista de nodos que apunto
		list<Nodo* >::iterator iteradorApuntadores = nodito->getNodosQueApunto()->begin();
		for(; iteradorApuntadores != nodito->getNodosQueApunto()->end(); iteradorApuntadores++){

			// Obtengo la palabra del nodo que estoy apuntando
			string palabraApuntada = (*iteradorApuntadores)->getPalabra();

			// Si la palabra del nodo al que estoy apuntando es igual a la del nodo que estoy buscando
			// agrego el nodo por el que estoy iterando, a la lista
			if(nodo->getPalabra().compare(palabraApuntada) == 0){
				lista->push_back(nodito);
			}
		}
	}
}
