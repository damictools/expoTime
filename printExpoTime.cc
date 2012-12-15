#include <string.h>
#include <stdio.h>
#include "fitsio.h"

#include <iostream>
#include <sstream>

#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>

using namespace std;
bool gVerbosity;

bool fileExist(const char *fileName){
  ifstream in(fileName,ios::in);
  
  if(in.fail()){
    //cout <<"\nError reading file: " << fileName <<"\nThe file doesn't exist!\n\n";
    in.close();
    return false;
  }
  
  in.close();
  return true;
}

void printHelp(const char *exeName, bool printFullHelp=false){
  
  if(printFullHelp){
    cout << endl;
    cout << "This program reads (and prints on screen) the 'open shuter time'\n" 
         << "from the input fit file. If invoked without verbosity option it\n"
         << "prints the number of seconds elapsed from Jan 1, 1970\n\n";
  }
  cout << "==========================================================================\n";
  cout << "\nUsage:\n";
  cout << "  "   << exeName << " <input file 1> [-v for verbosity] \n\n";
  cout << "For any problems or bugs contact Javier Tiffenberg <javiert@fnal.gov>\n\n";
  cout << "==========================================================================\n\n";
}

int processCommandLineArgs(const int argc, char *argv[], string &inFile){
  
  if(argc == 1) return 1;
  
  int opt=0;
  while ( (opt = getopt(argc, argv, "vVhH?")) != -1) {
    switch (opt) {
      case 'V':
      case 'v':
        gVerbosity = 1;
        break;
      case 'h':
      case 'H':
        default: /* '?' */
          return 1;
    }
  }
  
  if(argc-optind != 1){
    cerr << "\nError: invalid number of parameters!\n";
    return 1;
  }
  inFile = argv[optind];
  if(!fileExist(argv[optind])){
    cout << "\nError reading input file: " << argv[optind] <<"\nThe file doesn't exist!\n\n";
    return 1;
  }
  
  return 0;
}


int main(int argc, char *argv[])
{

  string inFile;
  int returnCode = processCommandLineArgs( argc, argv, inFile);
  if(returnCode!=0){
    if(returnCode == 1) printHelp(argv[0],true);
    if(returnCode == 2) printHelp(argv[0]);
    return returnCode;
  }
  
  fitsfile *fptr;
  int status = 0,  nkeys;  /* MUST initialize status */

  fits_open_file(&fptr, inFile.c_str(), READONLY, &status);
  fits_get_hdrspace(fptr, &nkeys, NULL, &status);
  
  char keyName[] = "EXPSTART";
  char record[1024] = "";
  fits_read_card(fptr, keyName, record, &status);
  if(status==KEY_NO_EXIST){/* should return an error */
    status=0;
  }
  else{
    string sRec(record);
    size_t tPos = sRec.find("=");
    istringstream recISS( sRec.substr(tPos+1) );
    long expoStart=0;
    recISS >> expoStart;
    cout << expoStart << endl;
  }
  
  fits_close_file(fptr, &status);

  if(status)          /* print any error messages */
    fits_report_error(stderr, status);
  return(status);
}
