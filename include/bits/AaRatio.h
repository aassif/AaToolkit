#ifndef __AA_RATIO__
#define __AA_RATIO__

#include <stdexcept>
#include "AaException.h"
#include "AaMath.h"

namespace Aa
{
  namespace Math
  {
    class Ratio
    {
      private:
        int m_num, m_den;

      private:
        static int gcd (int a, int b);

      public:
        // Constructor.
        Ratio (int a = 0, int b = 1) throw (div_by_zero);
        int numerator () const {return m_num;}
        int denominator () const {return m_den;}
        bool isInteger () const {return m_den == 1;}
        // Assignment.
        void assign (int a, int b) throw (div_by_zero);
        // Negative / inverse.
        Ratio operator- () const;
        Ratio operator! () const throw (div_by_zero);
        // Modifiers.
        Ratio & operator+= (const Ratio &);
        Ratio & operator-= (const Ratio &);
        Ratio & operator*= (const Ratio &);
        Ratio & operator/= (const Ratio &) throw (div_by_zero);
        // Comparators.
        bool operator<  (const Ratio &) const;
        bool operator== (const Ratio &) const;
        // Conversion.
        operator double () const;

      public:
        // I/O.
        void write (std::ostream &, bool verbose = false) const;
        void read  (std::istream &) throw (ParseError);
    };

    Ratio operator+ (const Ratio &, const Ratio &);
    Ratio operator- (const Ratio &, const Ratio &);
    Ratio operator* (const Ratio &, const Ratio &);
    Ratio operator/ (const Ratio &, const Ratio &) throw (div_by_zero);

    bool operator< (const Ratio &, double);
    bool operator< (double, const Ratio &);

    bool operator== (const Ratio &, double);
    bool operator== (double, const Ratio &);

  } // namespace Math
} // namespace Aa

std::ostream & operator<< (std::ostream &, const Aa::Math::Ratio &);
std::istream & operator>> (std::istream &, /***/ Aa::Math::Ratio &);

#endif // __AA_RATIO__

