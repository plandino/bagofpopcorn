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
	bool correrBayes = true;
	bool correrPerceptron = false;
	bool ponderar = true;

//	PARA MASMENOSUNO:
	bool pesarBag = true; 		// Indica si quiero pesar o no la bag
	bool esPrueba = false;		// Indica si estoy utilizando el modo prueba -> Itera cambiando entre funcion polinomica y exponencial
								// con distintos pasos, potencias y porcentaje de aceptación (frecPositiva >= k*frecTotal)

	// WARNING: Si esPrueba = TRUE, poner la constante de CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en 15000
	// WARNING: Si esPrueba = FALSE, poner la constante de CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en 25000
	// WARNING:	Utilizar la funcion de parser->leerPalabrasYFrecuenciasDesdeTSV unicamente si antes se corrio el parser->parsearReviews con la
	// 			constante CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en el estado deseado.
	// WARNING:	Si esPrueba = TRUE y pesarBag = FALSE, solo tiene sentido variar el k.
	vector<string> vectorIdsMasMenosUno;
	vector<numeroReal> vectorProbabilidadesMasMenosUno;

	if ( correrMasMenosUno ) {
		MasMenosUno* masMenosUno = new MasMenosUno();
		masMenosUno->realizarPrediccion(bag, parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, pesarBag, esPrueba);
		parser->agregarAlCSV(vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, NOMBRE_ARCHIVO_CSV_MASMENOSUNO);
		delete masMenosUno;
	}


	vector<string> vectorIdsBayes;
	vector<numeroReal> vectorProbabilidadesBayes;

	if ( correrBayes ) {
		Bayes* bayes = new Bayes();
		bayes->realizarPrediccion(bag, parser, vectorIdsBayes, vectorProbabilidadesBayes);
		parser->agregarAlCSV(vectorIdsBayes, vectorProbabilidadesBayes, NOMBRE_ARCHIVO_CSV_BAYES);
		delete bayes;
	}


	vector<string> vectorIdsTron;
	vector<numeroReal> vectorProbabilidadesTron;

	if ( correrPerceptron ) {
		cout << "Empezando Perceptron" << endl;
		Perceptron* tron = new Perceptron(bag, parser, false, true);
		tron->entrenar();
		tron->predecir(vectorIdsTron, vectorProbabilidadesTron);
		parser->agregarAlCSV(vectorIdsTron, vectorProbabilidadesTron, NOMBRE_ARCHIVO_CSV_TRON);
		delete tron;
	}



	if (ponderar) {
		vector<numeroReal> probabilidadesFinales;
		vector<string> idsFinales;
		const double pesoBayes = 0.9;
		if ( vectorProbabilidadesMasMenosUno.size() == vectorIdsBayes.size() ){
			for (unsigned int i = 0; i < vectorProbabilidadesMasMenosUno.size(); i++){
				numeroReal probabilidadFinal = ( (vectorProbabilidadesMasMenosUno[i] * (1-pesoBayes)) + (vectorProbabilidadesBayes[i] * pesoBayes) );
				probabilidadesFinales.push_back(probabilidadFinal);
				if ( vectorIdsMasMenosUno[i] != vectorIdsBayes[i] ){
					cout << "TODO MAL GUACHIN" << endl;
					exit(1);
				}
				idsFinales.push_back(vectorIdsMasMenosUno[i]);
			}
			parser->agregarAlCSV(idsFinales, probabilidadesFinales, NOMBRE_ARCHIVO_CSV_PONDERADO);
		}
	}


	delete parser;
	return 0;
}
