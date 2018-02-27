#ifndef KEYWORDTOKEN_H
#define KEYWORDTOKEN_H

using namespace std;

#include "token.h"
#include <string>

typedef enum keyword_attr { KW_PROGRAM  = 100, 
			    KW_PROCEDURE = 101,
			    KW_INT      = 102, 
			    KW_BOOL     = 103,
			    KW_BEGIN    = 104, 
			    KW_END      = 105,
			    KW_IF       = 106, 
			    KW_THEN     = 107,
			    KW_ELSE     = 108, 
			    KW_WHILE    = 109,
			    KW_LOOP     = 110,
			    KW_PRINT    = 111, 
			    KW_NOT      = 112,
			    KW_NO_ATTR  = 199 } keyword_attr_type;		 
// All token classes are subclasses of Token
class KeywordToken : public Token
{
 public:
  // Construct an uninitialized keyword token.
  KeywordToken();

  ~KeywordToken();

  // Construct a keywork token with an attribute.
  KeywordToken(keyword_attr_type attr);

  // Return the attribute of this token
  keyword_attr_type get_attribute();

  // Set the attribute of this token
  void set_attribute(keyword_attr_type attr);

  // For debugging only.  Won't be used by lexical analyzer.
  string *to_string() const;

 private:

  // This tokens attribute
  keyword_attr_type attribute;
};

#endif
