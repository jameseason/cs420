#include "muloptoken.h"

MulopToken::MulopToken():Token()
{
  set_token_type( TOKEN_MULOP );
  attribute = MULOP_NO_ATTR;
}

MulopToken::MulopToken( mulop_attr_type attr ):Token()
{
  set_token_type( TOKEN_MULOP );
  attribute = attr;
}

MulopToken::~MulopToken() 
{
  this->~Token();
}

mulop_attr_type MulopToken::get_attribute()
{
  return attribute;
}

void MulopToken::set_attribute( mulop_attr_type attr )
{
  attribute = attr;
}

string* MulopToken::to_string()
{
  string* s;
  switch( attribute )
  {
    case MULOP_MUL:
      s = new string( "MULTIPLICATION" );
      break;
    case MULOP_DIV:
      s = new string( "DIVISION" );
      break;
    case MULOP_AND:
      s = new string( "AND" );
      break;
    case MULOP_NO_ATTR:
      s = new string( "MUL UNDEFINED ATTR" );
      break;
    default:
      s = new string( "(GARBAGE ATTR VAL)" );
      break;
  }
  string* result = new string( "MULTIPLICATIVE OPERATOR: " + *s );
  delete s;
  return result;
}
