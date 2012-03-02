#ifndef __AA_VECTOR__
#define __AA_VECTOR__

#include <stdexcept>
#include <sstream>

namespace Aa
{

#ifndef BOOST_UTILITY_ENABLE_IF_HPP
  // Boost: enable_if.hpp
  template <bool, class T = void> struct enable_if_c {typedef T type;};
  template <class T> struct enable_if_c<false, T> {};
  //template <class Cond, class T = void> struct enable_if : public enable_if_c<Cond::value, T> {};
#endif

////////////////////////////////////////////////////////////////////////////////
// Vecteur multi-composantes. //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, unsigned int m>
  class AA_TOOLKIT_API V : public V<T, m-1>
  {
    private:
      // Vérification d'un indice.
      inline static void CheckRange (unsigned int i) throw (std::out_of_range);

    public:
      // Désambiguïsation des opérateurs (m > 1).
      typedef typename enable_if_c< (m > 1), V<T, m> >::type NotSingle;

    protected:
      T m_value;

      inline /***/ V<T, m-1> & _p () /***/ {return *this;}
      inline const V<T, m-1> & _p () const {return *this;}

    public:
      // Constructeurs.
      V (const T & = T ());
      V (const V<T, m-1> &, const T & = T ());
      // Accès aux données.
      inline /***/ T & operator[] (unsigned int) /***/ throw (std::out_of_range);
      inline const T & operator[] (unsigned int) const throw (std::out_of_range);
      // Addition.
      inline V<T, m> & operator+= (const V<T, m> &);
      inline V<T, m>   operator+  (const V<T, m> &) const;
      // Soustraction.
      inline V<T, m> & operator-= (const V<T, m> &);
      inline V<T, m>   operator-  (const V<T, m> &) const;
      // Négation.
      inline V<T, m>   operator-  () const;
      // Multiplication entiere.
      inline V<T, m> & operator*= (int);
      inline V<T, m>   operator*  (int) const;
      // Multiplication flottante.
      inline V<double, m> & operator*= (double);
      inline V<double, m>   operator*  (double) const;
      // Division entiere.
      inline V<T, m> & operator/= (int) /***/ throw (div_by_zero);
      inline V<T, m>   operator/  (int) const throw (div_by_zero);
      // Division flottante.
      inline V<double, m> & operator/= (double) /***/ throw (div_by_zero);
      inline V<double, m>   operator/  (double) const throw (div_by_zero);
      // Combinaison linéaire.
      inline double operator* (const V<double, m> &) const;
      // Minimum et maximum.
      inline unsigned int min () const;
      inline unsigned int max () const;
  };

  AA_TOOLKIT_API
    template <class T, unsigned int m>
      inline V<T, m> operator* (int k, const typename V<T, m>::NotSingle & v)
      {
        return v * k;
      }

  AA_TOOLKIT_API
    template <class T, unsigned int m>
      inline V<double, m> operator* (double k, const typename V<T, m>::NotSingle & v)
      {
        return v * k;
      }

  template <class T, unsigned int m>
  void V<T, m>::CheckRange (unsigned int i) throw (std::out_of_range)
  {
    if (i >= m)
    {
      std::ostringstream oss;
      oss << i << " n'est pas inclus dans l'intervalle [0, " << (m-1) << ']';
      throw std::out_of_range (oss.str ());
    }
  }

  template <class T, unsigned int m>
  V<T, m>::V (const T & t) : V<T, m-1> (t), m_value (t) {}

  template <class T, unsigned int m>
  V<T, m>::V (const V<T, m-1> & v, const T & t) : V<T, m-1> (v), m_value (t) {}

  template <class T, unsigned int m>
  T & V<T, m>::operator[] (unsigned int i) throw (std::out_of_range)
  {
    CheckRange (i);
    return V<T, 1>::operator[] (i);
  }

  template <class T, unsigned int m>
  const T & V<T, m>::operator[] (unsigned int i) const throw (std::out_of_range)
  {
    CheckRange (i);
    return V<T, 1>::operator[] (i);
  }

