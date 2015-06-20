#include "masmenosuno.h"
#include "bayes.h"
#include "perceptron.h"

int main(int argc, char* argv[]){
	const bool biWord = true;
	const bool triWord = true;
	const bool sinStopWords = true;
	Parser* parserSinStopWords = new Parser(sinStopWords);
	Parser* parserConStopWords = new Parser(not sinStopWords);



//	Con esto parseo con el nuevo parser todas las reviews y genero el TSV
	cout << "-------------------------Bag Of Words sin stopwords-------------------------" << endl;
	BagOfWords* bagSinStopWords = parserSinStopWords->parsearReviews(NOMBRE_ARCHIVO_LABELED_REVIEWS, biWord, not triWord);
	cout << "-------------------------Bag Of Words con stopwords-------------------------" << endl;
	BagOfWords* bagConStopWords = parserConStopWords->parsearReviews(NOMBRE_ARCHIVO_LABELED_REVIEWS, biWord, triWord);
	parserConStopWords->generarTSV();

//	Con esto leo frecuencias desde el TSV generado por el parser de C++
//	WARNING: HAY QUE CAMBIAR ESTO PARA QUE LEA BIWORDS!!
//	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV(NOMBRE_ARCHIVO_FRECUENCIAS);



//	Para facilitar el activar o desactivar de correr uno y/u otro algoritmo
	bool correrMasMenosUno = true;
	bool levantarCsvMasMenosUno = false;
	bool correrBayes = true;
	bool levantarCsvBayes = false;
	bool correrPerceptron = true;
	bool levantarCsvPerceptron = false;
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
		cout << endl << endl << "------------------------------ MAS MENOS UNO ------------------------------" << endl;
		masMenosUno->realizarPrediccion(bagSinStopWords, parserSinStopWords, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, pesarBag, esPrueba);
		parserSinStopWords->agregarAlCSV(vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, NOMBRE_ARCHIVO_CSV_MASMENOSUNO);
		delete masMenosUno;
	}
	if (levantarCsvMasMenosUno){
		parserSinStopWords->leerCsvProbas(NOMBRE_ARCHIVO_CSV_MASMENOSUNO, vectorProbabilidadesMasMenosUno, vectorIdsMasMenosUno);
	}


	vector<string> vectorIdsBayes;
	vector<numeroReal> vectorProbabilidadesBayes;

	if ( correrBayes ) {
		Bayes* bayes = new Bayes();
		cout << endl << endl << "------------------------------ BAYES ------------------------------" << endl;
		bayes->realizarPrediccion(bagSinStopWords, parserSinStopWords, vectorIdsBayes, vectorProbabilidadesBayes);
		parserSinStopWords->agregarAlCSV(vectorIdsBayes, vectorProbabilidadesBayes, NOMBRE_ARCHIVO_CSV_BAYES);
		delete bayes;
	}
	if (levantarCsvBayes){
		parserSinStopWords->leerCsvProbas(NOMBRE_ARCHIVO_CSV_BAYES, vectorProbabilidadesBayes, vectorIdsBayes);
	}


	vector<string> vectorIdsTron;
	vector<numeroReal> vectorProbabilidadesTron;

	if ( correrPerceptron ) {
		Perceptron* tron = new Perceptron(bagConStopWords, parserConStopWords, true);
		cout << endl << endl << "------------------------------ PERCEPTRON ------------------------------" << endl;
		tron->entrenar();
		tron->predecir(vectorIdsTron, vectorProbabilidadesTron);
		parserConStopWords->agregarAlCSV(vectorIdsTron, vectorProbabilidadesTron, NOMBRE_ARCHIVO_CSV_TRON);
		delete tron;
	}
	if (levantarCsvPerceptron){
		parserSinStopWords->leerCsvProbas(NOMBRE_ARCHIVO_CSV_TRON, vectorProbabilidadesTron, vectorIdsTron);
	}


	if (ponderar) {
		vector<numeroReal> probabilidadesFinales;
		vector<string> idsFinales;
		const double pesoTron = 0.87;
		const double pesoBayes = 0.0;
		if ( ( vectorProbabilidadesMasMenosUno.size() == vectorIdsBayes.size() ) and ( vectorIdsBayes.size() == vectorIdsTron.size() ) ){
			for (unsigned int i = 0; i < vectorProbabilidadesMasMenosUno.size(); i++){
				numeroReal probabilidadFinal = ( (vectorProbabilidadesMasMenosUno[i] * (1-pesoTron-pesoBayes)) + (vectorProbabilidadesBayes[i] * pesoBayes) + (vectorProbabilidadesTron[i] * pesoTron) );
				probabilidadesFinales.push_back(probabilidadFinal);
				if ( ( vectorIdsMasMenosUno[i] != vectorIdsBayes[i] ) or ( vectorIdsMasMenosUno[i] != vectorIdsTron[i] ) or ( vectorIdsBayes[i] != vectorIdsTron[i] ) ){
					cout << "ERROR: LOS IDS NO COINCIDEN AL PONDERAR." << endl;
				}
				idsFinales.push_back(vectorIdsMasMenosUno[i]);
			}
			parserSinStopWords->agregarAlCSV(idsFinales, probabilidadesFinales, NOMBRE_ARCHIVO_CSV_PONDERADO);
		}
	}


	delete parserSinStopWords;
	delete parserConStopWords;
	return 0;
}
