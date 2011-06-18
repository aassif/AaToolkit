#include <cstdlib> // exit
#include <queue>
#include "AaCmdLine"

using namespace std;

namespace Aa
{
  namespace CmdLine
  {
    /* DuplicateKey */

    string DuplicateKey::msg (const string & key)
    {
      ostringstream o;
      o << "duplicate key " << Aa::printable (key) << '!';
      return o.str ();
    }

    DuplicateKey::DuplicateKey (const string & key) :
      runtime_error (DuplicateKey::msg (key)) {}

    DuplicateKey::~DuplicateKey () throw () {}

    /* NotEnoughValues */

    string NotEnoughValues::msg (const string & longKey,
                                 unsigned int numValues, const string & strType)
    {
      ostringstream o;
      o << "option " << Aa::printable (longKey)
        << " expects " << numValues << " x " << strType << '!';
      return o.str ();
    }

    NotEnoughValues::NotEnoughValues (const string & longKey,
                                      unsigned int numValues, const string & strType) :
      runtime_error (NotEnoughValues::msg (longKey, numValues, strType)) {}

    NotEnoughValues::~NotEnoughValues () throw () {}

    /* MissingMandatoryOption */

    string MissingMandatoryOption::msg (const string & longKey)
    {
      ostringstream o;
      o << "option " << Aa::printable (longKey) << " is mandatory!";
      return o.str ();
    }

    MissingMandatoryOption::MissingMandatoryOption (const string & longKey) :
      runtime_error (MissingMandatoryOption::msg (longKey)) {}

    MissingMandatoryOption::~MissingMandatoryOption () throw () {}

    /* MissingArgument */

    string MissingArgument::msg (unsigned int min, unsigned int n)
    {
      ostringstream o;
      o << "missing argument! (" << n << '<' << min << ')';
      return o.str ();
    }

    MissingArgument::MissingArgument (unsigned int min, unsigned int n) :
      runtime_error (MissingArgument::msg (min, n)) {}

    MissingArgument::~MissingArgument () throw () {}

    /* UnexpectedArgument */

    string UnexpectedArgument::msg (unsigned int max, unsigned int n)
    {
      ostringstream o;
      o << "unexpected argument! (" << n << '>' << max << ')';
      return o.str ();
    }

    UnexpectedArgument::UnexpectedArgument (unsigned int max, unsigned int n) :
      runtime_error (UnexpectedArgument::msg (max, n)) {}

    UnexpectedArgument::~UnexpectedArgument () throw () {}

    /* AbstractOption */

    AbstractOption::AbstractOption (Parser * parser,
                                    char shortKey,
                                    const string & longKey,
                                    const string & description,
                                    bool mandatory) :
      m_parser (parser),
      m_shortKey (shortKey), m_longKey (longKey),
      m_description (description),
      m_mandatory (mandatory),
      m_used (false)
    {
      m_parser->__add (this);
    }

    AbstractOption::~AbstractOption () {}

    /* Parser */

    Parser::Parser (const string & abstract,
                    unsigned int min,
                    unsigned int max) :
      m_cmd (),
      m_abstract (abstract),
      m_args (),
      m_options (),
      m_filters (),
      m_help (false),
      m_end (false),
      m_min (min),
      m_max (max)
    {
      this->add ('-', "", "end of parsing", &m_end, false);
      this->add ('h', "help", "this message", &m_help, false);
    }

    Parser::Parser (const string & abstract,
                    char helpShort,
                    const string & helpLong,
                    const string & helpDescription,
                    unsigned int min,
                    unsigned int max)
      throw (DuplicateKey) :
      m_cmd (),
      m_abstract (abstract),
      m_args (),
      m_options (),
      m_filters (),
      m_help (false),
      m_end (false),
      m_min (min),
      m_max (max)
    {
      this->add ('-', "", "end of parsing", &m_end, false);
      this->add (helpShort, helpLong, helpDescription, &m_help, false);
    }

    Parser::~Parser ()
    {
      m_filters.clear ();
      for (OptionList::iterator
             i = m_options.begin (), e = m_options.end (); i != e;) delete *(i++);
      m_options.clear ();
    }

    void Parser::__add (AbstractOption * o)
      throw (DuplicateKey)
    {
      char shortKey = o->getShort ();
      if (shortKey != 0)
      {
        string s1 = string ("-") + shortKey;
        FilterMap::const_iterator f1 = m_filters.find (s1);
        if (f1 != m_filters.end ()) throw DuplicateKey (s1);
        m_filters [s1] = o;
      }

      const string & longKey = o->getLong ();
      if (! longKey.empty ())
      {
        string s2 = string ("--") + longKey; 
        FilterMap::const_iterator f2 = m_filters.find (s2);
        if (f2 != m_filters.end ()) throw DuplicateKey (s2);
        m_filters [s2] = o;
      }

      m_options.push_back (o);
    }

    void Parser::add (char shortKey,
                      const string & longKey,
                      const string & description,
                      bool * target,
                      bool mandatory) throw (DuplicateKey)
    {
      new MultipleOption<bool> (this, shortKey, longKey, description, 0, target, mandatory);
    }

