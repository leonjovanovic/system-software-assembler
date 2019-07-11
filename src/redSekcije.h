/*
 * sekcija.h
 *
 *  Created on: Jun 12, 2019
 *      Author: leon
 */

#ifndef REDSEKCIJE_H_
#define REDSEKCIJE_H_


#include <iostream>
#include <list>
#include <iterator>
#include <sstream>
#include <vector>
using namespace std;



class redSekcije{
private:
	list<int> hex;

public:
	redSekcije();
	void dodajHex(int hex);
	void dodajHex2(int hex);
	void dodajHex4(int hex);
	string toString();
	list<int> getHex(){
		return hex;
	}
};



#endif /* REDSEKCIJE_H_ */
