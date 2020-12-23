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


struct PIF { //a row of the PIF table

	string tokenType;
	int lineNumber;
	int tokenId;
	string token;

	PIF(string tokenType_, int lineNumber_, int tokenId_, string token_) {
		tokenType = tokenType_; lineNumber = lineNumber_; tokenId = tokenId_; token = token_;
	}
};

map<string, int> state; //holding state representation
list<PIF> pifTable; 
set<string> symbolTable; //could not get a hash based array to work in VS2019, weird syntax errors

void init() { //populate our states
	state.insert(std::pair<string, int>("if", 1));
	state.insert(std::pair<string, int>("int", 2));
	state.insert(std::pair<string, int>("class", 3));
	state.insert(std::pair<string, int>("for", 4));
	state.insert(std::pair<string, int>("false", 5));
	state.insert(std::pair<string, int>("new", 6));
	state.insert(std::pair<string, int>("void", 7));
	state.insert(std::pair<string, int>("this", 8));
	state.insert(std::pair<string, int>("true", 9));
	state.insert(std::pair<string, int>("public", 10));
	state.insert(std::pair<string, int>("static", 11));
	state.insert(std::pair<string, int>("booleean", 12));
	state.insert(std::pair<string, int>("else", 13));
	state.insert(std::pair<string, int>("=", 14));
	state.insert(std::pair<string, int>("!", 15));
	state.insert(std::pair<string, int>("&&", 16));
	state.insert(std::pair<string, int>("<", 17));
	state.insert(std::pair<string, int>("+", 18));
	state.insert(std::pair<string, int>("-", 19));
	state.insert(std::pair<string, int>("*", 20));
	state.insert(std::pair<string, int>("/", 21));
	state.insert(std::pair<string, int>("{", 22));
	state.insert(std::pair<string, int>("}", 23));
	state.insert(std::pair<string, int>("[", 24));
	state.insert(std::pair<string, int>("]", 25));
	state.insert(std::pair<string, int>("(", 26));
	state.insert(std::pair<string, int>(")", 27));
	state.insert(std::pair<string, int>(".", 28));
	state.insert(std::pair<string, int>(",", 29));
	state.insert(std::pair<string, int>(":", 30));
	state.insert(std::pair<string, int>(";", 31));
}

void finiteAutomata(string token, int line) {
	if (state.count(token)) {//check the first 31 states for Keywords, Operators or Separators
		switch (state.find(token)->second) 
		{
		case 1: pifTable.push_back(PIF{ "K",line,6,"if" }); break; //insert a row in PIF table
		case 2: pifTable.push_back(PIF{ "K",line,92,"int" }); break;
		case 3: pifTable.push_back(PIF{ "K",line,2,"class" }); break;
		case 4: pifTable.push_back(PIF{ "K",line,7,"for" }); break;
		case 5: pifTable.push_back(PIF{ "K",line,0,"false" }); break;
		case 6: pifTable.push_back(PIF{ "K",line,8,"new" }); break;
		case 7: pifTable.push_back(PIF{ "K",line,90,"void" }); break;
		case 8: pifTable.push_back(PIF{ "K",line,5,"this" }); break;
		case 9: pifTable.push_back(PIF{ "K",line,1,"true" }); break;
		case 10: pifTable.push_back(PIF{ "K",line,3,"public" }); break;
		case 11: pifTable.push_back(PIF{ "K",line,4,"static" }); break;
		case 12: pifTable.push_back(PIF{ "K",line,91,"booleean" }); break;
		case 13: pifTable.push_back(PIF{ "K",line,9,"else" }); break;
		case 14: pifTable.push_back(PIF{ "O",line,73,"=" }); break;
		case 15: pifTable.push_back(PIF{ "O",line,70,"!" }); break;
		case 16: pifTable.push_back(PIF{ "O",line,71,"&&" }); break;
		case 17: pifTable.push_back(PIF{ "O",line,72,"<" }); break;
		case 18: pifTable.push_back(PIF{ "O",line,80,"+" }); break;
		case 19: pifTable.push_back(PIF{ "O",line,81,"-" }); break;
		case 20: pifTable.push_back(PIF{ "O",line,82,"*" }); break;
		case 21: pifTable.push_back(PIF{ "O",line,83,"/" }); break;
		case 22: pifTable.push_back(PIF{ "P",line,50,"{" }); break;
		case 23: pifTable.push_back(PIF{ "P",line,51,"}" }); break;
		case 24: pifTable.push_back(PIF{ "P",line,52,"[" }); break;
		case 25: pifTable.push_back(PIF{ "P",line,53,"]" }); break;
		case 26: pifTable.push_back(PIF{ "P",line,54,"(" }); break;
		case 27: pifTable.push_back(PIF{ "P",line,55,")" }); break;
		case 28: pifTable.push_back(PIF{ "P",line,60,"." }); break;
		case 29: pifTable.push_back(PIF{ "P",line,61,"," }); break;
		case 30: pifTable.push_back(PIF{ "P",line,62,":" }); break;
		case 31: pifTable.push_back(PIF{ "P",line,63,";" }); break;
		

			

		}
		
	}
	else { //we have an identifier or literal
		if (token[0] >= 48 && token[0] <= 57) { pifTable.push_back(PIF{ "N",line,-1,token });  } //state32, number literal
		else if (token[0] == '"') { pifTable.push_back(PIF{ "S",line,-1,token });  }	//state33, string literal
		else if(token[0]!=32 && token.size()!=0 && token[0]!='\n'){ pifTable.push_back(PIF{ "I",line,-1,token });  //state34, identifier
			symbolTable.insert(token);
		}
	}
}

void writeCSV() { //PIF table output to file
	std::ofstream myfile;
	myfile.open("PIF.csv");
	for (PIF p : pifTable)
		if(p.tokenId!=-1)
			myfile << p.tokenType << ", " << p.lineNumber << ", " << p.tokenId << ", " << p.token << '\n';
		else
			myfile << p.tokenType << ", " << p.lineNumber << ", " << "#" << ", " << p.token << '\n';
	myfile.close();
}

void writeSymbolTable() { //symbol table output to file
	std::ofstream myfile;
	myfile.open("symboltable.txt");
	for (string identifier : symbolTable)
		myfile << identifier << endl;
	myfile.close();
}
int main(int argc, char* argv[])
{
	
	
	init();
	int line = 1; //line in source code
	string inputFilename;
	
	
	inputFilename=argv[1];
	char ch;
	fstream fin(inputFilename, fstream::in);
	string token = "";

	//reading every character
	while (fin >> noskipws >> ch) {
		if (ch == '\n')
			line++;
		//comments are ignored
		if (ch == '/') {
			fin >> noskipws >> ch;
			if (ch == '/') {
				while (fin >> noskipws >> ch)
					if (ch == '\n')
						break;
			}
		}
		//token
		if (ch == ' ' || ch == '(' || ch == ')' || ch == ';' 
			|| ch == '\n' || ch == '\t' || ch == '<' || ch == '>' 
			|| ch == '#' || ch == '\"' || ch == '[' || ch == ']' || ch == '{' 
			|| ch == '}' || ch == ',' || ch == '.' || ch == '+' || ch == '-') {	//marks the end of a string or the beginning of a Constant
			
			string s;
			s.append(1,ch); //checking with finiteAutomata() the char(separator) that ended a token and also the token (one of Identifier/Keyword/Literal)
			finiteAutomata(s,line);
			
			finiteAutomata(token, line);
			token.clear();
		}
		//reading the next character		
		else
			token.push_back(ch);

	}
	
	writeCSV();
	writeSymbolTable();

	return 0;
}


