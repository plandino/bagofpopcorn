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

void Network::agregarPalabra(string palabra) {
	if(! hayNodoConPalabraEnLaLista(palabra)){
		Nodo * nodo = new Nodo(palabra);
//		cout << "Meto un nodo" << endl;
		agregarNodo(nodo);
//		cout << "Pude meter un nodo" << endl;
//		cout << endl << " ---------------------------------------------------------------------------" << endl << endl;
	}
}

bool Network::hayNodoConPalabraEnLaLista(string palabra) {
	list<Nodo* >::iterator iteradorNodos = listaNodos->begin();
//	cout << "La palabra a meter: " << palabra.c_str() << endl << endl;
	// Checkeo contra todos los nodos a ver si es igual
	for( ; iteradorNodos != listaNodos->end(); iteradorNodos++){
		Nodo * nodito = (*iteradorNodos);
//		cout << "Comparo contra la palabra: " << nodito->getPalabra().c_str() << endl;
//		cout << "La comparacion me dio: " << palabra.compare(nodito->getPalabra()) << endl;
		if(palabra.compare(nodito->getPalabra()) == 0){

			return true;
		}
	}
	return false;
}

void Network::agregarPalabra(string palabra, string palabrasAnteriores) {
	if(! hayNodoConPalabraEnLaLista(palabra)){
		Nodo * nodo = new Nodo(palabra);
//		cout << "Meto un nodo" << endl;
		agregarNodo(nodo);
		cout << "lcdtm" << endl;
		Nodo * nodosad = buscarNodoConPalabraAnterior(palabrasAnteriores);
		nodo->agregarNodoQueMeApunta(buscarNodoConPalabraAnterior(palabrasAnteriores));
		cout << "Pude meter un nodo" << endl;
//		cout << endl << " ---------------------------------------------------------------------------" << endl << endl;
	}
}

Nodo* Network::buscarNodoConPalabraAnterior(string palabraAnterior) {
	list<Nodo* >::iterator iteradorNodos = listaNodos->begin();
//	cout << "La palabra a meter: " << palabraAnterior.c_str() << endl << endl;
	// Checkeo contra todos los nodos a ver si es igual
	for( ; iteradorNodos != listaNodos->end(); iteradorNodos++){
		Nodo * nodito = (*iteradorNodos);
//		cout << "Comparo contra la palabra: " << nodito->getPalabra().c_str() << endl;
//		cout << "La comparacion me dio: " << palabraAnterior.compare(nodito->getPalabra()) << endl;
		if( palabraAnterior.compare(nodito->getPalabra()) == 0){
//			cout << "Encontre el nodo" << endl;
			return nodito;
		}
	}
	return NULL;
}

list<Nodo*>* Network::getListaNodos() {
	return listaNodos;
}