#include "perceptron.h"
#include <iomanip>


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

double* Perceptron::getPesos() {
	for (int i=0; i < VEC_SIZE; i++) {
		if (pesos[i] != 0) cout << std::fixed << std::setprecision(8) << pesos[i] << ", ";
	}
	return pesos;
}

double Perceptron::productoInterno(vector<string> features) {
	double productoInterno = 0;
	vector<string>::iterator iterador = features.begin();
	for ( ; iterador != features.end(); iterador++ ) {
		string palabra = *iterador;
		if (bag->estaEnBag(palabra) != -1) {
			int indice = bag->posicionEnBag(palabra);
			double pesoPalabra = pesos[indice];	
			productoInterno += pesoPalabra * 1; // 1 es el "value" en el perceptron. 
		}
	}
	return productoInterno;
}


double* Perceptron::entrenar(bool biWord) {
	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, 0, true, biWord);
	cout << "\nPass\t\tErrors\t\tNr. Samples\tSince Start" << endl;
	for (int pasada = 0; pasada < numeroPasadas; pasada++) {
		int contadorError = 0;
		bool productoInterno;
		int error = 0;
		int dotp;

		vector<Review>::iterator iterador = reviews->begin();
		for ( ; iterador != reviews->end() ; iterador++) {
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
						pesos[j] += learningRate * error * log(2); // Log(2) devuelve un numero con menos decimales que su equiv. en python.
					}
				}
			}
		}
		cout << pasada << "\t" << contadorError << "\t" << reviews->size() << endl;

		if ((contadorError == 0) or (contadorError < toleranciaErrores)) {
			cout << contadorError << " errores encontrados en el entrenamiento. Detenido." << endl;
			break;
		}
	}
	delete reviews;
	return pesos;
}


bool comparador_pred(prediccion a, prediccion b) {
	return a.productoInterno < b.productoInterno;
}


void Perceptron::predecir(std::vector<string>& ids, std::vector<numeroReal>& predicciones, bool biWord) {
	int contadorError = 0;
	bool posONeg;
	double dotp;
	vector<prediccion> preds;

	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_TEST_DATA, 0, false, biWord);
	cout << "\nTesting online\nErrors\t\tAverage\t\tNr. Samples\tSince Start" << endl;
	vector<Review>::iterator iterador = reviews->begin();

	for ( ; iterador != reviews->end() ; iterador++) {
		Review rev = (*iterador);
		vector<string> features = rev.getPalabras();
		dotp = this->productoInterno(features);
		posONeg = dotp > 0.5;
		prediccion pred;
		pred.id = rev.getId();
		pred.productoInterno = dotp;
		preds.push_back(pred);

		if ( ( rev.getSentiment()- posONeg)!= 0) {
			contadorError += 1;
		}
	}
	cout << contadorError << "\t" << endl;
	double maxProd = (*max_element(preds.begin(), preds.end(), comparador_pred)).productoInterno;
	double minProd = (*min_element(preds.begin(), preds.end(), comparador_pred)).productoInterno;
	double divisor = maxProd - minProd;

	vector<prediccion>::iterator it = preds.begin();
	for ( ; it != preds.end(); it++) {
		prediccion p = *it;
		it->productoInternoNormalizado = ( (p.productoInterno - minProd) / divisor );
		predicciones.push_back(it->productoInternoNormalizado);
		ids.push_back(it->id);
	}
	delete reviews;
}


void Perceptron::tirarACSV(const vector<prediccion>& predicciones) {
	ofstream archivo("data/dataout/perceptron.csv");
	if ( archivo.is_open() ) {
		archivo << "\"id\",\"sentiment\"" << "\n";
		vector<prediccion>::const_iterator it = predicciones.begin();
		for ( ; it != predicciones.end(); it++) {
			prediccion p = *it;
			archivo << p.id.c_str() << "," << std::fixed << std::setprecision(6) << p.productoInternoNormalizado << "\n";
		}
	}
	archivo.close();
}
