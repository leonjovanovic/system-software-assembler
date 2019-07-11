
//TODO .long etc direktive ubaciti simbole i sekcije kao varijable//srediti operacije nad labelama tj sekcijama

//TODO u svaku instrukciju drugog prolaza proveriti da li se nalazi u BSS sekciji

//TODO u ALLIGN ukoliko je u text sekciji ne upisivati 0 vec neku nop instrukciju


#include "asembler.h"
#include "Symbol.h"
#include "sekcija.h"
#include <regex>
#include <sstream>
#include <cmath>
#include <ctype.h>

list<string> Asembler::inputText = list<string>();
list<Symbol*> Asembler::listaSimbola = list<Symbol*>();
list<Sekcija*> Asembler::listaSekcija = list<Sekcija*>();
int Asembler::lc =0, Asembler::sekcija = 0, Asembler::relSekcija = -1;
bool Asembler::end=false;

string Asembler::prviProlaz() {
	listaSimbola.push_back(new Symbol("UND", 0, 0, 'l', 0, 0, "rw"));
	list<string>::iterator it = inputText.begin();
	for(int i = 0; i<inputText.size(); i++){
		string line = *it;
		advance(it, 1);
		if(line.empty()) continue;
		if(end)break;
		line = izbrisiBlanko(line);
		//cout<<line<<'\n';
		detektujSimbol(line);
	}
	return "";
}

string Asembler::drugiProlaz() {
	string drugiProlaz;
	lc = 0;
	sekcija = 0;
	end = false;

	list<string>::iterator it = inputText.begin();
	for(int i = 0; i<inputText.size(); i++){
		string line = *it;
		advance(it, 1);
		if(line.empty()) continue;
		if(end)break;
		line = izbrisiBlanko(line);
		//cout<<line<<'\n';
		detektujSimbol2(line);
	}


	drugiProlaz.append(ispisiTabelu());
	//upisivanje tabela
	list<Sekcija*>::iterator it2 = listaSekcija.begin();
	for(int i = 0; i<listaSekcija.size(); i++){
		Sekcija* sek = *it2;
		advance(it2, 1);
		drugiProlaz.append(sek->ispisiRelTabele()+'\n');
	}
	it2 = listaSekcija.begin();
	for(int i = 0; i<listaSekcija.size(); i++){
		Sekcija* sek = *it2;
		advance(it2, 1);
		drugiProlaz.append(sek->ispisiSekTabele()+'\n');
	}
	obrisiSekcije();
	return drugiProlaz;
}

string Asembler::ispisiTabelu(){
	string prolaz;
	prolaz.append("NAME | TYPE | VALUE | SECTION | SCOPE | INDEX | SIZE | RWX \n");
	//cout<<prolaz;
	list<Symbol*>::iterator it2 = listaSimbola.begin();
	for(int i = 0; i<listaSimbola.size(); i++){
		Symbol* sym = *it2;
		advance(it2, 1);
		prolaz.append(sym->toString());
		//cout<<sym->toString();
	}
	//cout<<"\n\n";
	prolaz.append("\n\n");
	return prolaz;
}

string Asembler::izbrisiBlanko(string str){
	int count = 0;
	string t;
	bool flag= false;
	for (int i = 0; str[i]; i++){
	        if ((str[i] == ' ' || str[i] == '\t') && !flag)
	        	continue;
	        else
	        	flag=true;
	        t  += str[i];
	}
	//t += '\0';
	return t;
}

string Asembler::izbrisiSveBlanko(string input){
	input.erase(std::remove(input.begin(),input.end(),' '),input.end());
	input.erase(std::remove(input.begin(),input.end(),'\t'),input.end());
	return input;
}

void Asembler::detektujSimbol(string linija){
	if(linija[0]=='.'){	//.extern .data .text .bss .equ .section
		unesiDirektivu(linija);
	}
	else{				//labela
		int poz = proveriLabelu(linija);
		if(poz!=-1)
			unesiLabelu(linija, poz);
		else
			unesiInstrukciju(linija);
	}
}

void Asembler::detektujSimbol2(string linija){
	if(linija[0]=='.'){	//.extern .data .text .bss .equ .section
		unesiDirektivu2(linija);
	}
	else{				//labela
		int poz = proveriLabelu(linija);
		if(poz!=-1)
			unesiLabelu2(linija, poz);
		else
			unesiInstrukciju2(linija);
	}
}

int Asembler::proveriLabelu(string str){
	for (int i = 0; str[i]; i++){
		if(str[i]==':') return i;
	}
	return -1;
}

