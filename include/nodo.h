#ifndef NODO_H_
#define NODO_H

#include <string>
#include <list>
#include <iostream>

using namespace std;

class Nodo{
private:
	string palabra;
	list<Nodo* > * nodosQueMeApuntan;
	list<Nodo* > * nodosQueApunto;
//	bool nodoNoEstaEnLista(int numeroDeLista, Nodo *nodo);

public:
	Nodo(string palabra);
	~Nodo();
	void agregarNodoQueMeApunta(Nodo *nodo);
	void apuntarANodo(Nodo *nodo);
	list<Nodo* > * getNodosQueApunto();
	list<Nodo* > * getNodosQueMeApuntan();
	string getPalabra();

};

#endif /* NODO_H_ */
