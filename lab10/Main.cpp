#include <iostream>
#include<list>
#include "Person.h"

int main()
{
	bool option;
	option = true;
	int age; 
	int count; 
	count = 0;  
	double averageAge;
	averageAge = 0; 
	std::string name;
	Person *persons[50];
	while (true) { 
		if (option == true) { 
			std::cout << "Insert name: "; 
			std::cin >> name; 
			std::cout<<"and age: "; 
			std::cin >> age;
			averageAge = averageAge + age; 
			count++;
		}
		else
			break;
		std::cout << "Do you wish to add another person ? 0/1\n";
		std::cin >> option;
	}
	
	return 0;
}
