//
//  dsaProg1.cpp
//
//  Created by Ethan Lusterman on 3/17/13.
//  The Cooper Union, Electrical Engineering Dept
//  ECE164 Data Structures & Algorithms I, Spring 2013
//
//  This program implements Stack and Queue datatypes and takes in a formatted
//  input file of valid commands and outputs a file of relevant output and 
//  error messages.

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
using namespace std;

// function declaration (parse input line to 3 arguments)
void parseInputStr(string line, string& arg0, string& arg1, string& arg2);

// implements abstract SimpleList class as doubly linked list
template <class T>
class SimpleList {
private:
	int len;
	string name;
	class SimpleNode {
	public:
		T data;
		SimpleNode *next, *prev;
	};
	SimpleNode *head, *tail;
protected:
	void pushToHead(T item);
	void pushToTail(T item);
	T popFromHead();
	T popFromTail();
	T peekAtTail();
	T peekAtHead();
public:
	virtual void push(T item) = 0;
	virtual T pop() = 0;
	SimpleList (string s) {
		head = new SimpleNode();
		tail = new SimpleNode();
		name = s;
		head -> next = tail;
		head -> prev = NULL;
		tail -> next = NULL;
		tail -> prev = head;
		len = 0;
	}
	string getName() {return name;}
	int getLen() {return len;}
}; 

// push new node to head (beginning) of list
template <class T>
void SimpleList<T>::pushToHead(T item) {
	SimpleNode *tmp = new SimpleNode;
	tmp -> data = item;
	tmp -> next = head -> next;
	tmp -> prev = head;
	(head -> next) -> prev = tmp;
	head -> next = tmp;
	len++;
}

// pop node from head.. also deletes node to free memory
template <class T>
T SimpleList<T>::popFromHead() {
	T t1 = peekAtHead();
	SimpleNode *tmp = head -> next;
	((head -> next) -> next) -> prev = head;
	head -> next = head -> next -> next;
	delete tmp;
	len--;
	return t1;
}

// peek at head: used for testing in case you don't want to pop the value
template <class T>
T SimpleList<T>::peekAtHead() {
	return head -> next -> data;
}

// push new node to tail (end) of list
template <class T>
void SimpleList<T>::pushToTail(T item) {
	SimpleNode *tmp = new SimpleNode;
	tmp -> next = tail;
	tmp -> prev = tail -> prev;
	(tail -> prev) -> next = tmp;
	tail -> prev = tmp;
	len++;
}

// pop node from tail.. also deletes node to free memory
template <class T>
T SimpleList<T>::popFromTail() {
	T t1 = peekAtTail();
	SimpleNode *tmp = tail -> prev;
	((tail -> prev) -> prev) -> next = tail;
	tail -> prev = tail -> prev -> prev;
	delete tmp;
	len--;
	return t1;
}

// peek at tail: used for testing in case you don't want to pop the value
template <class T>
T SimpleList<T>::peekAtTail() {
	return tail -> prev -> data;
}

// derived Stack class: push and pop are both from the head. same constructor
// as SimpleList used
template <class T>
class Stack: public SimpleList<T> {
public:
	void push(T item) { SimpleList<T>::pushToHead(item); }
	T pop() { return SimpleList<T>::popFromHead(); }
	Stack<T>(string s) : SimpleList<T>(s) { } 
};

// derived Queue class: push to the head of the list, pop from the tail. same 
// constructor as SimpleList used
template <class T>
class Queue: public SimpleList<T> {
public:
	void push(T item) { SimpleList<T>::pushToHead(item); }
	T pop() { return SimpleList<T>::popFromTail(); }
	Queue<T>(string s) : SimpleList<T>(s) { }
};

// check if a list exists. update a pointer to the list if it exists in case
// you need to push or pop from/to that list
template <class T>
bool checkIfList(string listName, list<SimpleList<T> *> list2Check, SimpleList<T> **pToSL);

