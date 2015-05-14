
#ifndef MASMENOSUNO_H_
#define MASMENOSUNO_H_

#include "parser.h"

class MasMenosUno {

private:
	double predecir(Review& review, BagOfWords* bag, float k);
	int iterarPorReviews(float k, vector<Review>* reviewsAPredecir, BagOfWords* bag, vector<string>& vectorIds, vector<numeroReal>& vectorProbabilidades, bool contar);
	void probar(BagOfWords* bag, Parser* parser, bool pesar);

public:
	MasMenosUno();
	virtual ~MasMenosUno();

	void realizarPrediccion(BagOfWords* bag, Parser* parser, vector<string>& vectorIds, vector<numeroReal>& vectorProbabilidades, bool pesar, bool esPrueba);
};

#endif /* MASMENOSUNO_H_ */
