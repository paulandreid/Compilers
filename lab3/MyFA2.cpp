// lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include<string>
using namespace std;

string inputString;
int n = 0;

bool state3(char c);
bool state2(char c);
bool state1(char c);

bool state3(char c) { //if char is '.' we transition to this state
	if (c >= 'a' && c <= 'z')
		return state1(inputString[++n]);
	else if (c <= '9' && c >= '0')
		return state2(inputString[++n]);
	else if (!c)
		return true;
	else
		return false;
}
bool state2(char c) { //end state
	if (!c)
		return true;
	else if (c == '.') //transition to the third state
		return state3(inputString[++n]);
	else if (c <= '9' && c >= '0')
		return state2(inputString[++n]); //transition to the same state
	else
		return false;
}
bool state1(char c) { //end state
	if (!c)
		return true;
	else if (c >= 'a' && c <= 'z')
		return state1(inputString[++n]);
	if (c == '.')
		return state3(inputString[++n]);
	else
		return false;
}
bool start(char c) {
	if (c >= 'a' && c<='z')
		return state1(inputString[++n]);
	else if(c<='9'&& c>='0')
		return state2(inputString[++n]);
	else
		return false;
}

int main()
{
	
	while (true) {
		cout << "Enter a string or EXIT to end the program: \n";
		getline(cin,inputString);
		if (inputString == "EXIT")
			return 0;
		n = 0;

		if (inputString.empty() || inputString.length() > 63) { //state 0
			cout << "Not accepted \n";
			continue;
		}
		else {
			if (start(inputString[0]))
				cout << "Accepted\n";
			else
				cout << "Rejected\n";
		}
			
		
	}
}
