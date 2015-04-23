#include "perceptron.h"

Perceptron::Perceptron(BagOfWords* bag, Parser* parser)  {
	this->bag = bag;
	this->parser = parser;
	this->pesos = new int[VEC_SIZE];
	//Inicializo el vector de pesos en 0.
	numeroPasadas = 80;
	toleranciaErrores = 0;
	learningRate = 0.1;
}


Perceptron::~Perceptron() { 
	delete [] pesos;
}

int Perceptron::productoInterno(int* pesos, vector<string> features) { }

int* Perceptron::entrenar() {

	cout << "\nPass\t\tErrors\t\tNr. Samples\tSince Start" << endl;
	for (int pasada = 0; pasada < numeroPasadas; pasada++) {
		int contadorError = 0;
		bool productoInterno;
		int error = 0;

		vector<Review>* reviews = parser->parsearReviewsAPredecir("labeledTrainData.tsv", 0);
		vector<Review>::iterator iterador = reviews->begin();
		for ( ; iterador != reviews->end() ; iterador++){
			Review rev = (*iterador);
			vector<string> features = rev.getPalabras();
			productoInterno = this->productoInterno(pesos, features) > 0.5;
			error = rev.getSentiment() - productoInterno;

			if (error != 0) {
				contadorError += 1;

				//Actualizamos los pesos
				vector<string>::iterator it = features.begin();
				for ( ; it != features.end() ; it++ ) {
					string palabra = *it;
					int j = this->bag->posicionEnBag(palabra);
					pesos[j] += learningRate * error * log(1.0 + 1); //	ver
				}
			}
		}
		cout << pasada << "\t" << contadorError << "\t" << reviews->size() << endl;

		if ((contadorError == 0) or (contadorError < toleranciaErrores)) {
			cout << contadorError << "errores encontrados en el entrenamiento. Detenido." << endl;
		}
	}
	return pesos;
}




vector<prediccion> Perceptron::predecir() {

	cout << "\nTesting online\nErrors\t\tAverage\t\tNr. Samples\tSince Start" << endl;
	int contadorError = 0;
	bool posONeg;
	int productoInterno;
	vector<prediccion> preds;

	vector<Review>* reviews = parser->parsearReviewsAPredecir("testData.tsv", 0);
	vector<Review>::iterator iterador = reviews->begin();

	for ( ; iterador != reviews->end() ; iterador++) {
		Review rev = (*iterador);
		vector<string> features = rev.getPalabras();
		productoInterno = this->productoInterno(pesos, features);
		posONeg = productoInterno > 0.5;
		prediccion pred;
		pred.id = rev.getId();
		pred.productoInterno = productoInterno;
		// normalizar

		preds.push_back(pred);

		if ( ( rev.getSentiment()- posONeg)!= 0) {
			contadorError += 1;
		}

	cout << contadorError << "\t" << endl;


	}
	return preds;
}



