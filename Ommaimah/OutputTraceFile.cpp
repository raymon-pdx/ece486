#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>

using namespace std;

#define timeStamp //**OPTIONAL TIMESTAMP

//trace file format:
// <type> <address>

int traceFile(int type, int address){

   #ifndef timeStamp
   //output time only when debugging
   time_t rawtime;
   time ( &rawtime );
   #endif

   ofstream outputTraceFile;
   outputTraceFile.open("TraceFile.txt");

   #ifndef timeStamp
   //add time stamp as header of tracefile
   outputTraceFile << "***** TIME STAMP *****\n";
   outputTraceFile << "Trace file generated: " << ctime(&rawtime);
   #endif

   //use outputTraceFile like cout, but into txt file 
   outputTraceFile << type << " " << address << endl;

   //TODO: Output in Octal value!!!

   outputTraceFile.close();
   cout << "Trace file generated.\n";

}

int main()
{

   int type = 0;
   int address = 0b1111;

   traceFile(type, address);

   return 0;

}


