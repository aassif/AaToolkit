#ifndef AA_VECTOR__H
#define AA_VECTOR__H

#include <cmath>
#include <sstream>

namespace Aa
{

// V<T, m> ou V<U, m> -- au mieux-disant.
#define AA_VP(T, U, m) V<AA_PROMOTE(T, U), m>

  namespace details
  {
    // Vérification d'un indice.
    AA_TOOLKIT_INLINE
    void CheckRange (AaUInt i, AaUInt m)
      throw (std::out_of_range)
    {
      if (i >= m)
      {
        std::ostringstream oss;
        oss << i << " n'est pas inclus dans l'intervalle [0, " << (m-1) << ']';
        throw std::out_of_range (oss.str ());
      }
    }
  }

////////////////////////////////////////////////////////////////////////////////
// Vecteur multi-composantes. //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, AaUInt m>
  class V : public V<T, m-1>
  {
    public:
      // Type propre.
      typedef V<T, m>   Self;
      typedef V<T, m-1> Parent;

    protected:
      T m_value;

      inline /***/ Parent & p () /***/ {return *this;}
      inline const Parent & p () const {return *this;}

    public:
      // Constructeurs.
      inline V (const T & = T ());
      inline V (const Parent &, const T & = T ());
      template <class U>
      inline V (const V<U, m> &);
      // Accès aux données.
      inline /***/ T & operator[] (AaUInt) /***/ throw (std::out_of_range);
      inline const T & operator[] (AaUInt) const throw (std::out_of_range);
      // Swizzling.
      template <AaUInt n>
      inline V<T, n> operator() (const V<AaUInt, n> &) const throw (std::out_of_range);
      // Addition.
      template <class U> inline Self       & operator+= (const U &);
      template <class U> inline AA_VP(T,U,m) operator+  (const U &) const;
      // Addition membre à membre.
      template <class U> inline Self       & operator+= (const V<U, m> &);
      template <class U> inline AA_VP(T,U,m) operator+  (const V<U, m> &) const;
      // Soustraction.
      template <class U> inline Self       & operator-= (const U &);
      template <class U> inline AA_VP(T,U,m) operator-  (const U &) const;
      // Soustraction membre à membre.
      template <class U> inline Self       & operator-= (const V<U, m> &);
      template <class U> inline AA_VP(T,U,m) operator-  (const V<U, m> &) const;
      // Négation.
      inline Self operator- () const;
      // Multiplication.
      template <class U> inline Self       & operator*= (const U &);
      template <class U> inline AA_VP(T,U,m) operator*  (const U &) const;
      // Multiplication membre à membre.
      template <class U> inline Self       & operator*= (const V<U, m> &);
      template <class U> inline AA_VP(T,U,m) operator*  (const V<U, m> &) const;
      // Division.
      template <class U> inline Self       & operator/= (const U &) /***/ throw (div_by_zero);
      template <class U> inline AA_VP(T,U,m) operator/  (const U &) const throw (div_by_zero);
      // Division membre à membre.
      template <class U> inline Self       & operator/= (const V<U, m> &) /***/ throw (div_by_zero);
      template <class U> inline AA_VP(T,U,m) operator/  (const V<U, m> &) const throw (div_by_zero);
      // Comparaison.
      inline bool operator== (const Self &) const;
      inline bool operator!= (const Self &) const;
      inline bool operator<  (const Self &) const;
      inline bool operator<= (const Self &) const;
      inline bool operator>  (const Self &) const;
      inline bool operator>= (const Self &) const;
      // Somme et produit.
      inline T sum  () const;
      inline T prod () const;
      // Norme.
      inline T      length2 () const;
      inline double length  () const;
      //template <class U>
      inline V<double, m> normalize () const throw (div_by_zero);
      // Minimum et maximum.
      inline AaUInt min () const;
      inline AaUInt max () const;
      // Sous-vecteur.
      inline V<T, m-1> sub (AaUInt) const;

