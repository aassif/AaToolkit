#ifndef AA_POLAR__H
#define AA_POLAR__H

//#include <cmath>
//#include <iostream>
//#include <stdexcept>

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// Aa::cartesian_to_polar //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  V<T, 3> cartesian_to_polar (const V<T, 3> & v)
  {
    T rho       = v.length ();
    T rho_phi   = std::sqrt (v[0] * v[0] + v[2] * v[2]);
    T cos_theta = (rho_phi != 0.0 ? v[0] / rho_phi : 1.0);
    T sin_phi   = (rho     != 0.0 ? v[1] / rho     : 0.0);
    T theta     = std::acos (cos_theta);
    T phi       = std::asin (sin_phi);
    return vec<T> (v[2] < 0.0 ? -theta : theta, phi, rho);
  }

////////////////////////////////////////////////////////////////////////////////
// Aa::polar_to_cartesian //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  V<T, 3> polar_to_cartesian (const V<T, 3> & v)
  {
    T cos_theta   = std::cos (v[0]);
    T sin_theta   = std::sin (v[0]);
    T rho_cos_phi = v[2] * std::cos (v[1]);
    T rho_sin_phi = v[2] * std::sin (v[1]);
    return vec<T> (rho_cos_phi * cos_theta,
                   rho_sin_phi,
                   rho_cos_phi * sin_theta);
  }

} // namespace Aa

#endif // AA_POLAR__H

