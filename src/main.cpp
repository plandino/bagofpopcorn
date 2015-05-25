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
	bool correrMasMenosUno;
	bool correrPerceptron;
	bool correrBayes;
	bool ponderar;
	bool pesarBag;
	bool esPrueba;
	bool cargarBagDesdeArchivoFrecuencias;
	string archivoDeFrecuencias;
} argumentos;

static const char *optString = "pP:bf:mh";

void mostrarAyuda() {
	cout << "Argumentos:" <<  "\n\t" << 	
	"-f <archivo>: Lee las frecuencias de las palabras desde un archivo, en lugar de parsear el archivo de labeledTrainData." << "\n\t" << 
 	"-p --perceptron: Corre el perceptron." << "\n\t" << 
	"-b --bayes: Corre el algoritmo de Bayes." <<" \n\t" <<
	"-m --mas-menos-uno: Corre el algoritmo de Mas menos uno." << "\n\t" << 
	"-P<y|n> --ponderar=<y|n>: Pondera los distintos algoritmos pedidos. Por defecto en y." << "\n\t"
	<< endl;
}

int main(int argc, char* argv[]) {

	Parser* parser = new Parser();
	BagOfWords* bag; /* Necesario, luego se le asigna un valor (sin falta) */
	int opt = 0;


	/*  En caso de no tener ningun argumento, por defecto:
		La Bag se genera con el parseo de reviews y se corre
		Bayes, Perceptron y MasMenosUno, y se pondera. */	
	argumentos.correrBayes = true;
	argumentos.correrPerceptron = true;
	argumentos.correrMasMenosUno = true;
	argumentos.pesarBag = true;		/* Para mas menos uno: */
	argumentos.esPrueba = false; 	/* Indica si estoy utilizando el modo prueba -> Itera cambiando entre funcion polinomica y exponencial
									con distintos pasos, potencias y porcentaje de aceptación 
									(frecPositiva >= k*frecTotal) */
	argumentos.ponderar = true;
	argumentos.cargarBagDesdeArchivoFrecuencias = false;
	argumentos.archivoDeFrecuencias = NOMBRE_ARCHIVO_FRECUENCIAS;

	opt = getopt( argc, argv, optString);
	while ( opt != -1 ) {
		switch (opt) {
			case 'p': /* Perceptron */
				cout << "Corriendo perceptron" << endl;
				argumentos.correrPerceptron = true;
				break;

			case 'P': { /* Ponderar, y | n */
				string stri(optarg);
				if (stri == "n") { cout << "No estamos ponderando" << endl; argumentos.ponderar = false; } 
				else { argumentos.ponderar = true; }
				break; }

			case 'b': /* Bayes */
				cout << "Corriendo bayes" << endl;
				argumentos.correrBayes = true;
				break;

			case 'f': {/* Leer desde archivo de frecuencias */ 
				if (optarg != 0) { string str(optarg); argumentos.archivoDeFrecuencias = str; }
				argumentos.cargarBagDesdeArchivoFrecuencias = true;
				break; }

			case 'm': /* Mas menos uno */
				cout << "Corriendo mas menos uno" << endl;
				argumentos.correrMasMenosUno = true;
				break;

			case '?': /* En caso de caracter invalido */
			case 'h': /* o -h, mostrar ayuda */
				mostrarAyuda();
				break;
			default: /* No se deberia llegar nunca */
				break;
		}

		opt = getopt(argc, argv, optString);
	}	

	if ( argumentos.cargarBagDesdeArchivoFrecuencias ) {
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

	if ( argumentos.correrMasMenosUno ) {
		MasMenosUno* masMenosUno = new MasMenosUno();
		masMenosUno->realizarPrediccion(bag, parser, vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, argumentos.pesarBag, argumentos.esPrueba);
		parser->agregarAlCSV(vectorIdsMasMenosUno, vectorProbabilidadesMasMenosUno, NOMBRE_ARCHIVO_CSV_MASMENOSUNO);
		delete masMenosUno;
	}


	vector<string> vectorIdsBayes;
	vector<numeroReal> vectorProbabilidadesBayes;

	if ( argumentos.correrBayes ) {
		Bayes* bayes = new Bayes();
		bayes->realizarPrediccion(bag, parser, vectorIdsBayes, vectorProbabilidadesBayes);
		parser->agregarAlCSV(vectorIdsBayes, vectorProbabilidadesBayes, NOMBRE_ARCHIVO_CSV_BAYES);
		delete bayes;
	}


	vector<string> vectorIdsTron;
	vector<numeroReal> vectorProbabilidadesTron;

	if ( argumentos.correrPerceptron ) {
		cout << "Empezando Perceptron" << endl;
		Perceptron* tron = new Perceptron(bag, parser, true, false);
		tron->entrenar();
		tron->predecir(vectorIdsTron, vectorProbabilidadesTron);
		parser->agregarAlCSV(vectorIdsTron, vectorProbabilidadesTron, NOMBRE_ARCHIVO_CSV_TRON);
		delete tron;
	}


	if ( argumentos.ponderar ) {
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
