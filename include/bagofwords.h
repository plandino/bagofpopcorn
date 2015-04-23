#ifndef BAGOFWORDS_H_
#define BAGOFWORDS_H_

#include <iostream>
#include <fstream> //files
#include <sstream> //stringstream
#include <algorithm> //toLower y remove
#include <map>
#include <vector>
#include "string.h"

using namespace std;

class BagOfWords {

// Un bow seria un diccionario que guarda word:elemarray
// siendo elemarray la posicion en donde se guardara la frecuencia
// en el feature vector generado en este proceso

private:
	map<string,int>* bag;
	vector<int>* frecuenciasPositivas;
	vector<int>* frecuenciasNegativas;
	vector<int>* pesosPositivos;
	vector<int>* pesosNegativos;
	vector<string>* words;
	int contador;

public:
	BagOfWords();
	virtual ~BagOfWords();

	void agregar(string key, int sentiment);
	void agregar(string key, int frecPos, int frecNeg);
	// Devuelve cuantas palabras solo aparecen en reviews positivas o negativas.
	// Si hay igual frecuencia en las dos, no cuenta para ninguna.
	int cantidadDePalabrasPositivas();
	int cantidadDePalabrasNegativas();
	int cantidadDePalabrasConFrecuenciaIgualPosyNeg();
	int cantidadDePalabrasTotales();
	int frecuencia(string key, int sentiment);
	bool estaEnBag(string key);
	int posicionEnBag(string key);

	vector<int>* getFrecuencias(int sentiment);
	vector<int>* getPesosPositivos();
	vector<int>* getPesosNegativos();
	vector<string>* getWords();

	void pesarBag();
};

#endif /* BAGOFWORDS_H_ */