Symbol* Asembler::vecPostoji(string ime, int tip){
	list<Symbol*>::iterator it = listaSimbola.begin();
	for(int i = 0; i<listaSimbola.size(); i++){
		Symbol* sym = *it;
		advance(it, 1);
		if(sym->getIme()==ime && sym->getTip()==tip){
			//cout<<"Error zadata labela vec postoji!"<<'\n';
			return sym;
		}
	}
	return NULL;
}

void Asembler::unesiLabelu(string linija, int poz){
	string ime = linija.substr(0,poz);
	string instrukcija = linija.substr(poz+1);

	//Da li vec postoji taj simbol
	list<Symbol*>::iterator it = listaSimbola.begin();
	for(int i = 0; i<listaSimbola.size(); i++){
		Symbol* sym = *it;
		advance(it, 1);
		if(sym->getIme()==ime && sym->getTip()==1){
			//cout<<"Error zadata labela vec postoji!"<<'\n';
			return;
		}
	}

	instrukcija = izbrisiBlanko(instrukcija);
	Symbol* sym = new Symbol(ime, sekcija, lc, 'l', 0, 1, "");
	listaSimbola.push_back(sym);
	if(instrukcija[0]!='\0')
		detektujSimbol(instrukcija);
}

void Asembler::unesiLabelu2(string linija, int poz){
	string ime = linija.substr(0,poz);
	string instrukcija = linija.substr(poz+1);
	instrukcija = izbrisiBlanko(instrukcija);
	if(instrukcija[0]!='\0')
		detektujSimbol2(instrukcija);
}

int Asembler::imeDirektive(string str){
	for (int i = 0; str[i]; i++){
		if(str[i]==' ' || str[i]=='\t') return i;
	}
	return -1;
}

