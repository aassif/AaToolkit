#ifndef __AA_TABLE__
#define __AA_TABLE__

#include <sstream>

namespace Aa
{
////////////////////////////////////////////////////////////////////////////////
// CheckMultiRange /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  namespace details
  {
    template <unsigned int m>
    inline
    void CheckMultiRange (const V<unsigned int, m> & i,
                          const V<unsigned int, m> & d) throw (std::out_of_range)
    {
#ifdef AA_TABLE_DEBUG
      std::cout << "CheckMultiRange<" << m << "> (i = " << i << ", d = " << d << ")\n";
#endif
      Aa::details::CheckMultiRange<m-1> (i, d);
      Aa::details::CheckRange (i[m-1], d[m-1]);
    }

    template <>
    inline
    void CheckMultiRange (const V<unsigned int, 1> & i,
                          const V<unsigned int, 1> & d) throw (std::out_of_range)
    {
#ifdef AA_TABLE_DEBUG
      std::cout << "CheckMultiRange<1> (i = " << i << ", d = " << d << ")\n";
#endif
      Aa::details::CheckRange (i, d);
    }
  }

////////////////////////////////////////////////////////////////////////////////
// Table<n, T, A> //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <unsigned int n, class T, class A = ADM<T> >
  class Table : public Buffer<T, A>
  {
    protected:
      V<unsigned int, n> m_dims;

    public:
      Table (const V<unsigned int, n> & d) throw (std::bad_alloc);
      // Gestion du buffer.
      void alloc (unsigned int);
      void alloc (const V<unsigned int, n> & d);
      void disconnect ();
      // Dimensions.
      inline V<unsigned int, n> dims () const {return m_dims;}
      inline unsigned int dim (unsigned int k) const {return m_dims [k];}
      // Accès à un élément.
      inline /***/ T & operator[] (const V<unsigned int, n> &) /***/ throw (std::out_of_range);
      inline const T & operator[] (const V<unsigned int, n> &) const throw (std::out_of_range);
  };

  template <unsigned int n, class T, class A>
  Table<n, T, A>::Table (const V<unsigned int, n> & d) throw (std::bad_alloc) :
    Buffer<T, A> (),
    m_dims (0u)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::Table (d = " << d << ")\n";
#endif
    alloc (d);
  }

  template <unsigned int n, class T, class A>
  void Table<n, T, A>::alloc (unsigned int d)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::alloc (d = " << d << ")\n";
#endif
    alloc (V<unsigned int, n> (d));
  }

  template <unsigned int n, class T, class A>
  void Table<n, T, A>::alloc (const V<unsigned int, n> & d)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::alloc (d = " << d << ")\n";
#endif
    Buffer<T, A>::alloc (d.prod ());
    m_dims = d;
  }

  template <unsigned int n, class T, class A>
  void Table<n, T, A>::disconnect ()
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::disconnect ()\n";
#endif
    Buffer<T, A>::disconnect ();
    m_dims = V<unsigned int, n> (0);
  }

////////////////////////////////////////////////////////////////////////////////
// Accès à un élément. /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <unsigned int n, class T, class A>
  T & Table<n, T, A>::operator[] (const V<unsigned int, n> & i)
    throw (std::out_of_range)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::operator[] (i = " << i << ")\n";
#endif
    details::CheckMultiRange (i, m_dims);
    unsigned int h = V<unsigned int, n>::Horner (i, m_dims, 0);
    return this->begin () [h];
  }

  template <unsigned int n, class T, class A>
  const T & Table<n, T, A>::operator[] (const V<unsigned int, n> & i) const
    throw (std::out_of_range)
  {
#ifdef AA_TABLE_DEBUG
    std::cout << this << " Table<" << n << ">::operator[] (i = " << i << ")\n";
#endif
    details::CheckMultiRange (i, m_dims);
    unsigned int h = V<unsigned int, n>::Horner (i, m_dims, 0);
    return this->begin () [h];
  }
}

#endif // __AA_TABLE__

