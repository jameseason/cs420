#include "parser.h"
#include <iostream>

Parser::Parser(Scanner* s) {
  lex := s;
  
  word = lex->next_token();
  stab = new Symbol_Table();

  e = new Emitter();
  ra = new Register_Allocator(e, stab);

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
  if (ra != NULL) {
    delete ra;
  }
  if (e != NULL)  {
    delete e;
  }
}

bool Parser::done_with_input() {
  return word->get_token_type() == TOKEN_EOF;
}

bool Parser::parse_program() {
  if (word->get_token_type() == TOKEN_KEYWORD && 
      static_cast<KeywordToken*>(word)->get_attribute() == KW_PROGRAM) {
    delete word;
    word = lex->next_token();

    if (word->get_token_type() == TOKEN_ID) {
      string* external = new string("_EXTERNAL");
      stab->install(static_cast<IdToken*>(word)->to_string(),
                    external, PROGRAM_T);
      current_env = static_cast<IdToken*>(word)->to_string();
      main_env = static_cast<IdToken*>(word)->to_string();

      string* program_label = e->get_new_label("_Start:");
      e->emit_label(program_label);
      delete(program_label);

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
              
              e->emit_halt();
              stab->emit_data_directives();

              return true;
            } 
            else { // failure to match second semicolon
              string* expected = new string("second ';' in program");
              parse_error(expected, word);
              delete expected;
              delete word;
              return false; 
            }
          }
          else {  // failure to parse block
            return false;
          }
        }
        else { // failure to parse decl_list
          return false;
        }
      }
      else { //failure to match first semicolon
        string* expected = new string("first ';' in program");
        parse_error(expected, word);
        delete expected;
        delete word;
        return false; 
      }
    }
    else { //failure to match identifier
      string* expected = new string("identifier in program");
      parse_error(expected, word);
      delete expected;
      delete word;
      return false;    
    }
  }
  else {
    string* expected = new string("keyword:program in program");
    parse_error(expected, word);
    delete expected;
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
    string* exp = new string("expected: ident, \"begin\", or \"procedure\"");
    parse_error(exp, word);
    delete exp;
    delete word;
    return false;
  }
}  

bool Parser::parse_variable_decl_list() {
  if (word->get_token_type() == TOKEN_ID) {
    if (parse_variable_decl()) {
      if (word->get_token_type() == TOKEN_PUNC &&
          static_cst<PuncToken*>(word)->get_attribute() == PUNC_SEMI) {
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
    parse_error(e, word);
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
        if (parse_standard_type(standard_type_type)) {
          stab->update_type(standard_type_type);
          return true;
        }
        else {
          return false;
        }
      }
      else {
        string* e = new string("colon :: varible_decl");
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
    parse_error(e, word);
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
  else {
    string* e = new string("predict set of procedure_decl_list");
    parse_error(e, word);
    delete e;
    delete word;
    return false;
  }
} //354




