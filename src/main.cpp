#include "masmenosuno.h"
#include "bayes.h"
#include "perceptron.h"

int main(int argc, char* argv[]){
	Parser* parser = new Parser();


//	Con esto parseo con el nuevo parser todas las reviews y genero el TSV
	BagOfWords* bag = parser->parsearReviews(NOMBRE_ARCHIVO_LABELED_REVIEWS);
	parser->generarTSV();

//	Con esto leo frecuencias desde el TSV generado por el parser de C++
//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV(NOMBRE_ARCHIVO_FRECUENCIAS);



//	Con esto genero el TSV desde el CSV de salida de python -> Solo sirve si se usan 15000 para entrenar y 10000 para predecir
//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeCSVPython("data/dataout/Bag_of_Words_model.csv");
//	parser->generarTSV();

//	Con esto leo frecuencias desde el TSV generado a partir del CSV de python -> Solo sirve si se usan 15000 para entrenar y 10000 para predecir
//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV("data/dataout/frecuencias_python.tsv");



//	Para facilitar el activar o desactivar de correr uno y/u otro algoritmo
	bool correrMasMenosUno = true;
	bool correrBayes = false;
	bool correrPerceptron = false;

//	PARA MASMENOSUNO:
	bool pesarBag = true; 		// Indica si quiero pesar o no la bag
	bool esPrueba = false;		// Indica si estoy utilizando el modo prueba -> Itera cambiando entre funcion polinomica y exponencial
								// con distintos pasos, potencias y porcentaje de aceptación (frecPositiva >= k*frecTotal)

	// Si esPrueba = TRUE, poner la constante de CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en 15000
	// Si esPrueba = FALSE, poner la constante de CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en 25000
	// WARNING:	Utilizar la funcion de parser->leerPalabrasYFrecuenciasDesdeTSV unicamente si antes se corrio el parser->parsearReviews con la
	// 			constante CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en el estado deseado.
	// WARNING:	Si esPrueba = TRUE y pesarBag = FALSE, solo tiene sentido variar el k.
	vector<string> vectorIdsMasMenosUno;
	vector<numeroReal> vectorProbabilidadesMasMenosUno;

	if ( correrMasMenosUno ) {
		MasMenosUno* masMenosUno = new MasMenosUno();
		masMenosUno->realizarPrediccion(bag, parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, pesarBag, esPrueba);
		parser->agregarAlCSV(vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno);
		delete masMenosUno;
	}

	vector<string> vectorIdsBayes;
	vector<numeroReal> vectorProbabilidadesBayes;

	if ( correrBayes ) {
		Bayes* bayes = new Bayes();
		bayes->realizarPrediccion(bag, parser, vectorIdsBayes, vectorProbabilidadesBayes);
		parser->agregarAlCSV(vectorIdsBayes, vectorProbabilidadesBayes);
		delete bayes;
	}

	if ( correrPerceptron ) {
		cout << "Empezando Perceptron" << endl;
		Perceptron* tron = new Perceptron(bag, parser);
		tron->entrenar();
		vector<prediccion> preds = tron->predecir();
		tron->tirarACSV(preds);
		tron->getPesos();
		delete tron;
	}
//	cout << "Palabras totales: " << bag->cantidadDePalabrasTotales() << endl;
//	cout << "Palabras Positivas: " << bag->cantidadDePalabrasPositivas() << endl;
//	cout << "Palabras Negativas: " << bag->cantidadDePalabrasNegativas() << endl;
//	cout << "Palabras Iguales: " << bag->cantidadDePalabrasConFrecuenciaIgualPosyNeg() << endl;
	delete parser;
	return 0;
}
