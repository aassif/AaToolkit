#ifndef __AA_VECTOR__
#define __AA_VECTOR__

#include <cmath>
#include <sstream>

namespace Aa
{

// V<T, m> ou V<U, m> -- au mieux-disant.
#define AA_VP(T, U, m) V<AA_PROMOTE(T, U), m>

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
      inline /* explicit */ V (const V<U, m> &);
      // Accès aux données.
      inline /***/ T & operator[] (unsigned int) /***/ throw (std::out_of_range);
      inline const T & operator[] (unsigned int) const throw (std::out_of_range);
      // Addition.
      inline Self & operator+= (const Self &);
      inline Self   operator+  (const Self &) const;
      // Soustraction.
      inline Self & operator-= (const Self &);
      inline Self   operator-  (const Self &) const;
      // Négation.
      inline Self   operator-  () const;
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
      inline unsigned int min () const;
      inline unsigned int max () const;

    public:
      //inline static T Horner (const Self & i, const Self & d, const T & accu);

      inline static Self Min (const Self &, const Self &);
      inline static Self Max (const Self &, const Self &);

      //inline static Self Id (unsigned int k);
  };

  AA_TOOLKIT_API
  template <class T, class U, unsigned int m>
  inline AA_VP(T,U,m) operator* (const U & u, const V<T, m> & v)
  {
    return v * u;
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
  V<T, m>::V (const T & t) : Parent (t), m_value (t) {}

  template <class T, unsigned int m>
  V<T, m>::V (const Parent & p, const T & t) : Parent (p), m_value (t) {}

  template <class T, unsigned int m>
  template <class U>
  V<T, m>::V (const V<U, m> & v) : Parent (v), m_value (static_cast<T> (v [m-1])) {}

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
    p ()    += v;
    m_value += v.m_value;
    return *this;
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::operator+ (const V<T, m> & v) const
  {
    return V (p () + v.p (), m_value + v.m_value);
  }

  template <class T, unsigned int m>
  V<T, m> & V<T, m>::operator-= (const V<T, m> & v)
  {
    p ()    -= v;
    m_value -= v.m_value;
    return *this;
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::operator- (const V<T, m> & v) const
  {
    return V (p () - v.p (), m_value - v.m_value);
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::operator- () const
  {
    return V (-p (), -m_value);
  }

  template <class T, unsigned int m>
  template <class U>
  V<T, m> & V<T, m>::operator*= (const U & u)
  {
    p ()    *= u;
    m_value *= u;
    return *this;
  }

  template <class T, unsigned int m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator* (const U & u) const
  {
    return AA_VP(T,U,m) (p () * u, m_value * u);
  }

  template <class T, unsigned int m>
  template <class U>
  V<T, m> & V<T, m>::operator*= (const V<U, m> & v)
  {
    p ()    *= v;
    m_value *= v.m_value;
    return *this;
  }

  template <class T, unsigned int m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator* (const V<U, m> & v) const
  {
    return AA_VP(T,U,m) (p () * v.p (), m_value * v.m_value);
  }

  template <class T, unsigned int m>
  template <class U>
  V<T, m> & V<T, m>::operator/= (const U & u) throw (div_by_zero)
  {
    if (u == 0) throw div_by_zero ();

    p ()    /= u;
    m_value /= u;
    return *this;
  }

  template <class T, unsigned int m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator/ (const U & u) const throw (div_by_zero)
  {
    return AA_VP(T,U,m) (p () / u, m_value / u);
  }

  template <class T, unsigned int m>
  template <class U>
  V<T, m> & V<T, m>::operator/= (const V<U, m> & v) throw (div_by_zero)
  {
    if (v.m_value == 0) throw div_by_zero ();

    p ()    /= v;
    m_value /= v.m_value;
    return *this;
  }

  template <class T, unsigned int m>
  template <class U>
  AA_VP(T,U,m) V<T, m>::operator/ (const V<U, m> & v) const throw (div_by_zero)
  {
    return AA_VP(T,U,m) (p () / v.p (), m_value / v.m_value);
  }

  template <class T, unsigned int m>
  bool V<T, m>::operator== (const V<T, m> & v) const
  {
    return p () == v.p () && m_value == v.m_value;
  }

  template <class T, unsigned int m>
  bool V<T, m>::operator!= (const V<T, m> & v) const
  {
    return p () != v.p () || m_value != v.m_value;
  }

  template <class T, unsigned int m>
  bool V<T, m>::operator< (const V<T, m> & v) const
  {
    return p () < v.p () || (p () == v.p () && m_value < v.m_value);
  }

  template <class T, unsigned int m>
  bool V<T, m>::operator<= (const V<T, m> & v) const
  {
    return p () <= v.p () || (p () == v.p () && m_value <= v.m_value);
  }

  template <class T, unsigned int m>
  bool V<T, m>::operator> (const V<T, m> & v) const
  {
    return p () > v.p () || (p () == v.p () && m_value > v.m_value);
  }

  template <class T, unsigned int m>
  bool V<T, m>::operator>= (const V<T, m> & v) const
  {
    return p () >= v.p () || (p () == v.p () && m_value >= v.m_value);
  }

  template <class T, unsigned int m>
  T V<T, m>::sum () const
  {
    return Parent::sum () + m_value;
  }

  template <class T, unsigned int m>
  T V<T, m>::prod () const
  {
    return Parent::prod () * m_value;
  }

  template <class T, unsigned int m>
  T V<T, m>::length2 () const
  {
    return ((*this) * (*this)).sum ();
  }

  template <class T, unsigned int m>
  double V<T, m>::length () const
  {
    return std::sqrt (length2 ());
  }

  template <class T, unsigned int m>
  V<double, m> V<T, m>::normalize () const throw (div_by_zero)
  {
    return (*this) / length ();
  }

  template <class T, unsigned int m>
  unsigned int V<T, m>::min () const
  {
    unsigned int k = Parent::min ();
    return (m_value < (*this) [k]) ? m-1 : k;
  }

  template <class T, unsigned int m>
  unsigned int V<T, m>::max () const
  {
    unsigned int k = Parent::min ();
    return (m_value > (*this) [k]) ? m-1 : k;
  }

#if 0
  template <class T, unsigned int m>
  T V<T, m>::Horner (const V<T, m> & i, const V<T, m> & d, const T & accu)
  {
    return Parent::Horner (i, d, accu * d [m-1] + i [m-1]);
  }
#endif

  template <class T, unsigned int m>
  V<T, m> V<T, m>::Min (const V<T, m> & v1, const V<T, m> & v2)
  {
    return V<T, m> (Parent::Min (v1, v2), v1.m_value < v2.m_value ? v1.m_value : v2.m_value);
  }

  template <class T, unsigned int m>
  V<T, m> V<T, m>::Max (const V<T, m> & v1, const V<T, m> & v2)
  {
    return V<T, m> (Parent::Max (v1, v2), v1.m_value < v2.m_value ? v2.m_value : v1.m_value);
  }

#if 0
  template <class T, unsigned int m>
  V<T, m> V<T, m>::Id (unsigned int k)
  {
    return V<T, m> (Parent::Id (k), k == m-1 ? 1 : 0);
  }
#endif

////////////////////////////////////////////////////////////////////////////////
// Vecteur mono-composante. ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  class AA_TOOLKIT_API V<T, 1>
  {
    public:
      // Type propre.
      typedef V<T, 1> Self;

    protected:
      T m_value;

    protected:
      inline /***/ T & operator[] (unsigned int i) /***/ {return (&m_value) [i];}
      inline const T & operator[] (unsigned int i) const {return (&m_value) [i];}

    public:
      // Constructeurs.
      inline V (const T & t = T ()) : m_value (t) {}
      // Accès à la donnée.
      inline operator /***/ T & () /***/ {return m_value;}
      inline operator const T & () const {return m_value;}
      // Somme et produit.
      inline T sum  () const {return m_value;}
      inline T prod () const {return m_value;}
      // Minimum et maximum.
      inline unsigned int min () const {return 0;}
      inline unsigned int max () const {return 0;}

    public:
      //inline static T Horner (const Self & i, const Self & d, const T & accu);

      inline static Self Min (const Self &, const Self &);
      inline static Self Max (const Self &, const Self &);

      //inline static Self Id (unsigned int);
  };

#if 0
  template <class T>
  T V<T, 1>::Horner (const V<T, 1> & i, const V<T, 1> & d, const T & accu)
  {
    return accu * d + i;
  }
#endif

  template <class T>
  V<T, 1> V<T, 1>::Min (const V<T, 1> & v1, const V<T, 1> & v2)
  {
    return v1 < v2 ? v1 : v2;
  }

  template <class T>
  V<T, 1> V<T, 1>::Max (const V<T, 1> & v1, const V<T, 1> & v2)
  {
    return v1 < v2 ? v2 : v1;
  }

#if 0
  template <class T>
  V<T, 1> V<T, 1>::Id (unsigned int k)
  {
    return V<T, 1> (k == 0 ? 1 : 0);
  }
#endif

////////////////////////////////////////////////////////////////////////////////
// Raccourcis. /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_API inline
  V<T, 1> vec (const T & t1)
  {
    return V<T, 1> (t1);
  }

  template <class T>
  AA_TOOLKIT_API inline
  V<T, 2> vec (const T & t1, const T & t2)
  {
    return V<T, 2> (vec (t1), t2);
  }

  template <class T>
  AA_TOOLKIT_API inline
  V<T, 3> vec (const T & t1, const T & t2, const T & t3)
  {
    return V<T, 3> (vec (t1, t2), t3);
  }

  template <class T>
  AA_TOOLKIT_API inline
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

  typedef V<uint,   2> uvec2;
  typedef V<uint,   3> uvec3;
  typedef V<uint,   4> uvec4;

  typedef V<bool,   2> bvec2;
  typedef V<bool,   3> bvec3;
  typedef V<bool,   4> bvec4;

////////////////////////////////////////////////////////////////////////////////
// Opérations. /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, unsigned int m>
  AA_TOOLKIT_API inline
  T DotProd (const V<T, m> & v1, const V<T, m> & v2)
  {
    return (v1 * v2).sum ();
  }

  template <class T>
  AA_TOOLKIT_API inline
  V<T, 3> CrossProd (const V<T, 3> & v1, const V<T, 3> & v2)
  {
    return vec<T> (v1[1] * v2[2] - v2[1] * v1[2],
                   v1[2] * v2[0] - v2[2] * v1[0],
                   v1[0] * v2[1] - v2[0] * v1[1]);
  }

////////////////////////////////////////////////////////////////////////////////
// Affichage. //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if 0
  template <class T, unsigned int m>
  std::ostream & operator<< (std::ostream & os, const V<T, m> & v)
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
#endif

  template <class T>
  AA_TOOLKIT_API
  std::ostream & operator<< (std::ostream & os, const V<T, 1> & v)
  {
    return os << '{' << (const T &) v << '}';
  }

  template <class T>
  AA_TOOLKIT_API
  std::ostream & operator<< (std::ostream & os, const V<T, 2> & v)
  {
    return os << '{' << v[0] << ", "
                     << v[1] << '}';
  }

  template <class T>
  AA_TOOLKIT_API
  std::ostream & operator<< (std::ostream & os, const V<T, 3> & v)
  {
    return os << '{' << v[0] << ", "
                     << v[1] << ", "
                     << v[2] << '}';
  }

  template <class T>
  AA_TOOLKIT_API
  std::ostream & operator<< (std::ostream & os, const V<T, 4> & v)
  {
    return os << '{' << v[0] << ", "
                     << v[1] << ", "
                     << v[2] << ", "
                     << v[3] << '}';
  }

} // namespace Aa

#endif // __AA_VECTOR__

