#include <cmath>
#include "AaMath"

using namespace std;

namespace Aa
{
  namespace Math
  {
////////////////////////////////////////////////////////////////////////////////
// pR3 /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    pR3::pR3 () :
      x (0.0), y (0.0), z (0.0) {}
    
    pR3::pR3 (double a, double b, double c) :
      x (a), y (b), z (c) {}
    
    pR3 & pR3::operator+= (const vR3 & v)
    {
      x += v.x;
      y += v.y;
      z += v.z;
      return (*this);
    }
    
    pR3 & pR3::operator*= (double f)
    {
      x *= f;
      y *= f;
      z *= f;
      return (*this);
    }

    pR3 operator+ (const pR3 & p, const vR3 & v)
    {
      return pR3 (p.x + v.x, p.y + v.y, p.z + v.z);
    }
    
    pR3 operator+ (const vR3 & v, const pR3 & p) {return (p + v);}
    
    vR3 operator- (const pR3 & p, const pR3 & q) {return vR3 (q, p);}
    
    pR3 operator* (const pR3 & p, double f)
    {
      return pR3 (p.x * f, p.y * f, p.z * f);
    }

    pR3 operator* (double f, const pR3 & p) {return (p * f);}

    pR3 operator/ (const pR3 & p, double f) throw (div_by_zero)
    {
      if (f == 0.0) throw div_by_zero ();
      return (p * (1.0 / f));
    }

    pR3 pR3::Barycenter (const pR3 & p1, const pR3 & p2) throw ()
    {
      return pR3::Barycenter (p1, 1.0, p2, 1.0);
    }

    pR3 pR3::Barycenter (const pR3 & p1, double w1,
                         const pR3 & p2, double w2) throw (div_by_zero)
    {
      return pR3 (p1.x * w1 + p2.x * w2,
                  p1.y * w1 + p2.y * w2,
                  p1.z * w1 + p2.z * w2) / (w1 + w2);
    }

    pR3 pR3::Barycenter (const pR3 & p1, const pR3 & p2, const pR3 & p3) throw ()
    {
      return pR3::Barycenter (p1, 1.0, p2, 1.0, p3, 1.0);
    }

    pR3 pR3::Barycenter (const pR3 & p1, double w1,
                         const pR3 & p2, double w2,
                         const pR3 & p3, double w3) throw (div_by_zero)
    {
      return pR3 (p1.x * w1 + p2.x * w2 + p3.x * w3,
                  p1.y * w1 + p2.y * w2 + p3.y * w3,
                  p1.z * w1 + p2.z * w2 + p3.z * w3) / (w1 + w2 + w3);
    }

    bool operator== (const pR3 & p, const pR3 & q)
    {
      return (p.x == q.x && p.y == q.y && p.z == q.z);
    }
    
    bool operator!= (const pR3 & p, const pR3 & q)
    {
      return (p.x != q.x || p.y != q.y || p.z != q.z);
    }
    
    istream & operator>> (istream & i, pR3 & p)
    {
      using namespace TextParsing;
      i >>= '{'; i >> p.x; i >>= ','; i >> p.y; i >>= ','; i >> p.z; i >>= '}';
      return i;
    }
    
    ostream & operator<< (ostream & o, const pR3 & p)
    {
      return o << '{' << p.x << ", " << p.y << ", " << p.z << '}';
    }
    
////////////////////////////////////////////////////////////////////////////////
// BaryHelper //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    BaryHelper::BaryHelper () :
      m_point (0.0, 0.0, 0.0),
      m_weight (0.0) {}

    void BaryHelper::reset ()
    {
      m_point  = pR3 (0.0, 0.0, 0.0);
      m_weight = 0.0;
    }

    void BaryHelper::feed (const pR3 & p, double w)
    {
      m_point.x += p.x;
      m_point.y += p.y;
      m_point.z += p.z;
      m_weight += w;
    }

