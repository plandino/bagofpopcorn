#ifndef PARSER_H_
#define PARSER_H_

#include "bagofwords.h"

const string NOMBRE_ARCHIVO_FRECUENCIAS = "data/dataout/frecuencias.tsv";
const string NOMBRE_ARCHIVO_STOPWORDS1 = "data/datain/stopwordsdictionary1.txt";
const string NOMBRE_ARCHIVO_STOPWORDS2 = "data/datain/stopwordsdictionary2.txt";

class Parser {

private:
	void cargarDiccionarioStopWords(string nombreArchivo);
	void limpiarReview(string review, int sentiment);
	bool esStopWord(string word);
	map<string, int> stopWords;
	BagOfWords* bag;

public:
	Parser();
	virtual ~Parser();

	BagOfWords* parsearReviews(string nombreArchivo);
	void generarTSV();
	BagOfWords* leerPalabrasYFrecuenciasDesdeTSV(string nombreArchivo);
};

#endif /* PARSER_H_ */
