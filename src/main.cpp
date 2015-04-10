#include "masmenosuno.h"


int main(int argc, char* argv[]){
	Parser* parser = new Parser();
	MasMenosUno* masMenosUno = new MasMenosUno();
//	BagOfWords* bag = parser->parsearReviews(NOMBRE_ARCHIVO_REVIEWS);
//	parser->generarTSV();

	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV("data/dataout/frecuencias.tsv");

//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeCSVPython("data/dataout/Bag_of_Words_model.csv");
//	parser->generarTSV();


	masMenosUno->realizarPrediccion(bag, parser);
//	masMenosUno->generarCSV();

	cout << "Palabras totales: " << bag->cantidadDePalabrasTotales() << endl;
//	cout << "Palabras Positivas: " << bag->cantidadDePalabrasPositivas() << endl;
//	cout << "Palabras Negativas: " << bag->cantidadDePalabrasNegativas() << endl;
//	cout << "Palabras Iguales: " << bag->cantidadDePalabrasConFrecuenciaIgualPosyNeg() << endl;
	delete parser;
	delete masMenosUno;
	return 0;
}
