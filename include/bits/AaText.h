#ifndef __AA_TEXT__
#define __AA_TEXT__

#include <iostream>

namespace Aa
{
  /// Text parsing.
  /** This namespace proudly introduces the expectation operators.
      They can be used to parse text files very easily. */
  namespace TextParsing
  {
    /// Expects a given character from a stream.
    /** @exception ParseError if the character does not match the expected one */
    void expect (std::istream & i       /** the input stream */,
                 unsigned char expected /** the expected character */) throw (ParseError);

    /// Expects a given string from a stream.
    /** @exception ParseError if the string does not match the expected one */
    void expect (std::istream & i             /** the input stream */,
                 const std::string & expected /** the expected string */) throw (ParseError);

    /// Expectation operator.
    /** \sa void expect (istream & i, unsigned char expected) throw (ParseError)
        @exception ParseError if the character does not match the expected one */
    void operator>>= (std::istream & i       /** the input stream */,
                      unsigned char expected /** the expected string */) throw (ParseError);

    /// Expectation operator.
    /** \sa void expect (istream & i, const string & expected) throw (ParseError)
        @exception ParseError if the string does not match the expected one */
    void operator>>= (std::istream & i             /** the input stream */,
                      const std::string & expected /** the expected string */) throw (ParseError);
  }
}

#endif // __AA_TEXT__
