#ifndef PREDICTORREDBAYESIANA_H_
#define PREDICTORREDBAYESIANA_H_

#include <string>
#include <list>
#include <iostream>
#include "network.h"
#include "parser.h"

using namespace std;

class PredictorRedBayesiana{

private:
	Network* redPositva;
	Network* redNegativa;
	Parser*  parser;

public:
	PredictorRedBayesiana(Network * redPositva, Network* redNegativa, Parser * parser);
	~PredictorRedBayesiana();
	void realizarPrediccion();
	bool predecir(Review& reviewAPRedecir, numeroReal * probabilidadPostiva);

};

#endif /* PREDICTORREDBAYESIANA_H_ */
