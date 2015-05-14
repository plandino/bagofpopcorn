#ifndef NETWORK_H_
#define NETWORK_H_

#include "nodo.h"

using namespace std;

class Network{

private:
	list<Nodo* > * listaNodos;
	bool hayNodoConPalabraEnLaLista(string palabra);

public:

	Network();
	~Network();
	void agregarNodo(Nodo* nodo);
	void agregarPalabra(string palabra);
	list<Nodo* > * getListaNodos();

};

#endif /* NETWORK_H */
