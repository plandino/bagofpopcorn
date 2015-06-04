#ifndef VEC_SIZE 
#define VEC_SIZE 33554432 // 2 ** 25
#endif /* VEC_SIZE */

#include "bagofwords.h"
#include "parser.h"
#include "string.h"
#include "review.h"
#include <cmath>

using namespace std;

struct prediccion { 
	string id;
	double productoInternoNormalizado;
	double productoInterno;
};

class Perceptron {

    private:
        double* pesos ; //TODO: Ver 2 ** 25, posiciones del vector
        BagOfWords* bag;
        Parser* parser;
        double productoInterno(vector<string> features);
        int toleranciaErrores;
        int numeroPasadas;
        double learningRate;
        bool usaBag;
    public:
        Perceptron(BagOfWords* bag, Parser* parser, bool usaBag);
        double* entrenar(bool biWord); //Devuelve el vector de pesos con los valores que resultan.
        void predecir(std::vector<string>& ids, std::vector<numeroReal>& predicciones, bool biWord);
        virtual ~Perceptron();
        void tirarACSV(const vector<prediccion>& predicciones);
        double* getPesos();
};

