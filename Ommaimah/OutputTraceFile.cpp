#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>

using namespace std;

#define timeStamp /*OPTIONAL TIMESTAMP*/

//trace file format:
// <type> <address>

int traceFile(int type, int address){

   #ifndef timeStamp
   //call timestamp function
   time_t rawtime;
   time (&rawtime);
   #endif

   //create an output trace file
   ofstream outputTraceFile;
   outputTraceFile.open("TraceFile.txt");

   #ifndef timeStamp
   //add time stamp as header of tracefile
   outputTraceFile << "***** TIME STAMP *****\n";
   outputTraceFile << "Trace file generated: " << ctime(&rawtime);
   #endif

   //use outputTraceFile like cout, but into txt file 
   outputTraceFile << type << " " << oct << address << endl;

   outputTraceFile.close();
   cout << "Trace file generated.\n";

}

int main()
{

   int type = 0;
   int address = 0b001010011;

   traceFile(type, address);

   return 0;

}


