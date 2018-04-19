#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <stdlib.h>

using namespace std;

// The max size of our symbol table.
#define MAX_SYMBOLS 128

/* Types of expressions that the semantic analyzer must
   manipulate.  These are stored in the symbol table
   or returned by parser functions as an attribute
   of a non-terminal.
*/
typedef enum expression_type {
            INT_T       = 700,  // int
			      BOOL_T      = 701,  // bool
			      PROGRAM_T   = 702,  // program name
			      PROCEDURE_T = 703,  // procedure name
			      UNKNOWN_T   = 704,  // type is currently unknown
			      NO_T        = 705,  // no type associated 
			                          //   with this attribute
			      GARBAGE_T   = 799} expr_type; // initial value

class Symbol_Table {

public:	
  Symbol_Table();
  
  ~Symbol_Table();

  /* 
     These are the methods that I used to manipulate the symbol table in
     my compiler.  You may or may not use all of these in yours.
  */
  
  /* Install an identifier in the symbol table if the environment
     and type are known. */
  void install (string *id, string *environment, 
		expr_type t);
  
  /* Install a formal parameter in the symbol table. */
  void install_parm (string *id, string *environment,
		     expr_type t, int position);
  
  /* Has an identifier been defined in this environment? */
  bool is_decl (string *id, string *environment);
  
  /* Get the type of an identifier in a given environment. Used
     when determining whether an expression or statment
     is semantically correct. */
  expr_type get_type (string *id, string *environment);
  
  /* Get the type of a formal parameter declared in environment
     by its position in the argument list. */
  expr_type get_type (string *environment, int position);
  
  /* Iterate over symbol table, updating all entries with 
     type == UNKNOWN_T to standard_type_type. */  
  void update_type (expr_type standard_type_type);

  /* Find the first identifier in the symbol table with an unknown
     type, and set its parameter position.  Note that this needs to
     be called before update_type in order to correctly set 
     the positions of parameters in the argument list.
   */
  void set_parm_pos (int pos);

  void set_parm_pos_arg_list( int &parm_pos, expr_type standard_type_type );
  
  // Convert an expr_type to a string - useful for debugging.
  string *to_string (expr_type t);

  void update_symbol_table( string* id, string* env, expr_type type, int pos );
  
 private:
  
  // One symbol table entry.
  typedef struct stab_entry {
    string *id;    // Identifier name
    string *env;   // Environment this id declared in
    int position;  // Position in formal parameter list if 
                   //   this id is a parm.  Undefined otherwise.
    expr_type type;  // Data type of this id.
  } STAB_ENTRY;
  
  // Storage for the symbol table.
  STAB_ENTRY table[MAX_SYMBOLS];
  
  // Current size of the symbol table.
  int size;
  
  // Symbol table overflow.
  void error_stab_overflow (string *id, string *env);
  
  /* A get_type() was attempted, but the symbol wasn't found.
     Shouldn't happen if you use is_decl() before searching.
   */
  void error_stab_not_found (string *id, string *env);
};

#endif

