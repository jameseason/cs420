#include "symbol_table.h"

Symbol_Table::Symbol_Table()
{
  // Init the symbol table object.
  for (int i = 0; i < MAX_SYMBOLS; i++) {
    table[i].id = NULL;
    table[i].env = NULL;
    table[i].position = -1;
    table[i].type = GARBAGE_T;
  }
  size = 0;
}


Symbol_Table::~Symbol_Table()
{
 
  for (int i = 0; i < size; i++) {
    if (table[i].id) {
      delete table[i].id;
    }
    if (table[i].env) {
      delete table[i].env;
    }
  }

}


void Symbol_Table::install (string *id, string *env, 
			    expr_type t)
{
  update_symbol_table( id, env, t, -1 );
  // Install an identifier from environment env with type
  // t into the symbol table.
  if (size == MAX_SYMBOLS) {
    error_stab_overflow (id, env);
  }
  table[size].id = new string (*id);
  table[size].env = new string (*env);
  table[size].type = t;
  size++;
}

void Symbol_Table::install_parm (string *id, string *env, 
				 expr_type t, int pos)
{
  update_symbol_table( id, env, t, pos );
  // Install a paramter from environment env, type t and
  // parameter list position into the symbol table.
  if (size == MAX_SYMBOLS) {
    error_stab_overflow (id, env);
  }
  table[size].id = new string (*id);
  table[size].env = new string (*env);
  table[size].type = t;
  table[size].position = pos;
  size++;
}

bool Symbol_Table::is_decl (string *id, string *env)
{
  // Has identifier id of environment env been declared?
  for (int i = 0; i < size; i++) {
    if (table[i].id->compare(*id) == 0 &&
	table[i].env->compare(*env) == 0) {
      return true;
    }
  }
  return false;
}


expr_type Symbol_Table::get_type (string *id, string *env)
{
  // Return the type of identifier id of environment env.  Results in
  // garbage garbage type if (*id, *env) are not in the table.
  for (int i = 0; i < size; i++) {
    if (table[i].id->compare(*id) == 0 &&
	table[i].env->compare(*env) == 0) {
      return table[i].type;
    }
  }
  
  return GARBAGE_T;
}


expr_type Symbol_Table::get_type (string *env, int position)
{
  // Return the type of the parameter in position pos of the specified
  // environment.  Returns garbage if pararmeter isn't found.
  for (int i = 0; i < size; i++) {
    if (table[i].env->compare(*env) == 0 &&
	table[i].position == position) {
      return table[i].type;
    }
  }

  return GARBAGE_T;
}

void Symbol_Table::update_type (expr_type standard_type_type)
{
  
  for (int i = 0; i < size; i++) {
    if (table[i].type == UNKNOWN_T) {
      update_symbol_table( table[i].id, table[i].env, standard_type_type, -1);
      table[i].type = standard_type_type;
    }
  }
}


void Symbol_Table::set_parm_pos (int pos)
{
  for (int i = 0; i < size; i++) {
    if (table[i].type == UNKNOWN_T) {
      update_symbol_table( table[i].id, table[i].env, table[i].type, pos );
      table[i].position = pos;
      return;
    }
  }
}

void Symbol_Table::set_parm_pos_arg_list( int &parm_pos, expr_type standard_type )
{
  int e;
  for( e = 0; e < size; e++ )
  {
    if( table[e].type == UNKNOWN_T )
    { 
      update_symbol_table( table[e].id, table[e].env, standard_type, parm_pos );
      table[e].position = parm_pos;
      parm_pos++;
      table[e].type = standard_type;
    }
  }
}


void Symbol_Table::error_stab_overflow (string *id, string *env)
{
  // Probably should be written to cerr.  Will fix in 
  // TruPL compiler v 2.0.
  cout << "Exiting on SYMBOL TABLE ERROR" << endl;
  cout << "Symbol table overflow while installing ";
  cout << *id << " in environment " << *env << endl;
  exit (-1);
}

string *Symbol_Table::to_string (expr_type t)
{
  string *result;

  switch (t) {
  case INT_T:
    result = new string ("INT_T");
    break;
  case BOOL_T:
    result = new string ("BOOL_T");
    break;
  case PROCEDURE_T:
    result = new string ("PROCEDURE_T");
    break;
  case PROGRAM_T:
    result = new string ("PROGRAM_T");
    break;
  case NO_T:
    result = new string ("NO_T");
    break;
  case UNKNOWN_T:
    result = new string ("UNKNOWN_T");
    break;
  default:
    result = new string ("GARBAGE_T");
    break;
  }

  return result;
}

void Symbol_Table::update_symbol_table( string* id, string* env, expr_type type, int pos )
{
  cout << "Symbol Table Updated: " << endl;
  cout << "ID Name: " << *id << endl;
  cout << "Environment: " << *env << endl;
  cout << "Type: " << *to_string(type) << endl;
  if( pos != -1 )
  {
    cout << pos << endl;
  }
}















