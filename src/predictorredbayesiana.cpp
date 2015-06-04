#include "predictorredbayesiana.h"

PredictorRedBayesiana::PredictorRedBayesiana(Network* redPositiva, Network* redNegativa, Parser * parser) {
	this->redPositva 	= redPositva;
	this->redNegativa	= redNegativa;
	this->parser 		= parser;
}

PredictorRedBayesiana::~PredictorRedBayesiana() {
}

void PredictorRedBayesiana::realizarPrediccion() {
	// Reviews a predecir
	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO, true);
//	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_TEST_DATA, 0, false);

	// Objetos para crear el CSV
	numeroReal probabilidadPositiva;
	vector<string> vectorIds;
	vector<numeroReal> vectorProbabilidades;

	// Contadores
	unsigned int i = 0;
	int contador = 0; // Cuenta la cantidad que coincidieron

	// Empiezo a iterar por las reviews
	vector<Review>::iterator iteradorReviews = reviewsAPredecir->begin();

	for ( ; iteradorReviews != reviewsAPredecir->end() ; iteradorReviews++){
		Review reviewAPredecir = (*iteradorReviews);

		if (i == 0) cout << "La primer review a predecir es " << reviewAPredecir.getId() << endl;

		// Si predigo bien sumo uno al contador
		if ( predecir(reviewAPredecir, &probabilidadPositiva) ) contador++;

//		vectorIds.push_back(reviewAPredecir.getId());
//		vectorProbabilidades.push_back(probabilidadPositiva);

		if ( (i+1) % 1000 == 0 ) cout <<  "Ya se predijeron " << (i+1) << " reviews de " << reviewsAPredecir->size() << endl;
		i++;
		if (i == reviewsAPredecir->size()) cout << "Ultima review a parsear para predecir: " << reviewAPredecir.getId() << endl << endl;
	}

//	float porcentaje = (contador * 100.0) / (25000 - CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);

	cout << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
//	parser->agregarAlCSV(vectorIds, vectorProbabilidades);
	delete reviewsAPredecir;
}

bool PredictorRedBayesiana::predecir(Review& review, numeroReal * probabilidadPostiva) {

	// Declaro probabilidades inciales
	numeroReal acumuladorProbaPositiva = 1.0;
	numeroReal acumuladorProbaNegativa = 1.0;

	numeroReal probabilidad;

	// Obtengo las palabras de la review
	vector<string> vectorPalabras = review.getPalabras();

	// Empiezo a iterar por las palabras
	vector<string>::iterator iteradorPalabras = vectorPalabras.begin();
	if(vectorPalabras.size() >= 2){
		string palabraAnterior = (*iteradorPalabras);
		iteradorPalabras++;
	}

	if(review.getSentiment()){
		for ( ; iteradorPalabras != vectorPalabras.end() ; iteradorPalabras++){
			string palabra = (*iteradorPalabras);
			Nodo* nodo = redPositva->hayNodoConPalabra(palabra);
			list<Nodo* >* listaDeNodosQueMeApuntan = nodo->getNodosQueMeApuntan();
			probabilidad = 1.0 / listaDeNodosQueMeApuntan->size();

		}
	}


//		if ( bag->estaEnBag(palabra) ){
//			numeroReal probaPositiva = bag->getProbabilidadesPositivasTradicionales() -> at( bag->posicionEnBag(palabra));
//			numeroReal probaNegativa = bag->getProbabilidadesNegativasTradicionales() -> at( bag->posicionEnBag(palabra));
//
//			if( (probaPositiva == 0) or (probaNegativa == 0)) cout << "QUILOMBOOOO" << endl;
//			acumuladorProbaNegativa = acumuladorProbaNegativa * probaNegativa;
//			acumuladorProbaPositiva = acumuladorProbaPositiva * probaPositiva;
//		}
//	}
//	//acumuladorProbaPositiva = acumuladorProbaPositiva;
//	//acumuladorProbaNegativa = acumuladorProbaNegativa;
//
//	numeroReal probaReviewPositiva = acumuladorProbaPositiva / (acumuladorProbaPositiva + acumuladorProbaNegativa); //acumuladorProbaPositiva / (acumuladorProbaPositiva + acumuladorProbaNegativa);
//	numeroReal probaReviewNegativa = acumuladorProbaNegativa / (acumuladorProbaPositiva + acumuladorProbaNegativa); //acumuladorProbaNegativa / (acumuladorProbaPositiva + acumuladorProbaNegativa);
////	float v1 = rand() % 100;
////	if(v1 < 30)
////	{
////		cout << "Proba review Positiva" << probaReviewPositiva << endl;
////		cout << "Proba review Negativas" << probaReviewNegativa << endl;
////	}
//
//	*probabilidadPositiva = probaReviewPositiva;
//	//TODO: MUCHO OJO CON ESTO QUE EL MENOR/MAYOR O IGUAL CAMBIA BASTANTE LOS RESULTADOS!
//	if ( ( (probaReviewPositiva >= probaReviewNegativa) and (review.getSentiment() == 1) ) or ( (probaReviewPositiva < probaReviewNegativa) and (review.getSentiment() == 0) ) ) return true;
//	else return false;



	return true;
}
