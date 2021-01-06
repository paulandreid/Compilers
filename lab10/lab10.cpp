


#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <unordered_set>
#include <map>
#include <fstream>
#include <set>
using namespace std;




class Token {
public:
	string typeCode;
	int lineNumber;
	int tokenId;
	string token;
	string scope;

	Token(string typeCode_, int lineNumber_, int tokenId_, string token_, string scope_) {
		typeCode = typeCode_;
		lineNumber = lineNumber_;
		tokenId = tokenId_;
		token = token_;
		scope = scope_;
	}
	Token() {};
};


class symbolTable {
public:
	Token* token;
	string kind;
	string type;
	int lineDeclared;
	string accesMod;
	

	symbolTable(Token* token_, string kind_, string type_, int lineDeclared_, string accesMod_ ) {
		token = token_;	
		kind = kind_;
		type = type_;
		lineDeclared = lineDeclared_;
		accesMod = accesMod_;
	}

	bool operator< (const Token* right) const
	{
		return !(token->token < right->token) && !(right->token < token->token);
	}
};



vector<string> scopes;
map<string, int> state; //holding state representation
vector<Token*> tokens;
vector<symbolTable*> ST; //could not get a hash based array to work in VS2019, weird syntax errors
int curentTokenPosition = -1;
string token;
int backtrack = 0;
void newScope();
void exitScope();
bool statement();

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
	state.insert(std::pair<string, int>("bool", 12));
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
	state.insert(std::pair<string, int>("#include", 32));
	state.insert(std::pair<string, int>(">", 33));
	state.insert(std::pair<string, int>("double", 34));
	state.insert(std::pair<string, int>("std::cout", 35));
	state.insert(std::pair<string, int>("std::cin", 36));
	state.insert(std::pair<string, int>("std::string", 37));
	state.insert(std::pair<string, int>("return", 38));
	state.insert(std::pair<string, int>("==", 39));

	scopes.push_back("global");
}

void lexicalError(string token, int line) {
	cout << "Lexical error at line " << line << "! " << token << " not alowed!";
	exit(EXIT_FAILURE);
}

void syntaxError() {
	cout << "Syntax error " << token << " on line " << tokens[curentTokenPosition]->lineNumber<<" not alowed!";
	exit(EXIT_FAILURE);
}

string writeCurrentScope() {
	string scope = "";
	for (int i = 0; i < scopes.size(); i++)
		scope =scope + "." + scopes[i];

	return scope;
}

string getAccessMod() {
	if (tokens.size() < 4) return "--";
	for (int i = tokens.size() - 1; i >= tokens.size()-3; i--) {
		if (tokens[i]->token =="public" || tokens[i]->token == "private" || tokens[i]->token == "protected")
			return tokens[i]->token;
	}
	return "--";
}

string getType() {
	for (int i = tokens.size() - 1; i >= 0; i--) {
		if (tokens[i]->token != "{"        //find the coresponding type token which is the fir
			&& tokens[i]->token != "."
			&& tokens[i]->token != "\""
			&& tokens[i]->token != "="
			&& tokens[i]->token != "("
			&& tokens[i]->token != "[")
			return tokens[i]->token;
	 }
}

bool verify() {
	if (!(tokens.size() - 1 > curentTokenPosition)) return false;
	return true;
}

void nextToken() {
	int tSize = tokens.size();
	if (tSize - 1 > curentTokenPosition) {
		curentTokenPosition++;
		token = tokens[curentTokenPosition]->token;
	}
	else
		return;
}

