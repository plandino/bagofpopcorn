
#include "review.h"

Review::Review(string id, vector<string> words, int sentimiento) {
	this->id = id;
	this->sentiment = sentimiento;
	this->palabras = words;
}

Review::~Review() {

}

