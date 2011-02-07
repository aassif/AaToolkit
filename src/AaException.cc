#include <sstream>
#include <cctype>
#include "bits/AaException.h"

using namespace std;

namespace Aa
{
  /* printable */

  string printable (unsigned char c) throw ()
  {
    ostringstream o;
    if (isprint (c))
      o << '\'' << c << '\'';
    else
      o << '\\' << (unsigned int) c;
    return o.str ();
  }

  string printable (const string & s) throw ()
  {
    if (s.empty ()) return "(null)";

    ostringstream o;
    o << '"' << s << '"';
    return o.str ();
  }

  /* FileNotFound */

  string FileNotFound::msg (const string & filename)
  {
    ostringstream o;
    o << "file " << printable (filename) << " not found!";
    return o.str ();
  }

  FileNotFound::FileNotFound (const string & filename) :
    runtime_error (FileNotFound::msg (filename)) {}

  FileNotFound::~FileNotFound () throw () {}

  /* ParseError */

  string ParseError::msg (const string & expected, const string & received)
  {
    ostringstream o;
    o << "expected " << expected << ", got " << received << '!';
    return o.str ();
  }

  ParseError::ParseError (unsigned char expected) :
    runtime_error (ParseError::msg (printable (expected), "<EOF>")) {}

  ParseError::ParseError (unsigned char expected, unsigned char received) :
    runtime_error (ParseError::msg (printable (expected),
                                    printable (received))) {}

  ParseError::ParseError (const string & expected) :
    runtime_error (ParseError::msg (printable (expected), "<EOF>")) {}

  ParseError::ParseError (const string & expected, const string & received) :
    runtime_error (ParseError::msg (printable (expected),
                                    printable (received))) {}

  ParseError::~ParseError () throw () {}

  /* FormatError */

  string FormatError::msg (const string & received, const string & expectedType)
  {
    ostringstream o;
    o << printable (received) << " is not of type " << expectedType << '!';
    return o.str ();
  }

  FormatError::FormatError (const string & received, const string & expectedType) :
    invalid_argument (FormatError::msg (received, expectedType)) {}

  FormatError::~FormatError () throw () {}
}
