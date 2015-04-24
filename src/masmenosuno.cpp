
#include "masmenosuno.h"

MasMenosUno::MasMenosUno() {

}

MasMenosUno::~MasMenosUno() {

}

void MasMenosUno::realizarPrediccion(BagOfWords* bag, Parser* parser, bool pesar) {
//	ofstream archivoSalida("data/dataout/distintosPasoPotencia2.txt");
//	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO, true);
	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_TEST_DATA, 0, false);
	double paso = 0.001;
	double potencia = 10.0;
	double pasoMayor = 0, potenciaMayor = 0;
	int resultadoMayor = 0;
	vector<string> vectorIds;
	vector<numeroReal> vectorProbabilidades;

//	for (int j = 0; j < 2; j++){
//		if (j==0) archivoSalida << "-----------------------------------------------------------------POLINOMICA-----------------------------------------------------------------" << endl;
//		if (j==1) archivoSalida << endl << endl <<"-----------------------------------------------------------------EXPONENCIAL-----------------------------------------------------------------" << endl;
//		for (paso = 0.004; paso < 0.008; paso += 0.001){
//			for (potencia = 8.0; potencia <= 12; potencia += 1.0 ){
//				if (pesar and j==0) bag->pesarBag(paso,potencia,false);
//				if (pesar and j==1) bag->pesarBag(paso,potencia,true);
				cout << "Pesando bag..." << endl;
				bag->pesarBag(paso,potencia,false);
				cout << "Pesaje terminado" << endl;
				float k = 0.7;

	//			for (k = 0.3; k <= 1; k+=0.01){
					int contador = 0; // Cuenta la cantidad que coincidieron
					vector<Review>::iterator iterador = reviewsAPredecir->begin();
					unsigned int i = 0;
					for ( ; iterador != reviewsAPredecir->end() ; iterador++){
						Review reviewAPredecir = (*iterador);
						double probabilidadPositiva = predecir(reviewAPredecir, bag, k);
						vectorIds.push_back(reviewAPredecir.getId());
						vectorProbabilidades.push_back(probabilidadPositiva);

						//TODO: MUCHO OJO CON ESTO QUE EL MENOR/MAYOR O IGUAL CAMBIA BASTANTE LOS RESULTADOS!
//						if ( ( (probabilidadPositiva > 0.5) and (reviewAPredecir.getSentiment() == 1) ) or ( (probabilidadPositiva < 0.5) and (reviewAPredecir.getSentiment() == 0) ) ) contador++;

						if (i == 0) cout << "La primer review a predecir es " << reviewAPredecir.getId() << endl;
						if ( (i+1) % 1000 == 0 ) cout <<  "Ya se predijeron " << (i+1) << " reviews de " << reviewsAPredecir->size() << endl;
						i++;
						if (i == reviewsAPredecir->size()) cout << "Ultima review a parsear para predecir: " << reviewAPredecir.getId() << endl << endl;
					}

//					float porcentaje = (contador * 100.0) / (25000 - CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);

	//				cout << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
	//				cout << "MaseMenosUno: Dando un porcentaje de acertar de %" << porcentaje << "." << endl << endl;

//					cout << "PASO: " << paso << " | POTENCIA: " << potencia << endl;
//					archivoSalida << "PASO: " << paso << " | POTENCIA: " << potencia << endl;
//					archivoSalida << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
//					archivoSalida << "Dando un porcentaje de acertar de %" << porcentaje << "." << endl;
//					archivoSalida << endl << "--------------------------------------------------------------------------------------------------------------------" << endl;
//					if (resultadoMayor < contador) {
//						resultadoMayor = contador;
//						pasoMayor = paso;
//						potenciaMayor = potencia;
//					}
	//			}
//			}
//		}
//		archivoSalida << endl << endl << "El mejor resultado fue " << resultadoMayor << " sobre 10000 reviews y se dio con paso=" << pasoMayor << " y potencia=" << potenciaMayor;
//	}
//	archivoSalida.close();
	parser->agregarAlCSV(vectorIds, vectorProbabilidades);
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
