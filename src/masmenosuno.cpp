
#include "masmenosuno.h"

MasMenosUno::MasMenosUno() {

}

MasMenosUno::~MasMenosUno() {

}

void MasMenosUno::realizarPrediccion(BagOfWords* bag, Parser* parser) {
	int contador = 0; // Cuenta la cantidad que coincidieron
	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_REVIEWS, CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);

	vector<Review>::iterator iterador = reviewsAPredecir->begin();
	unsigned int i = 0;
	for ( ; iterador != reviewsAPredecir->end() ; iterador++){
		Review reviewAPredecir = (*iterador);
		if (i == 0) cout << "La primer review a predecir es " << reviewAPredecir.getId() << endl;
		if ( predecir(reviewAPredecir, bag) ) contador++;
		if ( (i+1) % 1000 == 0 ) cout <<  "Ya se predijeron " << (i+1) << " reviews de " << reviewsAPredecir->size() << endl;
		i++;
		if (i == reviewsAPredecir->size()) cout << "Ultima review a parsear para predecir: " << reviewAPredecir.getId() << endl << endl;
	}

	float porcentaje = (contador * 100.0) / (25000 - CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);

	cout << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
	cout << "Dando un porcentaje de acertar de %" << porcentaje << "." << endl;

	delete reviewsAPredecir;
}

void MasMenosUno::generarCSV() {

}

bool MasMenosUno::predecir(Review& review, BagOfWords* bag) {
	int puntuacion = 0;
	vector<string> palabras = review.getPalabras();

	vector<string>::iterator iterador = palabras.begin();
	for ( ; iterador != palabras.end() ; iterador++){
		string palabra = (*iterador);
		if ( bag->estaEnBag(palabra) ){
			int frecPos = ( bag->getFrecuencias(1) )->at( bag->posicionEnBag(palabra) );
			int frecNeg = ( bag->getFrecuencias(0) )->at( bag->posicionEnBag(palabra) );
			int frecTotal = frecPos + frecNeg;

			int porcentaje = frecTotal*0.7;
//			cout << "La palabra " << palabra << " esta en la bag, con frecPos: " << frecPos << "| frecNeg: " << frecNeg << ", sobre un porcentaje de " << porcentaje << endl;

			if ( frecPos > porcentaje ) puntuacion++;
			else if ( frecNeg > porcentaje ) puntuacion--;
		}
	}

	//TODO: MUCHO OJO CON ESTO QUE EL MENOR/MAYOR O IGUAL CAMBIA BASTANTE LOS RESULTADOS!
	if ( ( (puntuacion >= 0) and (review.getSentiment() == 1) ) or ( (puntuacion <= 0) and (review.getSentiment() == 0) ) ) return true;
	else return false;
}