void finiteAutomata(string token, int line) {
	if (state.count(token)) {//check the first 31 states for Keywords, Operators or Separators
		switch (state.find(token)->second)
		{
		case 1: tokens.push_back(new Token( "K",line,6,"if", writeCurrentScope())); break; //insert a row in PIF table
		case 2: tokens.push_back(new Token("K",line,92,"int", writeCurrentScope())); break;
		case 3: tokens.push_back(new Token("K",line,2,"class", writeCurrentScope())); break;
		case 4: tokens.push_back(new Token("K",line,7,"for", writeCurrentScope()));  break;
		case 5: tokens.push_back(new Token("K",line,0,"false", writeCurrentScope())); break;
		case 6: tokens.push_back(new Token("K",line,8,"new", writeCurrentScope())); break;
		case 7: tokens.push_back(new Token("K",line,90,"void", writeCurrentScope())); break;
		case 8: tokens.push_back(new Token("K",line,5,"this", writeCurrentScope())); break;
		case 9: tokens.push_back(new Token("K",line,1,"true", writeCurrentScope())); break;
		case 10: tokens.push_back(new Token("K",line,3,"public", writeCurrentScope())); break;
		case 11: tokens.push_back(new Token("K",line,4,"static", writeCurrentScope())); break;
		case 12: tokens.push_back(new Token("K",line,91,"bool", writeCurrentScope())); break;
		case 13: tokens.push_back(new Token("K",line,9,"else", writeCurrentScope())); break;
		case 14: tokens.push_back(new Token("O",line,73,"=", writeCurrentScope())); break;
		case 15: tokens.push_back(new Token("O",line,70,"!", writeCurrentScope())); break;
		case 16: tokens.push_back(new Token("O",line,71,"&&", writeCurrentScope())); break;
		case 17: tokens.push_back(new Token("O",line,72,"<", writeCurrentScope())); break;
		case 18: tokens.push_back(new Token("O",line,80,"+", writeCurrentScope())); break;
		case 19: tokens.push_back(new Token("O",line,81,"-", writeCurrentScope())); break;
		case 20: tokens.push_back(new Token("O",line,82,"*", writeCurrentScope())); break;
		case 21: tokens.push_back(new Token("O",line,83,"/", writeCurrentScope())); break;
		case 22: tokens.push_back(new Token("P", line, 50, "{", writeCurrentScope())); newScope(); break;
		case 23: tokens.push_back(new Token("P", line, 51, "}", writeCurrentScope())); exitScope(); break;
		case 24: tokens.push_back(new Token("P",line,52,"[", writeCurrentScope())); break;
		case 25: tokens.push_back(new Token("P",line,53,"]", writeCurrentScope())); break;
		case 26: tokens.push_back(new Token("P", line, 54, "(", writeCurrentScope())); if(tokens[tokens.size()-2] == ST[ST.size() - 1]->token) ST[ST.size() - 1]->kind = "method"; break;
		case 27: tokens.push_back(new Token("P",line,55,")", writeCurrentScope())); break;
		case 28: tokens.push_back(new Token("P",line,60,".", writeCurrentScope())); break;
		case 29: tokens.push_back(new Token("P",line,61,",", writeCurrentScope())); break;
		case 30: tokens.push_back(new Token("P",line,62,":", writeCurrentScope())); break;
		case 31: tokens.push_back(new Token("P",line,63,";", writeCurrentScope())); break;
		case 32: tokens.push_back(new Token("K", line, 5, "#include", writeCurrentScope())); break;
		case 33: tokens.push_back(new Token("O", line, 72, ">", writeCurrentScope())); break;
		case 34: tokens.push_back(new Token("K", line, 92, "double", writeCurrentScope())); break;
		case 36: tokens.push_back(new Token("K", line, 92, "std::cin", writeCurrentScope())); break;
		case 35: tokens.push_back(new Token("K", line, 92, "std::cout", writeCurrentScope())); break;
		case 37: tokens.push_back(new Token("K", line, 92, "std::string", writeCurrentScope())); break;
		case 38: tokens.push_back(new Token("K", line, 92, "return", writeCurrentScope())); break;
		case 39: tokens.push_back(new Token("O", line, 80, "==", writeCurrentScope())); break;
		}
	}
	else { //we have an identifier or literal
		if (token[0] >= 48 && token[0] <= 57) { tokens.push_back(new Token("N",line,-1,token, writeCurrentScope())); } //state32, number literal
		else if (token[0] == '"') { 
			tokens.push_back(new Token("S",line,-1,token, writeCurrentScope())); ST.push_back(new symbolTable(tokens[tokens.size() - 1], "literal", "String", line, "global"));
		}	//state33, string literal
		else if (token[0] != 32 && token.size() != 0 && token[0] != '\n') {

			
			string type = "--"; //determine the type of identifier
			if (tokens[tokens.size() - 1]->token == "int")
				type = "int";
			else if (tokens[tokens.size() - 1]->token == "void")
				type = "void";
			else if (tokens[tokens.size() - 1]->token == "String")
				type = "String";
			else if (tokens[tokens.size() - 1]->token == "long")
				type = "long";
			else if (tokens[tokens.size() - 1]->typeCode == "I")
				type = tokens[tokens.size() - 1]->token;
			else if (tokens[tokens.size() - 1]->token == "]" && tokens[tokens.size() - 2]->token == "[")
				type = tokens[tokens.size() - 3]->token + "[]";
			else
				type = tokens[tokens.size() - 1]->token; //type of user defined type

			tokens.push_back(new Token("I",line,-1,token, writeCurrentScope()));  //state34, identifier

			bool alreadyInserted = false;
			for (symbolTable* symbol : ST){
				if (symbol->token->token == tokens[tokens.size() - 1]->token && symbol->token->scope == tokens[tokens.size() - 1]->scope) {
					alreadyInserted = true;
					break;
				}
			}
			if(!alreadyInserted)
				if(tokens[tokens.size()-2]->token=="class")	
					ST.push_back(new symbolTable(tokens[tokens.size() - 1], "class", "--", line, getAccessMod()));
				else if(token[0]=='"')
					ST.push_back(new symbolTable(tokens[tokens.size() - 1], "literal", type, line, getAccessMod()));
				else
					ST.push_back(new symbolTable(tokens[tokens.size() - 1], "variable", type, line, getAccessMod()));
		}
		else
			lexicalError(token, line);
	}
}

