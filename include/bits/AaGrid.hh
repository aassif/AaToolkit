#ifndef AA_GRID__H
#define AA_GRID__H

namespace Aa
{

  template <typename T, AaUInt m>
  class Grid
  {
    private:
      V<AaUInt, m> m_dims;
      V<T,      m> m_unit;
      V<T,      m> m_pos;

    public:
      inline
      Grid (const Box<T, m> & b, const V<T, m> & u) :
        m_dims (std::ceil (b.dims () / u)),
        m_unit (u),
        m_pos  (b.center () - 0.5f * m_dims * u)
      {
      }

      inline const V<AaUInt, m> & dims () const {return m_dims;}
      inline const V<T,      m> & unit () const {return m_unit;}
      inline const V<T,      m> & pos  () const {return m_pos;}

      inline Box<T, m> box () const {return Box<T, m> (m_pos, m_dims * m_unit);}

      inline GridIterator<m> begin () const {return GridIterator<m>::Begin (m_dims);}
      inline GridIterator<m> end   () const {return GridIterator<m>::End   (m_dims);}
  };

  typedef Grid<float,  1>  grid1;
  typedef Grid<float,  2>  grid2;
  typedef Grid<float,  3>  grid3;

  typedef Grid<double, 1> dgrid1;
  typedef Grid<double, 2> dgrid2;
  typedef Grid<double, 3> dgrid3;

} // namespace Aa

#endif // AA_GRID__H

