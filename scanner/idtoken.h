#ifndef IDTOKEN_H
#define IDTOKEN_H

using namespace std;

#include "token.h"
#include <string>

class IdToken : public Token
{
 public:
  IdToken();
  IdToken(const string& attr);
  ~IdToken();

  string *get_attribute();
  void set_attribute(const string& attr);

  string *to_string() const;

 private:
  string *attribute;
};

#endif