      template <class T2, AaUInt m2> friend class V;
  };

#if 1
  template <class T, class U, AaUInt m>
  AA_TOOLKIT_INLINE
  AA_VP(T,U,m) operator+ (const U & u, const V<T, m> & v)
  {
    return v + u;
  }

  template <class T, class U, AaUInt m>
  AA_TOOLKIT_INLINE
  AA_VP(T,U,m) operator- (const U & u, const V<T, m> & v)
  {
    return -v + u;
  }

  template <class T, class U, AaUInt m>
  AA_TOOLKIT_INLINE
  AA_VP(T,U,m) operator* (const U & u, const V<T, m> & v)
  {
    return v * u;
  }
#endif

  template <class T, AaUInt m>
  V<T, m>::V (const T & t) : Parent (t), m_value (t) {}

  template <class T, AaUInt m>
  V<T, m>::V (const Parent & p, const T & t) : Parent (p), m_value (t) {}

  template <class T, AaUInt m>
  template <class U>
  V<T, m>::V (const V<U, m> & v) : Parent (v), m_value (static_cast<T> (v.m_value)) {}

  template <class T, AaUInt m>
  T & V<T, m>::operator[] (AaUInt i) throw (std::out_of_range)
  {
    details::CheckRange (i, m);
    return V<T, 1>::value (i);
  }

  template <class T, AaUInt m>
  const T & V<T, m>::operator[] (AaUInt i) const throw (std::out_of_range)
  {
    details::CheckRange (i, m);
    return V<T, 1>::value (i);
  }

  template <class T, AaUInt m>
  template <AaUInt n>
  V<T, n> V<T, m>::operator() (const V<AaUInt, n> & v) const throw (std::out_of_range)
  {
    return V<T, 1>::swizzle (v, m);
  }

#if 1
  template <class T, AaUInt m>
  template <class U>
  V<T, m> & V<T, m>::operator+= (const U & u)
  {
    p ()    += u;
    m_value += u;
    return *this;
  }

  template <class T, AaUInt m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator+ (const U & u) const
  {
    return AA_VP(T,U,m) (p () + u, m_value + u);
  }
#endif

  template <class T, AaUInt m>
  template <class U>
  V<T, m> & V<T, m>::operator+= (const V<U, m> & v)
  {
    p ()    += v.p ();
    m_value += v.m_value;
    return *this;
  }

  template <class T, AaUInt m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator+ (const V<U, m> & v) const
  {
    return AA_VP(T,U,m) (p () + v.p (), m_value + v.m_value);
  }

#if 1
  template <class T, AaUInt m>
  template <class U>
  V<T, m> & V<T, m>::operator-= (const U & u)
  {
    p ()    -= u;
    m_value -= u;
    return *this;
  }

  template <class T, AaUInt m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator- (const U & u) const
  {
    return AA_VP(T,U,m) (p () - u, m_value - u);
  }
#endif

  template <class T, AaUInt m>
  template <class U>
  V<T, m> & V<T, m>::operator-= (const V<U, m> & v)
  {
    p ()    -= v.p ();
    m_value -= v.m_value;
    return *this;
  }

  template <class T, AaUInt m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator- (const V<U, m> & v) const
  {
    return AA_VP(T,U,m) (p () - v.p (), m_value - v.m_value);
  }

  template <class T, AaUInt m>
  V<T, m> V<T, m>::operator- () const
  {
    return V (-p (), -m_value);
  }

#if 1
  template <class T, AaUInt m>
  template <class U>
  V<T, m> & V<T, m>::operator*= (const U & u)
  {
    p ()    *= u;
    m_value *= u;
    return *this;
  }

  template <class T, AaUInt m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator* (const U & u) const
  {
    return AA_VP(T,U,m) (p () * u, m_value * u);
  }
#endif

