#include "predictorredbayesiana.h"

PredictorRedBayesiana::PredictorRedBayesiana(Network* redPositiva, Network* redNegativa, Parser * parser) {
	this->redPositva 	= redPositva;
	this->redNegativa	= redNegativa;
	this->parser 		= parser;
}

PredictorRedBayesiana::~PredictorRedBayesiana() {
}

void PredictorRedBayesiana::realizarPrediccion(int cantidadDePalabrasAnteriores) {
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
		if ( predecir(reviewAPredecir, &probabilidadPositiva, cantidadDePalabrasAnteriores) ) contador++;

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

bool PredictorRedBayesiana::predecir(Review& review, numeroReal * probabilidadPostiva, int cantidadDePalabrasAnteriores) {

	// Declaro probabilidades inciales
	numeroReal acumuladorProbaPositiva = 1.0;
	numeroReal acumuladorProbaNegativa = 1.0;

	numeroReal probabilidad;


	// Declaro vectores y cosas
	int cantidadDePalabrasAnterioresRevisadas = 0;
	unsigned int maxSize;
	unsigned int index;
	string palabraActual;
	unsigned int cantidadDeApuntadas = 0;
	vector<Network* > vectorDeNodos;
	vector<string> palabrasAnteriores;

	// Obtengo las palabras de la review
	vector<string> vectorPalabras = review.getPalabras();

	// Empiezo a iterar por las palabras
	vector<string>::iterator iteradorPalabras = vectorPalabras.begin();
	if(vectorPalabras.size() >= 2){
		string palabraAnterior = (*iteradorPalabras);
		palabrasAnteriores.push_back(palabraAnterior);
		cantidadDePalabrasAnterioresRevisadas++;
		iteradorPalabras++;
	}

//	if(review.getSentiment()){
//		for ( ; iteradorPalabras != vectorPalabras.end() ; iteradorPalabras++){
//			string palabra = (*iteradorPalabras);
////			vectorDeNodos.push_back(redPositva->hayNodoConPalabra(palabra));
//			Nodo* nodo = redPositva->hayNodoConPalabra(palabra);
//			list<Nodo* >* listaDeNodosQueMeApuntan = nodo->getNodosQueMeApuntan();
//			probabilidad = 1.0 / listaDeNodosQueMeApuntan->size();
//		}
//	}

	if(review.getSentiment() == 1){

		for(; iteradorPalabras != vectorPalabras.end(); iteradorPalabras++){

			string palabraActual = (*iteradorPalabras);

			// Primero obtengo las palabras que apuntan a la palabra actual

			Nodo* 		nodoActual 			= redPositva->hayNodoConPalabra(palabraActual);
			Network* 	nodosQueMeApuntan 	= new Network(nodoActual->getNodosQueMeApuntan());

			// Ahora, de las N palabras anteriores, quiero quedarme con X palabras anteriores que me apuntan
			// con X <= N ( De las N palabras anteriores solo tomo las que estan apuntando a la palabra actual)

			vector<string>* palabrasQueMeApuntan = new vector<string>();
//			if()

			// Con esto obtengo todos los nodos a los que apunto con mis palabras anteriores
			Nodo* nodo;
			for(int i = 0; i < cantidadDePalabrasAnterioresRevisadas; i++){
				nodo = redPositva->hayNodoConPalabra(palabrasAnteriores[0]);
				vectorDeNodos.push_back(new Network(nodo->getNodosQueApunto()));
				maxSize = nodo->getNodosQueApunto()->size();
				index = 0;

				for(int j = 1; j < cantidadDePalabrasAnterioresRevisadas; j++){
					nodo = redPositva->hayNodoConPalabra(palabrasAnteriores[j]);
					vectorDeNodos.push_back(new Network(nodo->getNodosQueApunto()));
					if( maxSize < nodo->getNodosQueApunto()->size()){
						maxSize = nodo->getNodosQueApunto()->size();
						index = j;
					}
				}
			}


			// Primero me fijo si la palabra esta en la lista con mayor cantidad de nodos
			if(vectorDeNodos[index]->hayNodoConPalabra(palabraActual)){
				// Ahora voy a hacer el join de todos los nodos con todas las palabras anteriores
				// para ver a que palabras en comun apuntan

				list<Nodo* >::iterator iteradorNodos = vectorDeNodos[index]->getListaNodos()->begin();

				for(; iteradorNodos != vectorDeNodos[index]->getListaNodos()->end(); iteradorNodos++){

					string palabraNodo = (*iteradorNodos)->getPalabra();
					for(unsigned int l = 0; l < vectorDeNodos[l]->getListaNodos()->size(); l++){
						if(l != index ){
							if(vectorDeNodos[l]->hayNodoConPalabra(palabraNodo)){
								cantidadDeApuntadas++;
							}
						}
					}
				}

				probabilidad = 1.0 / cantidadDeApuntadas;
			} else {
				probabilidad = probabilidad * 1.0;
			}

			palabrasAnteriores.push_back(palabraActual);
			if(cantidadDePalabrasAnterioresRevisadas < cantidadDePalabrasAnteriores){
				cantidadDePalabrasAnterioresRevisadas++;
				palabrasAnteriores.erase(palabrasAnteriores.begin());
			}
		}
	}



	// Me fijo si mi palabra esta entre los nodos en comun

	// Luego calculo la probabilidad de que una palabra este en todo ese nodo


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