void Asembler::unesiDirektivu(string linija){
	int poz = imeDirektive(linija);
	string ime = string(linija.c_str()), operandi;
	if(poz != -1){
		ime = linija.substr(0, poz), operandi = linija.substr(poz+1);
	}
	string op, op1, op2;
	int i=0,j=0;
	bool flag=false;
	Symbol* temp;
	regex reg;
	smatch match;
	list<Symbol*>::iterator it;
	switch(hashit(ime)){
	case EXTERN:
		i=0;
		while(operandi[i]!='\0'){
			j=i;//pocetak
			while(operandi[i]!='\0' && operandi[i]!=',')i++;
			op =operandi.substr(j,i-j);
			op = izbrisiBlanko(op);
			op= string(op.c_str());//NE ZNAM ZASTO OVO MORA
			if(operandi[i]==',')i++;//ako je , pomeramo na sledeci char jer ako ostavimo bez ovog moze da dodje do kraja i doda +1
			listaSimbola.push_back(temp = new Symbol(op, 0, lc, 'g', 0, 1, ""));
		}
		break;
	case EQU:
		reg="\s*(\\w+)\s*,\s*(\\w+)\s*";
		if (regex_search(operandi, match, reg)){
		    stringstream str(match[2]); str >> i;

			//Da li vec postoji taj simbol
			it = listaSimbola.begin();
			for(int i = 0; i<listaSimbola.size(); i++){
				Symbol* sym = *it;
				advance(it, 1);
				if(sym->getIme()==match[1] && sym->getTip()==1){
					//cout<<"Error zadata labela vec postoji!"<<'\n';
					return;
				}
			}

			listaSimbola.push_back(temp = new Symbol(match[1], sekcija, i, 'l', 0, 1, ""));
		}
		break;
	case SECTION:
		reg="\\w+";
		while (regex_search(operandi, match, reg)){
			if(op.empty())op=match[0];
			else if(op1.empty())op1=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			i++;
		}
		it = listaSimbola.begin();
		for(int i = 0; i<listaSimbola.size(); i++){
			Symbol* sym = *it;
			advance(it, 1);
			if(sym->getIme()==op && sym->getTip()==0)flag=true;
		}
		if(flag){
			//cout<<"Error ime vec postoji!"<<'\n';
			break;
		}
		it = listaSimbola.begin();
		advance(it, sekcija);
		temp = *it;
		temp->setVelicina(lc);
		lc = 0;
		sekcija = Symbol::getIndeks();
		if(op1.empty())listaSimbola.push_back(new Symbol(op, sekcija, 0, 'l', 0, 0, "rw"));
		else listaSimbola.push_back(new Symbol(op, sekcija, 0, 'l', 0, 0, op1));
		break;
	case TEXT:
		//storujemo stari lc
		it = listaSimbola.begin();
		advance(it, sekcija);
		temp = *it;
		temp->setVelicina(lc);
		lc = 0;
		sekcija = Symbol::getIndeks();
		listaSimbola.push_back(new Symbol("text", sekcija, 0, 'l', 0, 0, "x"));
		break;
	case DATA:
		it = listaSimbola.begin();
		advance(it, sekcija);
		temp = *it;
		temp->setVelicina(lc);
		lc = 0;
		sekcija = Symbol::getIndeks();
		listaSimbola.push_back(new Symbol("data", sekcija, 0, 'l', 0, 0, "rw"));
		//string ime, int sekcija, int vrednost, char doseg, int velicina, int tip
		break;
	case BSS:
		it = listaSimbola.begin();
		advance(it, sekcija);
		temp = *it;
		temp->setVelicina(lc);
		lc = 0;
		sekcija = Symbol::getIndeks();
		listaSimbola.push_back(new Symbol("bss", sekcija, 0, 'l', 0, 0, "rw"));
		break;
	case END:
		end = true;
		it = listaSimbola.begin();
		advance(it, sekcija);
		temp = *it;
		temp->setVelicina(lc);
		break;
	case CHAR:
	case BYTE:
		reg="([0-9]+|[a-zA-Z][a-zA-Z0-9]*)";

		it=listaSimbola.begin();
		advance(it,sekcija);
		temp = *it;
		if(temp->getIme()=="bss")break;

		while (regex_search(operandi, match, reg)){
		    op=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			i++;
		}
		lc+=i;
		break;
	case WORD:
	case SHORT:
		reg="([0-9]+|[a-zA-Z][a-zA-Z0-9]*)";

		it=listaSimbola.begin();
		advance(it,sekcija);
		temp = *it;
		if(temp->getIme()=="bss")break;

		while (regex_search(operandi, match, reg)){
		    op=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			i++;
		}
		lc+=i*2;
		break;
	case ALIGN:
		reg="\\d+";

		it=listaSimbola.begin();
		advance(it,sekcija);
		temp = *it;
		if(temp->getIme()=="bss")break;

		while (regex_search(operandi, match, reg)){
		    if(op.empty())op=match[0];
		    else if(op1.empty())op1=match[0];
		    else if(op2.empty())op2=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			i++;
		}
		//cout<<lc;
		j=(int)pow(2,atoi(op.c_str()));
		if(i==3 && j>atoi(op2.c_str()))break;
		if(lc%j!=0){
			lc+=j-(lc%j);
		}
		break;
	case SKIP:
		reg="\s*(\\w+)\s*";
		if (regex_search(operandi, match, reg)){
			stringstream str(match[1]); str >> i;
			lc += i;
		}
		break;
	case INT:
	case LONG:
		reg="([0-9]+|[a-zA-Z][a-zA-Z0-9]*)";

		it=listaSimbola.begin();
		advance(it,sekcija);
		temp = *it;
		if(temp->getIme()=="bss")break;

		while (regex_search(operandi, match, reg)){
		    op=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			i++;
		}
		lc+=i*4;
		break;
	case RODATA:
		it = listaSimbola.begin();
		advance(it, sekcija);
		temp = *it;
		temp->setVelicina(lc);
		lc = 0;
		sekcija = Symbol::getIndeks();
		listaSimbola.push_back(new Symbol("rodata", sekcija, 0, 'l', 0, 0, "r"));
		break;
	case GLOBAL:
		//cout<<"Global direktiva"<<'\n';
		break;
	}
}

