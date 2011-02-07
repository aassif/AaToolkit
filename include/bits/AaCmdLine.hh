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
    class DuplicateKey : public std::runtime_error
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
    class NotEnoughValues : public std::runtime_error
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
    class MissingMandatoryOption : public std::runtime_error
    {
    private:
      static std::string msg (const std::string & longKey);
    public:
      /// Constructor.
      MissingMandatoryOption (const std::string & longKey /** the name of the missing option */);
      /// Destructor.
      ~MissingMandatoryOption () throw ();
    };

    /// Demangled TargetType typeid.
    template <class TargetType>
    inline static std::string strTypeId ()
    {
#if 0 // cxxabi.h
      int status;
      char * demangled = abi::__cxa_demangle (typeid (TargetType).name (), NULL, NULL, &status);
      if (status != 0) return "?";
      std::string str (demangled);
      free (demangled);
      int nsIndex = str.rfind ("::");
      return '<' + (nsIndex != -1 ? str.substr (nsIndex + 2) : str) + '>';
#else
      return "?";
#endif
    }
    
#if 1
    template <> inline std::string strTypeId<char>           () {return "<char>";}
    template <> inline std::string strTypeId<unsigned char>  () {return "<uchar>";}
    template <> inline std::string strTypeId<int>            () {return "<int>";}
    template <> inline std::string strTypeId<unsigned int>   () {return "<uint>";}
    template <> inline std::string strTypeId<short>          () {return "<short>";}
    template <> inline std::string strTypeId<unsigned short> () {return "<ushort>";}
    template <> inline std::string strTypeId<long>           () {return "<long>";}
    template <> inline std::string strTypeId<unsigned long>  () {return "<ulong>";}
    template <> inline std::string strTypeId<bool>           () {return "<bool>";}
    template <> inline std::string strTypeId<float>          () {return "<float>";}
    template <> inline std::string strTypeId<double>         () {return "<double>";}
    template <> inline std::string strTypeId<std::string>    () {return "<string>";}
#endif

    /* StringParser */

    /// Functor which translates strings into anything else.
    struct StringParser
    {
      /// Parses a string to construct an object.
      /** @exception FormatError if the string does not match the right format */
      template <class TargetType>
      inline void operator() (const std::string & str /** the input string */,
                              TargetType * target     /** the object which will be constructed from str */)
        throw (FormatError)
      {
        std::istringstream i (str); i >> (*target) >> std::ws;
        if (i.fail () || ! i.eof ()) throw FormatError (str, strTypeId<TargetType> ());
      }
    };

    /* StringParser specializations */

    template <> void StringParser::operator() (const std::string & str, bool        * target) throw (FormatError);
    template <> void StringParser::operator() (const std::string & str, std::string * target) throw (FormatError);

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
      std::string _cmd;
      std::string _abstract;
      StringList _args;
      OptionList _options;
      FilterMap _filters;
      bool _help;
      bool _end;
    private:
      friend class AbstractOption;
      void __add (AbstractOption *) throw (DuplicateKey);
    public:
      /// Constructor.
      /** Constructs a new parser with a default help option ("-h" or "--help"). */
      Parser (const std::string & abstract /** what the program does */);
      /// Constructor.
      /** Constructs a new parser with a customized help option. */
      Parser (const std::string & abstract        /** what the program does */,
              char helpShortKey                   /** the short name of the help option */,
              const std::string & helpLongKey     /** the long name of the help option */,
              const std::string & helpDescription /** a description of the help option */)
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
      AbstractOption * getOption (const std::string & s /** the string */) const;
      /// Parses the command-line.
      /** @exception NotEnoughValues if an option needs more arguments
          @exception FormatError if an argument does not match the right format
          @exception MissingMandatoryOption if a mandatory option is missing */
      void operator() (int argc                    /** the number of arguments */,
                       char ** argv                /** the array of arguments */,
                       StringList * params         /** a list which will receive the parameters */,
                       StringList * ignored = NULL /** a list which will receive the ignored strings */)
        throw (NotEnoughValues, FormatError, MissingMandatoryOption);
      /// Parses the command-line.
      /** @exception NotEnoughValues if an option needs more arguments
          @exception FormatError if an argument does not match the right format
          @exception MissingMandatoryOption if a mandatory option is missing */
      std::string operator() (int argc                    /** the number of arguments */,
                              char ** argv                /** the array of arguments */)
        throw (NotEnoughValues, FormatError, MissingMandatoryOption);
      /// Returns the command name.
      inline const std::string & getCmd () const {return _cmd;}
      /// Displays the usage table.
      /** @param o the output stream */
      void usage (std::ostream & o) const;
    };

    /* AbstractOption */

    /// Abstract option.
    class AbstractOption
    {
    protected:
      Parser * _parser;
      char _shortKey;
      std::string _longKey;
      std::string _description;
      bool _mandatory;
      bool _used;
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
      inline bool isUsed () const {return _used;}
      /// Parses the strings in [first, last).
      /** @exception NotEnoughValues if an option needs more arguments
          @exception FormatError if an argument does not match the right format */
      virtual StringList::iterator parse (StringList::iterator first, StringList::iterator last)
        throw (NotEnoughValues, FormatError) = 0;
    public:
      /// Destructor.
      virtual ~AbstractOption ();
      /// Returns the short name of the option.
      inline char getShort () const {return _shortKey;}
      /// Returns the long name of the option.
      inline const std::string & getLong () const {return _longKey;}
      /// Returns a description of the option.
      inline const std::string & getDesc () const {return _description;}
      /// Returns true if the option mandatory.
      inline bool isMandatory () const {return _mandatory;}
    };

    /* MultipleOption */

    /// Option with multiple arguments.
    template <class TargetType>
    class MultipleOption :
      public AbstractOption
    {
    private:
      const unsigned int _num;
      TargetType * const _result;
    private:
      /// Returns the help string.
      inline std::string getHelp () const
      {
        std::ostringstream o;
        if (_shortKey != 0)
        {
          o << '-' << _shortKey;
          if (! _longKey.empty ()) o << ", " << "--" << _longKey;
        }
        else
        {
          o << ' ' << ' ';
          if (! _longKey.empty ()) o << "  " << "--" << _longKey;
        }
        std::string strType = strTypeId<TargetType> ();
        for (unsigned int k = _num; k--;) o << ' ' << strType;
        return o.str ();
      }
      /// Parses the strings in [first, last).
      inline StringList::iterator parse (StringList::iterator first, StringList::iterator last)
        throw (NotEnoughValues, FormatError)
      {
        _used = true;
        StringParser strParser;
        TargetType * target = _result;
        for (unsigned int k = _num; k--; ++first)
          {
            // Has the end of input been reached?
            if (first == last)
              throw NotEnoughValues (_longKey, _num, strTypeId<TargetType> ());
            // Get the next string.
            std::string s = (*first);
            // Is this string a keyword?
            AbstractOption * o = _parser->getOption (s);
            if (o != NULL) throw NotEnoughValues (_longKey, _num, strTypeId<TargetType> ());
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
                        mandatory), _num (num), _result (result) {}
    };

    // MultipleOption<bool> specialization.

    template <> StringList::iterator
    MultipleOption<bool>::parse (StringList::iterator first,
                                 StringList::iterator last) throw (NotEnoughValues, FormatError);

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
