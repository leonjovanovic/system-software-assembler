#include<iostream>
#include <string.h>
#include <fstream>
#include <list>
#include <iterator>
#include "asembler.h"

using namespace std;

int main(int argc, char *argv[]){
	string input, output;
	if(strcmp(argv[2],"-o")==0){
		input = argv[1];
		output = argv[3];
	}
	else if(strcmp(argv[1],"-o")==0){
		input = argv[3];
		output=argv[2];
	}
	else{
		cout<<"Nedozvoljeni argumenti";
	}

	ifstream inFile(input, ios::in);
	if (!inFile){
		cerr << "Greska u otvaranju fajla: " << input <<" \n";
	      return -1;
   }
   char linija[50];
   list <string> inputText;

	while (inFile) {
		inFile.getline(linija, 50);
		inputText.push_back(linija);
	}
	inFile.close();

	Asembler::inputText = inputText;
	string prvi = Asembler::prviProlaz();
	ofstream outFile(output, ios::out);
	string drugi = Asembler::drugiProlaz();
	cout<<drugi<<'\n';
	outFile<<drugi<<'\n';

	return 0;
}