  template <class T, unsigned int m>
  V<T, m> & V<T, m>::operator+= (const V<T, m> & v)
  {
    _p ()   += v;
    m_value += v.m_value;
    return *this;
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::operator+ (const V<T, m> & v) const
  {
    return V (*this) += v;
    //return V (_p () + v, m_value + v.m_value);
  }

  template <class T, unsigned int m>
  V<T, m> & V<T, m>::operator-= (const V<T, m> & v)
  {
    _p ()   -= v;
    m_value -= v.m_value;
    return *this;
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::operator- (const V<T, m> & v) const
  {
    return V (*this) -= v;
    //return V (_p () - v, m_value - v.m_value);
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::operator- () const
  {
    return V (-_p (), -m_value);
  }

  template <class T, unsigned int m>
  V<T, m> & V<T, m>::operator*= (int k)
  {
    _p ()   *= k;
    m_value *= k;
    return *this;
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::operator* (int k) const
  {
    return V (*this) *= k;
    //return V (_p () * k, m_value * k);
  }

  template <class T, unsigned int m>
  V<double, m> & V<T, m>::operator*= (double k)
  {
    _p ()   *= k;
    m_value *= k;
    return *this;
  }

  template <class T, unsigned int m>
  V<double, m> V<T, m>::operator* (double k) const
  {
    return V (*this) *= k;
    //return V (_p () * k, m_value * k);
  }

  template <class T, unsigned int m>
  V<T, m> & V<T, m>::operator/= (int k) throw (div_by_zero)
  {
    if (k == 0) throw div_by_zero ();

    _p ()   /= k;
    m_value /= k;
    return *this;
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::operator/ (int k) const throw (div_by_zero)
  {
    return V (*this) /= k;
    //return V (_p () / k, m_value / k);
  }

  template <class T, unsigned int m>
  V<double, m> & V<T, m>::operator/= (double k) throw (div_by_zero)
  {
    if (k == 0.0) throw div_by_zero ();

    _p ()   /= k;
    m_value /= k;
    return *this;
  }

  template <class T, unsigned int m>
  V<double, m> V<T, m>::operator/ (double k) const throw (div_by_zero)
  {
    return V (*this) /= k;
    //return V (_p () / k, m_value / k);
  }

  template <class T, unsigned int m>
  double V<T, m>::operator* (const V<double, m> & v) const
  {
    return _p () * v + m_value * v.m_value;
  }

  template <class T, unsigned int m>
  unsigned int V<T, m>::min () const
  {
    unsigned int k = V<T, m-1>::min ();
    return (m_value < (*this)[k]) ? m-1 : k;
  }

  template <class T, unsigned int m>
  unsigned int V<T, m>::max () const
  {
    unsigned int k = V<T, m-1>::min ();
    return (m_value > (*this)[k]) ? m-1 : k;
  }

////////////////////////////////////////////////////////////////////////////////
// Vecteur mono-composante. ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  class AA_TOOLKIT_API V<T, 1>
  {
    protected:
      T m_value;

    protected:
      inline /***/ T & operator[] (unsigned int i) /***/ {return (&m_value) [i];}
      inline const T & operator[] (unsigned int i) const {return (&m_value) [i];}

    public:
      // Constructeurs.
      inline V<T, 1> (const T & t = T ()) : m_value (t) {}
      // Accès à la donnée.
      inline operator /***/ T & () /***/ {return m_value;}
      inline operator const T & () const {return m_value;}
      // Minimum et maximum.
      inline unsigned int min () const {return 0;}
      inline unsigned int max () const {return 0;}
  };

////////////////////////////////////////////////////////////////////////////////
// Raccourcis. /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  // Inutile, non ?
  template <class T>
  inline V<T, 1> vec (const T & t1)
  {
    return V<T, 1> (t1);
  }

  template <class T>
  inline V<T, 2> vec (const T & t1, const T & t2)
  {
    return V<T, 2> (vec (t1), t2);
  }

  template <class T>
  inline V<T, 3> vec (const T & t1, const T & t2, const T & t3)
  {
    return V<T, 3> (vec (t1, t2), t3);
  }

  template <class T>
  inline V<T, 4> vec (const T & t1, const T & t2, const T & t3, const T & t4)
  {
    return V<T, 4> (vec (t1, t2, t3), t4);
  }

////////////////////////////////////////////////////////////////////////////////
// Typedefs (GLSL). ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  typedef V<float, 2> vec2;
  typedef V<float, 3> vec3;
  typedef V<float, 4> vec4;

  typedef V<int, 2> ivec2;
  typedef V<int, 3> ivec3;
  typedef V<int, 4> ivec4;

  typedef V<bool, 2> bvec2;
  typedef V<bool, 3> bvec3;
  typedef V<bool, 4> bvec4;

  typedef V<vec2, 2> mat2;
  typedef V<vec3, 3> mat3;
  typedef V<vec4, 4> mat4;

////////////////////////////////////////////////////////////////////////////////
// Affichage. //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, unsigned int m>
  std::ostream & operator<< (std::ostream & os, const typename V<T, m>::NotSingle & v)
  {
    os << '{';
    if (m > 0)
    {
      os << v[0];
      for (unsigned int i = 1; i < m; ++i) os << ", " << v[i];
    }
    os << '}';
    return os;
  }

} // namespace Aa

#endif // __AA_VECTOR__

