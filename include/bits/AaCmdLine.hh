#ifndef AA_CMD_LINE__H
#define AA_CMD_LINE__H

#include <sstream>
#include <list>
#include <map>
#include <queue>
#include <iostream>
#include <cstdlib> // exit
//#include <cxxabi.h>

namespace Aa
{
  /// Command-line parsing.
  namespace CmdLine
  {
    /// Exception thrown when a key is not available.
    class DuplicateKey : public std::runtime_error
    {
      private:
        static
        std::string msg (const std::string & key)
        {
          std::ostringstream o;
          o << "duplicate key " << Aa::printable (key) << '!';
          return o.str ();
        }

      public:
        /// Constructor.
        DuplicateKey (const std::string & key /** the diplicate key */) :
          runtime_error (DuplicateKey::msg (key))
        {
        }
    };

    /// Exception thrown when more values were expected for an option.
    class NotEnoughValues : public std::runtime_error
    {
      private:
        static
        std::string msg (const std::string & longKey,
                         unsigned int numValues,
                         const std::string & strType)
        {
          std::ostringstream o;
          o << "option " << Aa::printable (longKey)
            << " expects " << numValues << " x " << strType << '!';
          return o.str ();
        }

      public:
        /// Constructor.
        NotEnoughValues (const std::string & longKey /** the name of the option */,
                         unsigned int numValues      /** the number of expected values */,
                         const std::string & strType /** the type of the expected values */) :
          runtime_error (NotEnoughValues::msg (longKey, numValues, strType))
        {
        }
    };

    /// Exception thrown when a mandatory option is missing.
    class MissingMandatoryOption : public std::runtime_error
    {
      private:
        static
        std::string msg (const std::string & longKey)
        {
          std::ostringstream o;
          o << "option " << Aa::printable (longKey) << " is mandatory!";
          return o.str ();
        }

      public:
        /// Constructor.
        MissingMandatoryOption (const std::string & longKey /** the name of the missing option */) :
          runtime_error (MissingMandatoryOption::msg (longKey))
        {
        }
    };

    /// Exception thrown when an argument is missing.
    class MissingArgument : public std::runtime_error
    {
      private:
        static
        std::string msg (unsigned int min, unsigned int n)
        {
          std::ostringstream o;
          o << "missing argument! (" << n << '<' << min << ')';
          return o.str ();
        }

      public:
        MissingArgument (unsigned int min, unsigned int n) :
          runtime_error (MissingArgument::msg (min, n))
        {
        }
    };

    /// Exception thrown when an unexpected argument is found.
    class UnexpectedArgument : public std::runtime_error
    {
      private:
        static
        std::string msg (unsigned int max, unsigned int n)
        {
          std::ostringstream o;
          o << "unexpected argument! (" << n << '>' << max << ')';
          return o.str ();
        }

      public:
        UnexpectedArgument (unsigned int max, unsigned int n) :
          runtime_error (UnexpectedArgument::msg (max, n))
        {
        }
    };

    /* StringParser */

    /// Functor which translates strings into anything else.
    struct StringParser
    {
      /// Demangled TargetType typeid.
      template <class TargetType>
      inline static
      std::string TypeId (); // MUST BE SPECIALIZED
    
      /// Parses a string to construct an object.
      /** @exception ParseError if the string does not match the right format */
      template <class TargetType>
      inline
      void operator() (const std::string & str /** the input string */,
                       TargetType * target     /** the object which will be constructed from str */)
        throw (ParseError)
      {
        std::istringstream i (str); i >> (*target) >> std::ws;
        if (i.fail () || ! i.eof ()) throw ParseError::Type (TypeId<TargetType> (), str);
      }
    };

    /* StringParser specializations */

#define AA_DECLARE_TYPEID(T, id) \
    template <> inline std::string StringParser::TypeId<T> () {return id;}