void exitScope() {
	scopes.pop_back();
}

void newScope() {

	if(tokens[tokens.size()-2]->token==")")
	for (int i = tokens.size() - 1; i > 0; i--) {
		if (tokens[i]->token == "(") {
			scopes.push_back(tokens[i - 1]->token);
			return;
		}
	}
	else {
		scopes.push_back(tokens[tokens.size() - 2]->token);
	}
}

void writeCSV() { //PIF table output to file
	std::ofstream myfile;
	myfile.open("PIF.csv");
	for (Token *p : tokens)
		if (p->tokenId != -1)
			myfile << p->typeCode << ", " << p->lineNumber << ", " << p->tokenId << ", " << p->token << ", "<< p->scope<<'\n';
		else
			myfile << p->typeCode << ", " << p->lineNumber << ", " << "#" << ", " << p->token << ", " << p->scope << '\n';
	myfile.close();
}


void writeSymbolTable() { //symbol table output to file
	std::ofstream myfile;
	myfile.open("symboltable.txt");
	for (symbolTable* identifier : ST)
		myfile << identifier->token->token << ", " << identifier->kind << ", "<<identifier->type<<", "<<identifier->lineDeclared<<", " << identifier->accesMod << ", " <<identifier->token->scope <<endl;
	myfile.close();
}


//IDENTIFIER = LETTER{ LETTER | DIGIT | “_”} .
bool identifier() {
	int ch = 0;
	if ((token[0] >= 'a' && token[0] <= 'z') || (token[0] >= 'A' && token[0] <= 'Z') || token[0]=='*') {
		for (char ch : token)
			if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= 0 && ch <= 9) ||(ch=='*' && ch==token[0])))
				return false;
	}
	//else if(token[0]>='0' &&token[0]<='9') lexicalError(token, tokens[curentTokenPosition]->lineNumber);
		
	return true;
}

//INCLUDE = “#include”, (“<”, IDENTIFIER , “>” , “;”) | ( “ “ “, IDENTIFIER, “ “ “, “; ”) .
bool include() {
	nextToken();
	if (token == "#include")
	{
		nextToken();
		if (token == "<"){
			nextToken();
			if (!identifier())
				return false;
			nextToken();
			if (token != ">")
				return false;
			
			return true;
		}
		else if (token[0] == '\"' && token[token.size()-1]=='\"' && token[token.size() - 2]=='h' && token[token.size() - 3]=='.') {
			token=token.substr(1, token.size() - 4);
			if (identifier())
				return true;
			
			lexicalError(token, tokens[curentTokenPosition]->lineNumber);
		}
	}
}

//NUMBER = {DIGIT};
bool number() {
	for (char ch : token)
		if (!(ch >= '0' && ch <= '9'))
			return false;
	return true;
}

//TYPE = “int” | “bool” | “std::string” | “double” | “float” .
bool type() {
	nextToken();
	if (token == "int" || token == "bool" || token == "std::string" || token == "double" || token == "float")
		return true;

	return false;
}

//LP="(".
bool LP() {
	nextToken();
	if (token == "(")
		return true;
	syntaxError();
}

//RP=")".
bool RP() {
	nextToken();
	if (token == ")")
		return true;
	syntaxError();
}

//LCB="{".
bool LCB() {
	nextToken();
	if (token == "{")
		return true;
	syntaxError();
}