    AbstractOption * Parser::option (const string & s) const
    {
      FilterMap::const_iterator f = m_filters.find (s);
      return (f != m_filters.end ()) ? f->second : NULL;
    }

    void Parser::operator() (int argc, char ** argv,
                             StringList * params, StringList * ignored)
      throw (NotEnoughValues, FormatError, MissingMandatoryOption, MissingArgument, UnexpectedArgument)
    {
      string absCmd = *(argv++); --argc;
      m_cmd = absCmd.substr (absCmd.rfind ('/') + 1); // FIXME
      while (argc--) m_args.push_back (*(argv++));

      // Parameters are quarantined as they are found.
      // When a new option is found, the quarantine zone is appended to "ignored".
      // When "--" is found, the remaining args are appended to "params".
      StringList quarantine;
      try
        {
          for (StringList::iterator i = m_args.begin (), e = m_args.end (); i != e;)
            {
              string s = *(i++);
              AbstractOption * o = this->option (s);
              if (o == NULL)
                quarantine.push_back (s);
              else
                {
                  i = o->parse (i, e);

                  StringList * dst = (ignored != NULL ? ignored : params);
                  dst->insert (dst->end (), quarantine.begin (), quarantine.end ());
                  quarantine.clear ();

                  if (m_end)
                    {
                      params->insert (params->end (), i, e);
                      break;
                    }
                }
            }
        }
      catch (NotEnoughValues & e) {this->usage (cout); throw;}
      catch (FormatError & e)     {this->usage (cout); throw;}

      params->insert (params->end (), quarantine.begin (), quarantine.end ());
      quarantine.clear ();

      if (m_help) {this->usage (cout); exit (0);}

      for (OptionList::const_iterator
             i = m_options.begin (), e = m_options.end (); i != e;)
        {
          AbstractOption * o = *(i++);
          if (o->isMandatory () && ! o->isUsed ())
            {
              this->usage (cout);
              throw MissingMandatoryOption ("--" + o->getLong ());
            }
        }

      unsigned int n = params->size ();
      if (n < m_min) throw MissingArgument    (m_min, n);
      if (n > m_max) throw UnexpectedArgument (m_max, n);
    }

    StringList Parser::operator() (int argc, char ** argv)
      throw (NotEnoughValues, FormatError, MissingMandatoryOption, MissingArgument, UnexpectedArgument)
    {
      StringList arguments;
      this->operator() (argc, argv, &arguments, NULL);
      return arguments;
    }

    void Parser::usage (ostream & o) const
    {
      o << "Usage: " << m_cmd << " [options] arguments" << endl << endl;
 
      typedef pair<string, string> StringPair;
      queue<StringPair> q;
      unsigned int maxLength = 0;
      for (OptionList::const_iterator
             i = ++m_options.begin (), e = m_options.end (); i != e;) // Skip 1st option ("--").
        {
          AbstractOption * option = *(i++);
          string s1 = option->getHelp ();
          string s2 = option->getDesc ();
          unsigned int n = s1.length ();
          if (n > maxLength) maxLength = n;
          q.push (make_pair (s1, s2));
        }

      o << "Options:" << endl;

      while (! q.empty ())
        {
          StringPair p = q.front (); q.pop ();
          string s1 = p.first, s2 = p.second;
          o << "  " << s1;
          unsigned int n = maxLength - s1.length ();
          while (n--) o.put (' ');
          o << " : " << p.second << endl;
        }

      o << endl;
      o << m_abstract << endl;
      o << endl;
    }

    /* StringParser specializations */

    template <>
    void StringParser::operator() (const string & str, bool * target) throw (FormatError)
    {
      istringstream i (str); string s; i >> s >> ws;
      if (i.fail () || ! i.eof ()) throw FormatError (str, StringParser::TypeId<bool> ());
      if (s == "true"  || s == "on"  || s == "1") {(*target) = true;  return;}
      if (s == "false" || s == "off" || s == "0") {(*target) = false; return;}
      throw FormatError (str, StringParser::TypeId<bool> ());
    }

    template <>
    void StringParser::operator() (const string & str, string * target) throw (FormatError)
    {
      (*target) = str;
    }

    /* MultipleOption<bool> specialization */

    template <>
    StringList::iterator MultipleOption<bool>::parse (StringList::iterator first, StringList::iterator last)
      throw (NotEnoughValues, FormatError)
    {
      m_used = true;
      if (m_num == 0)
        (*m_result) = true;
      else
        {
          StringParser strParser;
          bool * target = m_result;
          for (unsigned int k = m_num; k--; ++first)
            {
              // Has the end of input been reached?
              if (first == last)
                throw NotEnoughValues (m_longKey, m_num, StringParser::TypeId<bool> ());
              // Get the next string.
              string s = (*first);
              // Is this string a keyword?
              AbstractOption * o = m_parser->option (s);
              if (o != NULL) throw NotEnoughValues (m_longKey, m_num, StringParser::TypeId<bool> ());
              // Parse the string.
              strParser (s, target++);
            }
        }
      return first;
    }
  }
}