void Asembler::unesiDirektivu2(string linija){
	int poz = imeDirektive(linija);
	string ime = string(linija.c_str()), operandi;
	if(poz != -1){
		ime = linija.substr(0, poz), operandi = linija.substr(poz+1);
	}
	string op, op1, op2;
	int i=0,j=0,k=0;
	bool flag=false;
	Symbol* temp;
	Sekcija* temp2;
	regex reg;
	smatch match;
	list<Symbol*>::iterator it;
	list<Sekcija*>::iterator it2;
	redSekcije* rs;
	switch(hashit(ime)){
	case EXTERN:
		break;
	case EQU:
		break;
	case SECTION:
		reg="\\w+";
		while (regex_search(operandi, match, reg)){
			if(op.empty())op=match[0];
			else if(op1.empty())op1=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			i++;
		}
		relSekcija++;
		lc = 0;
		listaSekcija.push_back(new Sekcija(op));
		break;
		break;
	case TEXT:
		relSekcija++;
		lc = 0;
		listaSekcija.push_back(new Sekcija("text"));
		break;
	case DATA:
		relSekcija++;
		lc = 0;
		listaSekcija.push_back(new Sekcija("data"));
		break;
	case BSS:
		relSekcija++;
		lc = 0;
		listaSekcija.push_back(new Sekcija("bss"));
		break;
	case END:
		end = true;
		break;
	case CHAR:
	case BYTE:
		reg="([0-9]+|[a-zA-Z][a-zA-Z0-9]*)";
		if(relSekcija==-1)break;
		flag=false;//da samo jednom pravi obj
		while (regex_search(operandi, match, reg)){
			op=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			if(atoi(op.c_str())>255){
				cout<<"Error broj preveliki!\n";
				continue;
			}
			it2=listaSekcija.begin();
			advance(it2,relSekcija);
			temp2 = *it2;
			if(!flag){
				rs = new redSekcije();
				temp2->dodajRedSekcije(rs);
				flag=true;
			}
			temp = vecPostoji(string(op), 1);
			if(temp==NULL) temp = vecPostoji(string(op), 0);
			if(temp==NULL){
				rs->dodajHex(atoi(op.c_str()));
				lc++;
			}
			else{//labela
				if(temp->getDoseg()=='g'){
					rs->dodajHex(0);//immed
					temp2->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getRedBr()));
					lc+=1;;
				}
				else{
					rs->dodajHex(temp->getVred());
					temp2->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getSekcija()));
					lc+=1;
				}
			}
		}
		break;
	case WORD:
	case SHORT:
		reg="([0-9]+|[a-zA-Z][a-zA-Z0-9]*)";
		if(relSekcija==-1)break;
		flag=false;//da samo jednom pravi obj
		while (regex_search(operandi, match, reg)){
			op=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			if(atoi(op.c_str())>65535){
				cout<<"Error broj preveliki!\n";
				continue;
			}
			it2=listaSekcija.begin();
			advance(it2,relSekcija);
			temp2 = *it2;
			if(!flag){
				rs = new redSekcije();
				temp2->dodajRedSekcije(rs);
				flag=true;
			}
			temp = vecPostoji(string(op), 1);
			if(temp==NULL) temp = vecPostoji(string(op), 0);
			if(temp==NULL){
				rs->dodajHex2(atoi(op.c_str()));
				lc+=2;
			}
			else{//labela
				if(temp->getDoseg()=='g'){
					rs->dodajHex2(0);//immed
					temp2->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getRedBr()));
					lc+=2;;
				}
				else{
					rs->dodajHex2(temp->getVred());
					temp2->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getSekcija()));
					lc+=2;
				}
			}
		}
		break;
	case ALIGN:
		reg="\\d+";
		if(relSekcija==-1)break;
		it2=listaSekcija.begin();
		advance(it2,relSekcija);
		temp2 = *it2;
		if(temp2->getIme()=="bss")break;

		while (regex_search(operandi, match, reg)){
			if(op.empty())op=match[0];
			else if(op1.empty())op1=match[0];
			else if(op2.empty())op2=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			i++;
		}
		j=(int)pow(2,atoi(op.c_str()));
		if(i==3 && j>atoi(op2.c_str()))break;
		if(lc%j!=0){
			k=j-(lc%j);
			lc+=k;
			rs = new redSekcije();
			for(int s=0; s<k;s++)
				if(op1.empty())
					rs->dodajHex(0);
				else
					rs->dodajHex(atoi(op1.c_str()));
			temp2->dodajRedSekcije(rs);
		}
		break;
	case SKIP:
		reg="\s*(\\w+)\s*";
		if(relSekcija==-1)break;
		if (regex_search(operandi, match, reg)){
			stringstream str(match[1]); str >> i;
			lc += i;

			it2=listaSekcija.begin();
			advance(it2,relSekcija);
			temp2 = *it2;
			rs = new redSekcije();
			for(int s=0; s<i;s++)
					rs->dodajHex(0);
			temp2->dodajRedSekcije(rs);
		}
		break;
	case INT:
	case LONG:
		reg="([0-9]+|[a-zA-Z][a-zA-Z0-9]*)";
		if(relSekcija==-1)break;
		flag=false;//da samo jednom pravi obj
		while (regex_search(operandi, match, reg)){
			op=match[0];
			operandi=regex_replace(operandi, reg, "", regex_constants::format_first_only);
			if(atoi(op.c_str())>2147483647){
				cout<<"Error broj preveliki!\n";
				continue;
			}
			it2=listaSekcija.begin();
			advance(it2,relSekcija);
			temp2 = *it2;
			if(!flag){
				rs = new redSekcije();
				temp2->dodajRedSekcije(rs);
				flag=true;
			}
			temp = vecPostoji(string(op), 1);
			if(temp==NULL) temp = vecPostoji(string(op), 0);
			if(temp==NULL){
				rs->dodajHex4(atoi(op.c_str()));
				lc+=4;
			}
			else{//labela
				if(temp->getDoseg()=='g'){
					rs->dodajHex4(0);//immed
					temp2->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getRedBr()));
					lc+=4;;
				}
				else{
					rs->dodajHex4(temp->getVred());
					temp2->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getSekcija()));
					lc+=4;
				}
			}
		}
		break;
	case RODATA:
		relSekcija++;
		lc = 0;
		listaSekcija.push_back(new Sekcija("rodata"));
		break;
	case GLOBAL:
		i=0;
		while(operandi[i]!='\0'){
			j=i;//pocetak
			while(operandi[i]!='\0' && operandi[i]!=',')i++;
			op =operandi.substr(j,i-j);
			op = izbrisiBlanko(op);
			op= string(op.c_str());//NE ZNAM ZASTO OVO MORA
			if(operandi[i]==',')i++;//ako je , pomeramo na sledeci char jer ako ostavimo bez ovog moze da dodje do kraja i doda +1
			it = listaSimbola.begin();
			flag=false;
			for(int k = 0; k<listaSimbola.size(); k++){
				Symbol* sym = *it;
				advance(it, 1);
				if(sym->getIme()==op && sym->getTip()==1){
					sym->setDoseg('g');
					flag=true;
				}
			}
			//if(!flag) cout<<"Error globalna promenjljiva ne postoji!"<<'\n';
		}
		break;
	}
}

