#include <sstream>
#include <cctype>
#include "AaException"

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

  string FileNotFound::Message (const string & filename)
  {
    ostringstream o;
    o << "file " << printable (filename) << " not found!";
    return o.str ();
  }

  FileNotFound::FileNotFound (const string & filename) :
    runtime_error (FileNotFound::Message (filename)) {}

  FileNotFound::~FileNotFound () throw () {}

  /* ParseError */

  string ParseError::Message (const string & expected, const string & received)
  {
    ostringstream o;
    o << "expected " << expected << ", got " << received << '!';
    return o.str ();
  }

  // ParseError::Value

  ParseError ParseError::Value (unsigned char expected)
  {
    return ParseError (printable (expected));
  }

  ParseError ParseError::Value (unsigned char expected, unsigned char received)
  {
    return ParseError (printable (expected), printable (received));
  }

  ParseError ParseError::Value (const string & expected)
  {
    return ParseError (printable (expected));
  }

  ParseError ParseError::Value (const string & expected, const string & received)
  {
    return ParseError (printable (expected), printable (received));
  }

  // ParseError::Type

  ParseError ParseError::Type (const string & expected)
  {
    return ParseError (expected);
  }

  ParseError ParseError::Type (const string & expected, unsigned char received)
  {
    return ParseError (expected, printable (received));
  }

  ParseError ParseError::Type (const string & expected, const string & received)
  {
    return ParseError (expected, printable (received));
  }

  // ParseError::ParseError

  ParseError::ParseError (const string & expected) :
    runtime_error (ParseError::Message (expected, "<EOF>")) {}

  ParseError::ParseError (const string & expected, const string & received) :
    runtime_error (ParseError::Message (expected, received)) {}

  ParseError::~ParseError () throw () {}

}