//RCB="}".
bool RCB() {
	nextToken();
	if (token == "}")
		return true;
	syntaxError();
}

//USERTYPE = IDENTIFIER. 
bool userType() {
	return identifier();
}

//ARRAYIDENTIFIER = IDENTIFIER.
bool arrayIdentifier() {
	return identifier();
}

// TYPE | USERTYPE, ARRAYIDENTIFIER, “[“, NONZERONUMBER,{NUMBER}, “]”, “; ” .
bool aray() {
	if (!(type() || userType()))
		return false;
	nextToken();
	if (!arrayIdentifier())
		return false;
	nextToken();
	if (token != "[")
		return false;
	nextToken();
	number();
	nextToken();
	if (token != "]")
		return false;
	nextToken();
	if (token != ";")
		return false;
	
	return true;
}

//(TYPE | USERTYPE, IDENTIFIER) | ARRAY, “;” . 
bool variableDeclaration() {
	backtrack = curentTokenPosition;

	if (aray()) return true;

	curentTokenPosition = backtrack;
	if (type() || userType()) {
		nextToken();
		identifier();
	}
	else {
		aray();
	}
	nextToken();
	if (token == ";")
		return true;

	return false;
}

//VARIABLE = IDENTIFIER.
bool variable() {
	return identifier();
}

// IDENTIFIER, “=”, ( “new”, USERTYPE | TYPE) | VARIABLE | NUMBER, “; ” .
bool assigment() {
	nextToken();
	identifier();
	nextToken();
	if (token != "=") return false;
	nextToken();
	if (token == "new")
		if (type() || userType())
		{
			nextToken();
			return token == ";";
		}
		else
			return false;
	else if (variable())
	{
		nextToken();
		if (token == ";") return true;
		if (token == "+") {
			nextToken();
			if (identifier()) {
				nextToken();
				if (token == ";") return true;
			}
		}
	}
	else if (number())
	{
		nextToken();
		return token == ";";
	}
	else
		return false;
}

//POSTINCREMENT = IDENTIFIER, “++” | “--“.
bool postincrement() {
	nextToken();
	if (identifier())
	{
		nextToken();
		if (token == "+")
		{
			nextToken();
			if (token == "+")
				return true;
		}
		else if (token == "-")
		{
			nextToken();
			if (token == "-")
				return true;
		}
	}
	return false;
}

//PREINCREMENT = "++" | "--", IDENTIFIER.
bool preincrement() {
	nextToken();
	if (token == "++" || token == "--")
	{
		nextToken();
		if (identifier())
			return true;
	}
	return false;
}

//ARITHMETICASSIGMENT = POSTINCREMENT | PREINCREMENT | ([“ + ” | “ - “], IDENTIFIER, { “ + ” | “ - “ | “ * ” | “ / ” , IDENTIFIER }, “; ” .
bool arithmeticAssigment() {
	backtrack = curentTokenPosition;
	if (postincrement()) { nextToken(); return token == ";"; }
	curentTokenPosition = backtrack;
	if (preincrement()) { nextToken(); return token == ";"; }
	curentTokenPosition = backtrack;
	nextToken();
	if (token == "+" || token == "-") {
		nextToken();
		if (identifier())
		{
			nextToken();
			if (token == "+" || token == "-" || token == "*" || token == "/")
			{
				nextToken();
				if (identifier())
					return true;
			}
		}

	}
	return false;
}

//CONDITION = IDENTIFIER | NUMBER, ("="|"<"|"<="|">"|">=" | “ != ”), IDENTIFIER | NUMBER | “true” | “false” .
bool condition() {
	nextToken();
	if (identifier())
	{
		nextToken();
		if (token == "==" || token == ">" || token == "<" || token == "!=" || token == ">=" || token == "<=") {
			nextToken();
			if (token == "true" || token == "false" || identifier() || number())
				return true;
		}
	}
	return false;
}

//LOOP = “while”, LP, (“true” | “false”) | ( CONDITION, {AND | OR, CONDITION}, RP, LCB, STATEMENT, { STATEMENT }, RCB.
bool loop() {
	nextToken();
	if (token == "while") {
		LP();
		nextToken();
		if (token == "true" || token == "false") {
			RP();
			LCB();
			while (statement()) {}
			RCB();
			return true;
		}
		else if (condition()) {
			RP();
			LCB();
			while (statement()) {}
			RCB();
			return true;
		}
	}
	return false;
}

