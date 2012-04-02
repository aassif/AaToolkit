#ifndef __AA_CMD_LINE__
#define __AA_CMD_LINE__

#include <sstream>
#include <list>
#include <map>
#include <iostream>
//#include <cxxabi.h>
#include "bits/AaException.h"

namespace Aa
{
  /// Command-line parsing.
  namespace CmdLine
  {
    /// Exception thrown when a key is not available.
    class AA_TOOLKIT_API DuplicateKey : public std::runtime_error
    {
    private:
      static std::string msg (const std::string & key);
    public:
      /// Constructor.
      DuplicateKey (const std::string & key /** the diplicate key */);
      /// Destructor.
      ~DuplicateKey () throw ();
    };

    /// Exception thrown when more values were expected for an option.
    class AA_TOOLKIT_API NotEnoughValues : public std::runtime_error
    {
    private:
      static std::string msg (const std::string & longKey,
                              unsigned int numValues,
                              const std::string & strType);
    public:
      /// Constructor.
      NotEnoughValues (const std::string & longKey /** the name of the option */,
                       unsigned int numValues      /** the number of expected values */,
                       const std::string & strType /** the type of the expected values */);
      /// Destructor.
      ~NotEnoughValues () throw ();
    };

    /// Exception thrown when a mandatory option is missing.
    class AA_TOOLKIT_API MissingMandatoryOption : public std::runtime_error
    {
    private:
      static std::string msg (const std::string & longKey);
    public:
      /// Constructor.
      MissingMandatoryOption (const std::string & longKey /** the name of the missing option */);
      /// Destructor.
      ~MissingMandatoryOption () throw ();
    };

    /// Exception thrown when an argument is missing.
    class AA_TOOLKIT_API MissingArgument : public std::runtime_error
    {
      private:
        static std::string msg (unsigned int min, unsigned int n);
      public:
        MissingArgument (unsigned int min, unsigned int n);
        ~MissingArgument () throw ();
    };

    /// Exception thrown when an unexpected argument is found.
    class AA_TOOLKIT_API UnexpectedArgument : public std::runtime_error
    {
      private:
        static std::string msg (unsigned int max, unsigned int n);
      public:
        UnexpectedArgument (unsigned int max, unsigned int n);
        ~UnexpectedArgument () throw ();
    };

    /* StringParser */

    /// Functor which translates strings into anything else.
    struct StringParser
    {
      /// Demangled TargetType typeid.
      template <class TargetType>
      inline static std::string TypeId () {return "?";}
    
      /// Parses a string to construct an object.
      /** @exception ParseError if the string does not match the right format */
      template <class TargetType>
      inline void operator() (const std::string & str /** the input string */,
                              TargetType * target     /** the object which will be constructed from str */)
        throw (ParseError)
      {
        std::istringstream i (str); i >> (*target) >> std::ws;
        if (i.fail () || ! i.eof ()) throw ParseError::Type (TypeId<TargetType> (), str);
      }
    };

    /* StringParser specializations */

    template <> inline std::string StringParser::TypeId<char>           () {return "<char>";}
    template <> inline std::string StringParser::TypeId<unsigned char>  () {return "<uchar>";}
    template <> inline std::string StringParser::TypeId<int>            () {return "<int>";}
    template <> inline std::string StringParser::TypeId<unsigned int>   () {return "<uint>";}
    template <> inline std::string StringParser::TypeId<short>          () {return "<short>";}
    template <> inline std::string StringParser::TypeId<unsigned short> () {return "<ushort>";}
    template <> inline std::string StringParser::TypeId<long>           () {return "<long>";}
    template <> inline std::string StringParser::TypeId<unsigned long>  () {return "<ulong>";}
    template <> inline std::string StringParser::TypeId<bool>           () {return "<bool>";}
    template <> inline std::string StringParser::TypeId<float>          () {return "<float>";}
    template <> inline std::string StringParser::TypeId<double>         () {return "<double>";}
    template <> inline std::string StringParser::TypeId<std::string>    () {return "<string>";}

    template <> void StringParser::operator() (const std::string & str, bool        * target) throw (ParseError);
    template <> void StringParser::operator() (const std::string & str, std::string * target) throw (ParseError);

    /* Parser */

    class AbstractOption;

    typedef std::list<std::string> StringList;
    typedef std::list<AbstractOption *> OptionList;
    typedef std::map<std::string, AbstractOption *> FilterMap;

    /// Command-line parser.
    /** This parser supports many kinds of options. */
    class AA_TOOLKIT_API Parser
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
      void __add (AbstractOption *) throw (DuplicateKey);

