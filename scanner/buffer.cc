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

  // ....
}

Buffer::~Buffer()
{
	source_file.close();
}



void Buffer::buffer_fatal_error() const
{
  cerr << "Exiting on BUFFER FATAL ERROR" << endl;
  exit (-1);
}
