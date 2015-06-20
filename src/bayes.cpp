#include "bayes.h"

Bayes::Bayes(){

}

Bayes::~Bayes(){

}

void Bayes::realizarPrediccion(BagOfWords* bag, Parser* parser, vector<string>& vectorIds, vector<numeroReal>& vectorProbabilidades) {
//	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO, true);
	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_TEST_DATA, 0, false);

	int contador = 0; // Cuenta la cantidad que coincidieron
	numeroReal probabilidadPositiva;
	vector<Review>::iterator iterador = reviewsAPredecir->begin();
	unsigned int i = 0;
	for ( ; iterador != reviewsAPredecir->end() ; iterador++){
		Review reviewAPredecir = (*iterador);
		if ( this->predecir(reviewAPredecir, bag, probabilidadPositiva) ) contador++;

		vectorIds.push_back(reviewAPredecir.getId());
		vectorProbabilidades.push_back(probabilidadPositiva);

		if ( (i+1) % 1000 == 0 ) cout <<  "Ya se predijeron " << (i+1) << " reviews de " << reviewsAPredecir->size() << " para Bayes." << endl;
		i++;
	}
//	float porcentaje = (contador * 100.0) / (25000 - CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);

	delete reviewsAPredecir;
}

bool Bayes::predecir(Review& review, BagOfWords* bag, numeroReal& probabilidadPositiva) {
	numeroReal acumuladorProbaPositiva = 0.0;
	numeroReal acumuladorProbaNegativa = 0.0;
	vector<string> palabras = review.getPalabras();

	vector<string>::iterator iterador = palabras.begin();
	for ( ; iterador != palabras.end() ; iterador++){
		string palabra = (*iterador);
		if ( bag->estaEnBag(palabra) ){
			numeroReal probaPositiva = bag->getProbabilidadesPositivasTradicionales() -> at( bag->posicionEnBag(palabra));
			numeroReal probaNegativa = bag->getProbabilidadesNegativasTradicionales() -> at( bag->posicionEnBag(palabra));

			if( (probaPositiva == 0) or (probaNegativa == 0)) cout << "ERROR AL PREDECIR EN BAYES: PROBABILIDADES=0" << endl;
			acumuladorProbaNegativa = acumuladorProbaNegativa + probaNegativa;
			acumuladorProbaPositiva = acumuladorProbaPositiva + probaPositiva;
		}
	}
	acumuladorProbaPositiva = exp(acumuladorProbaPositiva);
	acumuladorProbaNegativa = exp(acumuladorProbaNegativa);
	numeroReal probaReviewPositiva;
	numeroReal probaReviewNegativa;

	if (acumuladorProbaPositiva != 0 or acumuladorProbaNegativa != 0) {
		probaReviewPositiva = acumuladorProbaPositiva / (acumuladorProbaPositiva + acumuladorProbaNegativa); //acumuladorProbaPositiva / (acumuladorProbaPositiva + acumuladorProbaNegativa);
		probaReviewNegativa = acumuladorProbaNegativa / (acumuladorProbaPositiva + acumuladorProbaNegativa); //acumuladorProbaNegativa / (acumuladorProbaPositiva + acumuladorProbaNegativa);
	} else {
		probaReviewPositiva = 0.5;
		probaReviewNegativa = 0.5;
	}

	probabilidadPositiva = probaReviewPositiva;
	if ( ( (probaReviewPositiva >= probaReviewNegativa) and (review.getSentiment() == 1) ) or ( (probaReviewPositiva < probaReviewNegativa) and (review.getSentiment() == 0) ) ) return true;
	else return false;
}
