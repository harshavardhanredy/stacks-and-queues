//
//  filetest.cpp
//
//  Created by Ethan Lusterman on 3/17/13.
//  The Cooper Union, Electrical Engineering Dept
//  ECE164 Data Structures & Algorithms I, Spring 2013

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void parseInputStr(string line, string& arg0, string& arg1, string& arg2);

int main() {
	string line;
	string inFile;
	
	cout << "Enter name of input file: ";
	cin >> inFile;
	
	// input file stream
	ifstream in(inFile.c_str());
	
	if ( in.is_open() ) {
		while ( getline(in,line) ) {
			string a0, a1, a2;
			parseInputStr(line, a0, a1, a2);
			cout << a0 << "\t\t" << a1 << "\t\t" << a2 << endl;
		}
	}
	else {
		cout << inFile.c_str() << " is not a valid filename!" << endl;
	}
	
	return 0;
}

void parseInputStr(string line, string& arg0, string& arg1, string& arg2) {
	istringstream stream(line);
	stream >> arg0 >> arg1 >> arg2;
}