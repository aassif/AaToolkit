#ifndef __AA_EXCEPTION__
#define __AA_EXCEPTION__

#include <stdexcept>
#include <string>

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

  AA_TOOLKIT_API
  std::string printable (unsigned char c /** the character */) throw ();

  /// Translate a string into a printable string.
  /** Non-empty strings are surrounded by double quotes.
      Empty strings give the "(null)" string. */

  AA_TOOLKIT_API
  std::string printable (const std::string & s /** the string */) throw ();

  /// Exception thrown when a file cannot be opened.
  class AA_TOOLKIT_API FileNotFound : public std::runtime_error
  {
  private:
    static std::string Message (const std::string & filename);

  public:
    /// Constructor.
    FileNotFound (const std::string & filename /** the file which could not be opened */);

    /// Destructor.
    ~FileNotFound () throw ();
  };

  /// Exception thrown when something goes wrong while parsing an input.
  class AA_TOOLKIT_API ParseError : public std::runtime_error
  {
  private:
    static std::string Message (const std::string & expected, const std::string & received);

  public:
    /// Constructor designed for unexpected end-of-file.
    static ParseError Value (unsigned char expected /** the expected character */);

    /// Constructor designed for unexpected characters.
    static ParseError Value (unsigned char expected /** the expected character */,
                             unsigned char received /** the received character */);

    /// Constructor designed for unexpected end-of-file.
    static ParseError Value (const std::string & expected /** the expected string */);

    /// Constructor designed for unexpected strings.
    static ParseError Value (const std::string & expected /** the expected string */,
                             const std::string & received /** the received string */);

    /// Constructor designed for unexpected end-of-file.
    static ParseError Type  (const std::string & expected /** the expected string */);

    /// Constructor designed for unexpected characters.
    static ParseError Type  (const std::string & expected /** the expected string */,
                             unsigned char       received /** the received string */);

    /// Constructor designed for unexpected strings.
    static ParseError Type  (const std::string & expected /** the expected string */,
                             const std::string & received /** the received string */);

  public:
    /// Constructor designed for unexpected end-of-file.
    ParseError (const std::string & expected /** the expected string */);

    /// Constructor designed for unexpected strings.
    ParseError (const std::string & expected /** the expected string */,
                const std::string & received /** the received string */);

    /// Destructor.
    ~ParseError () throw ();
  };

#if 0
  /// Exception thrown when a string could not be translated into another type.
  class AA_TOOLKIT_API FormatError : public std::invalid_argument
  {
  private:
    static std::string Message (const std::string & received,
                                const std::string & expectedType);

  public:
    /// Constructor.
    FormatError (const std::string & received     /** the received string */,
                 const std::string & expectedType /** a string representation of the expected type */);

    /// Destructor.
    ~FormatError () throw ();
  };
#endif

}

#endif // __AA_EXCEPTION__
