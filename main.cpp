#include <iostream>
#include <fstream>
#include "RBTree.hpp"
using namespace std;

int main(int argc, char const *argv[]) {

	// instantiates a tree object and a filename string
	RBTree<string> *tree = new RBTree<string>();
	string filename;

	// checks if filename was specified, otherwise exits
	if (argv[1]==nullptr) {
		cout<<"No file specified, exiting..."<<endl;
		return 0;
	}
	// or use the provided filename in the command-line
	else {
		filename = string(argv[1]);
	}
	ifstream readfile (filename);
	string word, command;

	//iterates the file stream and process each line as a insert/delete command
	//each line should have (1st) the word to be inserted/deleted
	//and (2nd) the command to be processed (0 for delete, 1 for insert)
  	if (readfile.is_open()) {
		while (readfile>>word) {
			readfile >> command;
			// process an insert command
			if (command=="1") 
				tree->insert(word);
			// process an delete command
			else
				tree->erase(word);
		}
		readfile.close();
  	}
	else cout << "Could not open " << filename << endl; 

	// loops searching for keys by user input
	// a blank input will make the loop end
	cout << "> Search for key: ";
	while (getline(std::cin, word)) {
		if (word.empty()) break;
		tree->search(word);
		cout << "> Search for key: ";
	}

	// prints key values (print) and each nodes data (check)
	cout << "> Exiting..." << endl;
	tree->print();
	cout<<"Tree check:"<<endl;
	tree->check();
}