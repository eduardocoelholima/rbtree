#include <iostream>
#include <fstream>
#include "RBTree.hpp"
using namespace std;

int main(int argc, char const *argv[]) {

	// instantiates a tree object and a filename string
	RBTree<string> *tree = new RBTree<string>();
	string filename;

	// looks for "dicionario1.txt" if no filename is provided
	if (argv[1]==nullptr) {
		cout<<"No file specified, looking for dicionario1.txt..."<<endl;
		filename = "dicionario1.txt";
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
			if (command=="1") tree->insert(word);
			else tree->erase(word);
		}
		readfile.close();
  	}
	else cout << "Could not open "<<filename<<endl; 

	//prints key values (print) and each nodes data (check)
	tree->print();
	tree->check();

	return 0;
}