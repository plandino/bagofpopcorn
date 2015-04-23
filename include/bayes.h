#ifndef BAYES_H_
#define BAYES_H_

#include "parser.h"
#include <math.h>

// Esto lo hago para cambiar entre double y float por la precision en la multiciplicacion
// de probabilidades menores a 1
typedef double numeroReal;

class Bayes{
private:
	bool predecir(Review& review, BagOfWords* bag, float k, numeroReal *probabilidadPositiva);

public:
	Bayes();
	~Bayes();
	void realizarPrediccion(BagOfWords* bag, Parser* parser);
};

#endif /*BAYES_H_*/
