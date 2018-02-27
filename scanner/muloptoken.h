#ifndef MULOPTOKEN_H
#define MULOPTOKEN_H

using namespace std;

#include "token.h"
#include <string>

typedef enum mulop_attr { MULOP_MUL = 500,
			  MULOP_DIV = 501,
			  MULOP_AND = 502,
			  MULOP_NO_ATTR = 599 } mulop_attr_type;

class MulopToken : public Token{
 public:
  MulopToken();
  MulopToken(mulop_attr_type attr);
  ~MulopToken();

  mulop_attr_type get_attribute ();
  void set_attribute (mulop_attr_type attr);

  string *to_string() const;

 private:
  mulop_attr_type attribute;
};

#endif
