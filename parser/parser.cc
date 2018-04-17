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
        if (parse_decl_list())
        {

        }
      }
    }
  }
}