    AA_DECLARE_TYPEID (char,          "<char>")
    AA_DECLARE_TYPEID (unsigned char, "<uchar>")
    AA_DECLARE_TYPEID (int,           "<int>")
    AA_DECLARE_TYPEID (unsigned int,  "<uint>")
    AA_DECLARE_TYPEID (short,         "<short>")
    AA_DECLARE_TYPEID (unsigned short,"<ushort>")
    AA_DECLARE_TYPEID (long,          "<long>")
    AA_DECLARE_TYPEID (unsigned long, "<ulong>")
    AA_DECLARE_TYPEID (bool,          "<bool>")
    AA_DECLARE_TYPEID (float,         "<float>")
    AA_DECLARE_TYPEID (double,        "<double>")
    AA_DECLARE_TYPEID (std::string,   "<string>")

    template <> inline void StringParser::operator() (const std::string & str, bool        * target) throw (ParseError);
    template <> inline void StringParser::operator() (const std::string & str, std::string * target) throw (ParseError);

    /* Parser */

    class AbstractOption;

    typedef std::list<std::string> StringList;
    typedef std::list<AbstractOption *> OptionList;
    typedef std::map<std::string, AbstractOption *> FilterMap;

    /// Command-line parser.
    /** This parser supports many kinds of options. */
    class Parser
    {
    private:
      std::string m_cmd;
      std::string m_abstract;
      StringList m_args;
      OptionList m_options;
      FilterMap m_filters;
      bool m_help;
      bool m_end;
      unsigned int m_min, m_max;

    private:
      friend class AbstractOption;
      inline void add (AbstractOption *) throw (DuplicateKey);

    public:
      /// Constructor.
      /** Constructs a new parser with a default help option ("-h" or "--help"). */

      inline
      Parser (const std::string & abstract /** what the program does */,
              unsigned int min = 0         /** minimum number of arguments */,
              unsigned int max = 1         /** maximum number of arguments */);

      /// Constructor.
      /** Constructs a new parser with a customized help option. */

      inline
      Parser (const std::string & abstract        /** what the program does */,
              char helpShortKey                   /** the short name of the help option */,
              const std::string & helpLongKey     /** the long name of the help option */,
              const std::string & helpDescription /** a description of the help option */,
              unsigned int min = 0                /** minimum number of arguments */,
              unsigned int max = 1                /** maximum number of arguments */)
        throw (DuplicateKey);

      /// Destructor.
      inline ~Parser ();

      /// Adds a flag.
      /** This option does not expect any argument.
          Its target is set to true if the option is found.
          @exception DuplicateKey if one of the names is already used */

      inline
      void add (char shortKey                   /** the short name of the option (e.g. 'h') */,
                const std::string & longKey     /** the long name of the option (e.g. "help") */,
                const std::string & description /** a description of the option */,
                bool * target                   /** the boolean which may be set to true */,
                bool mandatory = false          /** is this option mandatory? */)
        throw (DuplicateKey);

      /// Adds an option with one argument.
      /** This option expects one argument. The target parameter defines
          both the object to modify and the type of the expected value.
          @exception DuplicateKey if one of the names is already used */

      template <class TargetType>
      inline
      void add (char shortKey                   /** the short name of the option (e.g. 'h') */,
                const std::string & longKey     /** the long name of the option (e.g. "help") */,
                const std::string & description /** a description of the option */,
                TargetType * target             /** the object which may be modified */,
                bool mandatory = false          /** is this option mandatory? */)
        throw (DuplicateKey);

      /// Adds an option with many arguments.
      /** This option expects many arguments. The result parameter defines
          both the objects to modify and the type of the expected values.
          @exception DuplicateKey if one of the names is already used */

      template <class TargetType>
      inline
      void add (char shortKey                   /** the short name of the option (e.g. 'h') */,
                const std::string & longKey     /** the long name of the option (e.g. "help") */,
                const std::string & description /** a description of the option */,
                unsigned int num                /** the number of required arguments */,
                TargetType * result             /** an array of objects which may be modified */,
                bool mandatory = false          /** is this option mandatory? */)
        throw (DuplicateKey);

      /// Returns the option associated to a string.
      /** If the string is not a keyword, this method returns NULL. */

