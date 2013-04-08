#ifndef AA_XML__H
#define AA_XML__H

#include <map>
#include <iostream>
#include <sstream>

namespace Aa
{
  typedef std::string XmlId;
  typedef std::string XmlString;

  class AA_TOOLKIT_API XmlAttribs
  {
    public:
      typedef std::map<XmlId, XmlString> XmlMap;
      typedef XmlMap::/***/ iterator /***/ iterator;
      typedef XmlMap::const_iterator const_iterator;

    private:
      XmlMap m_map;

    public:
      inline XmlAttribs ();

      inline /***/ iterator begin () /***/ {return m_map.begin ();}
      inline const_iterator begin () const {return m_map.begin ();}

      inline /***/ iterator end () /***/ {return m_map.end ();}
      inline const_iterator end () const {return m_map.end ();}

      inline bool insert (const XmlId &, const XmlString &);
      inline const XmlString & operator[] (const XmlId &) const;
  };

  class AA_TOOLKIT_API XmlTag
  {
    public:
      enum {END_OF_FILE = -1, OPEN = 1, CLOSE = 2, COMMENT = 4};

    public:
      XmlId id;
      XmlAttribs attribs;
      int status;
      inline XmlTag (const XmlId &, const XmlAttribs &, int = XmlTag::OPEN);
  };

  class AA_TOOLKIT_API XmlParser
  {
    protected:
      XmlId m_id;
      std::map<XmlId, XmlParser *> m_parsers;

    protected:
      typedef std::map<XmlId, XmlParser *>::      iterator       iterator;
      typedef std::map<XmlId, XmlParser *>::const_iterator const_iterator;

      inline       iterator begin ()       {return m_parsers.begin ();}
      inline const_iterator begin () const {return m_parsers.begin ();}

      inline       iterator end ()       {return m_parsers.end ();}
      inline const_iterator end () const {return m_parsers.end ();}

    public:
      inline XmlParser ();
      inline XmlParser (const XmlId & key, XmlParser * parent = NULL);
      inline virtual ~XmlParser ();
      inline virtual void parse (const XmlAttribs &)                 throw (ParseError);
      inline virtual void parse (                    std::istream &) throw (ParseError);
      inline virtual void parse (const XmlAttribs &, std::istream &) throw (ParseError);

    public:
      inline static void      ParseHeader (std::istream &);
      inline static XmlId     ParseId     (std::istream &);
      inline static XmlString ParseEntity (std::istream &);
      inline static XmlString ParseString (std::istream &, char quote);
      inline static XmlString ParseText   (std::istream &);
      inline static XmlTag    ParseTag    (std::istream &);
      inline static void      Skip        (std::istream &, const XmlId &);
      inline static void      ParseRoot   (std::istream &, XmlParser *) throw (ParseError);

      inline static int           ParseInt (const XmlString &)    throw (ParseError);
      inline static unsigned char ParseHex (char c)               throw (ParseError);
      inline static unsigned char ParseHex (char high, char low)  throw (ParseError);
  };

  template <class Data>
  class AA_TOOLKIT_API XmlParserImpl : public XmlParser
  {
    protected:
      Data * m_data;

    public:
      inline
      XmlParserImpl (const XmlString tag,
                     Data * data,
                     XmlParser * parent = NULL) :
        XmlParser (tag, parent),
        m_data (data)
      {
      }
  };

////////////////////////////////////////////////////////////////////////////////
// Aa::XmlAttribs //////////////////////////////////////////////////////////////
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
// Aa::XmlTag //////////////////////////////////////////////////////////////////
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
// Aa::XmlParser ///////////////////////////////////////////////////////////////
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

  void XmlParser::parse (std::istream & is)
    throw (ParseError)
  {
    ParseText (is);
    XmlTag tag = ParseTag (is);
    while ((tag.status & XmlTag::OPEN) != 0)
    {
      std::map<XmlId, XmlParser *>::const_iterator f = m_parsers.find (tag.id);
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
    if (tag.status == XmlTag::END_OF_FILE) throw ParseError::Value ('/' + m_id);
    if (tag.id     != m_id)                throw ParseError::Value ('/' + m_id, tag.id);
  }

  void XmlParser::parse (const XmlAttribs & attribs, std::istream & is)
    throw (ParseError)
  {
    this->parse (attribs);
    this->parse (is);
  }

  // FIXME
  void XmlParser::ParseHeader (std::istream & is)
  {
    using namespace Aa::TextParsing;
    using std::ws;

    is >> ws;
    is >>= "<?xml";
    while (is.get () != '>') {}
  }

  inline
  bool xml_accept_id_1st (char c)
  {
    if (c == '_' || c == ':') return true;
    if (isalpha (c)) return true;
    return false;
  }

  inline
  bool xml_accept_id (char c)
  {
    if (xml_accept_id_1st (c)) return true;
    if (c == '-' || c == '.') return true;
    if (isdigit (c)) return true;
    return false;
  }

  XmlId XmlParser::ParseId (std::istream & is)
  {
    XmlId id;
    if (xml_accept_id_1st (is.peek ())) id += is.get ();
    while (xml_accept_id (is.peek ())) id += is.get ();
    //cout << "(id:" << id << ')';
    return id;
  }

  XmlString XmlParser::ParseEntity (std::istream & is)
  {
    using namespace Aa::TextParsing;

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

  XmlId XmlParser::ParseString (std::istream & is, char quote)
  {
    using namespace Aa::TextParsing;

    is >>= quote;
    XmlString text;
    //while (is.peek () != quote) str += is.get ();
    for (int c = is.peek (); c != quote; c = is.peek ())
    {
      switch (c)
      {
        case -1  : throw ParseError::Value (quote);
        case '&' : text += ParseEntity (is); break;
        default  : text += is.get (); break;
      }
    }
    is >>= quote;
    return text;
  }

  XmlString XmlParser::ParseText (std::istream & is)
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

  XmlTag XmlParser::ParseTag (std::istream & is)
  {
    using namespace Aa::TextParsing;
    using std::ws;

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

  void XmlParser::Skip (std::istream & is, const XmlId & id)
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

  void XmlParser::ParseRoot (std::istream & is, XmlParser * p)
    throw (ParseError)
  {
    // Parse XML header.
    XmlParser::ParseHeader (is);

    // Parse main tag.
    XmlTag tag = XmlParser::ParseTag (is);
    if (tag.id != p->m_id) throw ParseError::Value (tag.id, p->m_id);

    p->parse (tag.attribs, is);
  }

  int XmlParser::ParseInt (const XmlString & xml)
    throw (ParseError)
  {
    int value;
    std::istringstream iss (xml);
    if (! (iss >> value)) throw ParseError::Type ("<int>", xml);
    return value;
  }

  unsigned char XmlParser::ParseHex (char c)
    throw (ParseError)
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
        throw ParseError::Type ("<hex-digit>", c);
    }
  }

  unsigned char XmlParser::ParseHex (char high, char low)
    throw (ParseError)
  {
    return 16 * ParseHex (high) + ParseHex (low);
  }

} // namespace Aa

#endif // AA_XML__H

