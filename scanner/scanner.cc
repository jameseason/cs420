#include "scanner.h"

#define EXIT_STATE -1

Scanner::Scanner (char *filename)
{
  // Create and fill the buffer.
  buf = new Buffer (filename);
}

Scanner::~Scanner ()
{
  delete buf;
}

Token *Scanner::next_token()
{
  Token *lexeme = NULL;
  string *attr = new string();
  char c;
  int state = 0;
  
  while (state != EXIT_STATE)
  {
    c = buf->next_char();
    switch( state )
    {
      case 0:
        if( c == EOF_MARKER )
        {
          state = -1;
          lexeme = new EofToken();
          break;
        }
        else if( c == ';' )
        {
          state = -1;
          lexeme = new PuncToken( PUNC_SEMI );
          break;
        }
        else if( c == ',' )
        {
          state = -1;
          lexeme = new PuncToken( PUNC_COMMA );
          break;
        }
        else if( c == '(' )
        {
          state = -1;
          lexeme = new PuncToken( PUNC_OPEN );
          break;
        }
        else if( c == ')' )
        { 
          state = -1;
          lexeme = new PuncToken( PUNC_CLOSE );
          break;
        }
        else if( c == '=' )
        {
          state = -1;
          lexeme = new RelopToken( RELOP_EQ );
          break;
        }
        else if( c == '+' )
        {
          state = -1;
          lexeme = new AddopToken( ADDOP_ADD );
          break;
        }
        else if( c == '-' )
        {
          state = -1;
          lexeme = new AddopToken( ADDOP_SUB );
          break;
        }
        else if( c == '*' )
        {
          state = -1;
          lexeme = new MulopToken( MULOP_MUL );
          break;
        }
        else if( c == '/' )
        {
          state = -1;
          lexeme = new MulopToken( MULOP_DIV );
          break;
        }
        else if( c == '<' )
        {
          state = 1;
          break;
        }
        else if( c == '>' )
        {
          state = 2;
          break;
        }
	      else if( c == ':' )
        { 
          state = 56;
          break;
        }
        else if( c == ' ' )
        {
          state = 0;
          break;
        }
        else if( is_digit( c ) )
        {
          state = 5;
          *attr+=c;
          break;
        }
        else if( c == 'b' )
        {
          state = 3;
          *attr+=c;
          break;
        }
        else if( c == 'e' )
        {
          state = 13;
          *attr+=c;
          break;
        }
        else if( c == 'i' )
        {
          state = 19;
          *attr+=c;
          break;
        }
        else if( c == 'n' )
        {
          state = 23;
          *attr+=c;
          break;
        }
        else if( c == 'p' )
        {
          state = 28;
          *attr+=c;
          break;
        }
        else if( c == 't' )
        {
          state = 44;
          *attr+=c;
          break;
        }
        else if( c == 'w' )
        {
          state = 48;
          *attr+=c;
          break;
        }
        else if( c == 'o' )
        {
          state = 26;
          *attr+=c;
          break;
        }
        else if( c == 'a' )
        {
          state = 53;
          *attr+=c;
          break;
        }
        else if( is_alpha( c ) )
        {
          state = 4;
          *attr+=c;
          break;
        }
        else
        {
          scanner_fatal_error( "error" );
        }
      case 1:
        if( c == '>' )
        {
          state = -1;
          lexeme = new RelopToken( RELOP_NE );
          break;
        }
        else if( c == '=' )
        {
          state = -1;
          lexeme = new RelopToken( RELOP_LE );
          break;
        }
        else
        {
          buf->unread_char( c );
          state = -1;
          lexeme = new RelopToken( RELOP_LT );
          break;
        }
      case 2:
        if( c == '=' )
        {
          state = -1;
          lexeme = new RelopToken( RELOP_GE );
          break;
        }
        else
        {
          buf->unread_char( c );
          state = -1;
          lexeme = new RelopToken( RELOP_GT );
          break;
        }
      case 3:
        if( c == 'o' )
        {
          state = 6;
          *attr+=c;
          break;
        }
        else if( c == 'e' )
        {
          state = 9;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      // accepting identifiers
      case 4:
        if( is_alphanum( c ) )
        {
          state = 4;
          *attr+=c;
          break;
        }
        else
        {
          buf->unread_char( c );
          state = -1;
          lexeme = new IdToken( attr );
          break;
        }
      // accepting digits
      case 5:
        if( is_digit( c ) )
        {
          state = 5;
          *attr+=c;
          break;
        }
        else
        {
          buf->unread_char( c );
          state = -1;
          lexeme = new NumToken( attr );
          break;
        }
      case 6:
        if( c == 'o' )
        {
          state = 7;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 7:
        if( c == 'l' )
        {
          state = 8;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 8:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_BOOL );
          break; 
        }
        else
        {
          state = 4;
          break;
        }
      case 9:
        if( c == 'g' )
        {
          state = 10;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 10:
        if( c == 'i' )
        {
          state = 11;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 11:
        if( c == 'n' )
        {
          state = 12;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 12:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_BEGIN ); 
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 13:
        if( c == 'n' )
        {
          state = 14;
          *attr+=c;
          break;
        }
        else if( c == 'l' )
        {
          state = 16;
          *attr+=c;
          break;
        }
        else
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 14:
        if( c == 'd' )
        {
          state = 15;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 15:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_END );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 16:
        if( c == 's' )
        {
          state = 17;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 17:
        if( c =='e' )
        {
          state = 18;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 18:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_ELSE );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 19:
        if( c == 'n' )
        {
          state = 20;
          *attr+=c;
          break;
        }
        else if( c == 'f' )
        {
          state = 22;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 20:
        if( c == 't' )
        {
          state = 21;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 21:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_INT );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 22:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_IF );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 23:
        if( c == 'o' )
        {
          state = 24;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 24:
        if( c == 't' )
        {
          state = 25;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 25:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_NOT );
          break;
        }
        else 
        {
          state = 4;
          break;
        }

      case 28:
        if( c == 'r' )
        {
          state = 29;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 29:
        if( c == 'o' )
        {
          state = 33;
          *attr+=c;
          break;
        }
        else if( c == 'i' )
        {
          state = 30;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 30:
        if( c == 'n' )
        {
          state = 31;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 31:
        if( c == 't' )
        {
          state = 32;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 32:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_PRINT );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 33:
        if( c == 'g' )
        {
          state = 34;
          *attr+=c;
          break;
        }
        else if( c == 'c' )
        {
          state = 38;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 34:
        if( c == 'r' )
        {
          state = 35;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 35:  
        if( c == 'a' )
        {
          state = 36;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 36:
        if( c == 'm' )
        {
          state = 37;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 37:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_PROGRAM );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 38:
        if( c == 'e' )
        {
          state = 39;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 39:
        if( c == 'd' )
        {
          state = 40;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 40:
        if( c == 'u' )
        {
          state = 41;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 41:
        if( c == 'r' )
        {
          state = 42;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 42:
        if( c == 'e' )
        {
          state = 43;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 43:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_PROCEDURE );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 44:
        if( c == 'h' )
        {
          state = 45;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 45:
        if( c == 'e' )
        {
          state = 46;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 46:
        if( c == 'n' )
        {
          state = 47;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 47:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_THEN );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 48:
        if( c == 'h' )
        {
          state = 49;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 49:
        if( c == 'i' )
        {
          state = 50;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 50:
        if( c == 'l' )
        {
          state = 51;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 51:
        if( c == 'e' )
        {
          state = 52;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 52:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new KeywordToken( KW_WHILE );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 26:
        if( c == 'r' )
        {
          state = 27;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 27:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new AddopToken( ADDOP_OR );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 53:
        if( c == 'n' )
        {
          state = 54;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 54:
        if( c == 'd' )
        {
          state = 55;
          *attr+=c;
          break;
        }
        else 
        {
          buf->unread_char( c );
          state = 4;
          break;
        }
      case 55:
        buf->unread_char( c );
        if( !is_alphanum( c ) )
        {
          state = -1;
          lexeme = new MulopToken( MULOP_AND );
          break;
        }
        else 
        {
          state = 4;
          break;
        }
      case 56:
	      if( c == '=' )
        {
          state = -1;
          lexeme = new PuncToken( PUNC_ASSIGN );
          break;
        }
        else
        {
          buf->unread_char( c );
          state = -1;
          lexeme = new PuncToken( PUNC_COLON );
	        break;
        }
    }
  }
  
  if( lexeme == NULL )
  {
    scanner_fatal_error("lexeme is null");
  }
  return lexeme; 
}



void Scanner::scanner_fatal_error(const string& message)
{
  cerr << "Exiting on Scanner Fatal Error: " << message << endl;
  exit (-1);
}




