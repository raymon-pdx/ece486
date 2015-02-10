// Author: Meng Lei
// PDP-8 memory simulator
// main.cpp: main functions verification and testing

#include <iostream>
#include <cstring>
#include <cctype>
#include "memory.h"

using namespace std;

int main()
{
	// Construct a pagetable with 32 pages, and 128 entries for each page
	pagetable test(32, 128);
	// so pagenumber goes as high as 32 and offset goes as high as 128

	// Try to display an address when nothing actually exists
	cout << "Display an address: ";
	int address;
	cin >> address;
	cin.ignore(100, '\n');
	// Test case hint:
	// page 1 offset 1 yields 128+1=129

	test.display(address);
	// page 1 offset 1 should return a "non-exist".

	if (test.store(address, 1001))
		cout << "Value stored;\n";
	// Store a value of 1001 into address 129 which is page 1 offset 1

	cout << "Display address again";
	test.display(address);

	// Now try to load an address
	int temp = 0;
	temp = test.load(address);
	cout << "Load value from memory address " << address << ";\n";
	cout << "Temp value: " << temp << "\n";

	cin.get();
	
}