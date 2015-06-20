#ifndef PARSER_H_
#define PARSER_H_

#include "bagofwords.h"
#include "review.h"

const string NOMBRE_ARCHIVO_FRECUENCIAS = "data/dataout/frecuencias.tsv";

const string NOMBRE_ARCHIVO_STOPWORDS1 = "data/datain/stopwordsdictionary1.txt";
const string NOMBRE_ARCHIVO_STOPWORDS2 = "data/datain/stopwordsdictionary2.txt";
const string NOMBRE_ARCHIVO_STOPWORDS_PYHTON = "data/datain/stopwords_english_python_nltk.txt";

const string NOMBRE_ARCHIVO_LABELED_REVIEWS = "data/datain/labeledTrainData.tsv";
const string NOMBRE_ARCHIVO_TEST_DATA = "data/datain/testData.tsv";

const string NOMBRE_ARCHIVO_CSV_PROBABILIDADES = "data/dataout/csvProbas.csv";
const string NOMBRE_ARCHIVO_CSV_PONDERADO = "data/dataout/csvProbasPONDERADO.csv";
const string NOMBRE_ARCHIVO_CSV_BAYES = "data/dataout/csvProbasBAYES.csv";
const string NOMBRE_ARCHIVO_CSV_MASMENOSUNO = "data/dataout/csvProbasMASMENOSUNO.csv";
const string NOMBRE_ARCHIVO_CSV_TRON = "data/dataout/csvProbasPERCEPTRON.csv";
const string NOMBRE_ARCHIVO_CSV_CEROSYUNO = "data/dataout/csvCerosYUnos.csv";

const int CANTIDAD_REVIEWS_A_CONSIDERAR_PARA_PARSEO = 25000;

class Parser {

private:
	void cargarDiccionarioStopWords(string nombreArchivo);
	vector<string> limpiarReview(string review, int sentiment, bool agregar);
	bool esStopWord(string word);
	string eliminarURL(string word, bool &continuar);
	vector<string> soloLetras(string word);
	map<string, int> stopWords;
	BagOfWords* bag;
	bool biWord;
	bool triWord;

public:
	Parser(bool sinStopWords);
	virtual ~Parser();

	BagOfWords* parsearReviews(string nombreArchivo, bool biWord, bool triWord);
	vector<Review>* parsearReviewsAPredecir(string nombreArchivo, int desde, bool tieneSentimiento);

	void generarTSV();
	BagOfWords* leerPalabrasYFrecuenciasDesdeTSV(string nombreArchivo);
	void agregarAlCSV(vector<string>& id, vector<numeroReal>& probabilidad, string nombreArchivoSalida);
	void leerCsvProbas(string nombreArchivo, vector<numeroReal>& vectorProbabilidades, vector<string>& vectorIds);
};

#endif /* PARSER_H_ */
