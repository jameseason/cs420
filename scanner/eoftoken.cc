#include "eoftoken.h"

EofToken::EofToken():Token()
{
  set_token_type( TOKEN_EOF );
}

EofToken::~EofToken() 
{
  this->~Token()
}

string* EofToken::to_string()
{
  string* s = new string( "EOF TOKEN" );
  return s;
}