int Asembler::razdvojOperande(string str){
	for (int i = 0; str[i]; i++){
		if(str[i]==',') return i;
	}
	return -1;
}

void Asembler::unesiInstrukciju(string linija){
	int poz = imeDirektive(linija);
	string ime = string(linija.c_str()), operandi="", op1="", op2="";
	if(poz != -1){
		ime = linija.substr(0, poz), operandi = linija.substr(poz+1);
		op1=izbrisiSveBlanko(operandi);

		poz = razdvojOperande(op1);
		op2="";
		if(poz!=-1){
			op2 = op1.substr(poz+1);
			op1 = op1.substr(0, poz);
		}
	}
	if(!smeInstrukcija())return;
	int bajtovi = 1;
	int bajt1 = adresiranjeBrojBajtova(ime, detektujAdresiranje(op1)) +1; //ako nema operanda vraca -1 sto se ponistava sa +1
	int bajt2 = adresiranjeBrojBajtova(ime, detektujAdresiranje(op2)) +1;
	bajtovi += bajt1;
	bajtovi += bajt2;
	lc+= bajtovi;
}

void Asembler::unesiInstrukciju2(string linija){
	int poz = imeDirektive(linija);
	string ime = string(linija.c_str()), operandi="", op1="", op2="";
	if(poz != -1){
		ime = linija.substr(0, poz), operandi = linija.substr(poz+1);
		op1=izbrisiSveBlanko(operandi);

		poz = razdvojOperande(op1);
		op2="";
		if(poz!=-1){
			op2 = op1.substr(poz+1);
			op1 = op1.substr(0, poz);
		}
	}

	list<Sekcija*>::iterator it = listaSekcija.begin();
	advance(it,relSekcija);
	Sekcija* sek = *it;
	redSekcije* red;
	int kod;

	switch(hashitI(ime)){
	case HALT:
		red = new redSekcije();
		red->dodajHex(0x8);
		lc++;
		sek->dodajRedSekcije(red);
		break;

	case XCHG:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<=0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x10;
		else
			kod = 0x14;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)));//cout<<"Error los unos operanada";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)));//cout<<"Error los unos operanada";
		sek->dodajRedSekcije(red);
		break;

	case INTI:
		if(detektujAdresiranje(op1)<0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		kod = 0x1c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		sek->dodajRedSekcije(red);
		break;

	case MOV:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x20;
		else
			kod = 0x24;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1 "+ime;
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2 "+ime;
		sek->dodajRedSekcije(red);
		break;

	case ADD:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x28;
		else
			kod = 0x2c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;

	case SUB:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x30;
		else
			kod = 0x34;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;

	case MUL:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x38;
		else
			kod = 0x3c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;

	case DIV:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x40;
		else
			kod = 0x44;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;
	case CMP:
		if(detektujAdresiranje(op1)<0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x48;
		else
			kod = 0x4c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;
	case NOT:
		if(detektujAdresiranje(op1)<=0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x50;
		else
			kod = 0x54;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		sek->dodajRedSekcije(red);
		break;
	case AND:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x58;
		else
			kod = 0x5c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;
	case OR:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x60;
		else
			kod = 0x64;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;
	case XOR:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x68;
		else
			kod = 0x6c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;
	case TEST:
		if(detektujAdresiranje(op1)<0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x70;
		else
			kod = 0x74;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;
	case SHL:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x78;
		else
			kod = 0x7c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;
	case SHR:
		if(detektujAdresiranje(op1)<=0 || detektujAdresiranje(op2)<0){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x80;
		else
			kod = 0x84;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		if(!obradiOp(sek,red,op2,daLiJeB(ime)))cout<<"Error los unos operanada2\n";
		sek->dodajRedSekcije(red);
		break;
	case PUSH:
		if(detektujAdresiranje(op1)<0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x88;
		else
			kod = 0x8c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		sek->dodajRedSekcije(red);
		break;
	case POP:
		if(detektujAdresiranje(op1)<=0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		if(daLiJeB(ime))
			kod = 0x90;
		else
			kod = 0x94;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		sek->dodajRedSekcije(red);
		break;
	case JMP:
		if(detektujAdresiranje(op1)<0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		kod = 0x9c;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		sek->dodajRedSekcije(red);
		break;
	case JEQ:
		if(detektujAdresiranje(op1)<0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		kod = 0xa4;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1";
		sek->dodajRedSekcije(red);
		break;
	case JNE:
		if(detektujAdresiranje(op1)<0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		kod = 0xac;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		sek->dodajRedSekcije(red);
		break;
	case JGT:
		if(detektujAdresiranje(op1)<0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		kod = 0xb4;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1";
		sek->dodajRedSekcije(red);
		break;
	case CALL:
		if(detektujAdresiranje(op1)<0 || op2!=""){
			cout<<"Error los broj operanada ili neposredno kao destinacija!\n";
			return;
		}
		kod = 0xbc;
		red = new redSekcije();
		red->dodajHex(kod);
		lc++;
		if(!obradiOp(sek,red,op1,daLiJeB(ime)))cout<<"Error los unos operanada1\n";
		sek->dodajRedSekcije(red);
		break;
	case RET:
		red = new redSekcije();
		red->dodajHex(0xc0);
		lc++;
		sek->dodajRedSekcije(red);
		break;
	case IRET:
		red = new redSekcije();
		red->dodajHex(0xc8);
		lc++;
		sek->dodajRedSekcije(red);
		break;
	}
}

bool Asembler::obradiOp(Sekcija* sek,redSekcije* red, string op, bool isB){
	regex reg;
	smatch match;
	int kod;

	reg="^\\$[a-zA-z][0-9a-zA-z]*$"; 		//$a u pc[a]
	if(regex_search(op, match, reg)){
		string s = string(match[0]);
		s=s.substr(1);
		red->dodajHex(0x8e);//operand
		lc++;
		Symbol* temp = vecPostoji(s, 1);
		if(temp){
			if(temp->getDoseg()=='g'){
				red->dodajHex2(-2);//immed
				sek->dodajRedRelTabele(new redRelTabele(lc, "R_386_PC32", temp->getRedBr()));
				lc+=2;;
			}
			else{
				kod = -2;
				kod+=temp->getVred();
				red->dodajHex2(kod);
				sek->dodajRedRelTabele(new redRelTabele(lc, "R_386_PC32", temp->getSekcija()));
				lc+=2;
			}
			return true;
		}
		return false;
	}
	//*********NEPOSREDNO***********
	//0x1234
	reg="^0x[0-9a-fA-F]+$";
	if(regex_search(op, match, reg)){
		string s = string(match[0]);
		red->dodajHex(0x00);//operand
		lc++;
		if(!isB){
			red->dodajHex2(stoi(s,0,16));
			lc+=2;
		}
		else{
			if(s.size()<=4){
				red->dodajHex(stoi(s,0,16));
				lc++;
			}
			else return false;
		}
		return true;
	}//-3/3
	reg="^-?[0-9]+$";
	if(regex_search(op, match, reg)){
		int s = atoi(string(match[0]).c_str());
		red->dodajHex(0x00);//operand
		lc++;
		if(!isB){
			red->dodajHex2(s);
			lc+=2;
		}
		else{
			if(s>=-128 && s<128){
				red->dodajHex(s);
				lc++;
			}
			else return false;
		}
		return true;

	}//$a
	reg="^&[a-zA-Z][a-zA-Z0-9]*$";
	if(regex_search(op, match, reg)){
		red->dodajHex(0x00);//operand
		lc++;
		string ime = string(match[0]).substr(1);
		Symbol* temp = vecPostoji(ime, 1);
		if(temp){
			if(temp->getDoseg()=='g'){
				red->dodajHex2(0x00);//immed
				sek->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getRedBr()));
				lc+=2;;
			}
			else{
				red->dodajHex2(temp->getVred());
				sek->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getSekcija()));
				lc+=2;
			}
			return true;
		}
		return false;
	}
	//******REG DIR***********
	//r4/pc/sp
	reg="^(r[0-7]|sp|pc)(l|h)?$";
	if(regex_search(op, match, reg)) {
		string reg = match[0];
		int regBr,high=0;
		if(isdigit(reg[1]))regBr = reg[1]-'0';
		else if(reg[0]=='p') regBr = 7;
		else if(reg[0]=='s') regBr=6;
		else return false;
		if(reg[reg.size()-1]=='h')high = 1;
		kod=(1<<5)|(regBr<<1)|high;
		red->dodajHex(kod);//regdir
		lc++;
		return true;
	}

	reg="^\\[(r[0-7]|sp|pc)\\]$";
	if(regex_search(op, match, reg)) {
		string reg = string(match[0]).substr(1, 2);
		int regBr;
		if(isdigit(reg[1]))regBr = reg[1]-'0';
		else if(reg[0]=='p') regBr = 7;
		else if(reg[0]=='s') regBr=6;
		else return false;
		kod=(2<<5)|(regBr<<1);
		red->dodajHex(kod);//regdir
		lc++;
		return true;
	}
	//********REGIND*************
	reg="^(r[0-7]|sp|pc)\\[(-?[0-9]+)\\]$";
	if(regex_search(op, match, reg)){
		string reg = match[1];
		int regBr;
		if(isdigit(reg[1]))regBr = reg[1]-'0';
		else if(reg[0]=='p') regBr = 7;
		else if(reg[0]=='s') regBr=6;
		else return false;
		int s = atoi(string(match[2]).c_str());
		if(s>=-128 && s<128){
			kod=(3<<5)|(regBr<<1);
			red->dodajHex(kod);//regind
			lc++;
			red->dodajHex(s);//reginds
			lc++;
			return true;
		}
		else if(s>=-32768 && s<32768){
			kod=(4<<5)|(regBr<<1);
			red->dodajHex(kod);//regdir
			lc++;
			red->dodajHex2(s);//regdir
			lc+=2;
			return true;
		}
		return false;
	}

	reg="^(r[0-7]|sp|pc)\\[([a-zA-z][0-9a-zA-z]*)\\]$";
	if(regex_search(op, match, reg)) {
		string reg = match[1];
		int regBr;
		if(isdigit(reg[1]))regBr = reg[1]-'0';
		else if(reg[0]=='p') regBr = 7;
		else if(reg[0]=='s') regBr=6;
		else return false;
		string s = string(match[2]).c_str();
		kod=(4<<5)|(regBr<<1);
		red->dodajHex(kod);//regind
		lc++;

		Symbol* temp = vecPostoji(s, 1);
		if(temp){
			if(temp->getDoseg()=='g'){
				red->dodajHex2(0x00);//immed
				sek->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getRedBr()));
				lc+=2;;
			}
			else{
				red->dodajHex2(temp->getVred());
				sek->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getSekcija()));
				lc+=2;
			}
			return true;
		}
		return false;
		return 4;
	}
	//*********MEMDIR****************
	reg="^([a-zA-z][0-9a-zA-z]*|\\*([0-9]*))$";
	if(regex_search(op, match, reg)){
		if(match[2]!=""){
			red->dodajHex(5<<5);
			lc++;
			int s = atoi(string(match[2]).c_str());
			if(s>=0 && s<65536) {
				red->dodajHex2(s);
				lc+=2;
				return true;
			}
		}
		else {
			red->dodajHex(5<<5);
			lc++;
			string ime = match[0];
			Symbol* temp = vecPostoji(ime, 1);
			if(temp){
				if(temp->getDoseg()=='g'){
					red->dodajHex2(0x00);
					sek->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getRedBr()));
					lc+=2;;
				}
				else{
					red->dodajHex2(temp->getVred());
					sek->dodajRedRelTabele(new redRelTabele(lc, "R_386_32", temp->getSekcija()));
					lc+=2;
				}
				return true;
			}
			return false;
		}
	}
	//if(op!="")cout<<"Error pogresno adresiranje";
	return false;
}

