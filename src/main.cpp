#include "parser.h"


int main(int argc, char* argv[]){
	Parser* parser = new Parser();
	BagOfWords* bag = parser->parsearReviews("data/datain/labeledTrainData.tsv");
//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV("data/frecuencias.tsv");
	parser->generarTSV();
	cout << "Palabras totales: " << bag->cantidadDePalabrasTotales() << endl;
	cout << "Palabras Positivas: " << bag->cantidadDePalabrasPositivas() << endl;
	cout << "Palabras Negativas: " << bag->cantidadDePalabrasNegativas() << endl;
	cout << "Palabras Iguales: " << bag->cantidadDePalabrasConFrecuenciaIgualPosyNeg() << endl;
	delete parser;
	return 0;
}
