#ifndef NODO_H_
#define NODO_H

#include <string>
#include <list>
#include <iostream>
#include <vector>

using namespace std;

class Nodo{
private:
	string palabra;
	list<Nodo* > * nodosQueMeApuntan;
	list<Nodo* > * nodosQueApunto;
	vector<int>* frecuenciasNodosQueMeApuntan;
	vector<int>* frecuenciasNodosQueApunto;
//	bool nodoNoEstaEnLista(int numeroDeLista, Nodo *nodo);

public:
	Nodo(string palabra);
	~Nodo();
	void agregarNodoQueMeApunta(Nodo *nodo);
	void apuntarANodo(Nodo *nodo);
	list<Nodo* > * getNodosQueApunto();
	list<Nodo* > * getNodosQueMeApuntan();
	vector<int>* getFrecuenciasNodosQueApunto();
	vector<int>* getFrecuenciasNodosQueMeApuntan();
	string getPalabra();

};

#endif /* NODO_H_ */