    pR3 BaryHelper::get () const throw (div_by_zero)
    {
      return m_point / m_weight;
    }
    
////////////////////////////////////////////////////////////////////////////////
// vR3 /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    vR3::vR3 () :
      x (0.0), y (0.0), z (0.0) {}
    
    vR3::vR3 (double a, double b, double c) :
      x (a), y (b), z (c) {}
    
    vR3::vR3 (const pR3 & p, const pR3 & q) :
      x (q.x - p.x), y (q.y - p.y), z (q.z - p.z) {}
    
    vR3 & vR3::operator*= (double f)
    {
      x *= f;
      y *= f;
      z *= f;
      return (*this);
    }
    
    vR3 & vR3::operator/= (double f) throw (div_by_zero)
    {
      if (f == 0.0) throw div_by_zero ();
      return (*this) *= (1.0 / f);
    }
    
    vR3 & vR3::operator+= (const vR3 & v)
    {
      x += v.x;
      y += v.y;
      z += v.z;
      return (*this);
    }
    
    // Scale.
    vR3 operator* (const vR3 & v, double f)
    {
      return vR3 (v.x * f, v.y * f, v.z * f);
    }

    vR3 operator* (double f, const vR3 & v) {return (v * f);}
    
    vR3 operator/ (const vR3 & v, double f) throw (div_by_zero)
    {
      if (f == 0.0) throw div_by_zero ();
      return v * (1.0 / f);
    }

    // Addition.
    vR3 operator+ (const vR3 & v, const vR3 & w)
    {
      return vR3 (v.x + w.x, v.y + w.y, v.z + w.z);
    }
    
    vR3 operator- (const vR3 & v, const vR3 & w)
    {
      return vR3 (v.x - w.x, v.y - w.y, v.z - w.z);
    }
    
    // Squared length.
    double operator! (const vR3 & v) {return (v.x * v.x + v.y * v.y + v.z * v.z);}

    vR3 vR3::Normalize (const vR3 & v) {return v / std::sqrt (!v);}
    
    // Dot product.
    double vR3::DotProduct (const vR3 & v, const vR3 & w) {return (v.x * w.x + v.y * w.y + v.z * w.z);}
    
    double operator% (const vR3 & v, const vR3 & w) {return vR3::DotProduct (v, w);}
    double operator% (const vR3 & v, const pR3 & w) {return (v.x * w.x + v.y * w.y + v.z * w.z);}
    double operator% (const pR3 & v, const vR3 & w) {return (v.x * w.x + v.y * w.y + v.z * w.z);}
    
    // Cross product.
    vR3 vR3::CrossProduct (const vR3 & v, const vR3 & w)
    {
      return vR3 (v.y * w.z - w.y * v.z,
                  v.z * w.x - w.z * v.x,
                  v.x * w.y - w.x * v.y);
    }

    vR3 operator^ (const vR3 & v, const vR3 & w) {return vR3::CrossProduct (v, w);}
    
    void ToPolar (const pR3 & p, double * ptr_theta, double * ptr_phi, double * ptr_rho)
    {
      double rho_phi = sqrt (p.x * p.x + p.z * p.z);
      double rho     = sqrt (p.x * p.x + p.y * p.y + p.z * p.z);
    
      double cos_theta = (rho_phi != 0.0 ? p.x / rho_phi : 1.0);
      double sin_phi   = (rho     != 0.0 ? p.y / rho     : 0.0);
      double theta     = acos (cos_theta);
      double phi       = asin (sin_phi);
      if (p.z < 0.0) theta = -theta;

      if (ptr_theta != NULL) *ptr_theta = theta;
      if (ptr_phi   != NULL) *ptr_phi   = phi;
      if (ptr_rho   != NULL) *ptr_rho   = rho;
    }
    
    pR3 FromPolar (double theta, double phi, double rho)
    {
      return pR3 (cos (phi) * cos (theta) * rho,
                  sin (phi) * rho,
                  cos (phi) * sin (theta) * rho);
    }
    
    
    bool operator== (const vR3 & v, const vR3 & w)
    {
      return (v.x == w.x && v.y == w.y && v.z == w.z);
    }
    
