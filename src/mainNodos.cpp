#include "network.h"
#include "parser.h"
#include "predictorredbayesiana.h"

//const string NOMBRE_ARCHIVO_LABELED_REVIEWS = "data/datain/labeledTrainData.tsv";


int main(int argc, char** argv){
	Network * redPositiva = new Network();
	Network * redNegativa = new Network();
	Parser * parser = new Parser();

	int desde = 0;
	vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, desde, 4000, true);

	vector<Review>::iterator iteradorReviews = reviews->begin();
	unsigned int i = desde;
	for ( ; iteradorReviews != reviews->end() ; iteradorReviews++){

		Review reviewAPredecir = (*iteradorReviews);
		if (i == desde) cout << "La primer review para crear el grafo es " << reviewAPredecir.getId() << endl;
		vector<string> palabras = reviewAPredecir.getPalabras();

		vector<string>::iterator iteradorPalabras = palabras.begin();
		string palabraAnterior = "";
		vector<string> palabrasAnteriores;

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
		if( (i+1-desde) % 1000 == 0 ) cout <<  "Se creo el grafo con " << (i+1-desde) << " reviews, de " << 25000-desde << endl;

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

	PredictorRedBayesiana* predictorRedBayesiana = new PredictorRedBayesiana(redPositiva, redNegativa, parser);
	predictorRedBayesiana->realizarPrediccion(1);

	delete redPositiva;
	delete redNegativa;
	delete parser;
	return 0;
}
