#ifndef AA_RATIO__H
#define AA_RATIO__H

#include <iostream>
#include <cmath>

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// Aa::Ratio ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  class Ratio
  {
    private:
      int m_num, m_den;

    private:
      inline static int gcd (int a, int b);

    public:
      // Constructor.
      inline Ratio (int a = 0, int b = 1) throw (div_by_zero);
      inline int numerator () const {return m_num;}
      inline int denominator () const {return m_den;}
      inline bool isInteger () const {return m_den == 1;}
      // Assignment.
      inline void assign (int a, int b) throw (div_by_zero);
      // Negative / inverse.
      inline Ratio operator- () const;
      inline Ratio operator! () const throw (div_by_zero);
      // Modifiers.
      inline Ratio & operator+= (const Ratio &);
      inline Ratio & operator-= (const Ratio &);
      inline Ratio & operator*= (const Ratio &);
      inline Ratio & operator/= (const Ratio &) throw (div_by_zero);
      // Comparators.
      inline bool operator<  (const Ratio &) const;
      inline bool operator== (const Ratio &) const;
      // Conversion.
      inline operator double () const;

    public:
      // I/O.
      inline void write (std::ostream &, bool verbose = false) const;
      inline void read  (std::istream &) throw (ParseError);
  };

  inline Ratio operator+ (const Ratio &, const Ratio &);
  inline Ratio operator- (const Ratio &, const Ratio &);
  inline Ratio operator* (const Ratio &, const Ratio &);
  inline Ratio operator/ (const Ratio &, const Ratio &) throw (div_by_zero);

  inline bool operator< (const Ratio &, double);
  inline bool operator< (double, const Ratio &);

  inline bool operator== (const Ratio &, double);
  inline bool operator== (double, const Ratio &);

////////////////////////////////////////////////////////////////////////////////
// Aa::Ratio ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  // Greatest common divisor.
  int Ratio::gcd (int a, int b)
  {
    if (b < a)
      return gcd (b, a);

    while (b > 0)
    {
      int r = a % b;
      a = b;
      b = r;
    }

    return a;
  }

  // Assignment.
  void Ratio::assign (int a, int b)
    throw (div_by_zero)
  {
    if (b == 0)
      throw div_by_zero ();

    if (b < 0)
    {
      a *= -1;
      b *= -1;
    }

    int s = (a < 0 ? -1 : 1);
    int g = gcd (s * a, b);

    m_num = a / g;
    m_den = b / g;
  }

  // Constructor.
  Ratio::Ratio (int a, int b)
    throw (div_by_zero) :
      m_num (0), m_den (1)
  {
    this->assign (a, b);
  }

  // Negative.
  Ratio Ratio::operator- () const
  {
    return Ratio (-m_num, m_den);
  }

  // Inverse.
  Ratio Ratio::operator! () const
    throw (div_by_zero)
  {
    return Ratio (m_den, m_num);
  }

  // Addition + assignment.
  Ratio & Ratio::operator+= (const Ratio & r)
  {
    this->assign (m_num * r.m_den + m_den * r.m_num, m_den * r.m_den);
    return (*this);
  }

  // Subtraction + assignment.
  Ratio & Ratio::operator-= (const Ratio & r)
  {
    return (*this) += -r;
  }

  // Product + assignment.
  Ratio & Ratio::operator*= (const Ratio & r)
  {
    this->assign (m_num * r.m_num, m_den * r.m_den);
    return (*this);
  }

  // Division + assignment.
  Ratio & Ratio::operator/= (const Ratio & r)
    throw (div_by_zero)
  {
    return (*this) *= !r;
  }

  bool Ratio::operator< (const Ratio & r) const
  {
    return (m_num * r.m_den) < (r.m_num * m_den);
  }

  bool Ratio::operator== (const Ratio & r) const
  {
    return (m_num == r.m_num) && (m_den == r.m_den);
  }

  // Conversion to "double".
  Ratio::operator double () const {return (double) m_num / m_den;}

  // Output to a stream.
  void Ratio::write (std::ostream & o, bool approx) const
  {
    if (m_den == 1)
      o << m_num;
    else
    {
      if (approx)
      {
        double d = (double) m_num / m_den;
        if (d * m_den != m_num) o << '~';
        o << d;
      }
      else
        o << m_num << '/' << m_den;
    }
  }

  // Input from a stream.
  void Ratio::read (std::istream & i)
    throw (ParseError)
  {
    i >> std::ws;
    bool approx = true;
    char c1 = i.get ();
    if (c1 != '~')
    {
      approx = false;
      i.putback (c1);
    }

    double x;
    if (! (i >> x)) throw ParseError::Type ("<double>", "FIXME");

    if (x == (int) x)
    {
      unsigned int d = 1; 
      char c2 = i.get ();
      if (c2 != '/')
        i.putback (c2);
      else
        if (! (i >> d)) throw ParseError::Type ("<int>", "FIXME");
      (*this) = Ratio ((int) x, d);
    }
    else
    {
      int d = 1;
      for (double y = fabs ((x * d) - rint (x * d));
           (approx ? y > 0.1 : y != 0.0);
           ++d, y = fabs ((x * d) - rint (x * d))) {/***/}
      (*this) = Ratio ((int) rint (x * d), d);
    }
  }

  Ratio operator+ (const Ratio & r1, const Ratio & r2)                     {return Ratio (r1) += r2;}
  Ratio operator- (const Ratio & r1, const Ratio & r2)                     {return Ratio (r1) -= r2;}
  Ratio operator* (const Ratio & r1, const Ratio & r2)                     {return Ratio (r1) *= r2;}
  Ratio operator/ (const Ratio & r1, const Ratio & r2) throw (div_by_zero) {return Ratio (r1) /= r2;}

  bool operator< (const Ratio & r, double d) {return (double) r < d;}
  bool operator< (double d, const Ratio & r) {return d < (double) r;}

  bool operator== (const Ratio & r, double d) {return (double) r == d;}
  bool operator== (double d, const Ratio & r) {return d == (double) r;}

} // namespace Aa

