#ifndef VEC_SIZE 
#define VEC_SIZE 33554432 // 2 ** 25
#endif /* VEC_SIZE */

#include "bagofwords.h"
#include "parser.h"
#include "string.h"
#include "review.h"

using namespace std;

struct prediccion { 
	string id;
	double productoInternoNormalizado;
	double productoInterno;
};

class Perceptron {

    private:
        double productoInterno(vector<string> features);
        double* pesos;
        BagOfWords* bag;
        Parser* parser;
        int toleranciaErrores;
        int numeroPasadas;
        double learningRate;
        bool usaBag;

    public:
        Perceptron(BagOfWords* bag, Parser* parser, bool usaBag);
        double* entrenar();
        void predecir(std::vector<string>& ids, std::vector<numeroReal>& predicciones);
        virtual ~Perceptron();
};

