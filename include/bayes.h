#ifndef BAYES_H_
#define BAYES_H_

#include "parser.h"
#include <math.h>

class Bayes{
private:
	bool predecir(Review& review, BagOfWords* bag, float k, numeroReal *probabilidadPositiva);

public:
	Bayes();
	~Bayes();
	void realizarPrediccion(BagOfWords* bag, Parser* parser);
};

#endif /*BAYES_H_*/
