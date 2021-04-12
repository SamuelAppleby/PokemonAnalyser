/*		 Created By Samuel Buzz Appleby
 *               06/02/2021
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

int main() {
    cout << "Loading Files..." << endl;
    ifstream typeSteam = LoadFile("typechart.txt");
    if (!typeSteam)
        return -1;

    cout << "Creating Type Manager..." << endl;
    TypeManager manager(typeSteam);
    manager.AnalyseResults();
    //manager.OutputResults();
   
    return 0;
}