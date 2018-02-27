#include "scanner.h"

Scanner::Scanner (char *filename)
{
  // Create and fill the buffer.
  buf = new Buffer (filename);
}

Scanner::~Scanner ()
{
  delete buf;
}

Token *Scanner::next_token()
{
  Token *lexeme = NULL;
  char c;
  int state = 0;

  // ...
  // This is where the heart of the lexical analyzer,
  // the direct coded scanner, will go.
}



void Scanner::scanner_fatal_error(const string& message)
{
  cerr << "Exiting on Scanner Fatal Error: " << message << endl;
  exit (-1);
}