  template <class T, AaUInt m>
  template <class U>
  V<T, m> & V<T, m>::operator*= (const V<U, m> & v)
  {
    p ()    *= v;
    m_value *= v.m_value;
    return *this;
  }

  template <class T, AaUInt m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator* (const V<U, m> & v) const
  {
    return AA_VP(T,U,m) (p () * v.p (), m_value * v.m_value);
  }

#if 1
  template <class T, AaUInt m>
  template <class U>
  V<T, m> & V<T, m>::operator/= (const U & u) throw (div_by_zero)
  {
    if (u == 0) throw div_by_zero ();

    p ()    /= u;
    m_value /= u;
    return *this;
  }

  template <class T, AaUInt m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator/ (const U & u) const throw (div_by_zero)
  {
    if (u == 0) throw div_by_zero ();

    return AA_VP(T,U,m) (p () / u, m_value / u);
  }
#endif

  template <class T, AaUInt m>
  template <class U>
  V<T, m> & V<T, m>::operator/= (const V<U, m> & v) throw (div_by_zero)
  {
    if (v.m_value == 0) throw div_by_zero ();

    p ()    /= v;
    m_value /= v.m_value;
    return *this;
  }

  template <class T, AaUInt m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator/ (const V<U, m> & v) const throw (div_by_zero)
  {
    if (v.m_value == 0) throw div_by_zero ();

    return AA_VP(T,U,m) (p () / v.p (), m_value / v.m_value);
  }

  template <class T, AaUInt m>
  bool V<T, m>::operator== (const V<T, m> & v) const
  {
    return p () == v.p () && m_value == v.m_value;
  }

  template <class T, AaUInt m>
  bool V<T, m>::operator!= (const V<T, m> & v) const
  {
    return p () != v.p () || m_value != v.m_value;
  }

  template <class T, AaUInt m>
  bool V<T, m>::operator< (const V<T, m> & v) const
  {
    return p () < v.p () || (p () == v.p () && m_value < v.m_value);
  }

  template <class T, AaUInt m>
  bool V<T, m>::operator<= (const V<T, m> & v) const
  {
    return p () <= v.p () || (p () == v.p () && m_value <= v.m_value);
  }

  template <class T, AaUInt m>
  bool V<T, m>::operator> (const V<T, m> & v) const
  {
    return p () > v.p () || (p () == v.p () && m_value > v.m_value);
  }

  template <class T, AaUInt m>
  bool V<T, m>::operator>= (const V<T, m> & v) const
  {
    return p () >= v.p () || (p () == v.p () && m_value >= v.m_value);
  }

  template <class T, AaUInt m>
  T V<T, m>::sum () const
  {
    return Parent::sum () + m_value;
  }

  template <class T, AaUInt m>
  T V<T, m>::prod () const
  {
    return Parent::prod () * m_value;
  }

  template <class T, AaUInt m>
  T V<T, m>::length2 () const
  {
    return ((*this) * (*this)).sum ();
  }

  template <class T, AaUInt m>
  double V<T, m>::length () const
  {
    return std::sqrt (length2 ());
  }

  template <class T, AaUInt m>
  V<double, m> V<T, m>::normalize () const throw (div_by_zero)
  {
    return (*this) / length ();
  }

  template <class T, AaUInt m>
  AaUInt V<T, m>::min () const
  {
    AaUInt k = Parent::min ();
    return (m_value < (*this) [k]) ? m-1 : k;
  }

  template <class T, AaUInt m>
  AaUInt V<T, m>::max () const
  {
    AaUInt k = Parent::min ();
    return (m_value > (*this) [k]) ? m-1 : k;
  }

