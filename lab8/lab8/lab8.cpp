

#include<stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <unordered_set>
#include <map>
#include <fstream>
#include <set>
using namespace std;

/*
Expr->Term Expr2
Expr2 -> + Term Expre2 | - Term Expr2 |epsilon
Term->Factor Term2
Term2 -> * Factor Term2 | / Factor Term2 |epsilon   not T-> F * F| F/F because of left factorising
Factor  -> terminalNumber
*/




string currentToken; //curent token from the list of tokens
int currentTokenPos = 0;

vector<string> tokens; //list of tokens

int Expr();
int Term();
int Factor();
int Term2(int terminalNumber); //terminal number to store the left factor of *,/

void scanner() {
	if(currentTokenPos<tokens.size())
	currentToken=tokens[currentTokenPos++]; 
}



void syntaxError(string token) {
	cout << "Syntax Error: " << token << " not allowed!";
	exit(EXIT_FAILURE);
}

void lexicalError(int position, char ch) {
	cout << "Lexical Error at position "<<position<< ": character "<<ch<<" not allowed!";
	exit(EXIT_FAILURE);
}

//to insert Tokens into list 
void finiteAutomata(string token) {

	if (token[0] >= 48 && token[0] <= 57 || token == "*" || token == "+" || token == "-" || token == "/") {
		tokens.push_back(token);
	}
	else {
		syntaxError(token);
	}
}

//converts string to int
int isTerminalNumber() {
	int number = 0;
	if (currentToken[0] >= 48 && currentToken[0] <= 57)
		for(char ch : currentToken)
		{
			number = number *10 + ch - 48;
		}
	else
		syntaxError(currentToken);
	return number;
}

//start of the recursive decent parser
int LL1() {
	currentToken = tokens[0];
	return Expr();
}

//jumps to the next token and returns its number
int Factor() {
	scanner();
	int ret= isTerminalNumber();
	scanner();
	return ret;
}

int Term2(int terminalNumber) {
	int value = 0;
	
	if (currentToken == "*") {
		value = terminalNumber * Factor() ;
		return Term2(value);
	}
	else if (currentToken == "/") {
		value = terminalNumber / Factor();
		return Term2(value);
	}
	return terminalNumber;
}

int Term() {
	int value = 0;
	
	value =  Term2(Factor());
	return value;
}

int Expr2() {
	int value=0;
	if (currentToken == "+") {
		value = value + Term()+Expr2();
	}
	else if(currentToken == "-")
		value = value - Term()+ Expr2();
	return value;
}

int Expr() {
	int value= Term() + Expr2();
	return value;
}

int main(int argc, char* argv[])
{

	int position = 1;  //position in input string
	string token = ""; //token to be built and sent to finiteAutomata()
	string inputString; //the input string from console
	

	inputString =argv[1];
	inputString += '\0'; //added to delimitate end of string


	//reading every character
	for (char ch : inputString) {

		//token
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '\0') {	//marks end of token

			finiteAutomata(token);
			if (ch != '\0') {
				string s;
				s.append(1, ch); //checking with finiteAutomata() the operators
				finiteAutomata(s);
			}
			token.clear();
		}
		//reading the next character		
		else if (ch >= 48 && ch <= 57) {
			token.push_back(ch);
			position++;
		}
		else
			lexicalError(++position, ch);
	}

	cout<<LL1();
	
	return 0;
}


