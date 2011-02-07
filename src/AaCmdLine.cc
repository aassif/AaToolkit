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

    /* AbstractOption */

    AbstractOption::AbstractOption (Parser * parser,
                                    char shortKey,
                                    const string & longKey,
                                    const string & description,
                                    bool mandatory) :
      _parser (parser),
      _shortKey (shortKey), _longKey (longKey),
      _description (description),
      _mandatory (mandatory),
      _used (false)
    {
      _parser->__add (this);
    }

    AbstractOption::~AbstractOption () {}

    /* Parser */

    Parser::Parser (const string & abstract) :
      _cmd (),
      _abstract (abstract),
      _args (),
      _options (),
      _filters (),
      _help (false),
      _end (false)
    {
      this->add ('-', "", "end of parsing", &_end, false);
      this->add ('h', "help", "this message", &_help, false);
    }

    Parser::Parser (const string & abstract,
                    char helpShort,
                    const string & helpLong,
                    const string & helpDescription)
      throw (DuplicateKey) :
      _cmd (),
      _abstract (abstract),
      _args (),
      _options (),
      _filters (),
      _help (false),
      _end (false)
    {
      this->add ('-', "", "end of parsing", &_end, false);
      this->add (helpShort, helpLong, helpDescription, &_help, false);
    }

    Parser::~Parser ()
    {
      _filters.clear ();
      for (OptionList::iterator
             i = _options.begin (), e = _options.end (); i != e;) delete *(i++);
      _options.clear ();
    }

    void Parser::__add (AbstractOption * o)
      throw (DuplicateKey)
    {
      char shortKey = o->getShort ();
      if (shortKey != 0)
      {
        string s1 = string ("-") + shortKey;
        FilterMap::const_iterator f1 = _filters.find (s1);
        if (f1 != _filters.end ()) throw DuplicateKey (s1);
        _filters [s1] = o;
      }

      const string & longKey = o->getLong ();
      if (! longKey.empty ())
      {
        string s2 = string ("--") + longKey; 
        FilterMap::const_iterator f2 = _filters.find (s2);
        if (f2 != _filters.end ()) throw DuplicateKey (s2);
        _filters [s2] = o;
      }

      _options.push_back (o);
    }

    void Parser::add (char shortKey,
                      const string & longKey,
                      const string & description,
                      bool * target,
                      bool mandatory) throw (DuplicateKey)
    {
      new MultipleOption<bool> (this, shortKey, longKey, description, 0, target, mandatory);
    }

    AbstractOption * Parser::getOption (const string & s) const
    {
      FilterMap::const_iterator f = _filters.find (s);
      return (f != _filters.end ()) ? f->second : NULL;
    }

    void Parser::operator() (int argc, char ** argv,
                             StringList * params, StringList * ignored)
      throw (NotEnoughValues, FormatError, MissingMandatoryOption)
    {
      string absCmd = *(argv++); --argc;
      _cmd = absCmd.substr (absCmd.rfind ('/') + 1);
      while (argc--) _args.push_back (*(argv++));

      list<string> quarantine;
      try
        {
          for (StringList::iterator i = _args.begin (), e = _args.end (); i != e;)
            {
              string s = *(i++);
              AbstractOption * o = this->getOption (s);
              if (o == NULL)
                quarantine.push_back (s);
              else
                {
                  i = o->parse (i, e);
                  if (_end)
                    {
                      params->insert (params->end (), quarantine.begin (), quarantine.end ());
                      quarantine.clear ();
                      params->insert (params->end (), i, e);
                      break;
                    }
                  else
                    {
                      StringList * dst = (ignored != NULL ? ignored : params);
                      dst->insert (dst->end (), quarantine.begin (), quarantine.end ());
                      quarantine.clear ();
                    }
                }
            }
        }
      catch (NotEnoughValues e) {this->usage (cout); throw;}
      catch (FormatError e)     {this->usage (cout); throw;}

      params->insert (params->end (), quarantine.begin (), quarantine.end ());
      quarantine.clear ();

      if (_help) {this->usage (cout); exit (0);}

      for (OptionList::const_iterator
             i = _options.begin (), e = _options.end (); i != e;)
        {
          AbstractOption * o = *(i++);
          if (o->isMandatory ())
            if (! o->isUsed ())
              {
                this->usage (cout);
                throw MissingMandatoryOption ("--" + o->getLong ());
              }
        }
    }

    string Parser::operator() (int argc, char ** argv)
      throw (NotEnoughValues, FormatError, MissingMandatoryOption)
    {
      StringList arguments;
      this->operator() (argc, argv, &arguments, NULL);
      return arguments.front (); // FIXME
    }

    void Parser::usage (ostream & o) const
    {
      o << "Usage: " << _cmd << " [options] arguments..." << endl << endl;
 
      typedef pair<string, string> StringPair;
      queue<StringPair> q;
      unsigned int maxLength = 0;
      for (OptionList::const_iterator
             i = ++_options.begin (), e = _options.end (); i != e;) // Skip 1st option ("--").
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
      o << _abstract << endl;
      o << endl;
    }

    /* StringParser specializations */

    template <>
    void StringParser::operator() (const string & str, bool * target) throw (FormatError)
    {
      istringstream i (str); string s; i >> s >> ws;
      if (i.fail () || ! i.eof ()) throw FormatError (str, strTypeId<bool> ());
      if (str == "true"  || str == "on"  || str == "1") {(*target) = true;  return;}
      if (str == "false" || str == "off" || str == "0") {(*target) = false; return;}
      throw FormatError (str, strTypeId<bool> ());
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
      _used = true;
      if (_num == 0)
        (*_result) = true;
      else
        {
          StringParser strParser;
          bool * target = _result;
          for (unsigned int k = _num; k--; ++first)
            {
              // Has the end of input been reached?
              if (first == last)
                throw NotEnoughValues (_longKey, _num, strTypeId<bool> ());
              // Get the next string.
              string s = (*first);
              // Is this string a keyword?
              AbstractOption * o = _parser->getOption (s);
              if (o != NULL) throw NotEnoughValues (_longKey, _num, strTypeId<bool> ());
              // Parse the string.
              strParser (s, target++);
            }
        }
      return first;
    }
  }
}
