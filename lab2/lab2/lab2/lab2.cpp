// lab2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <string>
#include <fstream>
#include <list>

using namespace std;
int main()
{
	string inputFilename;
	list <string> keywords; //symbol tables
	list<string> constants;
	list<string> identifiers;
	list<string> operators;
	std::cout << "Enter Filename:";
	cin >> inputFilename;
	char ch;
	fstream fin(inputFilename, fstream::in);
	string token= "";

	//reading every character
	while (fin >> noskipws >> ch) {
		//comments
		if (ch == '/') {
			fin >> noskipws >> ch;
			if (ch == '/') {
				while (fin >> noskipws >> ch)
					if (ch == '\n')
						break;
			}
			else if(ch=='=')
				operators.push_back("/=");
			else
				operators.push_back("/");
		}
		//token
		if (ch == ' ' || ch=='(' || ch==')' || ch==';'|| ch == '\n' || ch == '\t'||ch=='<'||ch=='>' ||ch=='#' ||ch== '\"' || ch=='[' || ch==']' || ch == '{' || ch == '}'||ch==',') { //marks the end of a string or the beginning of a Constant
			
			//keyword
			if (token == "if" || token== "while" || token =="else" || token == "break"|| token == "for" || token == "int" || token == "double" || token == "bool" || token == "return" || token == "std::string" || token == "true" || token == "false" || token == "new" || token == "std::cin" || token == "std::cout" || token == "else" || token == "include" ||token=="system") {
				keywords.push_back(token);
			}
			//constant
			else if (ch == '\"') {
				while (fin >> noskipws >> ch)
					if (ch == '\"') {
						constants.push_back(token);
						break;
					}
					else
						token.push_back(ch);

			}
			//number constant
			else if (token[0] >= 48 && token[0] <= 57) {
				constants.push_back(token);

			}
			//identifier
			else {
				if (token != "") {
					bool exists = false;
					for (string existingIdent : identifiers)
						if (existingIdent == token)
							exists = true;
					if(!exists)
					identifiers.push_back(token);
				}
			}
			
			token.clear();
		}
		//operators
		else if (ch == '+') {
			fin >> noskipws >> ch;
			if (ch == '+')
				operators.push_back("++");
			else if(ch=='=')
				operators.push_back("+=");
			else
				operators.push_back("+");
		}
		else if (ch == '=') {
			fin >> noskipws >> ch;
			if(ch=='=')
				operators.push_back("==");
			else
				operators.push_back("=");
		}
		else if (ch == '<') {
			fin >> noskipws >> ch;
			if(ch=='<')
				operators.push_back("<<");
			if(ch=='=')
				operators.push_back("<=");
			else
				operators.push_back("<");
		}
		//reading the next character		
		else
			token.push_back(ch);
		
	}

	//sorting the symbol tables
	identifiers.sort();
	constants.sort();
	keywords.sort();
	operators.sort();

	//overview output
	std::cout <<"Tokens <"<< identifiers.size()+ constants.size() + keywords.size()+ operators.size() <<">"<<endl;
	std::cout << "Identifiers <" << identifiers.size() << ">" << endl;
	std::cout << "Constants <" << constants.size() << ">" << endl;
	std::cout << "Keywords <" << keywords.size() << ">" << endl;
	std::cout << "Operators <" << operators.size() << ">" << endl;
	std::cout << endl;

	//symbol tables output
	std::cout << "Identifiers:" << endl;
	for (string keyword : identifiers) {
		std::cout << "    <" << keyword <<">"<< endl;
	}
	std::cout << endl;
	std::cout << "Constants:" << endl;
	for (string keyword : constants) {
		std::cout << "    <" << keyword << ">" << endl;
	}
	std::cout << endl;
	std::cout << "Keywords:" << endl;
	for (string keyword : keywords) {
		std::cout << "    <" << keyword << ">" << endl;
	}


	cin >> inputFilename;//to hold console open
	return 0;
}

