#include <iostream>
#include<string>
using namespace std;

string inputString;
int n = 0;

//every state is an ending state excepting the initial state (start)

bool state9(char c) { 
	if (!c)
		return true;
	else
		return false;
}
bool state8(char c) {
	if (c == 'b')
		return state9(inputString[++n]);
	else if (!c)
		return true;
	else
		return false;
}
bool state7(char c) {
	if (c == 'b')
		return state8(inputString[++n]);
	else if (!c)
		return true;
	else
		return false;
}
bool state6(char c) {
	if (c)
		return false;
	else
		return true;
}
bool state5(char c) {
	if (c == 'b')
		return state6(inputString[++n]);
	else if (!c)
		return true;
	else
		return false;
}
bool state4(char c) {
	if (c)
		return false;
	else
		return true;
}
bool state3(char c) {
	if (c)
		return false;
	else
		return true;
}
bool state2(char c) {
	if (c == 'a')
		return state3(inputString[++n]);
	else if (c == 'b')
		return state4(inputString[++n]);
	else if (!c)
		return true;
	else
		return false;
}
bool state1(char c) {
	if (c == 'a')
		return state2(inputString[++n]);
	else if (c == 'b')
		return state5(inputString[++n]);
	else if (!c)
		return true;
	else
		return false;
}
bool start(char c) {
	if (c == 'a')
		return state1(inputString[++n]);
	else if(c=='b')
		return state7(inputString[++n]);
	else
		return false;
}











int main()
{
	
	while (true) {
		cout << "Enter a string: \n";
		getline(cin,inputString);
		n = 0;

		if (inputString.empty() || inputString.length() > 3) { //state 0
			cout << "Not accepted \n";
			continue;
		}
		else
			cout<<start(inputString[0]);
		
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