  template <class T, AaUInt m>
  V<T, m-1> V<T, m>::sub (AaUInt k) const
  {
    V<AaUInt, m-1> v;
    for (AaUInt i = 0; i < m; ++i)
      v [i] = (i < k ? i : i+1);
    return this->swizzle (v, m);
  }

////////////////////////////////////////////////////////////////////////////////
// Vecteur mono-composante. ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  class V<T, 1>
  {
    public:
      // Type propre.
      typedef V<T, 1> Self;

    protected:
      T m_value;

    protected:
      inline /***/ T & value (AaUInt i) /***/ {return (&m_value) [i];}
      inline const T & value (AaUInt i) const {return (&m_value) [i];}

      template <AaUInt n>
      inline V<T, n> swizzle (const V<AaUInt, n> &, AaUInt max) const throw (std::out_of_range);

    public:
      // Constructeurs.
      inline V (const T & t = T ()) : m_value (t) {}
      template <class U>
      inline V (const V<U, 1> &);
      // Accès aux données.
      inline /***/ T & operator[] (AaUInt) /***/ throw (std::out_of_range);
      inline const T & operator[] (AaUInt) const throw (std::out_of_range);
      // Swizzling.
      template <AaUInt n>
      inline V<T, n> operator() (const V<AaUInt, n> &) const throw (std::out_of_range);
      // Addition.
      template <class U> inline Self       & operator+= (const U &);
      template <class U> inline AA_VP(T,U,1) operator+  (const U &) const;
      // Addition membre à membre.
      template <class U> inline Self       & operator+= (const V<U, 1> &);
      template <class U> inline AA_VP(T,U,1) operator+  (const V<U, 1> &) const;
      // Soustraction.
      template <class U> inline Self       & operator-= (const U &);
      template <class U> inline AA_VP(T,U,1) operator-  (const U &) const;
      // Soustraction membre à membre.
      template <class U> inline Self       & operator-= (const V<U, 1> &);
      template <class U> inline AA_VP(T,U,1) operator-  (const V<U, 1> &) const;
      // Négation.
      inline Self operator- () const;
      // Multiplication.
      template <class U> inline Self       & operator*= (const U &);
      template <class U> inline AA_VP(T,U,1) operator*  (const U &) const;
      // Multiplication membre à membre.
      template <class U> inline Self       & operator*= (const V<U, 1> &);
      template <class U> inline AA_VP(T,U,1) operator*  (const V<U, 1> &) const;
      // Division.
      template <class U> inline Self       & operator/= (const U &) /***/ throw (div_by_zero);
      template <class U> inline AA_VP(T,U,1) operator/  (const U &) const throw (div_by_zero);
      // Division membre à membre.
      template <class U> inline Self       & operator/= (const V<U, 1> &) /***/ throw (div_by_zero);
      template <class U> inline AA_VP(T,U,1) operator/  (const V<U, 1> &) const throw (div_by_zero);
      // Comparaison.
      inline bool operator== (const Self &) const;
      inline bool operator!= (const Self &) const;
      inline bool operator<  (const Self &) const;
      inline bool operator<= (const Self &) const;
      inline bool operator>  (const Self &) const;
      inline bool operator>= (const Self &) const;
      // Somme et produit.
      inline T sum  () const {return m_value;}
      inline T prod () const {return m_value;}
      // Minimum et maximum.
      inline AaUInt min () const {return 0;}
      inline AaUInt max () const {return 0;}

      template <class T2, AaUInt m2> friend class V;
  };

  template <class T>
  template <class U>
  V<T, 1>::V (const V<U, 1> & v) :
    m_value (static_cast<T> (v [0]))
  {
  }

  template <class T>
  template <AaUInt n>
  V<T, n> V<T, 1>::swizzle (const V<AaUInt, n> & v, AaUInt max) const throw (std::out_of_range)
  {
    V<T, n> r;
    for (AaUInt i = 0; i < n; ++i)
    {
      AaUInt j = v [i];
      details::CheckRange (j, max);
      r [i] = this->value (j);
    }
    return r;
  }

  template <class T>
  T & V<T, 1>::operator[] (AaUInt i) throw (std::out_of_range)
  {
    details::CheckRange (i, 1);
    return m_value;
  }

