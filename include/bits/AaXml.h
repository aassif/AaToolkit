#ifndef __AA_XML__
#define __AA_XML__

#include <map>
#include <iostream>

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
      XmlAttribs ();

      inline /***/ iterator begin () /***/ {return m_map.begin ();}
      inline const_iterator begin () const {return m_map.begin ();}

      inline /***/ iterator end () /***/ {return m_map.end ();}
      inline const_iterator end () const {return m_map.end ();}

      bool insert (const XmlId &, const XmlString &);
      const XmlString & operator[] (const XmlId &) const;
  };

  class AA_TOOLKIT_API XmlTag
  {
    public:
      enum {END_OF_FILE = -1, OPEN = 1, CLOSE = 2, COMMENT = 4};

    public:
      XmlId id;
      XmlAttribs attribs;
      int status;
      XmlTag (const XmlId &, const XmlAttribs &, int = XmlTag::OPEN);
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
      XmlParser ();
      XmlParser (const XmlId & key, XmlParser * parent = NULL);
      virtual ~XmlParser ();
      virtual void parse (const XmlAttribs &)                 throw (ParseError);
      virtual void parse (                    std::istream &) throw (ParseError);
      virtual void parse (const XmlAttribs &, std::istream &) throw (ParseError);

    public:
      static void      ParseHeader (std::istream &);
      static XmlId     ParseId     (std::istream &);
      static XmlString ParseEntity (std::istream &);
      static XmlString ParseString (std::istream &, char quote);
      static XmlString ParseText   (std::istream &);
      static XmlTag    ParseTag    (std::istream &);
      static void      Skip        (std::istream &, const XmlId &);
      static void      ParseRoot   (std::istream &, XmlParser *) throw (ParseError);

      static int           ParseInt (const XmlString &)    throw (ParseError);
      static unsigned char ParseHex (char c)               throw (ParseError);
      static unsigned char ParseHex (char high, char low)  throw (ParseError);
  };

  template <class Data>
  class AA_TOOLKIT_API XmlParserImpl : public XmlParser
  {
    protected:
      Data * m_data;

    public:
      XmlParserImpl (const XmlString tag,
                     Data * data,
                     XmlParser * parent = NULL) :
        XmlParser (tag, parent),
        m_data (data)
      {
      }
  };
}

#endif // __AA_XML__

