#ifndef BAGOFWORDS_H_
#define BAGOFWORDS_H_

#include <iostream>
#include <fstream> //files
#include <sstream> //stringstream
#include <algorithm> //toLower y remove
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <cmath>

// Esto lo hago para cambiar entre double y float por la precision en la multiciplicacion
// de probabilidades menores a 1
typedef long double numeroReal;

using namespace std;

class BagOfWords {

// Un bow seria un diccionario que guarda word:elemarray
// siendo elemarray la posicion en donde se guardara la frecuencia
// en el feature vector generado en este proceso

private:
	unordered_map<string,int>* bag;
	vector<int>* frecuenciasPositivas;
	vector<int>* frecuenciasNegativas;
	vector<int>* pesosPositivos;
	vector<int>* pesosNegativos;
	vector<numeroReal>* probabilidadesPositivas;
	vector<numeroReal>* probabilidadesNegativas;
	vector<numeroReal>* probabilidadesNegativasTrad;
	vector<numeroReal>* probabilidadesPositivasTrad;
	vector<string>* words;
	int contador;

public:
	BagOfWords();
	virtual ~BagOfWords();

	void agregar(string key, int sentiment);
	void agregar(string key, int frecPos, int frecNeg);
	bool estaEnBag(string key);
	int posicionEnBag(string key);

	void crearVectorConProbabilidades();

	vector<int>* getFrecuencias(int sentiment);
	vector<int>* getPesosPositivos();
	vector<int>* getPesosNegativos();
	vector<numeroReal>* getProbabilidadesPositivasTradicionales();
	vector<numeroReal>* getProbabilidadesNegativasTradicionales();
	vector<string>* getWords();

	void pesarBag(double paso, double potencia, bool exponencial);
};

#endif /* BAGOFWORDS_H_ */