  template <class T>
  const T & V<T, 1>::operator[] (AaUInt i) const throw (std::out_of_range)
  {
    details::CheckRange (i, 1);
    return m_value;
  }

  template <class T>
  template <AaUInt n>
  V<T, n> V<T, 1>::operator() (const V<AaUInt, n> & v) const throw (std::out_of_range)
  {
    return V<T, 1>::swizzle (v, 1);
  }

#if 1
  template <class T>
  template <class U>
  V<T, 1> & V<T, 1>::operator+= (const U & u)
  {
    m_value += u;
    return *this;
  }

  template <class T>
  template <class U>
  AA_VP(T,U,1) V<T, 1>::operator+ (const U & u) const
  {
    return AA_VP(T,U,1) (m_value + u);
  }
#endif

  template <class T>
  template <class U>
  V<T, 1> & V<T, 1>::operator+= (const V<U, 1> & v)
  {
    m_value += v.m_value;
    return *this;
  }

  template <class T>
  template <class U>
  AA_VP(T,U,1) V<T, 1>::operator+ (const V<U, 1> & v) const
  {
    return AA_VP(T,U,1) (m_value + v.m_value);
  }

#if 1
  template <class T>
  template <class U>
  V<T, 1> & V<T, 1>::operator-= (const U & u)
  {
    m_value -= u;
    return *this;
  }

  template <class T>
  template <class U>
  AA_VP(T,U,1) V<T, 1>::operator- (const U & u) const
  {
    return AA_VP(T,U,1) (m_value - u);
  }
#endif

  template <class T>
  template <class U>
  V<T, 1> & V<T, 1>::operator-= (const V<U, 1> & v)
  {
    m_value -= v.m_value;
    return *this;
  }

  template <class T>
  template <class U>
  AA_VP(T,U,1) V<T, 1>::operator- (const V<U, 1> & v) const
  {
    return AA_VP(T,U,1) (m_value - v.m_value);
  }

  template <class T>
  V<T, 1> V<T, 1>::operator- () const
  {
    return V (-m_value);
  }

#if 1
  template <class T>
  template <class U>
  V<T, 1> & V<T, 1>::operator*= (const U & u)
  {
    m_value *= u;
    return *this;
  }

  template <class T>
  template <class U>
  AA_VP(T,U,1) V<T, 1>::operator* (const U & u) const
  {
    return AA_VP(T,U,1) (m_value * u);
  }
#endif

  template <class T>
  template <class U>
  V<T, 1> & V<T, 1>::operator*= (const V<U, 1> & v)
  {
    m_value *= v.m_value;
    return *this;
  }

  template <class T>
  template <class U>
  AA_VP(T,U,1) V<T, 1>::operator* (const V<U, 1> & v) const
  {
    return AA_VP(T,U,1) (m_value * v.m_value);
  }

#if 1
  template <class T>
  template <class U>
  V<T, 1> & V<T, 1>::operator/= (const U & u) throw (div_by_zero)
  {
    if (u == 0) throw div_by_zero ();

    m_value /= u;
    return *this;
  }

  template <class T>
  template <class U>
  AA_VP(T,U,1) V<T, 1>::operator/ (const U & u) const throw (div_by_zero)
  {
    if (u == 0) throw div_by_zero ();

    return AA_VP(T,U,1) (m_value / u);
  }
#endif

  template <class T>
  template <class U>
  V<T, 1> & V<T, 1>::operator/= (const V<U, 1> & v) throw (div_by_zero)
  {
    if (v.m_value == 0) throw div_by_zero ();

    m_value /= v.m_value;
    return *this;
  }

  template <class T>
  template <class U>
  AA_VP(T,U,1) V<T, 1>::operator/ (const V<U, 1> & v) const throw (div_by_zero)
  {
    if (v.m_value == 0) throw div_by_zero ();

    return AA_VP(T,U,1) (m_value / v.m_value);
  }

