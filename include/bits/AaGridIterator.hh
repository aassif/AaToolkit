#ifndef AA_GRID_ITERATOR__H
#define AA_GRID_ITERATOR__H

#include <cstddef>
#include <iterator>

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// Aa::GridIterator<m> /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <AaUInt m>
  class GridIterator :
    public std::iterator<std::random_access_iterator_tag, V<AaUInt, m> >
  {
    private:
      V<AaUInt, m> m_dim;
      AaUInt       m_pos;

    public:
      inline static
      GridIterator Begin (const V<AaUInt, m> & d)
      {
        return GridIterator (d, V<AaUInt, m> (0));
      }

      inline static
      GridIterator End (const V<AaUInt, m> & d)
      {
        return GridIterator (d);
      }

    private:
      inline explicit
      GridIterator (const V<AaUInt, m> & d) :
        m_dim (d),
        m_pos (d.prod ())
      {
      }

      inline explicit
      GridIterator (const V<AaUInt, m> & d,
                    const V<AaUInt, m> & p) :
        m_dim (d),
        m_pos (Flatten (p, d))
      {
      }

    public:
      inline
      V<AaUInt, m> operator* () const
      {
        return Unflatten (m_pos, m_dim);
      }

      inline
      GridIterator & operator++ ()
      {
        ++m_pos;
        return *this;
      }

      inline
      GridIterator operator++ (int /*postfix*/)
      {
        GridIterator it (*this);
        ++(*this);
        return it;
      }

      inline
      ptrdiff_t operator- (const GridIterator & it) const
      {
        return m_pos - it.m_pos;
      }

      inline
      GridIterator & operator+= (ptrdiff_t n)
      {
        m_pos += n;
        return (*this);
      }

      inline
      bool operator== (const GridIterator & it) const
      {
        return m_pos == it.m_pos;
      }

      inline
      bool operator!= (const GridIterator & it) const
      {
        return m_pos != it.m_pos;
      }
  };

} // namespace Aa

#endif // AA_GRID_ITERATOR__H