bool Asembler::smeInstrukcija(){
	Symbol* temp;
	list<Symbol*>::iterator it = listaSimbola.begin();
	advance(it,sekcija);
	temp = *it;
	if(temp->hasX()) return true;
	//cout<<"Error instrukcija u nedozvoljenoj sekciji!"<<'\n';
	return false;
}

int Asembler::detektujAdresiranje(string op){//dodati errore
	regex reg;
	smatch match;

	reg="^\\$[a-zA-z][0-9a-zA-z]*$";
	if(regex_search(op, match, reg)){
		return 4;
	}

	reg="^0x[0-9a-fA-F]+$";
	if(regex_search(op, match, reg)){
		string s = string(match[0]);
		if(s.size()<=6)return 0;
	}
	reg="^-?[0-9]+$";
	if(regex_search(op, match, reg)){
		int s = atoi(string(match[0]).c_str());
		if(s>=-32768 && s<32768) return 0;
	}
	reg="^&[a-zA-Z][a-zA-Z0-9]*$";
	if(regex_search(op, match, reg)){
		return 0;
	}

	reg="^(r[0-7]|sp|pc)(?:l|h)$";
	if(regex_search(op, match, reg))
		return 1;

	reg="^\\[(r[0-7]|sp|pc)\\]$";
	if(regex_search(op, match, reg)) return 2;

	reg="^(r[0-7]|sp|pc)\\[(-?[0-9]+)\\]$";
	if(regex_search(op, match, reg)){
		int s = atoi(string(match[2]).c_str());
		if(s>=-128 && s<128)return 3;
		else if(s>=-32768 && s<32768) return 4;
	}

	reg="^(r[0-7]|sp|pc)\\[[a-zA-z][0-9a-zA-z]*\\]$";
	if(regex_search(op, match, reg)) {
		return 4;
	}

	reg="^([a-zA-z][0-9a-zA-z]*|\\*([0-9]*))$";
	if(regex_search(op, match, reg)){
		if(match[2]!=""){
			int s = atoi(string(match[2]).c_str());
			if(s>=0 && s<65536) return 5;
		}
		else return 5;
	}
	//if(op!="")cout<<"Error pogresno adresiranje";
	return -1;
}

