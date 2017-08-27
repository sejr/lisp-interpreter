#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

/*
int main(int argc, char *argv[]) {

    for (int i = 0; i < argc; i++) {
        cout << argv[i] << endl;
    }

    LexicalAnalyzer l;
    Token next = l.getNextToken();
    cout << next.getValue() << endl;
}
*/

void readData(std::istream& in)
{
    std::cout << in.rdbuf();
}

int main(int argc, char** argv)
{
   if ( argc > 1 )
   {
      // The input file has been passed in the command line.
      // Read the data from it.
      std::ifstream ifile(argv[1]);
      if ( ifile )
      {
         readData(ifile);
      }
      else
      {
         // Deal with error condition
      }
   }
   else
   {
      // No input file has been passed in the command line.
      // Read the data from stdin (std::cin).
      readData(std::cin);
   }

   // Do the needful to process the data.
}