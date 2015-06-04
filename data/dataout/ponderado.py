import csv
import numpy as np
from sklearn.metrics import roc_auc_score

"""
Levanta un csv del tipo
VALOR_REAL,PROBABILIDAD_DE_1
"""
def abrirCSV(pathArchivo):
	scores = []
	true_values = []
	with open(pathArchivo, 'rb') as archivo:
		reader = csv.reader(archivo)
		for l in reader:
			scores.append( float(l[1]) )
			true_values.append( int(l[0]) )
	n_scores = np.array(scores)
	n_true = np.array(true_values)

	return n_scores, n_true

def my_range(start, end, step):
    while start <= end:
        yield start
        start += step

"""
1 - Cargar los archivos de cada uno de los algoritmos
2 - Guardar en vectores de numpy -.- los valores reales ( 1 vez ) y las probabilidades de cada uno
3 - Calcular ROC

4 - Englobar 2 y 3 en un for donde multiplico c/ elemento del vector por su peso correspondiente

5 - ???
6 - Profit
"""

tron_scores, true_values = abrirCSV('csvProbasPERCEPTRONconPosta.csv')
m_scores, true_values = abrirCSV('csvProbasMASMENOSUNOconPosta.csv')
bayes_scores, true_values = abrirCSV('csvProbasBAYESconPosta.csv')


max_auc = 0.0
scores_ponderados = [0.0] * 5000
e = 0.00001
for pesoBayes in my_range(0.0, 1.0, 0.01): 
	for pesoM in my_range(0.0, 1.0, 0.01):
		for pesoTron in my_range(0.0, 1.0, 0.01): 
			if ( ((pesoTron + pesoBayes + pesoM) <= 1.0 +e ) and (pesoTron+pesoBayes+pesoM) >= 1.0-e):
				for i in xrange(0, 5000):
					scores_ponderados[i] = (tron_scores[i] * pesoTron) + (bayes_scores[i] * pesoBayes) + (m_scores[i] * pesoM)  
				result = roc_auc_score(true_values, scores_ponderados)
				if ( result >= max_auc ):
					max_auc = result
					print ("Los pesos del mejor hasta ahora son %s para el tron, %s para el bayes y %s para el mMU" % (pesoTron, pesoBayes, pesoM))
					print ("Para un AUC de %s" % (max_auc))
print("El mejor AUC fue: ")
print max_auc