
#ifndef REVIEW_H_
#define REVIEW_H_

#include <vector>
#include <iostream>
#include "string.h"

using namespace std;

class Review {

private:
	vector<string> palabras;
	string id;
	int sentiment;

public:
	Review(string id, vector<string> words, int sentimiento);
	virtual ~Review();

	int getSentiment() {
		return sentiment;
	}

	void setSentiment(int sentiment) {
		this->sentiment = sentiment;
	}

	vector<string> getPalabras() {
		return palabras;
	}
};

#endif /* REVIEW_H_ */
