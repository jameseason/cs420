#include "buffer.h"

Buffer::Buffer (char *filename)
{
  // Open the file and fill the buffer.
  source_file.open (filename);
  if (source_file.fail()) {
    // Failed to open source file.
    cerr << "Can't open source file " << *filename << endl;
    buffer_fatal_error();
  }
  buff = new list<char>();
}

Buffer::~Buffer()
{
  source_file.close();
}

char Buffer::next_char()
{
  if( buff->empty() )
  {
    fill_buf();
  }
  char next = *buff->begin();
  buff->pop_front();
  return next;
}

void Buffer::unread_char( char c )
{
  if (buff->size() > MAX_BUFFER_SIZE)
  {
    buffer_fatal_error();
  }
  else
  {
    buff->push_front(c);
  }
}

void Buffer::fill_buf()
{
  char c;
  int bufloc = 0;
  while (bufloc < MAX_BUFFER_SIZE)
  {
    c = source_file->get();
    if (!source_file->eof())
    {
      if (is_whitespace(c))
      {
        char buf_c = buff->back();
        if (!is_whitespace(buf_c))
        {
          buff->push_back(' ');
        }
      }
      else if (c == COMMENT_MARKER)
      {
        move_through_comment();
      }
      else
      {
        if (!is_accepted_char(c)) 
        {
          buffer_fatal_error();
        }
        else
        {
          buff->push_back(c);
        }
      }
    }
    else
    {
      buff->push_back(EOF_MARKER);
      bufloc = MAX_BUFFER_SIZE;
    }
    bufloc++;
  }
}

void Buffer::move_through_comment()
{
  char contents;
  contents = source_file->get();
  while(contents != '\n')
  {
    contents = source_file->get();
  }
}

bool Buffer::is_accepted_char(char c)
{
  return is_alphanumeric(c) || is_acceptedsymbol(c);
}

void Buffer::clear_buff()
{
  while(!buff->empty())
  {
    buff->pop_front();
  }
}

void Buffer::buffer_fatal_error() const
{
  cerr << "Exiting on BUFFER FATAL ERROR" << endl;
  exit (-1);
}
