#include "addoptoken.h"

AddopToken::AddopToken():Token()
{
  set_token_type( TOKEN_ADDOP );
  attribute = ADDOP_NO_ATTR;
}

AddopToken::AddopToken( addop_attr_type attr ): Token()
{
  set_token_type( TOKEN_ADDOP );
  attribute = attr;
}

AddopToken::~AddopToken() 
{
  this->~Token();
}

addop_attr_type AddopToken::get_attribute()
{
  return attribute;
}

void AddopToken::set_attribute( addop_attr_type attr )
{
  attribute = attr;
}

string* AddopToken::to_string()
{
  string* s;
  switch( attribute )
  {
    case ADDOP_ADD:
      s = new string( "Addition" );
      break;
    case ADDOP_SUB:
      s = new string( "Subtraction" );
      break;
    case ADDOP_OR: 
      s = new string( "OR Operation" );
      break;
    case ADDOP_NO_ATTR:
      s = new string( "UNDEFINED ATTRIBUTE" );
      break;
    default:  
      s = new string( "GARBAGE ATTR VAL" );
      break;
  }

  string* result = new string( "ADDITIVE OPERATOR " + *s );
  return result;
}