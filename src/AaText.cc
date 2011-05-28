#include "AaText"

using namespace std;

namespace Aa
{
  namespace TextParsing
  {
    /* expect */

    void expect (istream & i, unsigned char expected) throw (ParseError)
    {
      unsigned char c; i >> c;
      // Has <EOF> been reached?
      if (i.fail ()) throw ParseError (expected);
      // Was it the goog character?
      if (c != expected)
        {
          i.putback (c);
          throw ParseError (expected, c);
        }
    }

    void expect (istream & i, const string & expected) throw (ParseError)
    {
      string s; i >> s;
      // Has <EOF> been reached?
      if (i.fail ()) throw ParseError (expected);
      // Was it the good string?
      if (s != expected)
        {
          for (string::reverse_iterator
                 j = s.rbegin (), e = s.rend (); j != e;) i.putback (*(j++));
          throw ParseError (expected, s);
        }
    }

    /* operators */

    void operator>>= (istream & i, unsigned char expected) throw (ParseError)
    {
      Aa::TextParsing::expect (i, expected);
    }

    void operator>>= (istream & i, const string & expected) throw (ParseError)
    {
      Aa::TextParsing::expect (i, expected);
    }
  }
}
