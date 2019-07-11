/*
 * asembler.h
 *
 *  Created on: Jun 6, 2019
 *      Author: leon
 */

#ifndef ASEMBLER_H_
#define ASEMBLER_H_

#include<iostream>
#include <list>
#include <iterator>
#include "Symbol.h"
#include "sekcija.h"
using namespace std;

enum Direktive {
    EXTERN,
    EQU,
    SECTION,
    TEXT,
	DATA,
	BSS,
	END,
	BYTE,
	WORD,
	SHORT,
	ALIGN,
	SKIP,
	INT,
	LONG,
	CHAR,
	RODATA,
	GLOBAL
};

enum Instrukcije {
	HALT,
	XCHG,
	INTI,
	MOV,
	ADD,
	SUB,
	MUL,
	DIV,
	CMP,
	NOT,
	AND,
	OR,
	XOR,
	TEST,
	SHL,
	SHR,
	PUSH,
	POP,
	JMP,
	JEQ,
	JNE,
	JGT,
	CALL,
	RET,
	IRET
};

class Asembler{
private:

public:
	static list<string> inputText;
	static list<Symbol*> listaSimbola;
	static list<Sekcija*> listaSekcija;
	static int lc, sekcija, relSekcija;
	static bool end;
	static string prviProlaz();
	static string drugiProlaz();
	static string izbrisiBlanko(string ulaz);
	static string izbrisiSveBlanko(string input);
	static void detektujSimbol(string linija);
	static void detektujSimbol2(string linija);
	static int proveriLabelu(string linija);
	static void unesiLabelu(string linija, int poz);
	static void unesiLabelu2(string linija, int poz);
	static void unesiDirektivu(string linija);
	static void unesiDirektivu2(string linija);
	static void unesiInstrukciju(string linija);
	static void unesiInstrukciju2(string linija);
	static void obrisiSimbole();
	static void obrisiSekcije();
	static Direktive hashit (string inString);
	static Instrukcije hashitI (string inString);
	static int imeDirektive(string str);
	static int razdvojOperande(string str);
	static string ispisiTabelu();
	static bool smeInstrukcija();
	static int detektujAdresiranje(string op);
	static int adresiranjeBrojBajtova(string op, int i);
	static bool daLiJeB(string ime);
	static bool obradiOp(Sekcija* sek,redSekcije* red,string op, bool isB);
	static Symbol* vecPostoji(string ime, int tip);
};



#endif /* ASEMBLER_H_ */
