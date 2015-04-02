#include "parser.h"


int main(int argc, char* argv[]){
	Parser* parser = new Parser();
	BagOfWords* bag = parser->parsearReviews("data/labeledTrainData.tsv");
//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV("data/frecuencias.tsv");
	parser->generarTSV();
	delete parser;
	return 0;
}
