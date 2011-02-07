#include <sstream>
#include "AaXml"

using namespace std;

namespace Aa
{
  namespace details
  {
    void expect (istream & i, char expected) throw (ParseError)
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
  }

  void operator>>= (istream & i, char expected) throw (ParseError)
  {
    details::expect (i, expected);
  }

  void operator>>= (istream & i, const string & expected) throw (ParseError)
  {
    details::expect (i, expected);
  }

////////////////////////////////////////////////////////////////////////////////
// XmlAttribs //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  XmlAttribs::XmlAttribs () : m_map ()
  {
  }

  bool XmlAttribs::insert (const XmlId & xmlKey, const XmlString & xmlVal)
  {
    return m_map.insert (make_pair (xmlKey, xmlVal)).second;
  }

  const XmlString & XmlAttribs::operator[] (const XmlId & xmlId) const
  {
    const_iterator f = m_map.find (xmlId);
    static const XmlString DEFAULT = XmlString ();
    return (f != m_map.end () ? f->second : DEFAULT);
  }

////////////////////////////////////////////////////////////////////////////////
// XmlTag //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  XmlTag::XmlTag (const XmlId & xmlId,
                  const XmlAttribs & xmlAttribs,
                  int xmlStatus) :
    id (xmlId),
    attribs (xmlAttribs),
    status (xmlStatus)
  {
  }

