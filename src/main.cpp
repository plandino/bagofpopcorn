#include "masmenosuno.h"
#include "bayes.h"
#include "perceptron.h"
#include <unistd.h>


/* Lista de argumentos posibles:
	-f <archivo>: Lee las frecuencias de las palabras desde un archivo, en lugar de parsear el archivo de labeledTrainData.
	-p --perceptron: Corre el perceptron.
	-b --bayes: Corre el algoritmo de Bayes.
	-m --mas-menos-uno: Corre el algoritmo de Mas menos uno.
	-P<y|n> --ponderar=<y|n>: Pondera los distintos algoritmos pedidos. Por defecto en y.
*/
struct argumentos_t {
	bool pesarBag;
	bool esPrueba;
	string archivoDeFrecuencias;
} argumentos;

static const char *optString = "pP:bf:mh";


void correrBayes(BagOfWords* bag, Parser* parser, vector<string>& vectorIdsBayes, vector<numeroReal>& vectorProbabilidadesBayes) {
	Bayes* bayes = new Bayes();
	bayes->realizarPrediccion(bag, parser, vectorIdsBayes, vectorProbabilidadesBayes);
	parser->agregarAlCSV(vectorIdsBayes, vectorProbabilidadesBayes, NOMBRE_ARCHIVO_CSV_BAYES);
	delete bayes;
}

void correrMasMenosUno(BagOfWords* bag, Parser* parser, vector<string>& vectorIdsMasMenosUno, vector<numeroReal>& vectorProbabilidadesMasMenosUno) {
	MasMenosUno* masMenosUno = new MasMenosUno();	
	masMenosUno->realizarPrediccion(bag, parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, argumentos.pesarBag, argumentos.esPrueba);
	parser->agregarAlCSV(vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, NOMBRE_ARCHIVO_CSV_MASMENOSUNO);
	delete masMenosUno;
}

void correrPerceptron(BagOfWords* bag, Parser* parser, vector<string>& vectorIdsTron, vector<numeroReal>& vectorProbabilidadesTron) {
	cout << "Empezando Perceptron" << endl;
	Perceptron* tron = new Perceptron(bag, parser, true, false); /* Bools son usaBag y usaBigramas */
	tron->entrenar();
	tron->predecir(vectorIdsTron, vectorProbabilidadesTron);
	parser->agregarAlCSV(vectorIdsTron, vectorProbabilidadesTron, NOMBRE_ARCHIVO_CSV_TRON);
	delete tron;
}

BagOfWords* cargarBagDesdeArchivoFrecuencias(Parser* parser) {
	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV(argumentos.archivoDeFrecuencias);
	return bag;
}

