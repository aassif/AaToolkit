#ifndef AA_QUATERNION__H
#define AA_QUATERNION__H

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// Aa::Q<T> ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  class Q : public V<T, 4>
  {
    public:
      inline
      Q () :
        V<T, 4> (vec<T> (0, 0, 0, 1))
      {
      }

      inline
      Q (const T & angle, const V<T, 3> & axis) AA_THROW (div_by_zero) :
        V<T, 4> (axis.normalize () * std::sin (0.5 * angle), std::cos (0.5 * angle))
      {
      }

      inline
      Q (const M2<T, 3> & m) :
        V<T, 4> ()
      {
        (*this) = m;
      }

      inline
      Q (const V<T, 4> & v) :
        V<T, 4> (v.normalize ())
      {
      }

      inline
      Q & operator= (const M2<T, 3> & m)
      {
        T m00 = m [0][0], m01 = m [0][1], m02 = m [0][2];
        T m10 = m [1][0], m11 = m [1][1], m12 = m [1][2];
        T m20 = m [2][0], m21 = m [2][1], m22 = m [2][2];

        if (m00 + m11 + m22 > 0)
        {
          T t = 1 + m00 + m11 + m22;
          (*this) = (0.5f * std::sqrt (t)) * vec<T> (m12 - m21, m20 - m02, m01 - m10, t);
        }
        else if (m00 > m11 && m00 > m22)
        {
          T t = 1 + m00 - m11 - m22;
          (*this) = (0.5f * std::sqrt (t)) * vec<T> (t, m01 + m10, m20 + m02, m12 - m21);
        }
        else if (m11 > m22)
        {
          T t = 1 - m00 + m11 - m22;
          (*this) = (0.5f * std::sqrt (t)) * vec<T> (m01 + m10, t, m12 + m21, m20 - m02);
        }
        else
        {
          T t = 1 - m00 - m11 + m22;
          (*this) = (0.5f * std::sqrt (t)) * vec<T> (m20 + m02, m12 + m21, t, m01 - m10);
        }

        return (*this);
      }

      inline
      M2<T, 3> matrix () const
      {
        //std::cout << "length: " << this->length () << std::endl;

        T x = (*this) [0];
        T y = (*this) [1];
        T z = (*this) [2];
        T w = (*this) [3];

        T xw = 2 * x * w, x2 = 2 * x * x;
        T yw = 2 * y * w, xy = 2 * x * y, y2 = 2 * y * y;
        T zw = 2 * z * w, xz = 2 * x * z, yz = 2 * y * z, z2 = 2 * z * z;

        return mat (vec (1 - y2 - z2,     xy - zw,     xz + yw),
                    vec (    xy + zw, 1 - x2 - z2,     yz - xw),
                    vec (    xz - yw,     yz + xw, 1 - x2 - y2));
      }

#if 0
      inline
      operator M2<T, 3> () const
      {
        return this->matrix ();
      }
#endif
  };

  typedef Q<float>   quat;
  typedef Q<double> dquat;

} // namespace Aa

#endif // AA_QUATERNION__H