  template <class T>
  bool V<T, 1>::operator== (const V<T, 1> & v) const
  {
    return m_value == v.m_value;
  }

  template <class T>
  bool V<T, 1>::operator!= (const V<T, 1> & v) const
  {
    return m_value != v.m_value;
  }

  template <class T>
  bool V<T, 1>::operator< (const V<T, 1> & v) const
  {
    return m_value < v.m_value;
  }

  template <class T>
  bool V<T, 1>::operator<= (const V<T, 1> & v) const
  {
    return m_value <= v.m_value;
  }

  template <class T>
  bool V<T, 1>::operator> (const V<T, 1> & v) const
  {
    return m_value > v.m_value;
  }

  template <class T>
  bool V<T, 1>::operator>= (const V<T, 1> & v) const
  {
    return m_value >= v.m_value;
  }

////////////////////////////////////////////////////////////////////////////////
// Raccourcis. /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  V<T, 1> vec (const T & t1)
  {
    return V<T, 1> (t1);
  }

  template <class T>
  AA_TOOLKIT_INLINE
  V<T, 2> vec (const T & t1, const T & t2)
  {
    return V<T, 2> (vec (t1), t2);
  }

  template <class T>
  AA_TOOLKIT_INLINE
  V<T, 3> vec (const T & t1, const T & t2, const T & t3)
  {
    return V<T, 3> (vec (t1, t2), t3);
  }

  template <class T>
  AA_TOOLKIT_INLINE
  V<T, 4> vec (const T & t1, const T & t2, const T & t3, const T & t4)
  {
    return V<T, 4> (vec (t1, t2, t3), t4);
  }

////////////////////////////////////////////////////////////////////////////////
// Typedefs (GLSL). ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  typedef V<float,  2>  vec2;
  typedef V<float,  3>  vec3;
  typedef V<float,  4>  vec4;

  typedef V<double, 2> dvec2;
  typedef V<double, 3> dvec3;
  typedef V<double, 4> dvec4;

  typedef V<int,    2> ivec2;
  typedef V<int,    3> ivec3;
  typedef V<int,    4> ivec4;

  typedef V<AaUInt, 2> uvec2;
  typedef V<AaUInt, 3> uvec3;
  typedef V<AaUInt, 4> uvec4;

  typedef V<bool,   2> bvec2;
  typedef V<bool,   3> bvec3;
  typedef V<bool,   4> bvec4;

////////////////////////////////////////////////////////////////////////////////
// Opérations. /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, class U, AaUInt m>
  AA_TOOLKIT_INLINE
  AA_PROMOTE(T,U) DotProd (const V<T, m> & v1, const V<U, m> & v2)
  {
    return (v1 * v2).sum ();
  }

  template <class T, class U>
  AA_TOOLKIT_INLINE
  AA_VP(T,U,3) CrossProd (const V<T, 3> & v1, const V<U, 3> & v2)
  {
    return vec (v1[1] * v2[2] - v2[1] * v1[2],
                v1[2] * v2[0] - v2[2] * v1[0],
                v1[0] * v2[1] - v2[0] * v1[1]);
  }

////////////////////////////////////////////////////////////////////////////////
// Affichage. //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if 0
  template <class T, AaUInt m>
  std::ostream & operator<< (std::ostream & os, const V<T, m> & v)
  {
    os << '{';
    if (m > 0)
    {
      os << v[0];
      for (AaUInt i = 1; i < m; ++i) os << ", " << v[i];
    }
    os << '}';
    return os;
  }
#endif

  template <class T>
  AA_TOOLKIT_INLINE
  std::ostream & operator<< (std::ostream & os, const V<T, 1> & v)
  {
    return os << '{' << (const T &) v << '}';
  }

  template <class T>
  AA_TOOLKIT_INLINE
  std::ostream & operator<< (std::ostream & os, const V<T, 2> & v)
  {
    return os << '{' << v[0] << ", "
                     << v[1] << '}';
  }

