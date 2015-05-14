#include "network.h"
#include "parser.h"

//const string NOMBRE_ARCHIVO_LABELED_REVIEWS = "data/datain/labeledTrainData.tsv";


int main(int argc, char** argv){
	Network * redBayesiana = new Network();
	Parser * parser = new Parser();

	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, 24997, true);

	vector<Review>::iterator iteradorReviews = reviews->begin();
	unsigned int i = 0;
	for ( ; iteradorReviews != reviews->end() ; iteradorReviews++){

		Review reviewAPredecir = (*iteradorReviews);
		if (i == 0) cout << "La primer review a predecir es " << reviewAPredecir.getId() << endl;
		vector<string> palabras = reviewAPredecir.getPalabras();

		vector<string>::iterator iteradorPalabras = palabras.begin();
		for ( ; iteradorPalabras != palabras.end() ; iteradorPalabras++){
			string palabra = (*iteradorPalabras);
			redBayesiana->agregarPalabra(palabra);
		}
	}
	list<Nodo* > * listaDeNodos = redBayesiana->getListaNodos();
	list<Nodo* >::iterator iteradorLista = listaDeNodos->begin();
	for(; iteradorLista != listaDeNodos->end(); iteradorLista++){
		Nodo* nodito = (*iteradorLista);
		cout << "Palabra: " << nodito->getPalabra().c_str() << endl;
	}

	cout << " termine" << endl;

	delete redBayesiana;
	delete parser;
	return 0;
}
