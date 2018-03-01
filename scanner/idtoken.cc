#include "idtoken.h"

IdToken::IdToken():Token()
{
  set_token_type (TOKEN_ID);
  attribute = new string("UNINITIALIZED IDENTIFIER ATTRIBUTE");
}

IdToken::IdToken (string *attr):Token()
{
  set_token_type (TOKEN_ID);
  attribute = new string(*attr);
}

IdToken::~IdToken()
{  
  if (attribute != NULL) 
  {
    delete attribute;
  }
}


string *IdToken::get_attribute()
{
  return attribute
}


void IdToken::set_attribute(string *attr)
{ 
  if (attribute != NULL) {
    delete attribute;
  }
  attribute = new string (*attr);
}


string *IdToken::to_string()
{
  string *attribute_name = new string ("IDENTIFIER " + *attribute);
  return attribute_name;
}