//IF = “if” , LP , CONDITION, [AND | OR] {CONDITION}, RP , STATEMENT | (LCB, STATEMENT, { STATEMENT }, RCB) [(“else”, STATEMENT) | “elseif”, LP, CONDITION, [AND | OR] { CONDITION }, RP, LCB, { STATEMENT }, RCB .
bool ifStatement() {
	nextToken();
	if (token != "if") return false;
	LP();
	condition();
	RP();
	LCB();
	while (statement()) {}
	RCB();
	nextToken();
	if (token == "else") {
		return statement();
	}
	return true;
}

//BREAK = “break”, “;” . 
bool breakStatement() {
	nextToken();
	if (token == "break") {
		nextToken();
		return token == ";";
	}
	return false;
}

//INPUT = “std::cin>>”, IDENTIFER, “;” . 
bool input() {
	nextToken();
	if (token != "std::cin") return false;
	nextToken();
	if (token != ">") syntaxError();
	nextToken();
	if (token != ">") syntaxError();
	nextToken();
	identifier();
	nextToken();
	return token == ";";
}

//OUTPUT = “std::cout<<”, IDENTIFIER | NUMBER | (“”” , {ASCII CODE}, “””), “; ” .
bool output() {
	nextToken();
	if (token != "std::cout") return false;
	nextToken();
	if (token != "<") syntaxError();
	nextToken();
	if (token != "<") syntaxError();
	nextToken();
	identifier();
	nextToken();
	nextToken();
	return token == ";";
}

//STATEMENT = VARIABLEDECLARATION | ASSIGMENT | ARITHMETICASSIGMENT | BREAK | LOOP | OUTPUT | INPUT | IF.
bool statement() {
	backtrack = curentTokenPosition;
	
	if (assigment()) return true;
	curentTokenPosition = backtrack;
	if(arithmeticAssigment()) return true;
	curentTokenPosition = backtrack;
	if (loop()) return true;
	curentTokenPosition = backtrack;
	if (ifStatement()) return true;
	curentTokenPosition = backtrack;
	if (breakStatement()) return true;
	curentTokenPosition = backtrack;
	if (input()) return true;
	curentTokenPosition = backtrack;
	if (output()) return true;
	curentTokenPosition = backtrack;
	if (variableDeclaration()) return true;
	curentTokenPosition = backtrack;


	return false;
	
}

//{INCLUDE}, TYPE, “main”, LP , {IDENTIFIER}, RP, LCB, {STATEMENT} .
bool program() {
	
	
	while (include()) {}
	type();
	nextToken();
	if (token != "main") syntaxError();
	LP();
	RP();
	LCB();
	if(tokens[curentTokenPosition]->token!="return")  while(statement()){}
	return true;
}

//Start of Parser
void LL1() {
	curentTokenPosition = 0; //marks the curent token position in tokens[]
	token = tokens[0]->token;
	program();

	writeSymbolTable();

}

int main(int argc, char* argv[])
{


	init();
	int line = 1; //line in source code
	string inputFilename;


	inputFilename = argv[1];
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
		if (ch == '\"') {
			token.clear();
			token += "\"";
			while (fin >> noskipws >> ch && ch != '\"')
				token += ch;
			if (ch != '\"')
				lexicalError(" missing \" ", line);
			else
				finiteAutomata(token + "\"", line);
		}
		else if (ch == ' ' || ch == '(' || ch == ')' || ch == ';'
			|| ch == '\n' || ch == '\t' || ch == '<' || ch == '>'
		  || ch == '[' || ch == ']' || ch == '{'
			|| ch == '}' || ch == ',' || ch == '.' || ch == '+' || ch == '-') {	//marks the end of a string or the beginning of a Constant

			//if (ch == '<' && tokens[tokens.size() - 1]->token == "#include") continue;
			if (token != "") finiteAutomata(token, line);
			if (ch != ' ' && ch != '\n' && ch!='\t' ) {// ' ' is a separator between tokens but will not be treated as one
				string s;
				s.append(1, ch); //checking with finiteAutomata() the char(separator) that ended a token and also the token (one of Identifier/Keyword/Literal)
				finiteAutomata(s, line);
			}
			
			
			token.clear();
		}
		//reading the next character		
		else
			token.push_back(ch);

	}


	program();
	writeCSV();
	writeSymbolTable();

	return 0;
}


