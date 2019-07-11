/*
 * redRelTabele.h
 *
 *  Created on: Jun 13, 2019
 *      Author: leon
 */

#ifndef REDRELTABELE_H_
#define REDRELTABELE_H_

#include <iostream>
using namespace std;

class redRelTabele{
private:
	int relOffset;
	string relTip;
	int relRedBr;

public:
	redRelTabele(int relOffset, string relTip, int relRedBr);

	int getOffset(){
		return this->relOffset;
	}

	string getTip(){
		return this->relTip;
	}

	int getRedBr(){
		return this->relRedBr;
	}
};



#endif /* REDRELTABELE_H_ */
