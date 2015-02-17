// Author: Meng Lei
// PDP-8 memory simulator
// main.cpp: main functions verification and testing

#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <bitset>
#include "memory.h"

using namespace std;

int main()
{

	// Test register size, number of pages and page capacity

	// Construct a pagetable with 32 pages, and 128 entries for each page
	pagetable test(int(pow(2, PageSize)), int(pow(2, LineSize)));
	// NOTICE: type conversion works in Visual Studio
	// so pagenumber goes as high as 32 and offset goes as high as 128

	test.store(1, 1);
	test.store(2, 2);
	test.store(3, 3);
	test.store(5, 4);
	test.store(6, 5);
	test.store(7, 6);
	test.store(129, 7);
	test.store(130, 8);
	test.store(258, 9);
	test.store(259, 10);
	test.store(261, 11);
	test.display_all();


	// Try to display an address when nothing actually exists
	cout << "Display an address: ";
	int address;
	cin >> address;
	cin.ignore(100, '\n');
	// Test case hint:
	// page 1 offset 1 yields 128+1=129

	test.display(address);
	// page 1 offset 1 should return a "non-exist".

	cout << "Value to store: ";
	int test_value;
	cin >> test_value;
	cin.ignore(100, '\n');
	cout << "Storing test value " << test_value << endl;
	if (test.store(address, test_value))
		cout << "Value stored;\n";
	// Store a value of 1001 into address 129 which is page 1 offset 1

	cout << "Display address again\n";
	test.display(address);

	// Now try to load an address
	int temp = 0;
	temp = test.load(address);
	cout << "Load value from memory address " << address << ";\n";
	cout << "Temp value: " << temp << "\n";

	// Now try to load a memory that does not exist, temp value should be -1
	cout << "Loading from empty entry at address " << address + 1 << endl;
	temp = test.load(address + 1);
	if (temp != -1)
	{
		cout << "Load value from memory address " << address + 1 << ";\n";
		cout << "Temp value: " << temp << "\n";
	}

	// Now try to overwrite a memory address
	cout << "Value to store: ";
	cin >> test_value;
	cin.ignore(100, '\n');
	cout << "Storing test value " << test_value << endl;

	if (test.store(address, test_value))
		cout << "Overwrite works;\n";
	cout << "Display address again\n";
	test.display(address);

	// Now try to delete an entry that does not exist
	cout << "Deleting from empty address " << address + 1 << ";\n";
	if (test.clear(address + 1))
		cout << "Something non-existent got deleted\n";
	else cout << "Test successful\n";

	// Now try to delete an entry that does exist
	cout << "Deleting from address " << address << ";\n";
	if (test.clear(address))
		cout << "Delete successful\n";
	cout << "Display address again\n";
	test.display(address);

	cin.get();

}