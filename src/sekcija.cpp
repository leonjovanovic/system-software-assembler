/*
 * sekcija.cpp
 *
 *  Created on: Jun 12, 2019
 *      Author: leon
 */
#include"sekcija.h"

Sekcija::Sekcija(string ime){
	this->sekTabela = list<redSekcije*>();
	this->relTabela = list<redRelTabele*>();
	this->ime=ime;
}

void Sekcija::dodajRedSekcije(redSekcije *r){
	this->sekTabela.push_back(r);
}

void Sekcija::dodajRedRelTabele(redRelTabele *r){
	this->relTabela.push_back(r);
}

string Sekcija::ispisiSekTabele(){
	if(this->ime=="bss")return "";
	list<redSekcije*>::iterator it2 = this->sekTabela.begin();
	string s;
	s.append("#."+this->getIme()+'\n');
	for(int i = 0; i<this->sekTabela.size(); i++){
		redSekcije* sek = *it2;
		advance(it2, 1);
		s.append(sek->toString());
		s.append("\n");
	}
	return s;
}

string Sekcija::ispisiRelTabele(){
	if(this->ime=="bss")return "";
	list<redRelTabele*>::iterator it2 = this->relTabela.begin();
	string s;
	s.append("#.ret."+this->getIme()+'\n');
	s.append("OFFSET | TYPE | VALUE\n");
	for(int i = 0; i<this->relTabela.size(); i++){
		redRelTabele* sek = *it2;
		advance(it2, 1);
		s.append(to_string(sek->getOffset()));
		s.append(" | ");
		s.append(sek->getTip());
		s.append(" | ");
		s.append(to_string(sek->getRedBr())+'\n');
	}
	return s;
}

Sekcija::~Sekcija(){
	list<redSekcije*>::iterator it = sekTabela.begin();
	for(int i = 0; i<sekTabela.size(); i++){
		redSekcije* sek = *it;
		advance(it, 1);
		delete sek;
	}

	list<redRelTabele*>::iterator it2 = relTabela.begin();
	for(int i = 0; i<relTabela.size(); i++){
		redRelTabele* sek = *it2;
		advance(it2, 1);
		delete sek;
	}
}

