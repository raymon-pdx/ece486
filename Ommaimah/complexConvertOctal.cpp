#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <sstream>
using namespace std;

//function that converts value to octal
int convert2octal(int inputNum)
{
  long int octalNum = 0;

  long int num1, num2, num3, num4 = 0;
  
  ostringstream concatenate;
 
  num1 = inputNum & 0x7;
  //cout << "First digit:" << num1 << "\n"; 
  num2 = (inputNum >> 3) & 0x7;
  //cout << "Second digit:" << num2 << "\n";   
  num3 = (inputNum >> 6) & 0x7;
  //cout << "Third digit:" << num3 << "\n";
  num4 = (inputNum >> 9) & 0x7;
  //cout << "Fourth digit:" << num4 << "\n";

  //concatenate all the numbers in proper order
  concatenate << num4 << num3 << num2 << num1;

  istringstream iss(concatenate.str());
  iss >> octalNum;

  return octalNum;
}


int main(){

   long int value = 0;

   cout << "Enter a value to convert to Octal: ";
   cin >> value;

   //error check to make sure value entered is not more than 12 bits
   if(value < 4096){
      cout << "Value in Octal: " << convert2octal(value) << "\n";
   }else{
     cout << "ERROR. Value is larger than 12 bits.\n";
     cout << "This is PDP-8. Not some joke.\n";
   }

   return 0;

}