    bool operator!= (const vR3 & v, const vR3 & w)
    {
      return (v.x != w.x || v.y != w.y || v.z != w.z);
    }
    
    istream & operator>> (istream & i, vR3 & v)
    {
      using namespace TextParsing;
      i >>= '{'; i >> v.x; i >>= ','; i >> v.y; i >>= ','; i >> v.z; i >>= '}';
      return i;
    }
    
    ostream & operator<< (ostream & o, const vR3 & v)
    {
      return o << '{' << v.x << ", " << v.y << ", " << v.z << '}';
    }

////////////////////////////////////////////////////////////////////////////////
// Box /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Box::Box () :
      m_pos (0.0, 0.0, 0.0),
      m_dim (0.0, 0.0, 0.0) {}
    
    Box::Box (const pR3 & p, const vR3 & d) :
      m_pos (p),
      m_dim (d) {}
    
    Box::Box (double px, double py, double pz,
              double dx, double dy, double dz) :
      m_pos (px, py, pz),
      m_dim (dx, dy, dz)
    {
    }
    
    // Translation.
    void Box::translate (const vR3 & v) {m_pos += v;}
    
    Box & Box::operator+= (const vR3 & v)
    {
      this->translate (v);
      return (*this);
    }
    
    Box Box::operator+ (const vR3 & v) const
    {
      return (Box (*this) += v);
    }
    
    // Union.
    void Box::unite (const Box & b)
    {
      double xMin = min (m_pos.x, b.m_pos.x), xMax = max (m_pos.x + m_dim.x, b.m_pos.x + b.m_dim.x);
      double yMin = min (m_pos.y, b.m_pos.y), yMax = max (m_pos.y + m_dim.y, b.m_pos.y + b.m_dim.y);
      double zMin = min (m_pos.z, b.m_pos.z), zMax = max (m_pos.z + m_dim.z, b.m_pos.z + b.m_dim.z);

      m_pos.x = xMin; m_dim.x = xMax - xMin;
      m_pos.y = yMin; m_dim.y = yMax - yMin;
      m_pos.z = zMin; m_dim.z = zMax - zMin;
    }
    
    Box & Box::operator|= (const Box & b)
    {
      this->unite (b);
      return (*this);
    }
    
    Box Box::operator| (const Box & b) const
    {
      return (Box (*this) |= b);
    }
    
    // Intersection.
    void Box::intersect (const Box & b)
    {
      double xMin = max (m_pos.x, b.m_pos.x), xMax = min (m_pos.x + m_dim.x, b.m_pos.x + b.m_dim.x);
      double yMin = max (m_pos.y, b.m_pos.y), yMax = min (m_pos.y + m_dim.y, b.m_pos.y + b.m_dim.y);
      double zMin = max (m_pos.z, b.m_pos.z), zMax = min (m_pos.z + m_dim.z, b.m_pos.z + b.m_dim.z);

      m_pos.x = xMin; m_dim.x = xMax - xMin;
      m_pos.y = yMin; m_dim.y = yMax - yMin;
      m_pos.z = zMin; m_dim.z = zMax - zMin;
    }
    
    Box & Box::operator&= (const Box & b)
    {
      this->intersect (b);
      return (*this);
    }
    
    Box Box::operator& (const Box & b) const
    {
      return (Box (*this) &= b);
    }
    
    // Data accessors.
    const pR3 & Box::pos () const {return m_pos;}
    const vR3 & Box::dim () const {return m_dim;}
    
    bool Box::isEmpty () const
    {
      if (m_dim.x <= 0.0) return true;
      if (m_dim.y <= 0.0) return true;
      if (m_dim.z <= 0.0) return true;
      return false;
    }
    
    double Box::volume () const
    {
      return max (m_dim.x, 0.0) * max (m_dim.y, 0.0) * max (m_dim.z, 0.0);
    }
    
