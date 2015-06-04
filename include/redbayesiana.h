#ifndef REDBAYESIANA_H_
#define REDBAYESIANA_H_

#include "parser.h"
#include <math.h>

class RedBayesiana{
private:
	bool predecir(Review& review, BagOfWords* bag, float k, numeroReal *probabilidadPositiva);

public:
	RedBayesiana();
	~RedBayesiana();
	void realizarPrediccion(BagOfWords* bag, Parser* parser);
};

#endif /*REDBAYESIANA_H_*/
