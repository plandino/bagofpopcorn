#ifndef NETWORK_H_
#define NETWORK_H_

#include "nodo.h"
#include <vector>

using namespace std;

class Network{

private:
	list<Nodo* > * listaNodos;
	bool networkPropia;


public:

	Network();
	Network(list<Nodo* >* listaDeNodos);
	~Network();
	void agregarNodo(Nodo* nodo);
	void agregarPalabra(string palabra, string palabrasAnteriores);
	list<Nodo* > * getListaNodos();
	Nodo * hayNodoConPalabra(string palabra);
	void obtenerNodosQueApuntanA(Nodo* nodo, list<Nodo* >* lista);

};

#endif /* NETWORK_H */
