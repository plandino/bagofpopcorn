#ifndef BAGOFWORDS_H_
#define BAGOFWORDS_H_

#include <iostream>
#include <fstream> //files
#include <sstream> //stringstream
#include <algorithm> //toLower
#include <map>
#include <vector>
#include "string.h"

using namespace std;

class BagOfWords {

private:
	map<string,int>* bag;
	vector<int>* frecuenciasPositivas;
	vector<int>* frecuenciasNegativas;
	vector<string>* words;
	int contador;

public:
	BagOfWords();
	virtual ~BagOfWords();

	void agregar(string key, int sentiment);
	int frecuencia(string key, int sentiment);
	bool estaEnBag(string key);
	int posicionEnBag(string key);

	int getContador();
	vector<int>* getFrecuencias(int sentiment);
	vector<string>* getWords();
};

#endif /* BAGOFWORDS_H_ */
