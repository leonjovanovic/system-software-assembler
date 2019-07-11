/*
 * sekcija.cpp
 *
 *  Created on: Jun 12, 2019
 *      Author: leon
 */
#include "redSekcije.h"


redSekcije::redSekcije(){
	this->hex = list<int>();
}

void redSekcije::dodajHex(int h){
	this->hex.push_back(h);
}

void redSekcije::dodajHex2(int h){
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (h >> (i * 8));
	this->hex.push_back(arrayOfByte[3]);
	this->hex.push_back(arrayOfByte[2]);
}

void redSekcije::dodajHex4(int h){
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (h >> (i * 8));
	this->hex.push_back(arrayOfByte[3]);
	this->hex.push_back(arrayOfByte[2]);
	this->hex.push_back(arrayOfByte[1]);
	this->hex.push_back(arrayOfByte[0]);
}

string redSekcije::toString(){
	list<int>::iterator it = this->hex.begin();
	string s;
	stringstream hexstr;
	for(int i = 0; i<this->hex.size(); i++){
		int sek = *it;
		advance(it, 1);
		if(sek<16 && sek>=0)hexstr << 0;
		if(sek<0) sek = (sek & 0xFF);
		hexstr << std::hex << sek;
		s.append(hexstr.str()+" ");

		hexstr.str("");
		hexstr.clear();
	}
	return s;
}

