/*
 * sekcija.h
 *
 *  Created on: Jun 12, 2019
 *      Author: leon
 */


#ifndef SEKCIJA_H_
#define SEKCIJA_H_

#include <iostream>
#include "redSekcije.h"
#include "redRelTabele.h"
#include <list>
#include <iterator>
using namespace std;


class Sekcija{

private:
	list<redSekcije*> sekTabela;
	list<redRelTabele*> relTabela;
	string ime;

public:
	Sekcija(string ime);
	void dodajRedSekcije(redSekcije *r);
	void dodajRedRelTabele(redRelTabele *r);
	string ispisiSekTabele();
	string ispisiRelTabele();
	~Sekcija();

	string getIme(){
		return this->ime;
	}
};



#endif /* SEKCIJA_H_ */
