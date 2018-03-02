#include "scanner.h"
#include "string.h"

int main( int argc, char* argv[] )
{
  if( argc != 2 )
  {
    cout <<"Incorrect number of arguments" << endl;
    exit(1);
  }
  string filename = argv[1];
  char * cstr = new char [filename.length()+1];
  strcpy (cstr, filename.c_str());


  Scanner* s = new Scanner( cstr );
  Token* t = s->next_token();
  while( t->get_token_type() != TOKEN_EOF )
  {
    cout << *t->to_string() << endl;
    delete t;
    t = s->next_token();
  }
  cout << *t->to_string() << endl;

  delete s;
}