int main() {
 	string line;
 	string inFile;
	string outFile;
	ifstream in;
	ofstream out;
  
 	cout << "Enter name of input file: ";
 	cin >> inFile;
 	cout << "Enter name of output file: ";
 	cin >> outFile;
  
 	// update I/O file streams
 	in.open(inFile.c_str());
	out.open(outFile.c_str());

	// list of listnames
	list<SimpleList<int> *> list_i; // all int stacks/queues
	list<SimpleList<double> *> list_d; // all double stacks/queues
	list<SimpleList<string> *> list_s; // all string stacks/queues

 	if (in.is_open()) {
 		while (getline(in,line)) {
			out << "PROCESSING COMMAND: " << line << endl;
 			
			string a0, a1, a2;
 			parseInputStr(line, a0, a1, a2);
			
			int cmdNum; 
			// create = 0, push = 1, pop = 2
			cmdNum = (a0 == "create") ? 0 : (a0 == "push") ? 1 : 2;
			
			switch (cmdNum) {
				case 0: // CREATE
					// if list exists, output an ERROR
					// else, create new list and add to list of lists
					switch (a1[0]) {
						case 'i':
							SimpleList<int> *ip2SL;
							if (checkIfList(a1, list_i, &ip2SL)) {
								out << "ERROR: This name already exists!" << endl;
							}
							else {
								SimpleList<int> *tmp;
								if (a2 == "queue") { tmp = new Queue<int>(a1);}
								else if (a2 == "stack") { tmp = new Stack<int>(a1);}
								list_i.push_front(tmp);
							}
							break;
						case 's':
							SimpleList<string> *sp2SL;
							if (checkIfList(a1, list_s, &sp2SL)) {
								out << "ERROR: This name already exists!" << endl;
							}
							else {
								SimpleList<string> *tmp;
								if (a2 == "queue") {tmp = new Queue<string>(a1);}
								else if (a2 == "stack") {tmp = new Stack<string>(a1);}
								list_s.push_front(tmp);
							}
							break;
						case 'd':
							SimpleList<double> *dp2SL;
							if (checkIfList(a1, list_d, &dp2SL)) {
								out << "ERROR: This name already exists!" << endl;
							}
							else {
								SimpleList<double> *tmp;
								if (a2 == "queue") { tmp = new Queue<double>(a1);}
								else if (a2 == "stack") { tmp = new Stack<double>(a1);}
								list_d.push_front(tmp);
							}
							break;
						}
						break;
				case 1: // PUSH
					// if list name doesn't exist, output ERROR
					// else, push value to list
					switch (a1[0]) {
						case 'i':
							SimpleList<int> *ip2SL;
							if (!checkIfList(a1, list_i, &ip2SL)) {
								out << "ERROR: This name does not exist!" << endl;
							}
							else {
								ip2SL -> push(atoi(a2.c_str()));
							}
							break;
						case 's':
							SimpleList<string> *sp2SL;
							if (!checkIfList(a1, list_s, &sp2SL)) {
								out << "ERROR: This name does not exist!" << endl;
							}
							else {
								sp2SL -> push(a2);
							}
							break;
						case 'd':
							SimpleList<double> *dp2SL;
							if (!checkIfList(a1, list_d, &dp2SL)) {
								out << "ERROR: This name does not exist!" << endl;
							}
							else {
								dp2SL -> push(atof(a2.c_str()));
							}
							break;
					}
					break;					
				case 2: // POP
					// if list name doesn't exist, output ERROR
					// else
					// 	if list is empty, output ERROR
					// 	else, pop and display value
					switch (a1[0]) {
						case 'i':
							SimpleList<int> *ip2SL;
							if (!checkIfList(a1, list_i, &ip2SL)) {
								out << "ERROR: This name does not exist!" << endl;
							}
							else {
								if (! ip2SL -> getLen()) { 
									out << "ERROR: This list is empty!" << endl;
								}
								else {
									int valPop = ip2SL -> pop();
									out << "Value popped: " << valPop << endl;
								}
							}
							break;
						case 's':
							SimpleList<string> *sp2SL;
							if (!checkIfList(a1, list_s, &sp2SL)) {
								out << "ERROR: This name does not exist!" << endl;
							}
							else {
								if (! sp2SL -> getLen()) { 
									out << "ERROR: This list is empty!" << endl;
								}
								else {
									string valPop = sp2SL -> pop();
									out << "Value popped: " << valPop << endl;
								}
							}
							break;
						case 'd':
							SimpleList<double> *dp2SL;
							if (!checkIfList(a1, list_d, &dp2SL)) {
								out << "ERROR: This name does not exist!" << endl;
							}
							else {
								if (! dp2SL -> getLen()) { 
									out << "ERROR: This list is empty!" << endl;
								}
								else {
									double valPop = dp2SL -> pop();
									out << "Value popped: " << valPop << endl;
								}
							}
							break;
					}	
					break;
				}
 		}
 	}
 	else {
 		cout << inFile.c_str() << " is not a valid filename!" << endl;
 	}
	
	out.close();
 	return 0;
 }

// uses string streams
void parseInputStr(string line, string& arg0, string& arg1, string& arg2) {
	istringstream stream(line);
	stream >> arg0 >> arg1 >> arg2;
}

// uses iterators. true if list exists, false if list does not exist
template <class T>
bool checkIfList(string listName, list<SimpleList<T> *> list2Check, SimpleList<T> **pToSL) {
	typename list<SimpleList<T> *>::iterator iter;
	for(iter = list2Check.begin(); iter != list2Check.end(); ++iter) {
		if (!listName.compare((*iter)->getName())) {
			*pToSL = *iter;
			return true;
		}
	}
	return false;
}
