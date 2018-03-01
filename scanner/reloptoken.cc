#include "reloptoken.h"

RelopToken::RelopToken():Token()
{
  set_token_type( TOKEN_RELOP );
  attribute = RELOP_NO_ATTR;
}

RelopToken::RelopToken( relop_attr_type attr ):Token()
{
  set_token_type( TOKEN_RELOP );
  attribute = attr;
}

RelopToken::~RelopToken() 
{
this->~Token();    
}

relop_attr_type RelopToken::get_attribute()
{
  return attribute;
}

void RelopToken::set_attribute( relop_attr_type attr )
{
  attribute = attr;
}

string* RelopToken::to_string() 
{
  string* attribute_name;
  switch( attribute ) 
  {
    case RELOP_EQ:
      attribute_name = new string( "Equals" );
      break;
    case RELOP_NEQ:
      attribute_name = new string( "Not Equals" );
      break;
    case RELOP_GT:
      attribute_name = new string( "Greater than" );
      break;
    case RELOP_LT: 
      attribute_name = new string( "Less than" );
      break;
    case RELOP_LE:
      attribute_name = new string( "Less than or equal" );
      break;
    case RELOP_GE:
      attribute_name = new string( "Greater than or equal" );
      break;
    case RELOP_NO_ATTR:
      attribute_name = new string( "NO ATTRIBUTE" );
      break;
    default:
      attribute_name = new string( "GARBAGE ATTRIBUTE VALUE" );
      break;
  }
  
  string* result = new string( "RELATIONAL OPERATOR: " + *attribute_name );
  return result;
} 