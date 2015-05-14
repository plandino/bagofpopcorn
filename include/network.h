#ifndef NETWORK_H_
#define NETWORK_H_

#include "nodo.h"
#include <vector>

using namespace std;

class Network{

private:
	list<Nodo* > * listaNodos;
	bool hayNodoConPalabraEnLaLista(string palabra);
	Nodo * buscarNodoConPalabraAnterior(string palabraAnterior);

public:

	Network();
	~Network();
	void agregarNodo(Nodo* nodo);
	void agregarPalabra(string palabra );
	void agregarPalabra(string palabra, string palabrasAnteriores);
	list<Nodo* > * getListaNodos();

};

#endif /* NETWORK_H */