////////////////////////////////////////////////////////////////////////////////
// XmlParser ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  XmlParser::XmlParser () :
    m_id (),
    m_parsers ()
  {
  }

  XmlParser::XmlParser (const XmlId & id, XmlParser * parent) :
    m_id (id),
    m_parsers ()
  {
    if (parent != NULL)
      parent->m_parsers.insert (make_pair (m_id, this));
  }

  XmlParser::~XmlParser ()
  {
    for (const_iterator i = begin (); i != end (); ++i) delete i->second;
  }

  void XmlParser::parse (const XmlAttribs &)
    throw (ParseError)
  {
  }

  void XmlParser::parse (istream & is)
    throw (ParseError)
  {
    ParseText (is);
    XmlTag tag = ParseTag (is);
    while ((tag.status & XmlTag::OPEN) != 0)
    {
      map<XmlId, XmlParser *>::const_iterator f = m_parsers.find (tag.id);
      if (f != m_parsers.end ())
      {
        if ((tag.status & XmlTag::CLOSE) == 0)
          f->second->parse (tag.attribs, is);
        else
          f->second->parse (tag.attribs);
      }
      else
        if ((tag.status & XmlTag::CLOSE) == 0)
          Skip (is, tag.id);

      ParseText (is);
      tag = ParseTag (is);
    }
    if (tag.status == XmlTag::END_OF_FILE) throw ParseError ('/' + m_id);
    if (tag.id != m_id) throw ParseError ('/' + m_id, tag.id);
  }

  void XmlParser::parse (const XmlAttribs & attribs, istream & is)
    throw (ParseError)
  {
    this->parse (attribs);
    this->parse (is);
  }

  // FIXME
  void XmlParser::ParseHeader (istream & is)
  {
    is >> ws;
    is >>= "<?xml";
    while (is.get () != '>');
  }

  bool accept_id_1st (char c)
  {
    if (c == '_' || c == ':') return true;
    if (isalpha (c)) return true;
    return false;
  }
  bool accept_id (char c)
  {
    if (accept_id_1st (c)) return true;
    if (c == '-' || c == '.') return true;
    if (isdigit (c)) return true;
    return false;
  }

  XmlId XmlParser::ParseId (istream & is)
  {
    XmlId id;
    if (accept_id_1st (is.peek ())) id += is.get ();
    while (accept_id (is.peek ())) id += is.get ();
    //cout << "(id:" << id << ')';
    return id;
  }

  XmlString XmlParser::ParseEntity (istream & is)
  {
    is >>= '&';
    XmlId entity = ParseId (is);
    is >>= ';';

    /**/ if (entity == "amp")  return "&";
    else if (entity == "quot") return "\"";
    else if (entity == "apos") return "'";
    else if (entity == "lt")   return "<";
    else if (entity == "gt")   return ">";
    else                       return '&'+entity+';';
  }

  XmlId XmlParser::ParseString (istream & is, char quote)
  {
    is >>= quote;
    XmlString text;
    //while (is.peek () != quote) str += is.get ();
    for (int c = is.peek (); c != quote; c = is.peek ())
    {
      switch (c)
      {
        case -1  : throw ParseError (quote);
        case '&' : text += ParseEntity (is); break;
        default  : text += is.get (); break;
      }
    }
    is >>= quote;
    return text;
  }

  XmlString XmlParser::ParseText (istream & is)
  {
    XmlString text;
    for (int c = is.peek (); c != '<'; c = is.peek ())
    {
      switch (c)
      {
        case -1  : return text; // FIXME throw?
        case '&' : text += ParseEntity (is); break;
        default  : text += is.get (); break;
      }
    }
    //cout << "<[!CDATA[" << text << "]]>" << endl;
    return text;
  }

  XmlTag XmlParser::ParseTag (istream & is)
  {
    is >> ws;
    is >>= '<';
//    cout << "<!--";
    int status = XmlTag::OPEN;
    if (is.peek () == '!')
    {
      XmlString comment;
      for (char c = is.get (); c != '>'; c = is.get ()) comment += c;
//      cout << comment << endl;
      return XmlTag ("", XmlAttribs (), XmlTag::OPEN | XmlTag::CLOSE | XmlTag::COMMENT);
    }

    if (is.peek () == '/')
    {
      is.get ();
      status = XmlTag::CLOSE;
//    cout << '/';
    }
    XmlId id = ParseId (is);
//    cout << id;
    is >> ws;
    XmlAttribs attribs;
    while (is.peek () != '>' && is.peek () != '/') // FIXME
    {
      XmlId attribKey = ParseId (is); is >> ws;
//      cout << ' ' << attribKey;
      is >>= '='; is >> ws;
//      cout << '=';
      char quote = is.peek ();
      XmlString attribVal = ParseString (is, quote);
//      cout << '"' << attribVal << '"';
      attribs.insert (attribKey, attribVal);
      is >> ws;
    }
    //is >> ws;
    if (is.peek () == '/')
    {
      is.get ();
      status |= XmlTag::CLOSE;
//    cout << '/';
    }
    is >>= '>';
//    cout << "!>" << endl;
    return XmlTag (id, attribs, status);
  }

  void XmlParser::Skip (istream & is, const XmlId & id)
  {
    //cout << "Skip: " << id << endl;
    ParseText (is);
    for (XmlTag tag = ParseTag (is);
         tag.id != id || (tag.status & XmlTag::OPEN) != 0;
         tag = ParseTag (is))
    {
      if ((tag.status & XmlTag::CLOSE) == 0)
        Skip (is, tag.id);
      ParseText (is);
    }
  }

  void XmlParser::ParseRoot (istream & is, XmlParser * p)
    throw (ParseError)
  {
    // Parse XML header.
    XmlParser::ParseHeader (is);

    // Parse main tag.
    XmlTag tag = XmlParser::ParseTag (is);
    if (tag.id != p->m_id) throw ParseError (tag.id, p->m_id);

    p->parse (tag.attribs, is);
  }

  int XmlParser::ParseInt (const XmlString & xml)
    throw (FormatError)
  {
    int value;
    istringstream iss (xml);
    if (! (iss >> value)) throw FormatError (xml, "<int>");
    return value;
  }

  unsigned char XmlParser::ParseHex (char c)
    throw (FormatError)
  {
    switch (c)
    {
      case '0':           return  0;
      case '1':           return  1;
      case '2':           return  2;
      case '3':           return  3;
      case '4':           return  4;
      case '5':           return  5;
      case '6':           return  6;
      case '7':           return  7;
      case '8':           return  8;
      case '9':           return  9;
      case 'A': case 'a': return 10;
      case 'B': case 'b': return 11;
      case 'C': case 'c': return 12;
      case 'D': case 'd': return 13;
      case 'E': case 'e': return 14;
      case 'F': case 'f': return 15;

      default:
        throw FormatError (printable (c), "<hex-digit>");
    }
  }

  unsigned char XmlParser::ParseHex (char high, char low)
    throw (FormatError)
  {
    return 16 * ParseHex (high) + ParseHex (low);
  }
}