  template <class T>
  AA_TOOLKIT_INLINE
  std::ostream & operator<< (std::ostream & os, const V<T, 3> & v)
  {
    return os << '{' << v[0] << ", "
                     << v[1] << ", "
                     << v[2] << '}';
  }

  template <class T>
  AA_TOOLKIT_INLINE
  std::ostream & operator<< (std::ostream & os, const V<T, 4> & v)
  {
    return os << '{' << v[0] << ", "
                     << v[1] << ", "
                     << v[2] << ", "
                     << v[3] << '}';
  }

////////////////////////////////////////////////////////////////////////////////
// Promotion. //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if 0
  template <class T, class U, AaUInt m>
  struct Promote<V<T, m>, U> {typedef AA_VP(T,U,m) Type;};

  template <class T, class U, AaUInt m>
  struct Promote<V<T, m>, V<U, m> > {typedef AA_VP(T,U,m) Type;};
#endif

} // namespace Aa

namespace std
{
  
#define AA_VECTOR_UNARY(F, T, m) \
  inline\
  Aa::V<T, m> F (const Aa::V<T, m> & v)\
  {\
    const Aa::V<T, m-1> & w = v;\
    return Aa::V<T, m> (F (w), F (v[m-1]));\
  }

#define AA_VECTOR_UNARY_1_2_3_4(F, T) \
  inline\
  Aa::V<T, 1> F (const Aa::V<T, 1> & v)\
  {\
    return F (v[0]);\
  }\
  AA_VECTOR_UNARY (F, T, 2)\
  AA_VECTOR_UNARY (F, T, 3)\
  AA_VECTOR_UNARY (F, T, 4)\

  AA_VECTOR_UNARY_1_2_3_4 (floor, float)
  AA_VECTOR_UNARY_1_2_3_4 (floor, double)

  AA_VECTOR_UNARY_1_2_3_4 (ceil,  float)
  AA_VECTOR_UNARY_1_2_3_4 (ceil,  double)

#define AA_VECTOR_BINARY(F, T, m) \
  inline\
  Aa::V<T, m> F (const Aa::V<T, m> & v1, const Aa::V<T, m> & v2)\
  {\
    const Aa::V<T, m-1> & w1 = v1;\
    const Aa::V<T, m-1> & w2 = v2;\
    return Aa::V<T, m> (F (w1, w2), F (v1[m-1], v2[m-1]));\
  }

#define AA_VECTOR_BINARY_2_3_4(F, T) \
  inline\
  Aa::V<T, 1> F (const Aa::V<T, 1> & v1, const Aa::V<T, 1> & v2)\
  {\
    return F (v1[0], v2[0]);\
  }\
  AA_VECTOR_BINARY (F, T, 2)\
  AA_VECTOR_BINARY (F, T, 3)\
  AA_VECTOR_BINARY (F, T, 4)\

  AA_VECTOR_BINARY_2_3_4 (min, AaInt8)
  AA_VECTOR_BINARY_2_3_4 (min, AaUInt8)
  AA_VECTOR_BINARY_2_3_4 (min, AaInt)
  AA_VECTOR_BINARY_2_3_4 (min, AaUInt)
  AA_VECTOR_BINARY_2_3_4 (min, float)
  AA_VECTOR_BINARY_2_3_4 (min, double)

  AA_VECTOR_BINARY_2_3_4 (max, AaInt8)
  AA_VECTOR_BINARY_2_3_4 (max, AaUInt8)
  AA_VECTOR_BINARY_2_3_4 (max, AaInt)
  AA_VECTOR_BINARY_2_3_4 (max, AaUInt)
  AA_VECTOR_BINARY_2_3_4 (max, float)
  AA_VECTOR_BINARY_2_3_4 (max, double)

} // namespace std

#endif // AA_VECTOR__H