      inline
      AbstractOption * option (const std::string & s /** the string */) const;

      /// Parses the command-line.
      /** @exception NotEnoughValues if an option needs more arguments
          @exception ParseError if an argument does not match the right format
          @exception MissingMandatoryOption if a mandatory option is missing */

      inline
      void operator() (int          argc           /** the number of arguments */,
                       char      ** argv           /** the array of arguments */,
                       StringList * params         /** a list which will receive the parameters */,
                       StringList * ignored = NULL /** a list which will receive the ignored strings */)
        throw (NotEnoughValues, ParseError, MissingMandatoryOption, MissingArgument, UnexpectedArgument);

      /// Parses the command-line.
      /** @exception NotEnoughValues if an option needs more arguments
          @exception ParseError if an argument does not match the right format
          @exception MissingMandatoryOption if a mandatory option is missing */

      inline
      StringList operator() (int     argc /** the number of arguments */,
                             char ** argv /** the array of arguments */)
        throw (NotEnoughValues, ParseError, MissingMandatoryOption, MissingArgument, UnexpectedArgument);

      /// Returns the command name.
      inline const std::string & getCmd () const {return m_cmd;}

      /// Displays the usage table.
      /** @param o the output stream */
      inline void usage (std::ostream & o) const;
    };

    /* AbstractOption */

    /// Abstract option.
    class AbstractOption
    {
    protected:
      Parser * m_parser;
      char m_shortKey;
      std::string m_longKey;
      std::string m_description;
      bool m_mandatory;
      bool m_used;

    protected:
      /// Constructor.
      inline
      AbstractOption (Parser * parser,
                      char shortKey,
                      const std::string & longKey,
                      const std::string & description,
                      bool mandatory);
      /// Returns the help string of the option.
      virtual std::string getHelp () const = 0;
      /// Returns true if the option has been encountered.
      inline bool isUsed () const {return m_used;}
      /// Parses the strings in [first, last).
      /** @exception NotEnoughValues if an option needs more arguments
          @exception ParseError if an argument does not match the right format */
      virtual StringList::iterator parse (StringList::iterator first,
                                          StringList::iterator last)
        throw (NotEnoughValues, ParseError) = 0;

    public:
      /// Destructor.
      inline virtual ~AbstractOption ();
      /// Returns the short name of the option.
      inline char getShort () const {return m_shortKey;}
      /// Returns the long name of the option.
      inline const std::string & getLong () const {return m_longKey;}
      /// Returns a description of the option.
      inline const std::string & getDesc () const {return m_description;}
      /// Returns true if the option mandatory.
      inline bool isMandatory () const {return m_mandatory;}

      friend class Parser;
    };

    /* MultipleOption */

    /// Option with multiple arguments.
    template <class TargetType>
    class MultipleOption :
      public AbstractOption
    {
    private:
      const unsigned int m_num;
      TargetType * const m_result;

    private:
      /// Returns the help string.
      inline std::string getHelp () const
      {
        std::ostringstream o;
        if (m_shortKey != 0)
        {
          o << '-' << m_shortKey;
          if (! m_longKey.empty ()) o << ", " << "--" << m_longKey;
        }
        else
        {
          o << ' ' << ' ';
          if (! m_longKey.empty ()) o << "  " << "--" << m_longKey;
        }
        std::string id = StringParser::TypeId<TargetType> ();
        for (unsigned int k = m_num; k--;) o << ' ' << id;
        return o.str ();
      }
      /// Parses the strings in [first, last).
      inline StringList::iterator parse (StringList::iterator first, StringList::iterator last)
        throw (NotEnoughValues, ParseError)
      {
        m_used = true;
        StringParser strParser;
        TargetType * target = m_result;
        for (unsigned int k = m_num; k--; ++first)
          {
            // Has the end of input been reached?
            if (first == last)
              throw NotEnoughValues (m_longKey, m_num, StringParser::TypeId<TargetType> ());
            // Get the next string.
            std::string s = (*first);
            // Is this string a keyword?
            AbstractOption * o = m_parser->option (s);
            if (o != NULL) throw NotEnoughValues (m_longKey, m_num, StringParser::TypeId<TargetType> ());
            // Parse the string.
            strParser (s, target++);
          }
        return first;
      }

    public:
      /// Constructor.
      inline
      MultipleOption (Parser * parser                 /** the parser */,
                      unsigned char shortKey          /** the short name of the option (e.g. 'h') */,
                      const std::string & longKey     /** the long name of the option (e.g. "help") */,
                      const std::string & description /** a description of the option */,
                      unsigned int num                /** the number of required arguments */,
                      TargetType * result             /** an array of objects which may be modified */,
                      bool mandatory                  /** is this option mandatory? */) :
        AbstractOption (parser,
                        shortKey, longKey, description,
                        mandatory), m_num (num), m_result (result) {}
    };