int Asembler::adresiranjeBrojBajtova(string op, int i){
	if(i==0){
		if(op[op.size()-1]=='b'){
			if(op=="sub") return 2;
			return 1;
		}
		else return 2;
	}
	else if(i==1 || i==2) return 0;
	else if(i==3)return 1;
	else if(i==4 || i == 5)return 2;
	return -1;
}

bool Asembler::daLiJeB(string ime){
	if(ime[ime.size()-1]=='b'){
		if(ime == "sub") return false;
		return true;
	}
	else return false;
}

void Asembler::obrisiSimbole(){
	list<Symbol*>::iterator it = listaSimbola.begin();
	for(int i = 0; i<listaSimbola.size(); i++){
		Symbol* sym = *it;
		advance(it, 1);
		delete sym;
	}
}

void Asembler::obrisiSekcije(){
	list<Sekcija*>::iterator it = listaSekcija.begin();
	for(int i = 0; i<listaSekcija.size(); i++){
		Sekcija* sek = *it;
		advance(it, 1);
		delete sek;
	}
}

Direktive Asembler::hashit (string inString){
    if (inString == ".extern") return EXTERN;
    if (inString == ".equ") return EQU;
    if (inString == ".section") return SECTION;
    if (inString == ".text") return TEXT;
    if (inString == ".data") return DATA;
    if (inString == ".bss") return BSS;
    if (inString == ".end") return END;
    if (inString == ".byte") return BYTE;
    if (inString == ".word") return WORD;
    if (inString == ".short") return SHORT;
    if (inString == ".align") return ALIGN;
    if (inString == ".skip") return SKIP;
    if (inString == ".int") return INT;
    if (inString == ".long") return LONG;
    if (inString == ".char") return CHAR;
    if (inString == ".rodata") return RODATA;
    if (inString == ".global") return GLOBAL;
}

