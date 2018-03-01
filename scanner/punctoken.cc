#include "punctoken.h"

PuncToken::PuncToken():Token()
{
  set_token_type( TOKEN_PUNC );
  attribute = PUNC_NO_ATTR;
}

PuncToken::PuncToken( punc_attr_type attr ):Token()
{
  set_token_type( TOKEN_PUNC );
  attribute = attr;
}

PuncToken::~PuncToken()
{
    this->~Token();
}

punc_attr_type PuncToken::get_attribute()
{
  return attribute;
}

void PuncToken::set_attribute( punc_attr_type attr )
{
  attribute = attr;
}

string* PuncToken::to_string()
{
  string* attribute_name;

  switch( attribute )
  {
    case PUNC_SEMI: 
      attribute_name = new string( "SEMICOLON" );
      break;
    case PUNC_COLON:
      attribute_name = new string( "COLON" );
      break;
    case PUNC_COMMA:
      attribute_name = new string( "COMMA" );
      break;
    case PUNC_ASSIGN:
      attribute_name = new string( "ASSIGNMENT" );
      break;
    case PUNC_OPEN:
      attribute_name = new string( "OPEN PARENTHESE" );
      break;
    case PUNC_CLOSE:
      attribute_name = new string( "CLOSE PARENTHESE" );
      break;
    case PUNC_NO_ATTR:
      attribute_name = new string( "ATTRIBUTE UNDEFINED" );
  }
  string* result = new string( "PUNCTUATION: " + *attribute_name );
  return result;
}