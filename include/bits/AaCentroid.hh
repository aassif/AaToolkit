#ifndef AA_CENTROID__H
#define AA_CENTROID__H

//#include <cmath>
//#include <iostream>
//#include <stdexcept>

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// Aa::CentroidHelper<T, m> ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, unsigned int m>
  class CentroidHelper
  {
    public:
      inline static
      V<T, m> Centroid (const V<T, m> & v1,
                        const V<T, m> & v2) throw ();

      inline static
      V<T, m> Centroid (const V<T, m> & v1, double w1,
                        const V<T, m> & v2, double w2) throw (div_by_zero);

      inline static
      V<T, m> Centroid (const V<T, m> & v1,
                        const V<T, m> & v2,
                        const V<T, m> & v3) throw ();

      inline static
      V<T, m> Centroid (const V<T, m> & v1, double w1,
                        const V<T, m> & v2, double w2,
                        const V<T, m> & v3, double w3) throw (div_by_zero);

    private:
      V<T, m> m_point;
      double m_weight;

    public:
      inline CentroidHelper ();
      inline void reset ();
      inline void feed (const V<T, m> &, double w = 1.0);
      inline V<T, m> get () const throw (div_by_zero);
  };

  template <class T, unsigned int m>
  V<T, m> CentroidHelper<T, m>::Centroid (const V<T, m> & v1, const V<T, m> & v2) throw ()
  {
    return Centroid (v1, 1.0, v2, 1.0);
  }

  template <class T, unsigned int m>
  V<T, m> CentroidHelper<T, m>::Centroid (const V<T, m> & v1, double w1,
                                          const V<T, m> & v2, double w2) throw (div_by_zero)
  {
    return (v1 * w1 + v2 * w2) / (w1 + w2);
  }

  template <class T, unsigned int m>
  V<T, m> CentroidHelper<T, m>::Centroid (const V<T, m> & v1, const V<T, m> & v2, const V<T, m> & v3) throw ()
  {
    return Centroid (v1, 1.0, v2, 1.0, v3, 1.0);
  }

  template <class T, unsigned int m>
  V<T, m> CentroidHelper<T, m>::Centroid (const V<T, m> & v1, double w1,
                                          const V<T, m> & v2, double w2,
                                          const V<T, m> & v3, double w3) throw (div_by_zero)
  {
    return (v1 * w1 + v2 * w2 + v3 * w3) / (w1 + w2 + w3);
  }

  template <class T, unsigned int m>
  CentroidHelper<T, m>::CentroidHelper () :
    m_point (),
    m_weight (0.0)
  {
  }

  template <class T, unsigned int m>
  void CentroidHelper<T, m>::reset ()
  {
    m_point  = V<T, m> ();
    m_weight = 0.0;
  }

  template <class T, unsigned int m>
  void CentroidHelper<T, m>::feed (const V<T, m> & v, double w)
  {
    m_point  += v;
    m_weight += w;
  }

  template <class T, unsigned int m>
  V<T, m> CentroidHelper<T, m>::get () const throw (div_by_zero)
  {
    return m_point / m_weight;
  }

} // namespace Aa

#endif // AA_CENTROID__H

