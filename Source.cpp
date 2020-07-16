// Project for DSA 
// Submitted by zobia batool
// A memory optimized CPP implementation of trie 
// using unordered_map 


#include <iostream>
#include<fstream>
#include<string>
//for Time and date
#include<Windows.h>
//header files for time delay
#include <thread>
#include <chrono>
using namespace std;

//for trie tree
const int  ALPHABET_SIZE = 26;
#define CHAR_TO_INDEX(c) (c - (int)'a')

struct Trie {

	//array of pointers used to maintain parent child relationship
	struct Trie *children[ALPHABET_SIZE];
	bool isEndOfWord;
	string value;

	//built-in constructor
	Trie() {
		isEndOfWord = false;
	}
};


//method to make new node
Trie* getNewTrieNode(){

	Trie* node = new Trie;
	node->isEndOfWord = false;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		node->children[i] = NULL;

	return node;
}


//function to insert in trie
void insert(Trie*& root, const string& str, string value) {

	//if tree does not exist
	if (root == nullptr)
		root = getNewTrieNode();

	Trie* temp = root;
	int size = str.length();

	for (int i = 0; i < size; i++) {

		int index = CHAR_TO_INDEX(str[i]);
		if (!temp->children[index])
			temp->children[index] = getNewTrieNode();

		temp = temp->children[index];
	}

	temp->value = value;
	temp->isEndOfWord = true;
}


//save history of searched words
void saveHistory(string str) {

	//open file
	ofstream outfile("history.txt",ios::app);
	//if file not opened
	if (outfile.fail()) {
		cout << "file error" << endl;
	}
	//write on file
	else {
		outfile << str<<endl;
	}
}

//get local time
void getTime() {
	SYSTEMTIME a;
	GetLocalTime(&a);
	cout << a.wHour << ":" << a.wMinute << ":" << a.wSecond;
}

//get local date
void getDate() {
	SYSTEMTIME a;
	GetLocalTime(&a);
	cout << a.wDay << "/" << a.wMonth << "/" << a.wYear;
}


//to search if node exists
bool search(Trie* root, const string& str){

	Trie* temp = root;
	int size = str.length();

	//to help retrieve history
	saveHistory(str);

	//return false if Trie is empty
	if (root == nullptr)
		return false;

	// if trie is not empty

	for (int i= 0; i < size; i++)
	{
		int index = CHAR_TO_INDEX(str[i]);

		if (!temp->children[index])
			return false;

		temp = temp->children[index];
	}

	if (temp->isEndOfWord == true) {
		//format output from dictionary
		int position1, position2;
		string meaning, definition, definition1, definition2;

		//meaning
		position1 = temp->value.find("##");
		meaning = temp->value.substr(0, position1);
		cout << "			Meaning:	" << meaning << endl << endl;

		//definition
		definition = temp->value.substr(position1 + 2);
		position2 = definition.find("##");
		definition1 = definition.substr(0, position2);
		definition2 = definition.substr(position2 + 2);
		cout << "			Definition:" << endl << endl;
		cout << "			1)	" << definition1 << endl << endl;
		cout << "			2)	" << definition2 << endl << endl;
	}

	if (temp->isEndOfWord == false) {
		return 0;
	}

	return 1;
}


//add word in dictionary file
void writeToDictionary(string& str, string meaning,string definition ) {

	//open file
	ofstream outfile("Dictionary.txt", ios::in | ios::out | ios::app);
	//if file not opened
	if (outfile.fail()) {
		cout << "file error" << endl;
	}
	//write on file
	else {
		outfile << str << "#" << meaning<<"##"<<definition<<endl;
	}
	outfile.close();
	cout << "			your word has been added to dictionary..Please proceed" << endl<<endl<<endl;
}


//retrieve history of searched words
void retrieveHistory() {
	cout << "			list of searched words:"<<endl<<endl;
	string str;
	ifstream infile("history.txt");
	//if file not opened
	if (infile.fail()) {
		cout << "file error" << endl;
	}
	else {
		while (getline(infile, str)) {
			int pos = str.find("#");
			string key = str.substr(0, pos);
			cout << "			"<<key<<endl;
		}
	}
}


//display all words in dictionary
void displayDictionary() {

	int position, position1, position2;
	string word, meaning, definition, definition1, definition2 ,str;

	ifstream infile("Dictionary.txt");
	//if file not opened
	if (infile.fail()) {
		cout << "file error" << endl;
	}
	//if file opened, insert values in tree
	else {
		while (getline(infile, str)) {

			//format output from dictionary
			//word & meaning
			position = str.find("#");
			position1 = str.find("##");
			word = str.substr(0, position);
			int p = (position1-1) - position;
			meaning = str.substr(position+1,p);
			cout << endl << endl << endl;
			cout << "		==>";
			cout << "	Word:		" << word << endl << endl;
			cout << "			Meaning:	" << meaning << endl << endl;

			//definition
			definition = str.substr(position1 + 2);
			position2 = definition.find("##");
			definition1 = definition.substr(0, position2);
			definition2 = definition.substr(position2 + 2);
			cout << "			Definition:" << endl << endl;
			cout << "			1)	" << definition1 << endl << endl;
			cout << "			2)	" << definition2 << endl << endl;
			cout<<"			____________________________"<<endl<<endl<<endl;
		}
	}

}


//check if node is llast in trie tree
bool isLastNode(struct Trie* root)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (root->children[i])
			return 0;
	return 1;
}

