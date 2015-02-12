#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

int main()
{

   long int value = 0;

   cout << "Enter a value to convert to Octal: ";
   cin >> value;

   //error check to make sure value entered is not more than 12 bits
   if(value < 4096){
      cout << "Value in Octal: " << oct << value << "\n";
   }else{
     cout << "ERROR. Value is larger than 12 bits.\n";
     cout << "This is PDP-8. Not some joke.\n";
   }

   return 0;

}

