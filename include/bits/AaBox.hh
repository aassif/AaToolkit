#ifndef AA_BOX__H
#define AA_BOX__H

//#include <cmath>
//#include <iostream>
//#include <stdexcept>

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// Aa::Box<T, m> : axis-aligned bounding box ///////////////////////////////////
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
    // FIXME
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
    i >> m_pos,
    i >>= ',';
    i >> m_dim;
    i >>= '}';
  }

  template <class T, unsigned int m>
  void Box<T, m>::write (std::ostream & o) const
  {
    o << '{' << m_pos << ", " << m_dim << '}';
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

////////////////////////////////////////////////////////////////////////////////
// Aa::BoxHelper<T, m> : helps computing bounding boxes ////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, unsigned int m>
  class BoxHelper
  {
    public:
      inline static
      Box<T, m> Center (const V<T, m> & dim);

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
  Box<T, m> BoxHelper<T, m>::Center (const V<T, m> & dim)
  {
    return Box<T, m> (-0.5f * dim, dim);
  }

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
// Aa::OBB<T, m> : oriented bounding box ///////////////////////////////////////
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

  typedef Box<float,  2>  box2;
  typedef Box<float,  3>  box3;
  typedef Box<double, 2> dbox2;
  typedef Box<double, 3> dbox3;
  typedef Box<int,    2> ibox2;
  typedef Box<int,    3> ibox3;

  typedef OBB<float,  2>  obb2;
  typedef OBB<float,  3>  obb3;
  typedef OBB<double, 2> dobb2;
  typedef OBB<double, 3> dobb3;

  typedef BoxHelper<float,  2> BoxHelper2f;
  typedef BoxHelper<float,  3> BoxHelper3f;
  typedef BoxHelper<double, 2> BoxHelper2d;
  typedef BoxHelper<double, 3> BoxHelper3d;

} // namespace Aa

#endif // AA_BOX__H

