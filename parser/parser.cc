#include "parser.h"
#include <iostream>

Parser::Parser(Scanner* s) {
  lex = s;
  word = lex->next_token();

  stab = new Symbol_Table();

  current_env = NULL;
  main_env = NULL;
  parm_pos = -1;
}

Parser::~Parser() { 
  if (lex != NULL) {
    delete lex;
  }
  if (word != NULL) {
    delete word;
  }
}

bool Parser::done_with_input() {
  return word->get_token_type() == TOKEN_EOF;
}

bool Parser::parse_program() {
  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken *>(word)->get_attribute() == KW_PROGRAM) {
    delete word;
    word = lex->next_token();
    
    if (word->get_token_type() == TOKEN_ID) {
      string* external = new string("_EXTERNAL");
      stab->install(static_cast<IdToken*>(word)->to_string(),
                    external, PROGRAM_T);
      current_env = static_cast<IdToken*>(word)->to_string();
      main_env = static_cast<IdToken*>(word)->to_string();

      delete word;
      word = lex->next_token();
      
      if (word->get_token_type() == TOKEN_PUNC && 
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) {
        delete word;
        word = lex->next_token();

        if (parse_decl_list()) {
          if (parse_block()) {
            if (word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {
              delete word;
              word = lex->next_token();
              
              return true;
            } 
            else {
              string* e = new string("second ';' in program");
              parse_error(e, word);
              delete e;
              delete word;
              return false; 
            }
          }
          else  {
            return false;
          }
        }
        else {
          return false;
        }
      }
      else {
        string* e = new string("first ';' in program");
        parse_error(e, word);
        delete e;
        delete word;
        return false; 
      }
    }
    else {
      string* e = new string("identifier in program");
      parse_error(e, word);
      delete e;
      delete word;
      return false;    
    }
  }
  else {
      string* e = new string("keyword:program in program");
      parse_error(e, word);
      delete e;
      delete word;
      return false;    
  }
}

