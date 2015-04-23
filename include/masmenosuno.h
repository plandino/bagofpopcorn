
#ifndef MASMENOSUNO_H_
#define MASMENOSUNO_H_

#include "parser.h"

class MasMenosUno {

private:
	double predecir(Review& review, BagOfWords* bag, float k);

public:
	MasMenosUno();
	virtual ~MasMenosUno();

	void realizarPrediccion(BagOfWords* bag, Parser* parser);
};

#endif /* MASMENOSUNO_H_ */
