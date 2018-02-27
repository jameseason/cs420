#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <stdlib.h>

// The scanner reads from the buffer.
#include "buffer.h"

// The scanner returns objects from the Token class when 
// next_token() is called.
#include "token.h"
#include "keywordtoken.h"
#include "punctoken.h"
#include "reloptoken.h"
#include "addoptoken.h"
#include "muloptoken.h"
#include "idtoken.h"
#include "numtoken.h"
#include "eoftoken.h"

using namespace std;

class Scanner
{
 public:
  Scanner (char *filename);

  ~Scanner();

  // Return the next token in the file.
  Token *next_token();

 private:

  Buffer *buf;

  inline bool is_alpha (char c) const 
  {
    return c >= 'a' && c <= 'z';
  }

  inline bool is_digit (char c) const
  {
    return c >= '0' && c <= '9';
  }

  inline bool is_alphanum (char c) const
  {
    return is_alpha(c) || is_digit(c);
  }

  inline bool is_space(char c) const
  {
    return c == ' ';
  }

  /* If a lexical error OR an internal scanner error occurs,
     call this method.  It will print the mesg and exit. */
  void scanner_fatal_error(const string& mesg);

};
#endif
  