    // MultipleOption<bool> specialization.

    template <> StringList::iterator
    MultipleOption<bool>::parse (StringList::iterator first,
                                 StringList::iterator last) throw (NotEnoughValues, ParseError);

    /* Parser */

    template <class TargetType>
    void Parser::add (char shortKey,
                      const std::string & longKey,
                      const std::string & description,
                      TargetType * target,
                      bool mandatory) throw (DuplicateKey)
    {
      new MultipleOption<TargetType> (this, shortKey, longKey, description, 1, target, mandatory);
    }

    template <class TargetType>
    void Parser::add (char shortKey,
                      const std::string & longKey,
                      const std::string & description,
                      unsigned int num, TargetType * result,
                      bool mandatory) throw (DuplicateKey)
    {
      new MultipleOption<TargetType> (this,
                                      shortKey, longKey, description,
                                      num, result, mandatory);
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::CmdLine::AbstractOption /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    AbstractOption::AbstractOption (Parser * parser,
                                    char shortKey,
                                    const std::string & longKey,
                                    const std::string & description,
                                    bool mandatory) :
      m_parser (parser),
      m_shortKey (shortKey), m_longKey (longKey),
      m_description (description),
      m_mandatory (mandatory),
      m_used (false)
    {
      m_parser->add (this);
    }

    AbstractOption::~AbstractOption () {}

////////////////////////////////////////////////////////////////////////////////
// Aa::CmdLine::Parser /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Parser::Parser (const std::string & abstract,
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
      this->add ('-', "",     "end of parsing", &m_end,  false);
      this->add ('h', "help", "this message",   &m_help, false);
    }

    Parser::Parser (const std::string & abstract,
                    char                helpShort,
                    const std::string & helpLong,
                    const std::string & helpDescription,
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
      this->add ('-',       "",       "end of parsing", &m_end,  false);
      this->add (helpShort, helpLong, helpDescription,  &m_help, false);
    }

    Parser::~Parser ()
    {
      m_filters.clear ();
      for (OptionList::iterator
             i = m_options.begin (), e = m_options.end (); i != e;) delete *(i++);
      m_options.clear ();
    }

    void Parser::add (AbstractOption * o)
      throw (DuplicateKey)
    {
      char shortKey = o->getShort ();
      if (shortKey != 0)
      {
        std::string s1 = std::string ("-") + shortKey;
        FilterMap::const_iterator f1 = m_filters.find (s1);
        if (f1 != m_filters.end ()) throw DuplicateKey (s1);
        m_filters [s1] = o;
      }

      const std::string & longKey = o->getLong ();
      if (! longKey.empty ())
      {
        std::string s2 = std::string ("--") + longKey; 
        FilterMap::const_iterator f2 = m_filters.find (s2);
        if (f2 != m_filters.end ()) throw DuplicateKey (s2);
        m_filters [s2] = o;
      }

      m_options.push_back (o);
    }

    void Parser::add (char shortKey,
                      const std::string & longKey,
                      const std::string & description,
                      bool * target,
                      bool mandatory) throw (DuplicateKey)
    {
      new MultipleOption<bool> (this, shortKey, longKey, description, 0, target, mandatory);
    }

