/* This is a multiple include guard.  This prevents the programmer
   from inadvertently including a header more than once.  All of your
   header files should include an ifndef..define..endif include guard.
 */
#ifndef TOKEN_H
#define TOKEN_H

using namespace std;

#include <string>

/* An enum is like a series of int constant defintions,
   except the enum provides some rudimentary type checking.
*/
typedef enum token_type { TOKEN_KEYWORD =  0,
			  TOKEN_PUNC    =  1,
			  TOKEN_RELOP   =  2,
			  TOKEN_ADDOP   =  3,
			  TOKEN_MULOP   =  4,
			  TOKEN_ID      =  5,
			  TOKEN_NUM     =  6,
			  TOKEN_EOF     =  7,
			  TOKEN_NO_TYPE = 99 } token_type_type;

class Token
{

 public:
  // Default constructor
  Token();

  /* Default destructor
     
     We need to mark the destructor as virtual so that if it is called, we execute the
     destructor in the derived (sub) class instead.
  */
  virtual ~Token();

  /* Form a string consistsing of the token type and it's attribute.
     Useful for debugging. 
     
     The form of the output should be TOKEN_TYPE:Attribute.

     This odd declaration does two things: 1) The keyword "virtual"
     marks the function as a virtual function.  That means that if we
     override the function in a derived (sub) class, then the derived class 
     function will be called, even if we reference the object
     through a base (super) class object.
  */
  virtual string *to_string() const {
    return NULL;
  }

  // Set the token type.
  void set_token_type (token_type_type type);


  // Return the type of this token.
  token_type_type get_token_type() const;

 private:
  // The type of the tokens
  token_type_type type;
  
};

#endif
