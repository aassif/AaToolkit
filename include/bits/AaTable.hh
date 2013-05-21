#ifndef AA_TABLE__H
#define AA_TABLE__H

#include <sstream>

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// CheckMultiRange<m> //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  namespace details
  {
    template <AaUInt m>
    inline
    void CheckMultiRange (const V<AaUInt, m> & i,
                          const V<AaUInt, m> & d)
      throw (std::out_of_range)
    {
#ifdef AA_TABLE_DEBUG
      std::cout << "CheckMultiRange<" << m << "> (i = " << i << ", d = " << d << ")\n";
#endif
      Aa::details::CheckMultiRange<m-1> (i, d);
      Aa::details::CheckRange (i[m-1], d[m-1]);
    }

    template <>
    inline
    void CheckMultiRange (const V<AaUInt, 1> & i,
                          const V<AaUInt, 1> & d)
      throw (std::out_of_range)
    {
#ifdef AA_TABLE_DEBUG
      std::cout << "CheckMultiRange<1> (i = " << i << ", d = " << d << ")\n";
#endif
      Aa::details::CheckRange (i[0], d[0]);
    }
  }

////////////////////////////////////////////////////////////////////////////////
// Flatten<m> //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  namespace details
  {
    template <AaUInt m>
    inline
    AaUInt Flatten (const V<AaUInt, m> & i,
                    const V<AaUInt, m> & d,
                    AaUInt accu)
    {
      return Aa::details::Flatten<m-1> (i, d, accu * d[m-1] + i[m-1]);
    }

    template <>
    inline
    AaUInt Flatten (const V<AaUInt, 1> & i,
                    const V<AaUInt, 1> & d,
                    AaUInt accu)
    {
      return d[0] * accu + i[0];
    }
  }

  template <AaUInt m>
  inline
  AaUInt Flatten (const V<AaUInt, m> & i,
                  const V<AaUInt, m> & d)
  {
    return Aa::details::Flatten<m> (i, d, 0u);
  }

////////////////////////////////////////////////////////////////////////////////
// Unflatten<m> ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  namespace details
  {
    template <AaUInt m>
    inline
    std::pair<V<AaUInt, m>, AaUInt> Unflatten (AaUInt k, const V<AaUInt, m> & d)
    {
      std::pair<V<AaUInt, m-1>, AaUInt> p = Aa::details::Unflatten<m-1> (k, d);
      return std::make_pair (V<AaUInt, m> (p.first, p.second % d[m-1]), p.second / d[m-1]);
    }

    template <>
    inline
    std::pair<V<AaUInt, 1>, AaUInt> Unflatten (AaUInt k, const V<AaUInt, 1> & d)
    {
      return std::make_pair (V<AaUInt, 1> (k % d[0]), k / d[0]);
    }
  }

  template <AaUInt m>
  inline
  V<AaUInt, m> Unflatten (AaUInt k, const V<AaUInt, m> & d)
    throw (std::out_of_range)
  {
    details::CheckRange (k, d.prod ());
    return details::Unflatten<m> (k, d).first;
  }

////////////////////////////////////////////////////////////////////////////////
// Table<n, T, A> //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <AaUInt n, class T, class A = ADM<T> >
  class Table : public Buffer<T, A>
  {
    protected:
      V<AaUInt, n> m_dims;

    public:
      Table (const V<AaUInt, n> & d = V<AaUInt, n> (0)) throw (std::bad_alloc);
      // Gestion du buffer.
      inline void alloc (const V<AaUInt, n> & d);
      inline void disconnect ();
      // Dimensions.
      inline V<AaUInt, n> dims () const {return m_dims;}
      inline AaUInt dim (AaUInt k) const {return m_dims [k];}
      // Accès à un élément.
      inline /***/ T & operator[] (const V<AaUInt, n> &) /***/ throw (std::out_of_range);
      inline const T & operator[] (const V<AaUInt, n> &) const throw (std::out_of_range);
  };

  template <AaUInt n, class T, class A>
  Table<n, T, A>::Table (const V<AaUInt, n> & d) throw (std::bad_alloc) :
    Buffer<T, A> (),
    m_dims (0u)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::Table (d = " << d << ")\n";
#endif
    alloc (d);
  }

  template <AaUInt n, class T, class A>
  void Table<n, T, A>::alloc (const V<AaUInt, n> & d)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::alloc (d = " << d << ")\n";
#endif
    Buffer<T, A>::alloc (d.prod ());
    m_dims = d;
  }

  template <AaUInt n, class T, class A>
  void Table<n, T, A>::disconnect ()
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::disconnect ()\n";
#endif
    Buffer<T, A>::disconnect ();
    m_dims = V<AaUInt, n> (0u);
  }

////////////////////////////////////////////////////////////////////////////////
// Accès à un élément. /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <AaUInt n, class T, class A>
  T & Table<n, T, A>::operator[] (const V<AaUInt, n> & i)
    throw (std::out_of_range)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::operator[] (i = " << i << ")\n";
#endif
    details::CheckMultiRange (i, m_dims);
    AaUInt f = Flatten (i, m_dims);
    return this->begin () [f];
  }

  template <AaUInt n, class T, class A>
  const T & Table<n, T, A>::operator[] (const V<AaUInt, n> & i) const
    throw (std::out_of_range)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::operator[] (i = " << i << ")\n";
#endif
    details::CheckMultiRange (i, m_dims);
    AaUInt f = Flatten (i, m_dims);
    return this->begin () [f];
  }
}

#endif // AA_TABLE__H

