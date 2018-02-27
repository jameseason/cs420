#ifndef RELOPTOKEN_H
#define RELOPTOKEN_H

using namespace std;

#include <string>
#include "token.h"

typedef enum relop_attr { RELOP_EQ = 300,
			  RELOP_NE = 301,
			  RELOP_GT = 302,
			  RELOP_GE = 303,
			  RELOP_LT = 304,
			  RELOP_LE = 305,
			  RELOP_NO_ATTR = 399 } relop_attr_type;

class RelopToken : public Token
{
 public:
  RelopToken();

  RelopToken(relop_attr_type attr);

  ~RelopToken();

  relop_attr_type get_attribute();

  void set_attribute(relop_attr_type attr);

  string *to_string() const;

 private:
  relop_attr_type attribute;
};

#endif
