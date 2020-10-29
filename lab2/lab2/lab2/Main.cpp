// compilare1a.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <list>
#include "Person.h"

int main()
{
	bool option = true; //CONSTRUCT 1
	int age; //CONSTRUCT 1
	int count = 0; 
	double averageAge = 0;
	std::string name;
	Person *persons[50];
	while (true) { //CONSTRUCT 5
		if (option == true) { //CONSTRUCT 4
			std::cout << "Insert name: "; //CONSTRUCT 7
			std::cin >> name; //CONSTRUCT 6
			std::cout << "and age: "; 
			std::cin >> age;
			persons[count] = new Person(name, age); //CONSTRUCT 2
			averageAge += age; //CONSTRUCT 3
			count++;
		}
		else
			break;
		std::cout << "Do you wish to add another person ? 0/1\n";
		std::cin >> option;
	}
	
	
	std::cout << "Average age is:" << averageAge / count<<"\n";
	system("pause");
	return 0;
}

