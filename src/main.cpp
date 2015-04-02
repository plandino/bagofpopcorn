#include "parser.h"


int main(int argc, char* argv[]){
	Parser* parser = new Parser();
	BagOfWords* bag = parser->parsearReviews("data/labeledTrainDataTest.tsv");
	parser->generarTSV();
	delete parser;
	return 0;
}
