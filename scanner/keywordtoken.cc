#include "keywordtoken.h"

KeywordToken::KeywordToken():Token()
{
  set_token_type (TOKEN_KEYWORD);
  attribute = KW_NO_ATTR;
}

KeywordToken::KeywordToken (keyword_attr_type attr) : Token()
{
  set_token_type (TOKEN_KEYWORD);
  attribute = attr;
}

KeywordToken::~KeywordToken()
{
    this->~Token();
}

keyword_attr_type KeywordToken::get_attribute()
{
  return attribute;
}

void KeywordToken::set_attribute(keyword_attr_type attr)
{
  attribute = attr;
}

string *KeywordToken::to_string()
{
  string *attribute_name;

  switch (attribute) {
      case KW_PROGRAM:
        attribute_name = new string ("PROGRAM");
        break;
      case KW_PROCEDURE:
        attribute_name = new string ("PROCEDURE");
        break;
      case KW_INT:
        attribute_name = new string ("INT");
        break;
      case KW_BOOL:
        attribute_name = new string ("BOOL");
        break;
      case KW_BEGIN:
        attribute_name = new string ("BEGIN");
        break;
      case KW_END:
        attribute_name = new string ("END");
        break;
      case KW_IF:
        attribute_name = new string ("IF");
        break;
      case KW_THEN:
        attribute_name = new string ("THEN");
        break;
      case KW_ELSE:
        attribute_name = new string ("ELSE");
        break;
      case KW_WHILE:
        attribute_name = new string ("WHILE");
        break;
      case KW_PRINT:
        attribute_name = new string ("PRINT");
        break;
      case KW_NOT:
        attribute_name = new string ("NOT");
        break;
      case KW_NO_ATTR:
        attribute_name = new string ("NO ATTRIBUTE");
        break;
      default:
        attribute_name = new string ("GARBAGE ATTRIBUTE VALUE");
        break;
  }

  string *result = new string("KEYWORD: " + *attribute_name);
  return result;
}