inline AA_TOOLKIT_API
std::ostream & operator<< (std::ostream & o, const Aa::Ratio & r)
{
  r.write (o, false);
  return o;
}

inline AA_TOOLKIT_API
std::istream & operator>> (std::istream & i, Aa::Ratio & r)
{
  r.read (i);
  return i;
}

#if 0
int main (int argc, char ** argv)
{
  Ratio r1;
  Ratio r2 (1);
  Ratio r3 (2, 3);

  // Start values.
  cout << "r1 = "; try {cout << r1;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r2 = "; try {cout << r2;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r3 = "; try {cout << r3;} catch (exception & e) {cout << e.what ();} cout << endl;

  // Copy ctor, op-(r1, r2), op-=(r1, r2), op-(r) & op+=(r1, r2)
  cout << "r1 - r2 = "; try {cout << r1 - r2;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r1 - r3 = "; try {cout << r1 - r3;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r2 - r1 = "; try {cout << r2 - r1;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r2 - r3 = "; try {cout << r2 - r3;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r3 - r1 = "; try {cout << r3 - r1;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r3 - r2 = "; try {cout << r3 - r2;} catch (exception & e) {cout << e.what ();} cout << endl;

  // Copy ctor, op/(r1, r2), op/=(r1, r2), op!(r) & op*=(r1, r2)
  cout << "r1 / r2 = "; try {cout << r1 / r2;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r1 / r3 = "; try {cout << r1 / r3;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r2 / r1 = "; try {cout << r2 / r1;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r2 / r3 = "; try {cout << r2 / r3;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r3 / r1 = "; try {cout << r3 / r1;} catch (exception & e) {cout << e.what ();} cout << endl;
  cout << "r3 / r2 = "; try {cout << r3 / r2;} catch (exception & e) {cout << e.what ();} cout << endl;

  for (;;)
  {
    Ratio r4;
    cin >> r4;
    cout << r4 << endl;
  }

  return 0;
}
#endif

#endif // AA_RATIO__H

