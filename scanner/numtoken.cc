#include "numtoken.h"

NumToken::NumToken():Token()
{
  set_token_type( TOKEN_NUM );
  attribute = new string( "UNINITIALIZED NUMBER ATTRIBUTE" );
}

NumToken::NumToken( string* attr ):Token()
{
  set_token_type( TOKEN_NUM );
  attribute = new string(*attr);
}

NumToken::~NumToken() 
{
  if( attribute != NULL )
  {
    delete attribute;
  }
}

string* NumToken::get_attribute()
{
  return new string( *attribute );
}

void NumToken::set_attribute( const string* attr )
{
  if( attribute != NULL )
  {
    delete attribute;
  }
  attribute = new string (*attr);
}

string* NumToken::to_string()
{
  string* s;
  s = new string("NUMBER: " + *attribute );
  return s;
}