#ifndef PARSER_H_
#define PARSER_H_

#include "bagofwords.h"
#include "review.h"

const string NOMBRE_ARCHIVO_FRECUENCIAS = "data/dataout/frecuencias.tsv";
const string NOMBRE_ARCHIVO_STOPWORDS1 = "data/datain/stopwordsdictionary1.txt";
const string NOMBRE_ARCHIVO_STOPWORDS2 = "data/datain/stopwordsdictionary2.txt";
const string NOMBRE_ARCHIVO_STOPWORDS_PYHTON = "data/datain/stopwords_english_python_nltk.txt";
const string NOMBRE_ARCHIVO_REVIEWS = "data/datain/labeledTrainData.tsv";

const int CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO = 15000;

class Parser {

private:
	void cargarDiccionarioStopWords(string nombreArchivo);
	vector<string> limpiarReview(string review, int sentiment, bool agregar);
	bool esStopWord(string word);
	string eliminarURL(string word, bool &continuar);
	vector<string> soloLetras(string word);
	map<string, int> stopWords;
	BagOfWords* bag;

public:
	Parser();
	virtual ~Parser();

	BagOfWords* parsearReviews(string nombreArchivo);
	vector<Review>* parsearReviewsAPredecir(string nombreArchivo, int desde);
	void generarTSV();
	BagOfWords* leerPalabrasYFrecuenciasDesdeTSV(string nombreArchivo);
	BagOfWords* leerPalabrasYFrecuenciasDesdeCSVPython(string nombreArchivo);
};

#endif /* PARSER_H_ */
