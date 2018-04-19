#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <stdlib.h>

using namespace std;

#define MAX_SYMBOLS 128

typedef enum expression_type {
            INT_T       = 700, 
			      BOOL_T      = 701, 
			      PROGRAM_T   = 702, 
			      PROCEDURE_T = 703, 
			      UNKNOWN_T   = 704, 
			      NO_T        = 705, 			                         
			      GARBAGE_T   = 799
} expr_type;

class Symbol_Table {

public:	
  Symbol_Table();  
  ~Symbol_Table();
  void install (string *id, string *environment, expr_type t);
  void install_parm (string *id, string *environment, expr_type t, int position);
  bool is_decl (string *id, string *environment);
  expr_type get_type (string *id, string *environment);
  expr_type get_type (string *environment, int position);
  void update_type (expr_type standard_type_type);
  void set_parm_pos (int pos);
  void set_parm_pos_arg_list( int &parm_pos, expr_type standard_type_type );
  string *to_string (expr_type t);
  void update_symbol_table( string* id, string* env, expr_type type, int pos );
  
 private:
  
  typedef struct stab_entry {
    string *id; 
    string *env;
    int position;
    expr_type type; 
  } STAB_ENTRY;
  
  STAB_ENTRY table[MAX_SYMBOLS];
  int size;
  void error_stab_overflow (string *id, string *env);
  void error_stab_not_found (string *id, string *env);
};

#endif

