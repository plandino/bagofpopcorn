#include "bayes.h"

Bayes::Bayes(){

}

Bayes::~Bayes(){

}

void Bayes::realizarPrediccion(BagOfWords* bag, Parser* parser) {
	vector< Review >* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_REVIEWS, CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);
	float k = 0.7;
//	ofstream archivoSalida("data/dataout/distintosKFino.txt");
//	int resultadoMayor = 0;

//	for (k = 0.3; k <= 1; k+=0.01){
	int contador = 0; // Cuenta la cantidad que coincidieron
	numeroReal probabilidadPositiva;
	string vectorIds[CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO];
	numeroReal vectorProbabilidades[CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO];
	vector<Review>::iterator iterador = reviewsAPredecir->begin();
	unsigned int i = 0;
//	cout << "K = " << k << endl;
	for ( ; iterador != reviewsAPredecir->end() ; iterador++){
		Review reviewAPredecir = (*iterador);
		if (i == 0) cout << "La primer review a predecir es " << reviewAPredecir.getId() << endl;
		if ( predecir(reviewAPredecir, bag, k, &probabilidadPositiva) ) contador++;

		vectorIds[i] = reviewAPredecir.getId();
		vectorProbabilidades[i] = probabilidadPositiva;

		if ( (i+1) % 1000 == 0 ) cout <<  "Ya se predijeron " << (i+1) << " reviews de " << reviewsAPredecir->size() << endl;
		i++;
		if (i == reviewsAPredecir->size()) cout << "Ultima review a parsear para predecir: " << reviewAPredecir.getId() << endl << endl;
	}

	float porcentaje = (contador * 100.0) / (25000 - CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO);

	cout << "Se predijeron correctamente " << contador << " reviews de un total de " << 25000-CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO << "." << endl;
	cout << "Bayes: Dando un porcentaje de acertar de %" << porcentaje << "." << endl;
	parser->agregarAlCSV(vectorIds, vectorProbabilidades);
	delete reviewsAPredecir;
}

bool Bayes::predecir(Review& review, BagOfWords* bag, float k, numeroReal *probabilidadPositiva) {
	int puntuacion = 0;
	numeroReal acumuladorProbaPositiva = 0;
	numeroReal acumuladorProbaNegativa = 0;
	vector<string> palabras = review.getPalabras();

	bool inicio = true;
	vector<string>::iterator iterador = palabras.begin();
	for ( ; iterador != palabras.end() ; iterador++){
		string palabra = (*iterador);
		if ( bag->estaEnBag(palabra) ){
		//	int frecPos = ( bag->getFrecuencias(1) )->at( bag->posicionEnBag(palabra) );
		//	int frecNeg = ( bag->getFrecuencias(0) )->at( bag->posicionEnBag(palabra) );
		//	int frecTotal = frecPos + frecNeg;
			numeroReal probaPositiva = bag->getProbabilidadesPositivas() -> at( bag->posicionEnBag(palabra));
			numeroReal probaNegativa = bag->getProbabilidadesNegativas() -> at( bag->posicionEnBag(palabra));
			//probaPositiva = (probaPositiva);
			//probaNegativa = log(probaNegativa);

//			// Casteo ambos para que devuelva float
//			// La probabilidad de que la palabra venga de la bag positiva
//			if(frecPos > 0) {
//				probaPositiva = (1.0 * frecPos) / frecTotal;
//				probaPositiva = log(probaPositiva);
//			}
//
//			// La probabilidad de que la palabra venga de la bag negativa
//			if(frecNeg > 0) {
//				probaNegativa = (1.0 * frecNeg) / frecTotal;
//				probaNegativa = log(probaNegativa);
//			}
//
//				if(inicio)
//				{
//					cout << "Proba pos: " << probaPositiva << endl;
//					cout << "Proba neg: " << probaNegativa << endl;
//					inicio = false;
//				}
//			//if(probaPositiva == 0)
			acumuladorProbaNegativa = acumuladorProbaNegativa + probaNegativa;
			acumuladorProbaPositiva = acumuladorProbaPositiva + probaPositiva;
		}
	}
	acumuladorProbaPositiva = exp(acumuladorProbaPositiva);
	acumuladorProbaNegativa = exp(acumuladorProbaNegativa);

	numeroReal probaReviewPositiva = acumuladorProbaPositiva / (acumuladorProbaPositiva + acumuladorProbaNegativa); //acumuladorProbaPositiva / (acumuladorProbaPositiva + acumuladorProbaNegativa);
	numeroReal probaReviewNegativa = acumuladorProbaNegativa / (acumuladorProbaPositiva + acumuladorProbaNegativa); //acumuladorProbaNegativa / (acumuladorProbaPositiva + acumuladorProbaNegativa);
//	float v1 = rand() % 100;
//	if(v1 < 30)
//	{
//		cout << "Proba review Positiva" << probaReviewPositiva << endl;
//		cout << "Proba review Negativas" << probaReviewNegativa << endl;
//	}

	*probabilidadPositiva = probaReviewPositiva;
	//TODO: MUCHO OJO CON ESTO QUE EL MENOR/MAYOR O IGUAL CAMBIA BASTANTE LOS RESULTADOS!
	if ( ( (probaReviewPositiva >= probaReviewNegativa) and (review.getSentiment() == 1) ) or ( (probaReviewPositiva < probaReviewNegativa) and (review.getSentiment() == 0) ) ) return true;
	else return false;
}
