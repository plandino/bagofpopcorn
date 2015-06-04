#ifndef PREDICTORREDBAYESIANA_H_
#define PREDICTORREDBAYESIANA_H_

#include <string>
#include <list>
#include <iostream>
//#include <queue>          // std::queue

#include "network.h"
#include "parser.h"

using namespace std;

class PredictorRedBayesiana{

private:
	Network* redPositiva;
	Network* redNegativa;
	Parser*  parser;

public:
	PredictorRedBayesiana(Network * redPositva, Network* redNegativa, Parser * parser);
	~PredictorRedBayesiana();
	void realizarPrediccion(int cantidadDePalabrasAnteriores);
	bool predecir(Review& reviewAPRedecir, numeroReal * probabilidadPostiva, int cantidadDePalabrasAnteriores);

};

#endif /* PREDICTORREDBAYESIANA_H_ */
