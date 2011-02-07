#ifndef __AA_EXCEPTION__
#define __AA_EXCEPTION__

#include <stdexcept>
#include <string>

/// Aassif's namespace.
/** In this namespace, I define many exceptions
    and I also include some useful functions. */
namespace Aa
{
  /// Translate a character into a printable string.
  /** Printable characters are surrounded by single quotes.
      Others are translated into an hex string, preceded by a backslash.
      Every generated string is 3 char long. */
  std::string printable (unsigned char c /** the character */) throw ();

  /// Translate a string into a printable string.
  /** Non-empty strings are surrounded by double quotes.
      Empty strings give the "(null)" string. */
  std::string printable (const std::string & s /** the string */) throw ();

  /// Exception thrown when a file cannot be opened.
  class FileNotFound : public std::runtime_error
  {
  private:
    static std::string msg (const std::string & filename);
  public:
    /// Constructor.
    FileNotFound (const std::string & filename /** the file which could not be opened */);

    /// Destructor.
    ~FileNotFound () throw ();
  };

  /// Exception thrown when something goes wrong while parsing an input.
  class ParseError : public std::runtime_error
  {
  private:
    static std::string msg (const std::string & expected, const std::string & received);

  public:
    /// Constructor designed for unexpected end-of-file.
    ParseError (unsigned char expected /** the expected character */);

    /// Constructor designed for unexpected characters.
    ParseError (unsigned char expected /** the expected character */,
                unsigned char received /** the received character */);

    /// Constructor designed for unexpected end-of-file.
    ParseError (const std::string & expected /** the expected string */);

    /// Constructor designed for unexpected strings.
    ParseError (const std::string & expected /** the expected string */,
                const std::string & received /** the received string */);

    /// Destructor.
    ~ParseError () throw ();
  };

  /// Exception thrown when a string could not be translated into another type.
  class FormatError : public std::invalid_argument
  {
  private:
    static std::string msg (const std::string & received,
                            const std::string & expectedType);

  public:
    /// Constructor.
    FormatError (const std::string & received     /** the received string */,
                 const std::string & expectedType /** a string representation of the expected type */);

    /// Destructor.
    ~FormatError () throw ();
  };
}

#endif // __AA_EXCEPTION__
