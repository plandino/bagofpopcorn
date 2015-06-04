#include "network.h"
#include "parser.h"

//const string NOMBRE_ARCHIVO_LABELED_REVIEWS = "data/datain/labeledTrainData.tsv";


int main(int argc, char** argv){
	Network * redPositiva = new Network();
	Network * redNegativa = new Network();
	Parser * parser = new Parser();

	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, 24997, true);

	vector<Review>::iterator iteradorReviews = reviews->begin();
	unsigned int i = 0;
	for ( ; iteradorReviews != reviews->end() ; iteradorReviews++){

		Review reviewAPredecir = (*iteradorReviews);
		if (i == 0) cout << "La primer review a parsear es " << reviewAPredecir.getId() << endl;
		vector<string> palabras = reviewAPredecir.getPalabras();

		vector<string>::iterator iteradorPalabras = palabras.begin();
		string palabraAnterior = "";

		if(reviewAPredecir.getSentiment()){
			for ( ; iteradorPalabras != palabras.end() ; iteradorPalabras++){
				string palabra = (*iteradorPalabras);
				redPositiva->agregarPalabra(palabra, palabraAnterior);
				palabraAnterior = palabra;
			}
		} else {
			for ( ; iteradorPalabras != palabras.end() ; iteradorPalabras++){
				string palabra = (*iteradorPalabras);
				redNegativa->agregarPalabra(palabra, palabraAnterior);
				palabraAnterior = palabra;
			}
		}
		i++;

	}
	list<Nodo* > * listaDeNodosPositiva = redPositiva->getListaNodos();
	list<Nodo* >::iterator iteradorListaDeNodos = listaDeNodosPositiva->begin();



	cout << "Estas son las palabras positivas" << endl;

	for(; iteradorListaDeNodos != listaDeNodosPositiva->end(); iteradorListaDeNodos++){
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

	cout << endl << "Estas son las palabras negativas" << endl;


	list<Nodo* > * listaDeNodosNegativa = redNegativa->getListaNodos();
	iteradorListaDeNodos = listaDeNodosNegativa->begin();

	for(; iteradorListaDeNodos != listaDeNodosNegativa->end(); iteradorListaDeNodos++){
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

	delete redPositiva;
	delete redNegativa;
	delete parser;
	return 0;
}
