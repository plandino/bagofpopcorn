
#ifndef MASMENOSUNO_H_
#define MASMENOSUNO_H_

#include "parser.h"

class MasMenosUno {

private:
	bool predecir(Review& review, BagOfWords* bag);

public:
	MasMenosUno();
	virtual ~MasMenosUno();

	void realizarPrediccion(BagOfWords* bag, Parser* parser);
	void generarCSV();
};

#endif /* MASMENOSUNO_H_ */
