
#include "masmenosuno.h"

MasMenosUno::MasMenosUno() {

}

MasMenosUno::~MasMenosUno() {

}

void MasMenosUno::realizarPrediccion(BagOfWords* bag, Parser* parser) {
	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_REVIEWS, CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);
	float k = 0.7;
//	ofstream archivoSalida("data/dataout/distintosKFino.txt");
//	int resultadoMayor = 0;

//	for (k = 0.3; k <= 1; k+=0.01){
	int contador = 0; // Cuenta la cantidad que coincidieron
	vector<Review>::iterator iterador = reviewsAPredecir->begin();
	unsigned int i = 0;
//	cout << "K = " << k << endl;
	for ( ; iterador != reviewsAPredecir->end() ; iterador++){
		Review reviewAPredecir = (*iterador);
		if (i == 0) cout << "La primer review a predecir es " << reviewAPredecir.getId() << endl;

		double probabilidadPositiva = predecir(reviewAPredecir, bag, k);
		//TODO: MUCHO OJO CON ESTO QUE EL MENOR/MAYOR O IGUAL CAMBIA BASTANTE LOS RESULTADOS!
		if ( ( (probabilidadPositiva > 0.5) and (reviewAPredecir.getSentiment() == 1) ) or ( (probabilidadPositiva < 0.5) and (reviewAPredecir.getSentiment() == 0) ) ) contador++;

		if ( (i+1) % 1000 == 0 ) cout <<  "Ya se predijeron " << (i+1) << " reviews de " << reviewsAPredecir->size() << endl;
		i++;
		if (i == reviewsAPredecir->size()) cout << "Ultima review a parsear para predecir: " << reviewAPredecir.getId() << endl << endl;
	}

	float porcentaje = (contador * 100.0) / (25000 - CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);

	cout << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
	cout << "Dando un porcentaje de acertar de %" << porcentaje << "." << endl;

//		archivoSalida << "K = " << k << endl;
//		archivoSalida << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
//		archivoSalida << "Dando un porcentaje de acertar de %" << porcentaje << "." << endl;
//		archivoSalida << endl << "--------------------------------------------------------------------------------------------------------------------" << endl;
//		if (resultadoMayor < contador) resultadoMayor = contador;
//	}
//	archivoSalida << endl << endl << "El mejor resultado fueron " << resultadoMayor << " sobre 10000 reviews";
//	archivoSalida.close();
	delete reviewsAPredecir;
}

void MasMenosUno::generarCSV() {

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
	return probabilidad;
}
