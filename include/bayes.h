#ifndef BAYES_H_
#define BAYES_H_

#include "parser.h"
#include <math.h>

class Bayes{
private:
	bool predecir(Review& review, BagOfWords* bag, numeroReal& probabilidadPositiva);

public:
	Bayes();
	~Bayes();
	void realizarPrediccion(BagOfWords* bag, Parser* parser, vector< Review >* reviewsAPredecir, vector<string>& vectorIds, vector<numeroReal>& vectorProbabilidades);
};

#endif /*BAYES_H_*/
