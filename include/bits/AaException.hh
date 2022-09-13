#ifndef AA_EXCEPTION__H
#define AA_EXCEPTION__H

#include <stdexcept>
#include <string>
#include <sstream>
#include <cctype>

/// Aassif's namespace.
/** In this namespace, I define many exceptions
    and I also include some useful functions. */
namespace Aa
{
  class div_by_zero {};

  /// Translate a character into a printable string.
  /** Printable characters are surrounded by single quotes.
      Others are translated into an hex string, preceded by a backslash.
      Every generated string is 3 char long. */

  inline
  std::string printable (unsigned char c /** the character */) throw ();

  /// Translate a string into a printable string.
  /** Non-empty strings are surrounded by double quotes.
      Empty strings give the "(null)" string. */

  inline
  std::string printable (const std::string & s /** the string */) throw ();

  /// Exception thrown when a file cannot be opened.
  class FileNotFound : public std::runtime_error
  {
  private:
    inline static
    std::string Message (const std::string & filename);

  public:
    /// Constructor.
    inline FileNotFound (const std::string & filename /** the file which could not be opened */);

    /// Destructor.
    inline ~FileNotFound () throw ();
  };

  /// Exception thrown when something goes wrong while parsing an input.
  class ParseError : public std::runtime_error
  {
  private:
    inline static
    std::string Message (const std::string & expected, const std::string & received);

  public:
    /// Constructor designed for unexpected end-of-file.
    inline static
    ParseError Value (unsigned char expected /** the expected value */);

    /// Constructor designed for unexpected characters.
    inline static
    ParseError Value (unsigned char expected /** the expected value */,
                      unsigned char received /** the received char */);

    /// Constructor designed for unexpected end-of-file.
    inline static
    ParseError Value (const std::string & expected /** the expected value */);

    /// Constructor designed for unexpected strings.
    inline static
    ParseError Value (const std::string & expected /** the expected value */,
                      const std::string & received /** the received string */);

    /// Constructor designed for unexpected end-of-file.
    inline static
    ParseError Type  (const std::string & expected /** the expected type */);

    /// Constructor designed for unexpected characters.
    inline static
    ParseError Type  (const std::string & expected /** the expected type */,
                      unsigned char       received /** the received char */);

    /// Constructor designed for unexpected strings.
    inline static
    ParseError Type  (const std::string & expected /** the expected type */,
                      const std::string & received /** the received string */);

  public:
    /// Constructor designed for unexpected end-of-file.
    inline
    ParseError (const std::string & expected /** the expected string */);

    /// Constructor designed for unexpected strings.
    inline
    ParseError (const std::string & expected /** the expected string */,
                const std::string & received /** the received string */);

    /// Destructor.
    inline
    ~ParseError () throw ();
  };

////////////////////////////////////////////////////////////////////////////////
// Aa::printable ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  std::string printable (unsigned char c) throw ()
  {
    std::ostringstream o;
    if (isprint (c))
      o << '\'' << c << '\'';
    else
      o << '\\' << (unsigned int) c;
    return o.str ();
  }

  std::string printable (const std::string & s) throw ()
  {
    if (s.empty ()) return "(null)";

    std::ostringstream o;
    o << '"' << s << '"';
    return o.str ();
  }

////////////////////////////////////////////////////////////////////////////////
// Aa::FileNotFound ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  std::string FileNotFound::Message (const std::string & filename)
  {
    std::ostringstream o;
    o << "file " << printable (filename) << " not found!";
    return o.str ();
  }

  FileNotFound::FileNotFound (const std::string & filename) :
    runtime_error (FileNotFound::Message (filename)) {}

  FileNotFound::~FileNotFound () throw () {}

////////////////////////////////////////////////////////////////////////////////
// Aa::ParseError //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  std::string ParseError::Message (const std::string & expected, const std::string & received)
  {
    std::ostringstream o;
    o << "expected " << expected << ", got " << received << '!';
    return o.str ();
  }

// Aa::ParseError::Value ///////////////////////////////////////////////////////

  ParseError ParseError::Value (unsigned char expected)
  {
    return ParseError (printable (expected));
  }

  ParseError ParseError::Value (unsigned char expected, unsigned char received)
  {
    return ParseError (printable (expected), printable (received));
  }

  ParseError ParseError::Value (const std::string & expected)
  {
    return ParseError (printable (expected));
  }

  ParseError ParseError::Value (const std::string & expected, const std::string & received)
  {
    return ParseError (printable (expected), printable (received));
  }

// Aa::ParseError::Type ////////////////////////////////////////////////////////

  ParseError ParseError::Type (const std::string & expected)
  {
    return ParseError (expected);
  }

  ParseError ParseError::Type (const std::string & expected, unsigned char received)
  {
    return ParseError (expected, printable (received));
  }

  ParseError ParseError::Type (const std::string & expected, const std::string & received)
  {
    return ParseError (expected, printable (received));
  }

// Aa::ParseError::ParseError //////////////////////////////////////////////////

  ParseError::ParseError (const std::string & expected) :
    runtime_error (ParseError::Message (expected, "<EOF>")) {}

  ParseError::ParseError (const std::string & expected, const std::string & received) :
    runtime_error (ParseError::Message (expected, received)) {}

  ParseError::~ParseError () throw () {}

}

#if __cplusplus >= 201703L
  #define AA_THROW(...) //noexcept(false)
#else
  #define AA_THROW(...) throw (__VA_ARGS__)
#endif

#endif // AA_EXCEPTION__H

