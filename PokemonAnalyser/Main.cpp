/*		 Created By Samuel Buzz Appleby
 *               12/04/2021
 *			        Main			    */
#include <iostream>
#include <fstream>
#include "TypeManager.h"
using namespace std;

/* Takes in a file name and will return a file stream */
ifstream LoadFile(string fileName) {
	ifstream infile(fileName);
	if (!infile) {
		cout << fileName << " Does Not Exist";
	}
	return infile;
}

std::string UserInput() {
	std::string input;
	std::getline(std::cin, input);
	input[0] = tolower(input[0]);
	while (input.length() != 1 || (input[0] != 'y' && input[0] != 'n')) {
		std::cout << "Please Enter Y or N: ";
		std::getline(std::cin, input);
		input[0] = tolower(input[0]);
	}
	return input;
}

int main() {
	cout << "Loading Files..." << endl;
	ifstream typeSteam = LoadFile("typechart.csv");
	if (!typeSteam) return -1;

	cout << "Creating Types..." << endl;
	TypeManager manager(typeSteam);
	typeSteam.close();

	cout << "Analysing Types..." << endl;
	manager.AnalyseTypes();

	cout << "Would you like to view the results (Y/N)?: ";
	if (UserInput()[0] == 'y') manager.Summary(false);

	cout << "Would you like a breakdown of the types (Y/N)?: ";
	if (UserInput()[0] == 'y') manager.OutputResults(false);

	std::cout << "Analysing Dual Types..." << std::endl;
	manager.AnalyseDualTypes();

	cout << "Would you like to view the results (Y/N)?: ";
	if (UserInput()[0] == 'y') manager.Summary(true);

	cout << "Would you like a breakdown of the types (Y/N)?: ";
	if (UserInput()[0] == 'y') manager.OutputResults(true);

	return 0;
}