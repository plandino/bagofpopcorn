#ifndef PARSER_H_
#define PARSER_H_

#include "bagofwords.h"

class Parser {

private:
	void cargarDiccionarioStopWords();
	void limpiarReview(string review, int sentiment);
	bool esStopWord(string word);
	map<string, int> stopWords;
	BagOfWords* bag;

public:
	Parser();
	virtual ~Parser();

	BagOfWords* parsearReviews(string nombreArchivo);
	void generarTSV();
};

#endif /* PARSER_H_ */