    AbstractOption * Parser::option (const std::string & s) const
    {
      FilterMap::const_iterator f = m_filters.find (s);
      return (f != m_filters.end ()) ? f->second : NULL;
    }

    void Parser::operator() (int argc, char ** argv,
                             StringList * params, StringList * ignored)
      throw (NotEnoughValues, ParseError, MissingMandatoryOption, MissingArgument, UnexpectedArgument)
    {
      std::string absCmd = *(argv++); --argc;
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
              std::string s = *(i++);
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
      catch (NotEnoughValues & e) {this->usage (std::cout); throw;}
      catch (ParseError      & e) {this->usage (std::cout); throw;}

      params->insert (params->end (), quarantine.begin (), quarantine.end ());
      quarantine.clear ();

      if (m_help) {this->usage (std::cout); std::exit (0);}

      for (OptionList::const_iterator
             i = m_options.begin (), e = m_options.end (); i != e;)
        {
          AbstractOption * o = *(i++);
          if (o->isMandatory () && ! o->isUsed ())
            {
              this->usage (std::cout);
              throw MissingMandatoryOption ("--" + o->getLong ());
            }
        }

      unsigned int n = params->size ();
      if (n < m_min) throw MissingArgument    (m_min, n);
      if (n > m_max) throw UnexpectedArgument (m_max, n);
    }

    StringList Parser::operator() (int argc, char ** argv)
      throw (NotEnoughValues, ParseError, MissingMandatoryOption, MissingArgument, UnexpectedArgument)
    {
      StringList arguments;
      this->operator() (argc, argv, &arguments, NULL);
      return arguments;
    }

    void Parser::usage (std::ostream & o) const
    {
      o << "Usage: " << m_cmd << " [options] arguments" << std::endl << std::endl;
 
      typedef std::pair<std::string, std::string> StringPair;
      std::queue<StringPair> q;
      unsigned int maxLength = 0;
      for (OptionList::const_iterator
             i = ++m_options.begin (), e = m_options.end (); i != e;) // Skip 1st option ("--").
        {
          AbstractOption * option = *(i++);
          std::string s1 = option->getHelp ();
          std::string s2 = option->getDesc ();
          unsigned int n = s1.length ();
          if (n > maxLength) maxLength = n;
          q.push (make_pair (s1, s2));
        }

      o << "Options:" << std::endl;

      while (! q.empty ())
        {
          StringPair p = q.front (); q.pop ();
          std::string s1 = p.first, s2 = p.second;
          o << "  " << s1;
          unsigned int n = maxLength - s1.length ();
          while (n--) o.put (' ');
          o << " : " << p.second << std::endl;
        }

      o << std::endl;
      o << m_abstract << std::endl;
      o << std::endl;
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::CmdLine::StringParser ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <>
    inline
    void StringParser::operator() (const std::string & str, bool * target) throw (ParseError)
    {
      std::istringstream i (str); std::string s; i >> s >> std::ws;
      if (i.fail () || ! i.eof ()) throw ParseError::Type (StringParser::TypeId<bool> (), str);
      if (s == "true"  || s == "on"  || s == "1") {(*target) = true;  return;}
      if (s == "false" || s == "off" || s == "0") {(*target) = false; return;}
      throw ParseError::Type (StringParser::TypeId<bool> (), str);
    }

    template <>
    inline
    void StringParser::operator() (const std::string & str, std::string * target) throw (ParseError)
    {
      (*target) = str;
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::CmdLine::MultipleOption<bool> ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <>
    inline
    StringList::iterator
      MultipleOption<bool>::parse (StringList::iterator first,
                                   StringList::iterator last)
        throw (NotEnoughValues, ParseError)
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
              std::string s = (*first);
              // Is this string a keyword?
              AbstractOption * o = m_parser->option (s);
              if (o != NULL) throw NotEnoughValues (m_longKey, m_num, StringParser::TypeId<bool> ());
              // Parse the string.
              strParser (s, target++);
            }
        }
      return first;
    }

  } // namespace CmdLine
} // namespace Aa

#endif // AA_CMD_LINE__H

