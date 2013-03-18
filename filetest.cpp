//
//  filetest.cpp
//
//  Created by Ethan Lusterman on 3/17/13.
//  The Cooper Union, Electrical Engineering Dept
//  ECE164 Data Structures & Algorithms I, Spring 2013

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	string line;
	string inFile;
	
	cout << "Enter name of input file: ";
	cin >> inFile;
	
	// input file stream
	ifstream inData(inFile.c_str());
	
	if ( inData.is_open() ) {
		while ( getline(inData,line) ) {
			cout << line << endl;
		}
	}
	else {
		cout << inFile << " is not a valid filename!" << endl;
	}
	
	return 0;
}
