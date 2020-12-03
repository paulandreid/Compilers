//Product.java


#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <unordered_set>
#include <map>
#include <fstream>
#include <set>
using namespace std;

/*REGEX
Identifiers:[a-zA-z][a-zA-Z0-9]
Operators:==|!|&&|<|\+|\-|\*|\/
Keywords:if|int|class|for|false|new|void|this|true|public|static|booleean|else
String Literal:"[a-zA-Z0-9]*"
Number Literal: ((0\.)?[1-9]+[0-9]*)|0
*/



string currentToken = "";
list<string> tokens;

void syntaxError(string token) {
	cout << "Syntax Error: " << token << " not allowed!";
}

void finiteAutomata(string token) {
	
		if (token[0] >= 48 && token[0] <= 57 || token=="*" || token == "+" || token == "-" || token == "/") {
			tokens.push_back(token);
		} 
		else {
			syntaxError(token);
		}
		
}




int red(int argc, char* argv[])
{
	
	
	
	int line = 1; //line in source code
	string inputString;
	cin >> inputString;
	
	
	//inputString =argv[1];

	

	//reading every character
	for(char ch:inputString) {
	
		//token
		if (ch == ' ' || ch == '(' || ch == ')' || ch == ';' 
			|| ch == '\n' || ch == '\t' || ch == '<' || ch == '>' 
			|| ch == '#' || ch == '\"' || ch == '[' || ch == ']' || ch == '{' 
			|| ch == '}' || ch == ',' || ch == '.' || ch == '+' || ch == '-') {	//marks the end of a string or the beginning of a Constant
			
			string s;
			s.append(1,ch); //checking with finiteAutomata() the char(separator) that ended a token and also the token (one of Identifier/Keyword/Literal)
			finiteAutomata(s);
			
			finiteAutomata(currentToken);
			currentToken.clear();
		}
		//reading the next character		
		else
			currentToken.push_back(ch);

	}
	


	return 0;
}