    public:
      /// Constructor.
      /** Constructs a new parser with a default help option ("-h" or "--help"). */
      Parser (const std::string & abstract /** what the program does */,
              unsigned int min = 0         /** minimum number of arguments */,
              unsigned int max = 1         /** maximum number of arguments */);
      /// Constructor.
      /** Constructs a new parser with a customized help option. */
      Parser (const std::string & abstract        /** what the program does */,
              char helpShortKey                   /** the short name of the help option */,
              const std::string & helpLongKey     /** the long name of the help option */,
              const std::string & helpDescription /** a description of the help option */,
              unsigned int min = 0                /** minimum number of arguments */,
              unsigned int max = 1                /** maximum number of arguments */)
        throw (DuplicateKey);
      /// Destructor.
      ~Parser ();
      /// Adds a flag.
      /** This option does not expect any argument.
          Its target is set to true if the option is found.
          @exception DuplicateKey if one of the names is already used */
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
      void add (char shortKey                   /** the short name of the option (e.g. 'h') */,
                const std::string & longKey     /** the long name of the option (e.g. "help") */,
                const std::string & description /** a description of the option */,
                unsigned int num                /** the number of required arguments */,
                TargetType * result             /** an array of objects which may be modified */,
                bool mandatory = false          /** is this option mandatory? */)
        throw (DuplicateKey);
      /// Returns the option associated to a string.
      /** If the string is not a keyword, this method returns NULL. */
      AbstractOption * option (const std::string & s /** the string */) const;
      /// Parses the command-line.
      /** @exception NotEnoughValues if an option needs more arguments
          @exception ParseError if an argument does not match the right format
          @exception MissingMandatoryOption if a mandatory option is missing */
      void operator() (int argc                    /** the number of arguments */,
                       char ** argv                /** the array of arguments */,
                       StringList * params         /** a list which will receive the parameters */,
                       StringList * ignored = NULL /** a list which will receive the ignored strings */)
        throw (NotEnoughValues, ParseError, MissingMandatoryOption, MissingArgument, UnexpectedArgument);
      /// Parses the command-line.
      /** @exception NotEnoughValues if an option needs more arguments
          @exception ParseError if an argument does not match the right format
          @exception MissingMandatoryOption if a mandatory option is missing */
      StringList operator() (int argc                    /** the number of arguments */,
                             char ** argv                /** the array of arguments */)
        throw (NotEnoughValues, ParseError, MissingMandatoryOption, MissingArgument, UnexpectedArgument);
      /// Returns the command name.
      inline const std::string & getCmd () const {return m_cmd;}
      /// Displays the usage table.
      /** @param o the output stream */
      void usage (std::ostream & o) const;
    };

    /* AbstractOption */

    /// Abstract option.
    class AA_TOOLKIT_API AbstractOption
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
      AbstractOption (Parser * parser,
                      char shortKey,
                      const std::string & longKey,
                      const std::string & description,
                      bool mandatory);
      friend class Parser;
      /// Returns the help string of the option.
      virtual std::string getHelp () const = 0;
      /// Returns true if the option has been encountered.
      inline bool isUsed () const {return m_used;}
      /// Parses the strings in [first, last).
      /** @exception NotEnoughValues if an option needs more arguments
          @exception ParseError if an argument does not match the right format */
      virtual StringList::iterator parse (StringList::iterator first, StringList::iterator last)
        throw (NotEnoughValues, ParseError) = 0;
    public:
      /// Destructor.
      virtual ~AbstractOption ();
      /// Returns the short name of the option.
      inline char getShort () const {return m_shortKey;}
      /// Returns the long name of the option.
      inline const std::string & getLong () const {return m_longKey;}
      /// Returns a description of the option.
      inline const std::string & getDesc () const {return m_description;}
      /// Returns true if the option mandatory.
      inline bool isMandatory () const {return m_mandatory;}
    };

    /* MultipleOption */

    /// Option with multiple arguments.
    template <class TargetType>
    class AA_TOOLKIT_API MultipleOption :
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
      inline MultipleOption (Parser * parser                 /** the parser */,
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
    inline void Parser::add (char shortKey,
                             const std::string & longKey,
                             const std::string & description,
                             TargetType * target,
                             bool mandatory) throw (DuplicateKey)
    {
      new MultipleOption<TargetType> (this, shortKey, longKey, description, 1, target, mandatory);
    }

    template <class TargetType>
    inline void Parser::add (char shortKey,
                             const std::string & longKey,
                             const std::string & description,
                             unsigned int num, TargetType * result,
                             bool mandatory) throw (DuplicateKey)
    {
      new MultipleOption<TargetType> (this,
                                      shortKey, longKey, description,
                                      num, result, mandatory);
    }
  }
}

#endif // __AA_CMD_LINE__
