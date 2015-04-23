#include "masmenosuno.h"
#include "bayes.h"


int main(int argc, char* argv[]){
	Parser* parser = new Parser();
	MasMenosUno* masMenosUno = new MasMenosUno();
	Bayes* bayes = new Bayes();

//	Con esto parseo con el nuevo parser todas las reviews y genero el TSV
//	BagOfWords* bag = parser->parsearReviews(NOMBRE_ARCHIVO_REVIEWS);
//	parser->generarTSV();

//	Con esto genero el TSV desde el CSV de salida de python -> Solo sirve si se usan 15000 para entrenar y 10000 para predecir
//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeCSVPython("data/dataout/Bag_of_Words_model.csv");
//	parser->generarTSV();

//	Con esto leo frecuencias desde el TSV generado por el parser de C++
	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV("data/dataout/frecuencias.tsv");

//	Con esto leo frecuencias desde el TSV generado a partir del CSV de python -> Solo sirve si se usan 15000 para entrenar y 10000 para predecir
//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV("data/dataout/frecuencias_python.tsv");

	bag->pesarBag();

	masMenosUno->realizarPrediccion(bag, parser);
	bayes->realizarPrediccion(bag, parser);

//	cout << "Palabras totales: " << bag->cantidadDePalabrasTotales() << endl;
//	cout << "Palabras Positivas: " << bag->cantidadDePalabrasPositivas() << endl;
//	cout << "Palabras Negativas: " << bag->cantidadDePalabrasNegativas() << endl;
//	cout << "Palabras Iguales: " << bag->cantidadDePalabrasConFrecuenciaIgualPosyNeg() << endl;
	delete parser;
	delete masMenosUno;
	delete bayes;
	return 0;
}
