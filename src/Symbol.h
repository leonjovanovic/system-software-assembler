#include <iostream>
using namespace std;

#ifndef SYMBOL_H_
#define SYMBOL_H_

class Symbol{
private:
	string ime;
	int sekcija;
	int vrednost;
	char doseg;
	int redbr;
	int velicina;
	int tip;
	string rwx;
	static int globRedBr;

public:
	Symbol(string ime, int sekcija, int vrednost, char doseg, int velicina, int tip, string rwx);
	string toString();
	bool hasX();

	void setDoseg(char doseg) {
		this->doseg = doseg;
	}

	void setVelicina(int velicina) {
		this->velicina = velicina;
	}

	string getIme(){
		return this->ime;
	}

	int getTip(){
		return this->tip;
	}

	static int getIndeks(){
		return globRedBr;
	}

	int getRedBr(){
		return redbr;
	}

	int getVred(){
		return vrednost;
	}

	char getDoseg(){
		return this->doseg;
	}

	int getSekcija(){
		return this->sekcija;
	}
};



#endif /* SYMBOL_H_ */
