#ifndef TOKENEOF_H
#define TOKENEOF_H

using namespace std;

#include "token.h"
#include <string>

class EofToken: public Token
{
  public:
    EofToken();
    ~EofToken();
    string* to_string();
};

#endif
  
