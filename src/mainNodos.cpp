#include "network.h"
#include "parser.h"

//const string NOMBRE_ARCHIVO_LABELED_REVIEWS = "data/datain/labeledTrainData.tsv";


int main(int argc, char** argv){
	Network * redBayesiana = new Network();
	Parser * parser = new Parser();

	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, 24999, true);

	vector<Review>::iterator iteradorReviews = reviews->begin();
	unsigned int i = 0;
	for ( ; iteradorReviews != reviews->end() ; iteradorReviews++){

		Review reviewAPredecir = (*iteradorReviews);
		if (i == 0) cout << "La primer review a predecir es " << reviewAPredecir.getId() << endl;
		vector<string> palabras = reviewAPredecir.getPalabras();

		vector<string>::iterator iteradorPalabras = palabras.begin();
		string palabraAnterior = "";
		for ( ; iteradorPalabras != palabras.end() ; iteradorPalabras++){
			string palabra = (*iteradorPalabras);
//			cout << "MI PALABRA: " << palabra.c_str() << " palabra anterior " << palabraAnterior.c_str();
			redBayesiana->agregarPalabra(palabra, palabraAnterior);
			palabraAnterior.assign(palabra.c_str());
		}
	}
	bool inicio = true;
	list<Nodo* > * listaDeNodos = redBayesiana->getListaNodos();
	list<Nodo* >::iterator iteradorLista = listaDeNodos->begin();
	for(; iteradorLista != listaDeNodos->end(); iteradorLista++){
		Nodo* nodito = (*iteradorLista);
		if(!nodito->getNodosQueMeApuntan()->empty() ){
			list<Nodo* > * listaDeNodosApuntadores = nodito->getNodosQueMeApuntan();
			list<Nodo* >::iterator iteradorApuntadores = listaDeNodosApuntadores->begin();
			for(; iteradorApuntadores != listaDeNodosApuntadores->end(); iteradorApuntadores++){
				Nodo* nodoApuntaadsasdasd = (*iteradorApuntadores);
				if(inicio){
					inicio = false;
				}else {
					cout << "Palabra: " << nodito->getPalabra().c_str()  << " apuntada por: " << nodoApuntaadsasdasd->getPalabra().c_str() << endl;
				}
//				cout << "Palabra: " << nodito->getPalabra().c_str()  << " apuntada por: " << nodoApuntaadsasdasd->getPalabra().c_str() << endl;
			}
//
		} else {
			cout << "Palabra: " << nodito->getPalabra().c_str() << endl;
		}

	}

	cout << " termine" << endl;

	delete redBayesiana;
	delete parser;
	return 0;
}
