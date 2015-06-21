#include "perceptron.h"
#include <iomanip>

/*
 *  usaBag: Determina si el perceptron va a utilizar una Bag of Words para determinar el indice de una palabra en el
 *  vector de pesos. Si es FALSE se utiliza una funcion de hashing por defecto. 
 */
Perceptron::Perceptron(BagOfWords* bag, Parser* parser, bool usaBag) {
    this->bag = bag;
    this->parser = parser;
    this->usaBag = usaBag;
    this->pesos = new double[VEC_SIZE];
    /* Inicializo el vector de pesos en 0. */ 
    for (int i=0; i < VEC_SIZE; i++) {
        pesos[i] = 0;
    }
    numeroPasadas = 80;
    toleranciaErrores = 0;
    learningRate = 0.035;
}

Perceptron::~Perceptron() { 
    delete [] pesos;
}

/*
 *  SPOILER ALERT: No hay ningun producto interno.
 *  El metodo se encarga de calcular el resultado final para una review, sumando los pesos de 
 *  cada una de las palabras que contiene. El resultado final es un numero real, sin restricciones.
 */
double Perceptron::productoInterno(vector<string> features) {
    double productoInterno = 0;
    vector<string>::iterator iterador = features.begin();
    for ( ; iterador != features.end(); iterador++ ) {
        double pesoPalabra = 0;
        hash<string> hashFunction;
        string palabra = *iterador;
        int indice = -1;
        if (usaBag) {
            if (bag->estaEnBag(palabra)) {
                indice = bag->posicionEnBag(palabra);
            }
        } else {
            indice = hashFunction(palabra) % VEC_SIZE;
        }
        if (indice != -1) {
            pesoPalabra = pesos[indice];    
        }
        productoInterno += pesoPalabra * 1;
    }
    return productoInterno;
}

/* 
 * Metodo de aprendizaje supervisado que calcula los valores del vector de pesos.
 * Se siguen los siguientes pasos:
 * -Se predice un resultado para una review (cuyo resultado real se conoce).
 *  Si el resultado es incorrecto, se modifica el vector de pesos (con el parametro learningRate )
 *      en aquellos indices que se utilizaron para tomar la decision.
 *  Si el resultado es correcto, el vector de pesos no se modifica.
 * -Repetir hasta que la cantidad de errores sea <= toleranciaErrores o la cantidad de pasadas == numeroPasadas.    
 */
double* Perceptron::entrenar() {
    vector<Review>* reviews = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_LABELED_REVIEWS, 0, true); //Se utiliza el metodo parsearReviewsAPredecir aunque sean las labeled
    cout << "Pasada\t\tErrores\t" << endl;
    for (int pasada = 0; pasada < numeroPasadas; pasada++) {
        int contadorError = 0;
        bool productoInterno;
        int error = 0;
        int dotp;
        hash<string> hashFunction;
        vector<Review>::iterator iterador = reviews->begin();
        for ( ; iterador != reviews->end() ; iterador++) {
            Review rev = (*iterador);
            vector<string> features = rev.getPalabras();
            /* Si el producto interno es > 0.5, la prediccion es 1, caso contrario es 0 */
            productoInterno = ( this->productoInterno(features) > 0.5 );
            if (productoInterno) { dotp = 1; }
            else { dotp = 0; }
            /* El error es 0 si se predijo correctamente, -1 si predije 1 y el label es 0, y +1 en el caso restante. */
            error = rev.getSentiment() - dotp;

            if (error != 0) {
                contadorError += 1;

                /* Actualizamos los pesos */
                vector<string>::iterator it = features.begin();
                for ( ; it != features.end() ; it++ ) {
                    string palabra = *it;
                    if (usaBag) {
                        if (bag->estaEnBag(palabra) ) {
                            int j = this->bag->posicionEnBag(palabra);
                            pesos[j] += learningRate * error * log(2);
                        }
                    } else {
                        int indice;
                        indice = hashFunction(palabra) % VEC_SIZE;
                        pesos[indice] += learningRate * error * log(2);
                    }
                }
            }
        }
        cout << pasada << "\t\t" << contadorError << endl;

        if ((contadorError == 0) or (contadorError < toleranciaErrores)) {
            cout << contadorError << " errores encontrados en el entrenamiento. Detenido." << endl;
            break;
        }
    }
    delete reviews;
    return pesos;
}

bool comparador_pred(prediccion a, prediccion b) {
    return a.productoInterno < b.productoInterno;
}

/*
 * Metodo final del ciclo de vida del perceptron.
 *  Utiliza los pesos obtenidos con los pasos anteriores para predecir el resultado final de una review. 
 *  La decision se representa con el resultado de la funcion productoInterno, que luego se normaliza para tener 
 *  una "probabilidad" en lugar de un resultado deterministico.
 *  Los parametros que se piden son aquellos vectores donde se escriben los resultados finales, para ser manejados 
 *  luego por la ponderacion.
 */
void Perceptron::predecir(std::vector<string>& ids, std::vector<numeroReal>& predicciones) {
    int contadorError = 0;
    bool posONeg;
    double dotp;
    vector<prediccion> preds;

    vector<Review>* reviewsAPredecir = parser->parsearReviewsAPredecir(NOMBRE_ARCHIVO_TEST_DATA, 0, false);
    cout << "Perceptron prediciendo..." << endl << endl;
    vector<Review>::iterator iterador = reviewsAPredecir->begin();

    for ( ; iterador != reviewsAPredecir->end() ; iterador++) {
        Review rev = (*iterador);
        vector<string> features = rev.getPalabras();
        dotp = this->productoInterno(features);
        posONeg = dotp > 0.5; // Si es mayor, la prediccion es 1. Caso contrario, es 0.
        prediccion pred;
        pred.id = rev.getId();
        pred.productoInterno = dotp;
        preds.push_back(pred);

        if ( (rev.getSentiment() - posONeg)!= 0) {
            contadorError += 1;
        }
    }
    cout << "Finalizada la prediccion. Guardando resultados..." << endl;
    double maxProd = (*max_element(preds.begin(), preds.end(), comparador_pred)).productoInterno;
    double minProd = (*min_element(preds.begin(), preds.end(), comparador_pred)).productoInterno;
    double divisor = maxProd - minProd;

    vector<prediccion>::iterator it = preds.begin();
    for ( ; it != preds.end(); it++) {
        prediccion p = *it;
        it->productoInternoNormalizado = ( (p.productoInterno - minProd) / divisor );
        predicciones.push_back(it->productoInternoNormalizado);
        ids.push_back(it->id);
    }
    delete reviewsAPredecir;
}
