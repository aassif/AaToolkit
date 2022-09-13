#ifndef AA_TEXT__H
#define AA_TEXT__H

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
    inline
    void expect (std::istream & i       /** the input stream */,
                 unsigned char expected /** the expected character */) AA_THROW (ParseError);

    /// Expects a given string from a stream.
    /** @exception ParseError if the string does not match the expected one */
    inline
    void expect (std::istream & i             /** the input stream */,
                 const std::string & expected /** the expected string */) AA_THROW (ParseError);

    /// Expectation operator.
    /** \sa void expect (istream & i, unsigned char expected) throw (ParseError)
        @exception ParseError if the character does not match the expected one */
    inline
    void operator>>= (std::istream & i       /** the input stream */,
                      unsigned char expected /** the expected string */) AA_THROW (ParseError);

    /// Expectation operator.
    /** \sa void expect (istream & i, const string & expected) throw (ParseError)
        @exception ParseError if the string does not match the expected one */
    inline
    void operator>>= (std::istream & i             /** the input stream */,
                      const std::string & expected /** the expected string */) AA_THROW (ParseError);

////////////////////////////////////////////////////////////////////////////////
// Aa::TextParsing::expect /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    void expect (std::istream & i, unsigned char expected) AA_THROW (ParseError)
    {
      unsigned char c;
      i >> c;

      // Has <EOF> been reached?
      if (i.fail ()) throw ParseError::Value (expected);

      // Was it the goog character?
      if (c != expected)
        {
          i.putback (c);
          throw ParseError::Value (expected, c);
        }
    }

    void expect (std::istream & i, const std::string & expected) AA_THROW (ParseError)
    {
      std::string s;
      i >> s;

      // Has <EOF> been reached?
      if (i.fail ()) throw ParseError::Value (expected);

      // Was it the good string?
      if (s != expected)
        {
          for (std::string::reverse_iterator
                 j = s.rbegin (), e = s.rend (); j != e;) i.putback (*(j++));

          throw ParseError::Value (expected, s);
        }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::TextParsing::operator>>= ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    void operator>>= (std::istream & i, unsigned char expected) AA_THROW (ParseError)
    {
      Aa::TextParsing::expect (i, expected);
    }

    void operator>>= (std::istream & i, const std::string & expected) AA_THROW (ParseError)
    {
      Aa::TextParsing::expect (i, expected);
    }

  } // namespace TextParsing
} // namespace Aa

#endif // AA_TEXT__H

