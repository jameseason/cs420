#include "parser.h"
#include "scanner.h"
#include "string.h"

int main( int argc, char* argv[] )
{
  if( argc != 2 )
  {
    cout << "Incorrect number of arguments" << endl;
    exit(1);
  }
  string filename = argv[1];
  char * cstr = new char [filename.length()+1];
  strcpy (cstr, filename.c_str());

  Scanner* s = new Scanner( cstr );
  Parser* p = new Parser( s );

  if( p->parse_program() )
  {
    if( p->done_with_input() )
    {
      cout << "Parse Successful" << endl;
    }
    else
    { 
      cout << "Parse Failed" << endl;
      cout << "Left over input after parse" << endl;
    }
  }
  else
  {
    cout << "Parse Failed" << endl;
  }
  return 0;
}