#include "perceptron.h"


Perceptron::Perceptron(BagOfWords* bag, Parser* parser)  {
	this->bag = bag;
	this->parser = parser;
	this->pesos = new double[VEC_SIZE];
	for (int i=0; i < VEC_SIZE; i++) {
		pesos[i] = 0;
	}
	//Inicializo el vector de pesos en 0.
	numeroPasadas = 80;
	toleranciaErrores = 0;
	learningRate = 0.1;
}


Perceptron::~Perceptron() { 
	delete [] pesos;
}


int Perceptron::productoInterno(vector<string> features) {
	int productoInterno = 0;
	vector<string>::iterator iterador = features.begin();
	for ( ; iterador != features.end(); iterador++ ) {
		string palabra = *iterador;
		if (bag->estaEnBag(palabra) != -1) {
			int indice = bag->posicionEnBag(palabra);
			int pesoPalabra = pesos[indice];	
			productoInterno += pesoPalabra * 1; // 1 es el "value" en el perceptron. 
		}
	}
	// if (productoInterno) cout << "DEBUG: " << productoInterno << endl;
	return productoInterno;
}


double* Perceptron::entrenar() {
	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, 0, true);
	cout << "\nPass\t\tErrors\t\tNr. Samples\tSince Start" << endl;
	for (int pasada = 0; pasada < numeroPasadas; pasada++) {
		int contadorError = 0;
		bool productoInterno;
		int error = 0;

		int dotp;
		vector<Review>::iterator iterador = reviews->begin();
		for ( ; iterador != reviews->end() ; iterador++) {
			//cout << "ENTRANDO EN EL SEGUNDO FOR" << endl; //DEBUG
			Review rev = (*iterador);
			vector<string> features = rev.getPalabras();
			productoInterno = ( this->productoInterno(features) > 0.5 );
			if (productoInterno) { dotp = 1; }
			else { dotp = 0; }
			error = rev.getSentiment() - dotp;

			if (error != 0) {
				contadorError += 1;

				//Actualizamos los pesos
				vector<string>::iterator it = features.begin();
				for ( ; it != features.end() ; it++ ) {
					string palabra = *it;
					if (bag->estaEnBag(palabra) ) {
						int j = this->bag->posicionEnBag(palabra);
						pesos[j] += learningRate * error * log(2); //	ver
					}
				}
			}
		}
		cout << pasada << "\t" << contadorError << "\t" << reviews->size() << endl;

		if ((contadorError == 0) or (contadorError < toleranciaErrores)) {
			cout << contadorError << " errores encontrados en el entrenamiento. Detenido." << endl;
		}
	}
	delete reviews;
	return pesos;
}


bool comparador_pred(prediccion a, prediccion b) {
	return a.productoInterno < b.productoInterno;
}


vector<prediccion> Perceptron::predecir() {

	cout << "\nTesting online\nErrors\t\tAverage\t\tNr. Samples\tSince Start" << endl;
	int contadorError = 0;
	bool posONeg;
	int productoInterno;
	vector<prediccion> preds;

	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_TEST_DATA, 0, false);
	vector<Review>::iterator iterador = reviews->begin();

	for ( ; iterador != reviews->end() ; iterador++) {
		Review rev = (*iterador);
		vector<string> features = rev.getPalabras();
		productoInterno = this->productoInterno(features);
		posONeg = productoInterno > 0.5;
		prediccion pred;
		pred.id = rev.getId();
		pred.productoInterno = productoInterno;
		preds.push_back(pred);

		if ( ( rev.getSentiment()- posONeg)!= 0) {
			contadorError += 1;
		}
	}
	cout << contadorError << "\t" << endl;
	int maxProd = (*max_element(preds.begin(), preds.end(), comparador_pred)).productoInterno;
	int minProd = (*min_element(preds.begin(), preds.end(), comparador_pred)).productoInterno;
	int divisor = maxProd - minProd;

	vector<prediccion>::iterator it = preds.begin();
	for ( ; it != preds.end(); it++) {
		prediccion p = *it;
		p.productoInternoNormalizado = ( (p.productoInterno - minProd) / divisor );
	}
	delete reviews;
	return preds;
}


void Perceptron::tirarACSV(vector<prediccion> predicciones) {

	ofstream archivo("data/dataout/perceptron.csv");
	if ( archivo.is_open() ) {
		archivo << "\"id\",\"sentiment\"" << "\n";
		vector<prediccion>::iterator it = predicciones.begin();
		for ( ; it != predicciones.end(); it++) {
			prediccion p = *it;
			archivo << p.id.c_str() << "," << p.productoInternoNormalizado << "\n";
		}
	}
	archivo.close();
}
