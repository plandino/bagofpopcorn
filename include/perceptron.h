#ifndef VEC_SIZE 
#define VEC_SIZE 33554432 //2 ** 25
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
        double* pesos ; //Array de 2 ** 25 posiciones. Tamanio copiado del de python. REVISAR NUMERO
        BagOfWords* bag; //En lugar de funcion de hashing, usar la bag directamente. Colisiones == 0!!.
        Parser* parser;
        double productoInterno(vector<string> features);
        int toleranciaErrores;
        int numeroPasadas;
        double learningRate;
    public:
        double* entrenar(); //Devuelve el vector de pesos con los valores que resultan.
        vector<prediccion> predecir();
        Perceptron(BagOfWords* bag, Parser* parser);
        virtual ~Perceptron();
        void tirarACSV(const vector<prediccion>& predicciones);
        double* getPesos();
};