bool Parser::parse_decl_list() {
  if ((word->get_token_type() == TOKEN_ID) ||
      (word->get_token_type() == TOKEN_KEYWORD && 
       static_cast<KeywordToken*>(word)->get_attribute() == KW_PROCEDURE) ||
      (word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_BEGIN)) {
    if (parse_variable_decl_list()) {
      return parse_procedure_decl_list();
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("expected: identifier, \"begin\" or \"procedure\"");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_variable_decl_list() {
  if (word->get_token_type() == TOKEN_ID) {
    if (parse_variable_decl()) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) {
        delete word;
        word = lex->next_token();
        return parse_variable_decl_list();
      }
      else {
        string* e = new string("; ::variable_decl_list");
        parse_error(e, word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else if ((word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken*>(word)->get_attribute() == KW_PROCEDURE) ||
           (word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken*>(word)->get_attribute() == KW_BEGIN)) {
    return true;
  }
  else {
    string* e = new string("predict of variable_decl_list");
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_variable_decl() {
  if (word->get_token_type() == TOKEN_ID) {
    if (parse_identifier_list()) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_COLON) {
        delete word;
        word = lex->next_token();

        expr_type standard_type_type;
        if(parse_standard_type(standard_type_type)) {
          stab->update_type(standard_type_type);
          return true;
        }
        else {
          return false;
        }
      }
      else {
        string* e = new string("colon :: variable_decl");
        parse_error(e, word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("predict of variable_decl"); 
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_procedure_decl_list() {
  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_PROCEDURE) {
    if (parse_procedure_decl()) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) {
        delete word;
        word = lex->next_token();

        return parse_procedure_decl_list();
      }
      else {
        string* e = new string("punc_semi--procedure_decl_list");
        parse_error(e, word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else if (word->get_token_type() == TOKEN_KEYWORD &&
           static_cast<KeywordToken*>(word)->get_attribute() == KW_BEGIN) {
    return true;
  }
  else {
    string* e = new string("predict set of procedure_decl_list");
    parse_error(e, word); 
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_procedure_decl() {
  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_PROCEDURE) {
    delete word;
    word = lex->next_token();

    if (word->get_token_type() == TOKEN_ID) {
      stab->install (static_cast<IdToken*>(word)->to_string(),
                     current_env, PROCEDURE_T);
      current_env = word->to_string();
      parm_pos = 0;
      
      delete word;
      word = lex->next_token();
        
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_OPEN) {
        delete word;
        word = lex->next_token();
  
        if (parse_arg_list()) {
          if (word->get_token_type() == TOKEN_PUNC &&
              static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) {
            delete word;
            word = lex->next_token();
            
            if (parse_variable_decl_list()) {
              if (parse_block()) {
                current_env = main_env;
                return true;
              }
            }
            else {
              return false;
            }
          }
          else {
            string* e = new string("punc_close :: procedure_decl");
            parse_error(e, word);
            delete e;
            delete word;
            return false;
          }
        } 
        else {
          return false;
        }     
      }
      else {
        string* e = new string("open parenthesis in procedure_decl");
        parse_error(e, word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      string* e = new string("identifier in parse_procedure_decl");
      parse_error(e, word);
      delete e;
      delete word;
      return false;
    }
  }
  else {
    string* e = new string("keyword: procedure in procedure_decl");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_arg_list() {
  expr_type standard_type_type;

  if (word->get_token_type() == TOKEN_ID) {
    if (parse_identifier_list()) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_COLON) {
        delete(word);
        word = lex->next_token();

        if (parse_standard_type(standard_type_type)) {
		      stab->set_parm_pos_arg_list(parm_pos, standard_type_type);
          if (parse_arg_list_hat()) {
            return true;
          }
          else {
            return false;
          }
        }
        else {
          return false;
        }
      } 
      else {
        string* e = new string("colon :: arg_list ");
        parse_error(e, word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }

  }
  else if (word->get_token_type() == TOKEN_PUNC && 
           static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) {
    return true; 
  }
  else {
    string* e = new string("predict of arg_list");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_arg_list_hat() {
  if (word->get_token_type() == TOKEN_PUNC && 
      static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) {
    delete word;
    word = lex->next_token();

    if (parse_arg_list()) {
      return true;
    }
    else {
      return false;
    }
  }
  else if (word->get_token_type() == TOKEN_PUNC &&
           static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) {
    return true; 
  }
  else {
    string* e = new string("predict of arg_list_hat");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_identifier_list() {
  if (word->get_token_type() == TOKEN_ID) {
    stab->install(static_cast<IdToken*>(word)->to_string(),
                   current_env, UNKNOWN_T);
    delete word;
    word = lex->next_token();
    return parse_identifier_list_prime();
  }
  else {
    string* e = new string("identifier id_list (predict of id_list)");
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_identifier_list_prime() {
  if (word->get_token_type() == TOKEN_PUNC &&
      static_cast<PuncToken*>(word)->get_attribute() == PUNC_COMMA) {
    delete word;
    word = lex->next_token();
    if (word->get_token_type() == TOKEN_ID) {
      stab->install(static_cast<IdToken*>(word)->to_string(),
                     current_env, UNKNOWN_T);
      delete word;
      word = lex->next_token();

      return parse_identifier_list_prime();
    }
    else {
      string* e = new string("identifier :: parse_id_list_prime");
      parse_error(e, word);
      delete word;
      delete e;
      return false;
    }
  }
  else if (word->get_token_type() == TOKEN_PUNC &&
           static_cast<PuncToken*>(word)->get_attribute() == PUNC_COLON) {
    return true;
  }
  else {
    string* e = new string("predict of id_list_prime");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_standard_type(expr_type &standard_type_type) {
  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_INT) {
    standard_type_type = INT_T;
    delete word;
    word = lex->next_token();
    return true;
  }
  else if (word->get_token_type() == TOKEN_KEYWORD &&
           static_cast<KeywordToken*>(word)->get_attribute() == KW_BOOL) {
    standard_type_type = BOOL_T;
    
    delete word;
    word = lex->next_token();
    return true;
  }
  else {
    string* e = new string("predict of standard_type");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_block() {
  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_BEGIN) {
    delete word;
    word = lex->next_token();
    if (parse_stmt_list()) {
      if (word->get_token_type() == TOKEN_KEYWORD &&
          static_cast<KeywordToken*>(word)->get_attribute() == KW_END) {
        delete word;
        word = lex->next_token();
        return true;
      }
      else {
        string* e = new string("kw_end :: block");
        parse_error(e, word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("KW_BEGIN :: block");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_stmt_list() {
  if ((word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_IF) ||
      (word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_WHILE) ||
      (word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_PRINT) ||
      (word->get_token_type() == TOKEN_ID)) {
    if (parse_stmt()) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) {
        delete(word);
        word = lex->next_token();
        
        return parse_stmt_list_prm();
      }
      else {
        string* e = new string("; :: stmt_list");
        parse_error(e, word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("predict of stmt_list");
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_stmt_list_prm() {
  if ((word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_IF) ||
      (word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_WHILE) ||
      (word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_PRINT) ||
      (word->get_token_type() == TOKEN_ID)) {
    if (parse_stmt()) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) {
        delete word;
        word = lex->next_token();
        
        return parse_stmt_list_prm();
      }
      else {
        string* e = new string("; :: stmt_list_prm ");
        parse_error(e , word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else if (word->get_token_type() == TOKEN_KEYWORD &&
           static_cast<KeywordToken*>(word)->get_attribute() == KW_END) {
    return true;
  }
  else {
    string* e = new string("predict of stmt_list_prm");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_stmt() {
  expr_type stmt_ass_proc_tail_type;

  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_IF) {
    return parse_if_stmt();
  }
  else if (word->get_token_type() == TOKEN_KEYWORD &&
           static_cast<KeywordToken*>(word)->get_attribute() == KW_WHILE) {
    return parse_while_stmt();
  }
  else if (word->get_token_type() == TOKEN_KEYWORD &&
           static_cast<KeywordToken*>(word)->get_attribute() == KW_PRINT) {
    return parse_print_stmt();
  }
  else if (word->get_token_type() == TOKEN_ID) {
    expr_type id_type;
    proc_call_env = static_cast<IdToken*>(word)->to_string();
    if ((!stab->is_decl(static_cast<IdToken*>(word)->to_string(), current_env) &&
         !stab->is_decl(static_cast<IdToken*>(word)->to_string(), main_env))) {
      undeclared_id_error(word->to_string(), current_env);
    }
    
    id_type = stab->get_type(static_cast<IdToken*>(word)->to_string(), current_env);
    delete(word);
    word = lex->next_token();
    if (parse_stmt_ass_proc_tail(stmt_ass_proc_tail_type)) {
      if(id_type != stmt_ass_proc_tail_type) {
        type_error(word);
      }
      return true;
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("predict of stmt");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_stmt_ass_proc_tail(expr_type &stmt_ass_proc_tail_type) {
  expr_type assignment_stmt_tail_type;
  if (word->get_token_type() == TOKEN_PUNC &&
      static_cast<PuncToken*>(word)->get_attribute() == PUNC_ASSIGN) {
    if (parse_assignment_stmt_tail(assignment_stmt_tail_type)) {
      stmt_ass_proc_tail_type = assignment_stmt_tail_type;
      return true;
    }
    else {
      return false;
    }
  }
  else if (word->get_token_type() == TOKEN_PUNC &&
           static_cast<PuncToken*>(word)->get_attribute() == PUNC_OPEN) {
    if (parse_procedure_call_stmt_tail()) {
      stmt_ass_proc_tail_type = PROCEDURE_T;
      return true;
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("predict of stmt_ass_proc_tail :: stmt_ass_proc_tail");
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_assignment_stmt_tail(expr_type &assignment_stmt_tail_type) {
  expr_type expr_type;

  if (word->get_token_type() == TOKEN_PUNC &&
      static_cast<PuncToken*>(word)->get_attribute() == PUNC_ASSIGN) {
    delete(word);
    word = lex->next_token();
    if (parse_expr(expr_type)) {
      assignment_stmt_tail_type = expr_type;
      return true;
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string(":= :: assignment_stmt_tail ");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_if_stmt() {
  expr_type expr_type;

  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_IF) {
    delete(word);
    word = lex->next_token();
    if (parse_expr(expr_type)) {
      if (expr_type != BOOL_T) {
        type_error(word);
      }
      if (word->get_token_type() == TOKEN_KEYWORD &&
          static_cast<KeywordToken*>(word)->get_attribute() == KW_THEN) {
        delete word;
        word = lex->next_token(); 
        
        if (parse_block()) {
          return parse_if_stmt_hat();
        }
        else {
          return false;
        }
      }
      else {
        string* e = new string("kw_then :: if_stmt");
        parse_error(e , word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("kw_if :: if_stmt");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_if_stmt_hat() {
  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_ELSE) {
    delete(word);
    word = lex->next_token();

    return parse_block();
  }
  else if (word->get_token_type() == TOKEN_PUNC &&
           static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) {
    return true;
  }
  else {
    string* e = new string("predict of if_stmt_hat :: if_stmt_hat");
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_while_stmt() {
  expr_type expr_type;
  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_WHILE) {
    delete word;
    word = lex->next_token();
    if (parse_expr(expr_type)) {
      if (expr_type != BOOL_T) {
        type_error(word);
      }
      return parse_block();
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("KW_WHILE :: while_stmt");
    parse_error(e, word);
    delete(word);
    delete(e);
    return false;
  }
}

bool Parser::parse_print_stmt() {
  expr_type expr_type;
  if (word->get_token_type() == TOKEN_KEYWORD &&
      static_cast<KeywordToken*>(word)->get_attribute() == KW_PRINT) {
    delete(word);
    word = lex->next_token();
    if (parse_expr(expr_type)) {
      if (expr_type != INT_T) {
        type_error(word);
      }
      return true;
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("KW_PRINT :: print_stmt");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_procedure_call_stmt_tail() {
  if (word->get_token_type() == TOKEN_PUNC &&
      static_cast<PuncToken*>(word)->get_attribute() == PUNC_OPEN) {
    delete word;
    word = lex->next_token();
    
    parm_pos = 0;

    if (parse_expr_list()) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) {
        delete(word);
        word = lex->next_token();
        return true;
      }
      else {
        string* e = new string("(:: procedure_call_stmt_tail");
        parse_error(e , word);
        delete word;
        delete e;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string(") :: procedure_call_stmt_tail");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_expr_list() {
  expr_type expr_type;

  if ((word->get_token_type() == TOKEN_ID) ||
      (word->get_token_type() == TOKEN_NUM) ||
      (word->get_token_type() == TOKEN_PUNC &&
       static_cast<PuncToken*>(word)->get_attribute() == PUNC_OPEN) ||
      (word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_NOT) ||
      (word->get_token_type() == TOKEN_ADDOP &&
       static_cast<AddopToken*>(word)->get_attribute() == ADDOP_ADD) ||
      (word->get_token_type() == TOKEN_ADDOP &&
       static_cast<AddopToken*>(word)->get_attribute() == ADDOP_SUB)) {
    if (parse_expr(expr_type)) {
      if (expr_type != stab->get_type(proc_call_env, parm_pos)) {
        type_error(word);
      }
      parm_pos++;

      return parse_expr_list_hat();
    }
    else {
      return false;
    }
  }
  else if (word->get_token_type() == TOKEN_PUNC &&
           static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) {
    return true;
  }
  else {
    string* e = new string("predict of expr_list");
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_expr_list_hat() {
  if (word->get_token_type() == TOKEN_PUNC &&
      static_cast<PuncToken*>(word)->get_attribute() == PUNC_COMMA) {
    delete(word);
    word = lex->next_token();
  
    return parse_expr_list();
  }
  else if (word->get_token_type() == TOKEN_PUNC &&
           static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) {
    return true;
  }
  else {
    string* e = new string("predict of expr_list_hat");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_expr(expr_type &the_expr_type) {
  expr_type simple_expr_type;
  expr_type expr_hat_type;

  if ((word->get_token_type() == TOKEN_ID) ||
      (word->get_token_type() == TOKEN_NUM) ||
      (word->get_token_type() == TOKEN_KEYWORD && 
       static_cast<KeywordToken*>(word)->get_attribute() == KW_NOT) ||
      (word->get_token_type() == TOKEN_PUNC &&
       static_cast<PuncToken*>(word)->get_attribute() == PUNC_OPEN) ||
      (word->get_token_type() == TOKEN_ADDOP &&
       static_cast<AddopToken*>(word)->get_attribute() == ADDOP_ADD) ||
      (word->get_token_type() == TOKEN_ADDOP &&
       static_cast<AddopToken*>(word)->get_attribute() == ADDOP_SUB)) {
    if (parse_simple_expr(simple_expr_type)) {
      if (parse_expr_hat(expr_hat_type)) {
        if (expr_hat_type == NO_T) {
          the_expr_type = simple_expr_type;
        }
        else if (simple_expr_type == INT_T && expr_hat_type == INT_T) {
            the_expr_type = BOOL_T;
        }
        else {
          type_error(word);
        }
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("predict of expr");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_expr_hat(expr_type &expr_hat_type) {
  expr_type simple_expr_type;

  if (word->get_token_type() == TOKEN_RELOP) {
    delete(word);
    word = lex->next_token();
    
    if (parse_simple_expr(simple_expr_type)) {
      if (simple_expr_type == INT_T) {
        expr_hat_type = INT_T;
      }
      else {
        type_error(word);
      }
      return true;
    }
    else {
      return false;
    }
  }
  else if ((word->get_token_type() == TOKEN_PUNC &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) ||
           (word->get_token_type() == TOKEN_PUNC &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_COMMA) ||
           (word->get_token_type() == TOKEN_PUNC &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) ||
           (word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken*>(word)->get_attribute() == KW_BEGIN) ||
           (word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken*>(word)->get_attribute() == KW_THEN)) {
    expr_hat_type = NO_T;
    return true; 
  }
  else {
    string* e = new string("predict parse_expr_hat ");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_simple_expr(expr_type &simple_expr_type) {
  expr_type simple_expr_prm_type;
  expr_type term_type;

  if ((word->get_token_type() == TOKEN_ID) ||
      (word->get_token_type() == TOKEN_NUM) ||
      (word->get_token_type() == TOKEN_PUNC &&
       static_cast<PuncToken*>(word)->get_attribute() == PUNC_OPEN) ||
      (word->get_token_type() == TOKEN_ADDOP &&
       static_cast<AddopToken*>(word)->get_attribute() == ADDOP_ADD) ||
      (word->get_token_type() == TOKEN_ADDOP &&
       static_cast<AddopToken*>(word)->get_attribute() == ADDOP_SUB) ||
      (word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_NOT)) {
    if (parse_term(term_type)) {
      if (parse_simple_expr_prm(simple_expr_prm_type)) {
        if (simple_expr_prm_type == NO_T) {
          simple_expr_type = term_type;
        }
        else if (term_type == simple_expr_prm_type) {
          simple_expr_type = term_type;
        }
        else {
          type_error(word);
        }
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("predict simple_expr :: simple_expr ");
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_simple_expr_prm(expr_type &simple_expr_prm_type) {
  expr_type simple_expr_prm_inside;
  expr_type term_type;
  Token* addop;

  if (word->get_token_type() == TOKEN_ADDOP) {
    expr_type addop_type;
    if (static_cast<AddopToken*>(word)->get_attribute() == ADDOP_OR) {
      addop_type = BOOL_T;
    }
    else {
      addop_type = INT_T;
    }

    delete word;
    word = lex->next_token();
    
    if (parse_term(term_type)) {
      if (parse_simple_expr_prm(simple_expr_prm_inside)) {
        if (simple_expr_prm_inside == NO_T) {
          if (addop_type == term_type) {
            simple_expr_prm_type = addop_type;
          }
          else {
            type_error(word);
          }
        }
        else if (addop_type == term_type && term_type == simple_expr_prm_inside) {
          simple_expr_prm_type = addop_type;
        }
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  else if ((word->get_token_type() == TOKEN_RELOP) ||
           (word->get_token_type() == TOKEN_PUNC  &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) ||
           (word->get_token_type() == TOKEN_PUNC &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_COMMA) ||
           (word->get_token_type() == TOKEN_PUNC &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) ||
           (word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken*>(word)->get_attribute() == KW_BEGIN) ||
           (word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken*>(word)->get_attribute() == KW_THEN)) {
    simple_expr_prm_type = NO_T;
    return true;
  }
  else {
    string* e = new string("predict of simple_expr_prm :: simple_expr_prm ");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
}

bool Parser::parse_term(expr_type &term_type) {
  if ((word->get_token_type() == TOKEN_ID) ||
      (word->get_token_type() == TOKEN_NUM) ||
      (word->get_token_type() == TOKEN_PUNC &&
       static_cast<PuncToken*>(word)->get_attribute() == PUNC_OPEN) ||
      (word->get_token_type() == TOKEN_ADDOP &&
       static_cast<AddopToken*>(word)->get_attribute() == ADDOP_ADD) ||
      (word->get_token_type() == TOKEN_ADDOP &&
       static_cast<AddopToken*>(word)->get_attribute() == ADDOP_SUB) ||
      (word->get_token_type() == TOKEN_KEYWORD &&
       static_cast<KeywordToken*>(word)->get_attribute() == KW_NOT)) {
    expr_type factor_type;
    expr_type term_prm_type;
    if (parse_factor(factor_type)) {
      if (parse_term_prm(term_prm_type)) {
        if (term_prm_type == NO_T) {
          term_type = factor_type;
        }
        else if (factor_type == term_prm_type) {
          term_type = factor_type;
        }
        else {
          type_error(word);
        }
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    string* e = new string("predict parse_term :: parse_term ");
    parse_error(e, word);
    delete word;
    delete e;
    return false; 
  }

}

bool Parser::parse_term_prm(expr_type &term_prm_type) {
  expr_type term_prm_inside_type;
  expr_type factor_type;
  expr_type mulop_type;

  if (word->get_token_type() == TOKEN_MULOP) {
    if (static_cast<MulopToken*>(word)->get_attribute() == MULOP_AND) {
      mulop_type = BOOL_T;
    }
    else {
      mulop_type = INT_T;
    }
    delete(word);
    word = lex->next_token();
    
    if (parse_factor(factor_type)) {
      if (parse_term_prm(term_prm_inside_type)) {
        if (term_prm_inside_type == NO_T && mulop_type == factor_type) {
          term_prm_type = mulop_type;
        }
        else if (mulop_type == factor_type && factor_type == term_prm_inside_type) {
          term_prm_type = mulop_type;
        }
        else {
          type_error(word);
        }
        return true;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  else if ((word->get_token_type() == TOKEN_ADDOP) ||
           (word->get_token_type() == TOKEN_RELOP) ||
           (word->get_token_type() == TOKEN_PUNC &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_SEMI) ||
           (word->get_token_type() == TOKEN_PUNC &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_COMMA) ||
           (word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken*>(word)->get_attribute() == KW_THEN) ||
           (word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken*>(word)->get_attribute() == KW_BEGIN) ||
           (word->get_token_type() == TOKEN_PUNC  &&
            static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE)) {
              
    term_prm_type = NO_T;
    return true;   
  }
  else {
    string* e = new string("predict :: term_prm ");
    parse_error(e, word);
    delete(e);
    delete(word);
    return false;
  }
}

bool Parser::parse_factor(expr_type &factor_type) {
  if (word->get_token_type() == TOKEN_ID) {
    if (stab->is_decl(static_cast<IdToken*>(word)->to_string(), current_env)) {
      factor_type = stab->get_type(static_cast<IdToken*>(word)->to_string(),
                                   current_env);
    }
    else {
      undeclared_id_error(static_cast<IdToken*>(word)->to_string(), current_env);
    }
    
    delete(word);
    word = lex->next_token();
    
    return true;
  }
  else if (word->get_token_type() == TOKEN_NUM) {
    factor_type = INT_T;
    delete(word);
    word = lex->next_token();
    return true;
  }
  else if (word->get_token_type() == TOKEN_PUNC  &&
           static_cast<PuncToken*>(word)->get_attribute() == PUNC_OPEN) {
    delete(word);
    word = lex->next_token();

    expr_type expr_type;
    if (parse_expr(expr_type)) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cast<PuncToken*>(word)->get_attribute() == PUNC_CLOSE) {
        factor_type = expr_type;
        delete(word);
        word = lex->next_token();
        return true;
      }
      else {
        string* e = new string(") :: factor");
        parse_error(e , word);
        delete e;
        delete word;
        return false;
      }
    }
    else {
      return false;
    }
  }
  else if (word->get_token_type() == TOKEN_KEYWORD &&
           static_cast<KeywordToken*>(word)->get_attribute() == KW_NOT) {
    delete(word);
    word = lex->next_token();
    
    expr_type factor_inside_type;
    if (parse_factor(factor_inside_type)) {
      if (factor_inside_type == BOOL_T) {
        factor_type = INT_T;
      }
      else {
        type_error(word);
      }
      return true;
    }
    else {
      return false;
    }
  }
  else if (parse_sign()) {
    expr_type factor_inside_type;
    if (parse_factor(factor_inside_type)) {
      if (factor_inside_type == INT_T) {
        factor_type = INT_T;
      }
      else {
        type_error(word);
      }
      return true;
    }
    else {
      printf("failed parse factor :: factor ");
      return false;
    }
  }
  else {
    string* e = new string("id , num , (, not , + , - ") ;
    parse_error(e, word);
    delete word;
    delete e;
    return false;
  }
}

bool Parser::parse_sign() {
  if (word->get_token_type() == TOKEN_ADDOP) {
    if (static_cast<AddopToken*>(word)->get_attribute() == ADDOP_ADD) {
      delete(word);
      word = lex->next_token();
      
      return true;
    }
    else if (static_cast<AddopToken*>(word)->get_attribute() == ADDOP_SUB) {
      delete(word);
      word = lex->next_token();
      return true;
    }
    else {
      string* e = new string("+ / - ::sign");
      parse_error(e, word);
      delete e;
      delete word;
      return false;
    }
  }
  else {
    string* e = new string("predict of sign");
    parse_error(e , word);
    delete e;
    delete word;
    return false;
  }
}

void Parser::parse_error(string* e, Token* got) {
  cout << "Parse_Error" << endl;
  cout << "Expected: " << *e << endl;
  cout << "Got: " << *got->to_string() << endl;
}

void Parser::type_error(Token* where) {
  cout << "Type_Error" << endl;
  cout << "Where: " << *where->to_string() << endl;
  exit(1);
}

void Parser::undeclared_id_error(string* id, string* env) {
  cout << "undeclared_identifier" << endl;
  cout << "Identifier: " << *id << endl;
  cout << "Environment: " << *env << endl;
  exit(1);
}