// Recursive function to print auto-suggestions for given 
// node. 
//void suggestionsRec(struct TrieNode* root, string currPrefix)
void suggestionsRec(Trie* root, string currPrefix)
{
	Trie* temp = root;
	
	// found a string in Trie with the given prefix 
	if (temp->isEndOfWord==1)
	{
		cout << endl<<endl;
		cout << "			"<<currPrefix;
		cout << endl;
	}

	// All children struct node pointers are NULL
	if (root == nullptr)
		return;

	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (root->children[i])
		{
			// append current character to currPrefix string 
			currPrefix.push_back(97 + i);

			// recur over the rest 
			suggestionsRec(root->children[i], currPrefix);
		}
	}
}


// print suggestions for given query prefix. 
int printAutoSuggestions(Trie* root, const string query){
	struct Trie* temp = root;

	// Check if prefix is present and find the 
	// the node (of last level) with last character 
	// of given string. 
	int size = query.length();
	for (int i = 0; i < size; i++){

		int index = CHAR_TO_INDEX(query[i]);

		// no string in the Trie has this prefix 
		if (!temp->children[index])
			return 0;

		temp = temp->children[index];
	}

	// If prefix is present as a word. 
	bool isWord = (temp->isEndOfWord == true);

	// If prefix is last node of tree (has no 
	// children) 
	bool isLast = isLastNode(temp);

	// If prefix is present as a word, but 
	// there is no subtree below the last 
	// matching node. 
	if (isWord && isLast)
	{
		cout << query << endl;
		return -1;
	}

	// If there are are nodes below last 
	// matching character. 
	if (!isLast)
	{
		string prefix = query;
		suggestionsRec(temp, prefix);
		cout << endl;
		return 1;
	}
}



int main(){	

	string str;
	//key->value pair for trie tree
	string key, value;
	//variable to count words in dictionary
	int count = 0;
	//choose from main menu
	int choice;
	//initialize trie tree
	Trie* root = nullptr;
	//for switch
	int choice1;	
	int choice2;
	//for adding word to dictionary
	string word,meaning, definition;

	
	//insert data from file to Trie tree
	//read from file
	ifstream infile("Dictionary.txt");
	//if file not opened
	if (infile.fail()) {
		cout << "file error" << endl;
	}
	//if file opened, insert values in tree
	else {
		while (getline(infile, str)) {
			int pos = str.find("#");
			key = str.substr(0, pos);
			value = str.substr(pos + 1);

			insert(root, key, value);
		}
	}
	infile.close();

	
	//main menu
	do {
	cout << endl << endl<<endl << endl << endl;
	cout << "			";
	getTime();
	cout << "	  Welcome to Oxford Dictionary		";
	getDate();
	cout << endl;
	cout <<"			__________________________________________________________________"<< endl<<endl;
	cout << "					 1. Search Meaning/Definition				" << endl;
	cout << "					 2. View Search History						" << endl;
	cout << "					 3. Add Word to Dictionary					" << endl;
	cout << "					 4. Display Dictionary						" << endl;
	cout << "					 5. Exit						" << endl << endl << endl;

	cout << "				Select Corresponding Option to Perform Desired Action [Press]	" <<endl<<endl<< endl;
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	cin >> choice;
	cin.ignore();
	system("CLS");
	
		switch (choice) {

			//word search
			case 1: {
				do{
					cout << endl << endl << endl <<endl<<endl<< "					    Search Meaning/Definition				" << endl;
					cout << "			__________________________________________________________________" << endl << endl;
					cout << "			Enter a word:	";
					cin >> word;
					cout << endl;
					if (search(root, word) == 0) {
						cout << "			**word not found in dictionary, please look through suggestions**";
						int suggestions = printAutoSuggestions(root, word);
						if (suggestions == -1)
							cout << "No other strings found with this prefix\n";

						else if (suggestions == 0)
							cout <<endl<<"			"<< "No string found with this prefix\n";
					}
					cout << endl << endl << endl << "			Do you want to search for more, if Yes press 1 else press 0..[press]	";
					cin >> choice1;
					system("CLS");
				} while (choice1 > 0);
				break;
			}
			
			//search history
			case 2: {
				cout << endl << endl << endl << endl << endl;
				cout << "					    View Search History				" << endl;
				cout << "			__________________________________________________________________" << endl << endl;
				retrieveHistory();
				break;
			}
			
			//add word
			case 3: {
				cout << endl << endl << endl <<endl << endl<<"					    Add Word to Dictionary				" << endl;
				cout << "			__________________________________________________________________" << endl << endl;
				cout << "			Enter a word:	";
				cin >> word;
				cout <<endl<< "			Enter a meaning:	";
				cin >> meaning;
				cout <<endl<< "			Enter definition:	";
				cin >> definition;
				cout << endl;
				writeToDictionary(word,meaning, definition); 
				break;
			}

			//display dictionary
			case 4: {
				cout << endl << endl << endl << endl << endl << endl << endl << endl;
				cout << "\r					 Loading Oxford Dictionary";

				for (int i = 0; i < 3; i++) {
					cout << ".";
					this_thread::sleep_for(chrono::seconds(1));
				}
				system("CLS");

				cout << endl << endl << endl << endl << endl;
				cout << "					     Welcome to Oxford Dictionary				" << endl;
				cout << "			__________________________________________________________________" << endl << endl;
				displayDictionary();
				break;
			}

		
		}
		cout << "				Back to Main Menu [press 1], to Exit [press 0]...";
		cin >> choice2;
	} while (choice2 > 0);

	system("pause");
	return 0;
}
