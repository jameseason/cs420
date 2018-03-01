#ifndef NUMTOKEN_H
#define NUMTOKEN_H

using namespace std;

#include "token.h"
#include <string>

class NumToken : public Token
{
 public:
  NumToken();
  NumToken(string* s);
  ~NumToken();

  string *get_attribute();
  void set_attribute(const string* attr);

  string *to_string();

 private:
  string *attribute;
};

#endif
