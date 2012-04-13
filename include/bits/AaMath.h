#ifndef __AA_MATH__
#define __AA_MATH__

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace Aa
{
  namespace Math
  {

    //class pR3;
    //class vR3;

////////////////////////////////////////////////////////////////////////////////
// pR3 : 3d point //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if 0
    class AA_TOOLKIT_API pR3
    {
      public:
        double x, y, z;

      public:
        static pR3 Barycenter (const pR3 & p1,            const pR3 & p2)            throw ();
        static pR3 Barycenter (const pR3 & p1, double w1, const pR3 & p2, double w2) throw (div_by_zero);

        static pR3 Barycenter (const pR3 & p1,            const pR3 & p2,            const pR3 & p3)            throw ();
        static pR3 Barycenter (const pR3 & p1, double w1, const pR3 & p2, double w2, const pR3 & p3, double w3) throw (div_by_zero);

      public:
        pR3 ();
        pR3 (double, double, double);
        // default copy ctor.
        // default dtor.

        pR3 & operator+= (const vR3 &);
        pR3 & operator*= (double);
    };

    AA_TOOLKIT_API pR3 operator+ (const pR3 &, const vR3 &);
    AA_TOOLKIT_API pR3 operator+ (const vR3 &, const pR3 &);
    //pR3 operator+ (const pR3 &, const pR3 &); // ugly!

    AA_TOOLKIT_API vR3 operator- (const pR3 &, const pR3 &);

    AA_TOOLKIT_API pR3 operator* (const pR3 &, double);
    AA_TOOLKIT_API pR3 operator* (double, const pR3 &);

    AA_TOOLKIT_API pR3 operator/ (const pR3 &, double) throw (div_by_zero);

    AA_TOOLKIT_API bool operator== (const pR3 &, const pR3 &);
    AA_TOOLKIT_API bool operator!= (const pR3 &, const pR3 &);

    AA_TOOLKIT_API std::istream & operator>> (std::istream &, pR3 &);
    AA_TOOLKIT_API std::ostream & operator<< (std::ostream &, const pR3 &);
#endif

////////////////////////////////////////////////////////////////////////////////
// BaryHelper //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class T, unsigned int m>
    class BaryHelper
    {
      public:
        inline static
        V<T, m> Barycenter (const V<T, m> & v1,
                            const V<T, m> & v2) throw ();

        inline static
        V<T, m> Barycenter (const V<T, m> & v1, double w1,
                            const V<T, m> & v2, double w2) throw (div_by_zero);

        inline static
        V<T, m> Barycenter (const V<T, m> & v1,
                            const V<T, m> & v2,
                            const V<T, m> & v3) throw ();

        inline static
        V<T, m> Barycenter (const V<T, m> & v1, double w1,
                            const V<T, m> & v2, double w2,
                            const V<T, m> & v3, double w3) throw (div_by_zero);

      private:
        V<T, m> m_point;
        double m_weight;

      public:
        inline BaryHelper ();
        inline void reset ();
        inline void feed (const V<T, m> &, double w = 1.0);
        inline V<T, m> get () const throw (div_by_zero);
    };

    template <class T, unsigned int m>
    V<T, m> BaryHelper<T, m>::Barycenter (const V<T, m> & v1, const V<T, m> & v2) throw ()
    {
      return Barycenter (v1, 1.0, v2, 1.0);
    }

    template <class T, unsigned int m>
    V<T, m> BaryHelper<T, m>::Barycenter (const V<T, m> & v1, double w1,
                                          const V<T, m> & v2, double w2) throw (div_by_zero)
    {
      return (v1 * w1 + v2 * w2) / (w1 + w2);
    }

    template <class T, unsigned int m>
    V<T, m> BaryHelper<T, m>::Barycenter (const V<T, m> & v1, const V<T, m> & v2, const V<T, m> & v3) throw ()
    {
      return Barycenter (v1, 1.0, v2, 1.0, v3, 1.0);
    }

    template <class T, unsigned int m>
    V<T, m> BaryHelper<T, m>::Barycenter (const V<T, m> & v1, double w1,
                                          const V<T, m> & v2, double w2,
                                          const V<T, m> & v3, double w3) throw (div_by_zero)
    {
      return (v1 * w1 + v2 * w2 + v3 * w3) / (w1 + w2 + w3);
    }

    template <class T, unsigned int m>
    BaryHelper<T, m>::BaryHelper () :
      m_point (),
      m_weight (0.0)
    {
    }

    template <class T, unsigned int m>
    void BaryHelper<T, m>::reset ()
    {
      m_point  = V<T, m> ();
      m_weight = 0.0;
    }

    template <class T, unsigned int m>
    void BaryHelper<T, m>::feed (const V<T, m> & v, double w)
    {
      m_point  += v;
      m_weight += w;
    }

    template <class T, unsigned int m>
    V<T, m> BaryHelper<T, m>::get () const throw (div_by_zero)
    {
      return m_point / m_weight;
    }

////////////////////////////////////////////////////////////////////////////////
// vR3 : 3d vector /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if 0
    class AA_TOOLKIT_API vR3
    {
      public:
        double x, y, z;

      public:
        static vR3     Normalize (const vR3 &);
        static double DotProduct (const vR3 &, const vR3 &);
        static vR3  CrossProduct (const vR3 &, const vR3 &);

      public:
        vR3 ();
        vR3 (double, double, double);
        vR3 (const pR3 &, const pR3 &);
        // default copy ctor.
        // default dtor.

        vR3 & operator*= (double);
        vR3 & operator/= (double) throw (div_by_zero);

        vR3 & operator+= (const vR3 &);
    };

    // Scale.
    AA_TOOLKIT_API vR3 operator* (const vR3 &, double);
    AA_TOOLKIT_API vR3 operator* (double, const vR3 &);

    AA_TOOLKIT_API vR3 operator/ (const vR3 &, double) throw (div_by_zero);

    // Addition.
    AA_TOOLKIT_API vR3 operator+ (const vR3 &, const vR3 &);
    AA_TOOLKIT_API vR3 operator- (const vR3 &, const vR3 &);

    // Dot product.
    AA_TOOLKIT_API double operator% (const vR3 &, const vR3 &);
    AA_TOOLKIT_API double operator% (const vR3 &, const pR3 &);
    AA_TOOLKIT_API double operator% (const pR3 &, const vR3 &);

    // Cross product.
    AA_TOOLKIT_API vR3 operator^ (const vR3 &, const vR3 &);

    // Square length.
    AA_TOOLKIT_API double operator! (const vR3 &);

    AA_TOOLKIT_API pR3 FromPolar (double theta, double phi, double rho);
    AA_TOOLKIT_API void ToPolar (const pR3 &, double * theta, double * phi, double * rho);

    AA_TOOLKIT_API bool operator== (const vR3 &, const vR3 &);
    AA_TOOLKIT_API bool operator!= (const vR3 &, const vR3 &);

    AA_TOOLKIT_API std::istream & operator>> (std::istream &, vR3 &);
    AA_TOOLKIT_API std::ostream & operator<< (std::ostream &, const vR3 &);
#endif

////////////////////////////////////////////////////////////////////////////////

    template <class T>
    AA_TOOLKIT_INLINE
    V<T, 3> ToPolar (const V<T, 3> & v)
    {
      T rho       = v.length ();
      T rho_phi   = std::sqrt (v[0] * v[0] + v[2] * v[2]);
      T cos_theta = (rho_phi != 0.0 ? v[0] / rho_phi : 1.0);
      T sin_phi   = (rho     != 0.0 ? v[1] / rho     : 0.0);
      T theta     = std::acos (cos_theta);
      T phi       = std::asin (sin_phi);
      return vec<T> (v[2] < 0.0 ? -theta : theta, phi, rho);
    }

    template <class T>
    AA_TOOLKIT_INLINE
    V<T, 3> FromPolar (const V<T, 3> & v)
    {
      T cos_theta   = std::cos (v[0]);
      T sin_theta   = std::sin (v[0]);
      T rho_cos_phi = v[2] * std::cos (v[1]);
      T rho_sin_phi = v[2] * std::sin (v[1]);
      return vec<T> (rho_cos_phi * cos_theta,
                     rho_sin_phi,
                     rho_cos_phi * sin_theta);
    }

////////////////////////////////////////////////////////////////////////////////
// Box : 3d axis-aligned bounding box //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class T, unsigned int m>
    class Box
    {
      private:
        V<T, m> m_pos;
        V<T, m> m_dim;

      public:
        inline Box ();
        inline Box (const V<T, m> &, const V<T, m> &);

        // Translation.
        inline void  translate  (const V<T, m> &);
        inline Box & operator+= (const V<T, m> &);
        inline Box   operator+  (const V<T, m> &) const;

        // Union.
        inline void  unite      (const Box &);
        inline Box & operator|= (const Box &);
        inline Box   operator|  (const Box &) const;

        // Intersection.
        inline void  intersect  (const Box &);
        inline Box & operator&= (const Box &);
        inline Box   operator&  (const Box &) const;

        // Data accessors.
        inline const V<T, m> & pos () const;
        inline const V<T, m> & dim () const;

        // Volume.
        inline bool empty () const;
        inline T volume () const;

        // Equality test.
        inline bool equals     (const Box &) const;
        inline bool operator== (const Box &) const;
        inline bool operator!= (const Box &) const;

        // Inclusion test.
        inline bool contains (const Box &) const;

        // I/O.
        inline void read  (std::istream &);
        inline void write (std::ostream &) const;

      public:
        inline static Box Center (const V<T, m> & dim);
    };

    template <class T, unsigned int m>
    Box<T, m>::Box () :
      m_pos (),
      m_dim ()
    {
    }

    template <class T, unsigned int m>
    Box<T, m>::Box (const V<T, m> & p, const V<T, m> & d) :
      m_pos (p),
      m_dim (V<T, m>::Max (V<T, m> (), d))
    {
    }

    template <class T, unsigned int m>
    void Box<T, m>::translate (const V<T, m> & v)
    {
      m_pos += v;
    }

    template <class T, unsigned int m>
    Box<T, m> & Box<T, m>::operator+= (const V<T, m> & v)
    {
      this->translate (v);
      return (*this);
    }

    template <class T, unsigned int m>
    Box<T, m> Box<T, m>::operator+ (const V<T, m> & v) const
    {
      return (Box (*this) += v);
    }

    // Union.
    template <class T, unsigned int m>
    void Box<T, m>::unite (const Box & b)
    {
      V<T, m> minimum = V<T, m>::Min (m_pos,         b.m_pos);
      V<T, m> maximum = V<T, m>::Max (m_pos + m_dim, b.m_pos + b.m_dim);
      (*this) = Box (minimum, maximum - minimum);
    }

    template <class T, unsigned int m>
    Box<T, m> & Box<T, m>::operator|= (const Box & b)
    {
      this->unite (b);
      return (*this);
    }

    template <class T, unsigned int m>
    Box<T, m> Box<T, m>::operator| (const Box & b) const
    {
      return (Box (*this) |= b);
    }

    // Intersection.
    template <class T, unsigned int m>
    void Box<T, m>::intersect (const Box & b)
    {
      V<T, m> minimum = V<T, m>::Max (m_pos,         b.m_pos);
      V<T, m> maximum = V<T, m>::Min (m_pos + m_dim, b.m_pos + b.m_dim);
      (*this) = Box (minimum, maximum - minimum);
    }

    template <class T, unsigned int m>
    Box<T, m> & Box<T, m>::operator&= (const Box & b)
    {
      this->intersect (b);
      return (*this);
    }

    template <class T, unsigned int m>
    Box<T, m> Box<T, m>::operator& (const Box & b) const
    {
      return (Box<T, m> (*this) &= b);
    }

    // Data accessors.
    template <class T, unsigned int m>
    const V<T, m> & Box<T, m>::pos () const {return m_pos;}

    template <class T, unsigned int m>
    const V<T, m> & Box<T, m>::dim () const {return m_dim;}

    template <class T, unsigned int m>
    bool Box<T, m>::empty () const
    {
      return volume () == 0;
    }

    template <class T, unsigned int m>
    T Box<T, m>::volume () const
    {
      return m_dim.prod ();
    }

    template <class T, unsigned int m>
    bool Box<T, m>::equals (const Box & b) const
    {
      if (m_pos != b.m_pos) return false;
      if (m_dim != b.m_dim) return false;
      return true;
    }

    template <class T, unsigned int m>
    bool Box<T, m>::operator== (const Box & b) const
    {
      return this->equals (b);
    }

    template <class T, unsigned int m>
    bool Box<T, m>::operator!= (const Box & b) const
    {
      return ! this->equals (b);
    }

    template <class T, unsigned int m>
    bool Box<T, m>::contains (const Box & b) const
    {
      if (b.m_pos.x < m_pos.x || b.m_pos.x + b.m_dim.x > m_pos.x + m_dim.x) return false;
      if (b.m_pos.y < m_pos.y || b.m_pos.y + b.m_dim.y > m_pos.y + m_dim.y) return false;
      if (b.m_pos.z < m_pos.z || b.m_pos.z + b.m_dim.z > m_pos.z + m_dim.z) return false;
      return true;
    }

    template <class T, unsigned int m>
    void Box<T, m>::read (std::istream & i)
    {
      using namespace TextParsing;
      i >>= '{';
      i >> m_pos.x; i >>= ','; i >> m_pos.y; i >>= ','; i >> m_pos.z;
      i >>= ',';
      i >> m_dim.x; i >>= ','; i >> m_dim.y; i >>= ','; i >> m_dim.z;
      i >>= '}';
    }

    template <class T, unsigned int m>
    void Box<T, m>::write (std::ostream & o) const
    {
      o << '{'
        << m_pos.x << ", " << m_pos.y << ", " << m_pos.z
        << ", "
        << m_dim.x << ", " << m_dim.y << ", " << m_dim.z
        << '}';
    }

    template <class T, unsigned int m>
    AA_TOOLKIT_INLINE
    std::istream & operator >> (std::istream & i, Box<T, m> & b)
    {
      b.read (i);
      return i;
    }

    template <class T, unsigned int m>
    AA_TOOLKIT_INLINE
    std::ostream & operator << (std::ostream & o, const Box<T, m> & b)
    {
      b.write (o);
      return o;
    }

    template <class T, unsigned int m>
    Box<T, m> Box<T, m>::Center (const V<T, m> & dim)
    {
      return Box (-0.5 * dim, dim);
    }

////////////////////////////////////////////////////////////////////////////////
// BoxHelper : helps computing bounding boxes //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class T, unsigned int m>
    class BoxHelper
    {
      private:
        bool m_okay;
        Box<T, m> m_box;

      public:
        inline BoxHelper ();
        inline void reset ();
        inline void feed (const V<T, m> &);
        inline const Box<T, m> & get () const;
    };

    template <class T, unsigned int m>
    BoxHelper<T, m>::BoxHelper () :
      m_okay (false),
      m_box ()
    {
    }

    template <class T, unsigned int m>
    void BoxHelper<T, m>::reset ()
    {
      m_okay = false;
      m_box = Box<T, m> ();
    }

    template <class T, unsigned int m>
    void BoxHelper<T, m>::feed (const V<T, m> & v)
    {
      if (! m_okay)
      {
        m_box = Box<T, m> (v, V<T, m> ());
        m_okay = true;
      }
      else
      {
        V<T, m> minimum = V<T, m>::Min (v, m_box.pos ());
        V<T, m> maximum = V<T, m>::Max (v, m_box.pos () + m_box.dim ());
        m_box = Box<T, m> (minimum, maximum - minimum);
      }
    }

    template <class T, unsigned int m>
    const Box<T, m> & BoxHelper<T, m>::get () const
    {
      return m_box;
    }

////////////////////////////////////////////////////////////////////////////////
// OBB : 3d oriented bounding box //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class T, unsigned int m>
    class OBB
    {
      private:
        V<T, m> m_pos;
        V<T, m> m_axes [m]; // half-extents.

      public:
        inline OBB ();
        inline OBB (const V<T, m> &, const V<T, m> [m]);
    };

  } // namespace Math

  typedef Math::Box<float,  2>  box2;
  typedef Math::Box<float,  3>  box3;
  typedef Math::Box<double, 2> dbox2;
  typedef Math::Box<double, 3> dbox3;
  typedef Math::Box<int,    2> ibox2;
  typedef Math::Box<int,    3> ibox3;

  typedef Math::OBB<float,  2>  obb2;
  typedef Math::OBB<float,  3>  obb3;
  typedef Math::OBB<double, 2> dobb2;
  typedef Math::OBB<double, 3> dobb3;

} // namespace Aa

#endif // __AA_MATH__

