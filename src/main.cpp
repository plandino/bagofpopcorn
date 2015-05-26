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

static const char *optString = "pP:bf:mh";

void mostrarAyuda() {
	cout << endl << "Argumentos:" <<  "\n\t" << 	
	"-f <archivo>: Lee las frecuencias de las palabras desde un archivo, en lugar de parsear el archivo de labeledTrainData." << "\n\t" << 
 	"-p --perceptron: Corre el perceptron." << "\n\t" << 
	"-b --bayes: Corre el algoritmo de Bayes." <<" \n\t" <<
	"-m --mas-menos-uno: Corre el algoritmo de Mas menos uno." << "\n\t" << 
	"-P<y|n> --ponderar=<y|n>: Pondera los distintos algoritmos pedidos. Por defecto en y." << "\n"
	<< endl;
}

int main(int argc, char* argv[]) {

	Parser* parser = new Parser();
	BagOfWords* bag; /* Necesario, luego se le asigna un valor (sin falta) */
	int opt = 0;

	bool correrBayes = false;
	bool correrPerceptron = false;
	bool correrMasMenosUno = false;
	bool pesarBag = true;		/* Para mas menos uno: */
	bool esPrueba = false; 	/* Indica si estoy utilizando el modo prueba -> Itera cambiando entre funcion polinomica y exponencial
									con distintos pasos, potencias y porcentaje de aceptación 
									(frecPositiva >= k*frecTotal) */
	bool ponderar = false;
	bool cargarBagDesdeArchivoFrecuencias = false;
	string archivoDeFrecuencias = NOMBRE_ARCHIVO_FRECUENCIAS;

	opt = getopt( argc, argv, optString);
	while ( opt != -1 ) {
		switch (opt) {
			case 'p': /* Perceptron */
				cout << "Corriendo perceptron" << endl;
				correrPerceptron = true;
				break;

			case 'P': { /* Ponderar, y | n */
				string stri(optarg);
				if (stri == "n") { cout << "No estamos ponderando" << endl; ponderar = false; } 
				else { ponderar = true; }
				break; }

			case 'b': /* Bayes */
				cout << "Corriendo bayes" << endl;
				correrBayes = true;
				break;

			case 'f': {/* Leer desde archivo de frecuencias */ 
				cout << "Leyendo frecuencias desde TSV" << endl;
				if (optarg != 0) { string str(optarg); archivoDeFrecuencias = str; }
				cargarBagDesdeArchivoFrecuencias = true;
				break; }

			case 'm': /* Mas menos uno */
				cout << "Corriendo mas menos uno" << endl;
				correrMasMenosUno = true;
				break;

			case '?': /* En caso de caracter invalido */
			case 'h': /* o -h, mostrar ayuda */
				mostrarAyuda();
				exit(1);
				break;
			default: /* No se deberia llegar nunca */
				break;
		}

		opt = getopt(argc, argv, optString);
	}	

	/*  En caso de no tener ningun argumento, por defecto:
	La Bag se genera con el parseo de reviews y se corre
	Bayes, Perceptron y MasMenosUno, y se pondera. */
	if (argc == 1) { 
		cout << "Sin argumentos" << endl;
		correrBayes = true;
		correrPerceptron = true;
		correrMasMenosUno = true;
		pesarBag = true;		
		esPrueba = false;
		ponderar = true;
		cargarBagDesdeArchivoFrecuencias = false;
	}

	if ( cargarBagDesdeArchivoFrecuencias ) {
		bag = parser->leerPalabrasYFrecuenciasDesdeTSV(NOMBRE_ARCHIVO_FRECUENCIAS);
	} else { 
		bag = parser->parsearReviews(NOMBRE_ARCHIVO_LABELED_REVIEWS);
		/*	Con esto genero el TSV desde el CSV de salida de python -> Solo sirve si se usan 15000 para entrenar y 10000 para predecir */
		//	bag = parser->leerPalabrasYFrecuenciasDesdeCSVPython("data/dataout/Bag_of_Words_model.csv");
		/*	Con esto leo frecuencias desde el TSV generado a partir del CSV de python -> Solo sirve si se usan 15000 para entrenar y 10000 para predecir */
		//	bag = parser->leerPalabrasYFrecuenciasDesdeTSV("data/dataout/frecuencias_python.tsv");
	}

	parser->generarTSV();

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
		Perceptron* tron = new Perceptron(bag, parser, false, true);
		tron->entrenar();
		tron->predecir(vectorIdsTron, vectorProbabilidadesTron);
		parser->agregarAlCSV(vectorIdsTron, vectorProbabilidadesTron, NOMBRE_ARCHIVO_CSV_TRON);
		delete tron;
	}


	if ( ponderar ) {
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
