#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "keywordtoken.h"
#include "punctoken.h"
#include "reloptoken.h"
#include "addoptoken.h"
#include "muloptoken.h"
#include "idtoken.h"
#include "numtoken.h"
#include "eoftoken.h"
#include "scanner.h"
#include "symbol_table.h"
#include "register.h"
#include "register_allocator.h"
#include "emitter.h"

#include <iostream>

using namespace std;

class Parser {

  public:

    Parser (Scanner *s);
    ~Parser();
    bool parse_program();
    bool done_with_input();

  private:
  
    bool parse_decl_list();
    bool parse_variable_decl_list();
    bool parse_variable_decl();
    bool parse_procedure_decl_list();
    bool parse_procedure_decl();
    bool parse_arg_list();
    bool parse_arg_list_hat();
    bool parse_identifier_list();
    bool parse_identifier_list_prime();
    bool parse_standard_type(expr_type &standard_type_type);
    bool parse_block();
    bool parse_stmt_list();
    bool parse_stmt_list_prm();
    bool parse_stmt();
    bool parse_stmt_ass_proc_tail(expr_type &stmt_ass_proc_tail_type, Register* &tail_reg );
    bool parse_assignment_stmt_tail( expr_type &assignment_stmt_tail_type, Register* &tail_reg );
    bool parse_if_stmt();
    bool parse_if_stmt_hat();
    bool parse_while_stmt();
    bool parse_print_stmt();
    bool parse_procedure_call_stmt_tail();
    bool parse_expr_list();
    bool parse_expr_list_hat();
    bool parse_expr(expr_type &expr_type, Register* &expr_reg);
    bool parse_expr_hat(expr_type &expr_hat_type, Register* &parent_reg);
    bool parse_simple_expr(expr_type &simple_expr_type, Register* &simple_expr_reg);
    bool parse_simple_expr_prm(expr_type &simple_expr_prm_type, Register* &parent_reg);
    bool parse_term(expr_type &term_type, Register* &term_reg);
    bool parse_term_prm(expr_type &term_prime_type, Register* &parent_reg);
    bool parse_factor(expr_type &factor_type, Register* &factor_reg);
    bool parse_sign();
    Scanner *lex;
    Token* word;
    Symbol_Table* stab;
    Emitter* e;
    Register_Allocator* ra;
    string* current_env;
    string* main_env;
    string* proc_call_env;
    int parm_pos;
    void type_error( Token* where );
    void undeclared_id_error( string* id, string* env );
    void parse_error (string *expected, Token *found);  
};
#endif