    bool Box::equals (double px, double py, double pz,
                      double dx, double dy, double dz) const
    {
      if (m_pos.x != px || m_pos.y != py || m_pos.z != pz) return false;
      if (m_dim.x != dx || m_dim.y != dy || m_dim.z != dz) return false;
      return true;
    }
    
    bool Box::equals (const Box & b) const
    {
      if (m_pos != b.pos ()) return false;
      if (m_dim != b.dim ()) return false;
      return true;
    }
    
    bool Box::operator== (const Box & b) const
    {
      return this->equals (b);
    }
    
    bool Box::operator!= (const Box & b) const
    {
      return ! this->equals (b);
    }
    
    bool Box::contains (double px, double py, double pz,
                        double dx, double dy, double dz) const
    {
      if (px < m_pos.x || px + dx > m_pos.x + m_dim.x) return false; 
      if (py < m_pos.y || py + dy > m_pos.y + m_dim.y) return false; 
      if (pz < m_pos.z || pz + dz > m_pos.z + m_dim.z) return false; 
      return true;
    }
    
    bool Box::contains (const Box & b) const
    {
      if (b.m_pos.x < m_pos.x || b.m_pos.x + b.m_dim.x > m_pos.x + m_dim.x) return false; 
      if (b.m_pos.y < m_pos.y || b.m_pos.y + b.m_dim.y > m_pos.y + m_dim.y) return false; 
      if (b.m_pos.z < m_pos.z || b.m_pos.z + b.m_dim.z > m_pos.z + m_dim.z) return false; 
      return true;
    }
    
    void Box::read (istream & i)
    {
      using namespace TextParsing;
      i >>= '{';
      i >> m_pos.x; i >>= ','; i >> m_pos.y; i >>= ','; i >> m_pos.z;
      i >>= ',';
      i >> m_dim.x; i >>= ','; i >> m_dim.y; i >>= ','; i >> m_dim.z;
      i >>= '}';
    }
    
    void Box::write (ostream & o) const
    {
      o << '{'
        << m_pos.x << ", " << m_pos.y << ", " << m_pos.z
        << ", "
        << m_dim.x << ", " << m_dim.y << ", " << m_dim.z
        << '}';
    }
    
    istream & operator >> (istream & i, Box & b)
    {
      b.read (i);
      return i;
    }
    
    ostream & operator << (ostream & o, const Box & b)
    {
      b.write (o);
      return o;
    }

    Box Box::Center (double dx, double dy, double dz)
    {
      return Box (-0.5 * dx, -0.5 * dy, -0.5 * dz, dx, dy, dz);
    }

////////////////////////////////////////////////////////////////////////////////
// BoxHelper ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    BoxHelper::BoxHelper () :
      m_okay (false),
      m_box (0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
    {
    }
    
    BoxHelper::~BoxHelper ()
    {
    }
    
    void BoxHelper::reset ()
    {
      m_okay = false;
      m_box = Box (0.0, 0.0, 0.0, -1.0, -1.0, -1.0);
    }
    
    void BoxHelper::feed (const pR3 & p)
    {
      if (! m_okay)
      {
        m_okay = true;
        m_box = Box (p, vR3 (0.0, 0.0, 0.0));
      } 
    
      const pR3 & pos = m_box.pos ();
      const vR3 & dim = m_box.dim ();
      double xMin = pos.x, xMax = pos.x + dim.x;
      double yMin = pos.y, yMax = pos.y + dim.y;
      double zMin = pos.z, zMax = pos.z + dim.z;
      if (p.x < xMin) xMin = p.x;
      if (p.y < yMin) yMin = p.y;
      if (p.z < zMin) zMin = p.z;
      m_box = Box (xMin, yMin, zMin,
                   (p.x > xMax ? p.x : xMax) - xMin,
                   (p.y > yMax ? p.y : yMax) - yMin,
                   (p.z > zMax ? p.z : zMax) - zMin);
    }
    
    const Box & BoxHelper::get () const
    {
      return m_box;
    }
  } // namespace Math
} // namespace Aa

