#include "Symbol.h"

int Symbol::globRedBr = 0;


Symbol::Symbol(string ime, int sekcija, int vrednost, char doseg, int velicina, int tip, string rwx){
	this->ime = ime;
	this->vrednost = vrednost;
	this->doseg = doseg;
	this->velicina = velicina;
	this->tip = tip;

	this->redbr = globRedBr++;
	this->sekcija = sekcija;
	this->rwx = rwx;
}

string Symbol::toString(){
	string s = ime+" | " +(tip?"simbol":"sekcija")+" | "+to_string(vrednost)+" | "+to_string(sekcija)+" | "+doseg+" | "+to_string(redbr)+" | "+to_string(velicina)+" | "+rwx+'\n';
	//to_string(tip)
	return s;
}

bool Symbol::hasX(){
	if(this->rwx.find_first_of('x')!=string::npos) return true;
	else return false;
}


