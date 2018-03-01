#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <list>

/* We will use the dollar sign to mark the end of the file in the
   buffer. */
#define EOF_MARKER '$'

#define COMMENT_MARKER '#'
#define SPACE ' '
#define TAB '\t'
#define NEW_LINE '\n'

using namespace std;

class Buffer
{
 public:

  // Open the program file and initialize the scanner buffer.
  Buffer(char *filename);
  
  // Close the file and cleanup.
  ~Buffer();
  
  // Remove the next character from the buffer and
  // return it.
  char next_char();
  
  // Put a character back at the front of the buffer.
  void unread_char (char c);
  
  
 private:
 
 list<char> *buff
 
 static const int MAX_BUFFER_SIZE = 1024;
 
  // The stream object for the source file.
  ifstream source_file;
  
  /* If something catastrophic happens in the buffer, print
     an error message and then call this method to exit. */
  void buffer_fatal_error();

  void move_through_comment();
  void is_accepted_char(char c);
  void fill_buf();
  void clear_buff();
  // Useful utility function.  Is c a whitespace char?
  inline bool is_whitespace (const char c)
  {
    return (c == SPACE || c == TAB || c == NEW_LINE);
  }
  
  // Probably some other stuff too.
  
};

#endif