void ponderar(Parser* parser,
	vector<string>& vectorIdsMasMenosUno, vector<numeroReal>& vectorProbabilidadesMasMenosUno,
	vector<string>& vectorIdsBayes, vector<numeroReal>& vectorProbabilidadesBayes,
	vector<string>& vectorIdsTron, vector<numeroReal>& vectorProbabilidadesTron) {

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

void mostrarAyuda() {
	cout << "Argumentos:" <<  "\n" << 	
	"-f <archivo>: Lee las frecuencias de las palabras desde un archivo, en lugar de parsear el archivo de labeledTrainData." << "\n" << 
 	"-p --perceptron: Corre el perceptron." << "\n" << 
	"-b --bayes: Corre el algoritmo de Bayes." <<" \n" <<
	"-m --mas-menos-uno: Corre el algoritmo de Mas menos uno." << "\n" << 
	"-P<y|n> --ponderar=<y|n>: Pondera los distintos algoritmos pedidos. Por defecto en y." << "\n"
	<< endl;
}

int main(int argc, char* argv[]) {

	Parser* parser = new Parser();
	BagOfWords* bag;
	int opt = 0;

	argumentos.pesarBag = true;
	argumentos.esPrueba = false;
	argumentos.archivoDeFrecuencias = NOMBRE_ARCHIVO_FRECUENCIAS;

	vector<string> vectorIdsMasMenosUno;
	vector<numeroReal> vectorProbabilidadesMasMenosUno;
	vector<string> vectorIdsBayes;
	vector<numeroReal> vectorProbabilidadesBayes;
	vector<string> vectorIdsTron;
	vector<numeroReal> vectorProbabilidadesTron;


	opt = getopt( argc, argv, optString);
	while ( opt != -1 ) {
		switch (opt) {
			case 'p': /* Perceptron */
				correrPerceptron(bag, parser, vectorIdsTron, vectorProbabilidadesTron);
				break;

			case 'P': /* Ponderar, y | n */
				if (optarg == "n") { cout << "No estamos ponderando" << endl; } 
				else { ponderar(parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno,
								vectorIdsBayes, vectorProbabilidadesBayes,
								vectorIdsTron, vectorProbabilidadesTron); }
				break;

			case 'b': 
				correrBayes(bag, parser, vectorIdsBayes, vectorProbabilidadesBayes);
				break;

			case 'f':
				if (optarg != 0) { argumentos.archivoDeFrecuencias = optarg; }
				bag = cargarBagDesdeArchivoFrecuencias(parser);
				break;

			case 'm':
				correrMasMenosUno(bag, parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno);
				break;

			case '?':
			case '-h':
				mostrarAyuda();
				break;
			default: /* No se deberia llegar nunca */
				break;
		}
	}

	/*  En caso de no tener ningun argumento, por defecto:
		La Bag se genera con el parseo de reviews y se corre
		Bayes, Perceptron y MasMenosUno, y se pondera. */

	if (argc == 1) {
		parser = new Parser(); 

		bag = parser->parsearReviews(NOMBRE_ARCHIVO_LABELED_REVIEWS);
		parser->generarTSV();

		correrBayes(bag, parser, vectorIdsBayes, vectorProbabilidadesBayes);
		correrPerceptron(bag, parser, vectorIdsTron, vectorProbabilidadesTron);
		correrMasMenosUno(bag, parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno);

		ponderar(parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno,
			vectorIdsBayes, vectorProbabilidadesBayes,
			vectorIdsTron, vectorProbabilidadesTron);

		delete parser;
	}
	return 0;
}

// //	Con esto parseo con el nuevo parser todas las reviews y genero el TSV
// 	BagOfWords* bag = parser->parsearReviews(NOMBRE_ARCHIVO_LABELED_REVIEWS);
// 	parser->generarTSV();

// //	Con esto leo frecuencias desde el TSV generado por el parser de C++
// //	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV(argumentos.NOMBRE_ARCHIVO_FRECUENCIAS);



// //	Con esto genero el TSV desde el CSV de salida de python -> Solo sirve si se usan 15000 para entrenar y 10000 para predecir
// //	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeCSVPython("data/dataout/Bag_of_Words_model.csv");
// //	parser->generarTSV();

// //	Con esto leo frecuencias desde el TSV generado a partir del CSV de python -> Solo sirve si se usan 15000 para entrenar y 10000 para predecir
// //	BagOfWords* bag = parser->leerPalabrasYFrecuenciasDesdeTSV("data/dataout/frecuencias_python.tsv");



// //	Para facilitar el activar o desactivar de correr uno y/u otro algoritmo
// 	bool correrMasMenosUno = true;
// 	bool correrBayes = true;
// 	bool correrPerceptron = false;
// 	bool ponderar = true;

// //	PARA MASMENOSUNO:
// 	bool pesarBag = true; 		// Indica si quiero pesar o no la bag
// 	bool esPrueba = false;		// Indica si estoy utilizando el modo prueba -> Itera cambiando entre funcion polinomica y exponencial
// 								// con distintos pasos, potencias y porcentaje de aceptación (frecPositiva >= k*frecTotal)

// 	// WARNING: Si esPrueba = TRUE, poner la constante de CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en 15000
// 	// WARNING: Si esPrueba = FALSE, poner la constante de CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en 25000
// 	// WARNING:	Utilizar la funcion de parser->leerPalabrasYFrecuenciasDesdeTSV unicamente si antes se corrio el parser->parsearReviews con la
// 	// 			constante CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO en el estado deseado.
// 	// WARNING:	Si esPrueba = TRUE y pesarBag = FALSE, solo tiene sentido variar el k.
// 	if ( correrMasMenosUno ) {
// 		MasMenosUno* masMenosUno = new MasMenosUno();
// 		masMenosUno->realizarPrediccion(bag, parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, pesarBag, esPrueba);
// 		parser->agregarAlCSV(vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, NOMBRE_ARCHIVO_CSV_MASMENOSUNO);
// 		delete masMenosUno;
// 	}


// 	if ( correrBayes ) {
// 		Bayes* bayes = new Bayes();
// 		bayes->realizarPrediccion(bag, parser, vectorIdsBayes, vectorProbabilidadesBayes);
// 		parser->agregarAlCSV(vectorIdsBayes, vectorProbabilidadesBayes, NOMBRE_ARCHIVO_CSV_BAYES);
// 		delete bayes;
// 	}

// 	if ( correrPerceptron ) {
// 		cout << "Empezando Perceptron" << endl;
// 		Perceptron* tron = new Perceptron(bag, parser, true, false);
// 		tron->entrenar();
// 		tron->predecir(vectorIdsTron, vectorProbabilidadesTron);
// 		parser->agregarAlCSV(vectorIdsTron, vectorProbabilidadesTron, NOMBRE_ARCHIVO_CSV_TRON);
// 		delete tron;
// 	}



// 	if (ponderar) {
// 		vector<numeroReal> probabilidadesFinales;
// 		vector<string> idsFinales;
// 		const double pesoBayes = 0.9;
// 		if ( vectorProbabilidadesMasMenosUno.size() == vectorIdsBayes.size() ){
// 			for (unsigned int i = 0; i < vectorProbabilidadesMasMenosUno.size(); i++){
// 				numeroReal probabilidadFinal = ( (vectorProbabilidadesMasMenosUno[i] * (1-pesoBayes)) + (vectorProbabilidadesBayes[i] * pesoBayes) );
// 				probabilidadesFinales.push_back(probabilidadFinal);
// 				if ( vectorIdsMasMenosUno[i] != vectorIdsBayes[i] ){
// 					cout << "TODO MAL GUACHIN" << endl;
// 					exit(1);
// 				}
// 				idsFinales.push_back(vectorIdsMasMenosUno[i]);
// 			}
// 			parser->agregarAlCSV(idsFinales, probabilidadesFinales, NOMBRE_ARCHIVO_CSV_PONDERADO);
// 		}
// 	}


// 	delete parser;
// 	return 0;
// }
