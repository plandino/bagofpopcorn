
#include "masmenosuno.h"

MasMenosUno::MasMenosUno() {

}

MasMenosUno::~MasMenosUno() {

}

int MasMenosUno::iterarPorReviews(float k, vector<Review>* reviewsAPredecir, BagOfWords* bag, vector<string>& vectorIds, vector<numeroReal>& vectorProbabilidades, bool contar) {
	vector<Review>::iterator iterador = reviewsAPredecir->begin();
	int contador = 0;
	unsigned int i = 0;

	for (; iterador != reviewsAPredecir->end(); iterador++) {
		Review reviewAPredecir = (*iterador);
		double probabilidadPositiva = predecir(reviewAPredecir, bag, k);
		vectorIds.push_back(reviewAPredecir.getId());
		vectorProbabilidades.push_back(probabilidadPositiva);
		if ( contar and ( ( (probabilidadPositiva > 0.5) and (reviewAPredecir.getSentiment() == 1) )
						or ( (probabilidadPositiva < 0.5) and (reviewAPredecir.getSentiment() == 0) ) ) ) contador++;
		if ((i + 1) % 5000 == 0) cout << "Ya se predijeron " << (i + 1) << " reviews de " << reviewsAPredecir->size() << " para Mas Menos Uno."<< endl;
		i++;
	}
	return contador;
}

void MasMenosUno::probar(BagOfWords* bag, Parser* parser, bool pesar) {
	ofstream archivoSalida("data/dataout/distintosPasoPotenciaK.txt");
	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO, true);
	vector<string> vectorIds;
	vector<numeroReal> vectorProbabilidades;
	int resultadoMayor = 0;
	double potenciaMayor, pasoMayor, kMayor;
	double paso = 0.001, potencia = 10.0, k = 0.7;

	for (int j = 0; j < 2; j++){
		if (j==0) archivoSalida << "-----------------------------------------------------------------POLINOMICA-----------------------------------------------------------------" << endl;
		if (j==1) archivoSalida << endl << endl <<"-----------------------------------------------------------------EXPONENCIAL-----------------------------------------------------------------" << endl;
		for (paso = 0.001; paso <= 0.021; paso += 0.001){
			for (potencia = 7.0; potencia <= 13; potencia += 1.0 ){
				if (pesar and j==0) bag->pesarBag(paso,potencia,false);
				if (pesar and j==1) bag->pesarBag(paso,potencia,true);

				for (k = 0.5; k <= 0.81; k+=0.01){
					// Cuenta la cantidad que coincidieron
					int contador = iterarPorReviews(k, reviewsAPredecir, bag, vectorIds, vectorProbabilidades, true);
					float porcentaje = (contador * 100.0) / (25000 - CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);

//					cout << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
//					cout << "MasMenosUno: Dando un porcentaje de acertar de %" << porcentaje << "." << endl << endl;

					cout << "PASO: " << paso << " | POTENCIA: " << potencia << " | K: " << k <<endl;
					archivoSalida << "PASO: " << paso << " | POTENCIA: " << potencia << " | K: " << k <<endl;
					archivoSalida << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
					archivoSalida << "Dando un porcentaje de acertar de %" << porcentaje << "." << endl;
					archivoSalida << endl << "--------------------------------------------------------------------------------------------------------------------" << endl;
					if (resultadoMayor < contador) {
						resultadoMayor = contador;
						pasoMayor = paso;
						potenciaMayor = potencia;
						kMayor = k;
					}
				}
			}
		}
		archivoSalida << endl << endl << "El mejor resultado fue " << resultadoMayor << " sobre " << (25000 - CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO) << " reviews y se dio con paso=" << pasoMayor << ", potencia=" << potenciaMayor << " k=" << kMayor;
	}
	archivoSalida.close();
	delete reviewsAPredecir;
}

double MasMenosUno::predecir(Review& review, BagOfWords* bag, float k) {
	int puntuacionPos = 0;
	int puntuacionNeg = 0;
	vector<string> palabras = review.getPalabras();

	vector<string>::iterator iterador = palabras.begin();
	for ( ; iterador != palabras.end() ; iterador++){
		string palabra = (*iterador);
		if ( bag->estaEnBag(palabra) ){
			int frecPos = ( bag->getFrecuencias(1) )->at( bag->posicionEnBag(palabra) );
			int frecNeg = ( bag->getFrecuencias(0) )->at( bag->posicionEnBag(palabra) );
			int frecTotal = frecPos + frecNeg;

			int porcentaje = frecTotal*k;
//			cout << "La palabra " << palabra << " esta en la bag, con frecPos: " << frecPos << "| frecNeg: " << frecNeg << ", sobre un porcentaje de " << porcentaje << endl;
			if ( frecPos > porcentaje ) puntuacionPos += ( ( bag->getPesosPositivos() )->at( bag->posicionEnBag(palabra) ) );
			else if ( frecNeg > porcentaje ) puntuacionNeg += ( ( bag->getPesosNegativos() )->at( bag->posicionEnBag(palabra) ) );
		}
	}
	double probabilidad = ((puntuacionPos*1.0)/(puntuacionPos + puntuacionNeg));
	if (puntuacionPos == 0 and puntuacionNeg == 0) probabilidad = 0.5; //SI SE DIVIDE POR 0
	return probabilidad;
}

void MasMenosUno::realizarPrediccion(BagOfWords* bag, Parser* parser, vector<string>& vectorIds, vector<numeroReal>& vectorProbabilidades, bool pesar, bool esPrueba) {
	if (esPrueba) probar(bag, parser, pesar);
	else {
		vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_TEST_DATA, 0, false);
		double paso = 0.001;
		double potencia = 10.0;
		double k = 0.7;

		if (pesar){
			cout << "Pesando bag..." << endl << endl;
			bag->pesarBag(paso,potencia,false);
			cout << "\nPesaje terminado." << endl << endl;
		}
		iterarPorReviews(k, reviewsAPredecir, bag, vectorIds, vectorProbabilidades, false);

		delete reviewsAPredecir;
	}
}