Instrukcije Asembler::hashitI (string inString){
    if (inString == "halt") return HALT;
    if (inString == "xchg" || inString=="xchgb" || inString=="xchgw") return XCHG;
    if (inString == "inti") return INTI;
    if (inString == "mov" || inString=="movb" || inString=="movw") return MOV;
    if (inString == "add" || inString=="addb" || inString=="addw") return ADD;
    if (inString == "sub" || inString=="subb" || inString=="subw") return SUB;
    if (inString == "mul" || inString=="mulb" || inString=="mulw") return MUL;
    if (inString == "div" || inString=="divb" || inString=="divw") return DIV;
    if (inString == "cmp" || inString=="cmpb" || inString=="cmpw") return CMP;
    if (inString == "not" || inString=="notb" || inString=="notw") return NOT;
    if (inString == "and" || inString=="andb" || inString=="andw") return AND;
    if (inString == "or" || inString=="orb" || inString=="orw") return OR;
    if (inString == "xor" || inString=="xorb" || inString=="xorw") return XOR;
    if (inString == "test" || inString=="testb" || inString=="testw") return TEST;
    if (inString == "shl" || inString=="shlb" || inString=="shlw") return SHL;
    if (inString == "shr" || inString=="shrb" || inString=="shrw") return SHR;
    if (inString == "push" || inString=="pushb" || inString=="pushw") return PUSH;
    if (inString == "pop" || inString=="popb" || inString=="popw") return POP;
    if (inString == "jmp") return JMP;
    if (inString == "jeq") return JEQ;
    if (inString == "jne") return JNE;
    if (inString == "jgt") return JGT;
    if (inString == "call") return CALL;
    if (inString == "ret") return RET;
    if (inString == "iret") return IRET;
}

