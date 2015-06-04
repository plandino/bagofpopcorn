#include "network.h"
#include "parser.h"

//const string NOMBRE_ARCHIVO_LABELED_REVIEWS = "data/datain/labeledTrainData.tsv";


int main(int argc, char** argv){
	Network * redBayesiana = new Network();
	Parser * parser = new Parser();

	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, 24990, true);

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
	list<Nodo* > * listaDeNodos = redBayesiana->getListaNodos();
	list<Nodo* >::iterator iteradorListaDeNodos = listaDeNodos->begin();

	for(; iteradorListaDeNodos != listaDeNodos->end(); iteradorListaDeNodos++){
		Nodo* nodito = (*iteradorListaDeNodos);

		list<Nodo* > * nodosQueMeApuntan = nodito->getNodosQueMeApuntan();
		list<Nodo* >::iterator iteradorNodosQueMeApuntan = nodosQueMeApuntan->begin();

		cout << "Palabra: " << nodito->getPalabra().c_str()  << " apuntada por: ";
		for(; iteradorNodosQueMeApuntan != nodosQueMeApuntan->end() ; iteradorNodosQueMeApuntan++){
			Nodo* nodoQueMeApunta = (*iteradorNodosQueMeApuntan);
			if(nodoQueMeApunta != NULL){
				cout << nodoQueMeApunta->getPalabra().c_str() << ", ";
			} else {
				cout << " y no me apunta nadie.";
			}
		}
		cout << endl;

	}

	cout << " termine" << endl;

	delete redBayesiana;
	delete parser;
	return 0;
}
