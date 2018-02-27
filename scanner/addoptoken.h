#ifndef ADDOPTOKEN_H
#define ADDOPTOKEN_H

using namespace std;

#include "token.h"
#include <string>

typedef enum addop_attr { ADDOP_ADD = 400,
			  ADDOP_SUB = 401,
			  ADDOP_OR = 402,
			  ADDOP_NO_ATTR = 499 } addop_attr_type;

class AddopToken : public Token
{
 public:
  AddopToken();
  AddopToken(addop_attr_type attr);
  ~AddopToken();

  addop_attr_type get_attribute();
  void set_attribute (addop_attr_type attr);

  string *to_string() const;

 private:
  addop_attr_type attribute;
};

#endif
