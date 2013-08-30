#ifndef AA_INTERPOLATOR__H
#define AA_INTERPOLATOR__H

namespace Aa
{
  namespace details
  {

    template <typename T, AaUInt m>
    inline static
    V<float, m> Linear (const V<T, m> & v0, const V<T, m> & v1, float t)
    {
      return v0 * (1.0f - t) + v1 * t;
    }

    template <typename T, AaUInt m, AaUInt k>
    class InterpolationHelper
    {
      public:
        typedef InterpolationHelper<T, m, k-1> Parent;

      public:
        inline static
        V<float, m> Interpolate (const V<T, m> * data, const V<AaUInt, k> & d, const V<float, k> & p)
        {
          float x = p [k-1];
          int   n = d [k-1];
          float f = std::floor (x);
          int   i = (int) f;
          float t = x - f;

          const V<AaUInt, k-1> & d0 = d;
          const V<float,  k-1> & p0 = p;

          if (i <    0) return Parent::Interpolate (data,                      d0, p0);
          if (i >= n-1) return Parent::Interpolate (data + (n-1) * d0.prod (), d0, p0);

          V<T, m> v0 = Parent::Interpolate (data + (i + 0) * d0.prod (), d0, p0);
          V<T, m> v1 = Parent::Interpolate (data + (i + 1) * d0.prod (), d0, p0);
          return details::Linear (v0, v1, t);
        }
    };

    template <typename T, AaUInt m>
    class InterpolationHelper<T, m, 1>
    {
      public:
        inline static
        V<float, m> Interpolate (const V<T, m> * data, const V<AaUInt, 1> & d, const V<float, 1> & p)
        {
          float x = p [0];
          int   n = d [0];
          float f = std::floor (x);
          int   i = (int) f;
          float t = x - f;

          if (i <    0) return data [0];
          if (i >= n-1) return data [n-1];

          const V<T, m> & v0 = data [i+0];
          const V<T, m> & v1 = data [i+1];
          return details::Linear (v0, v1, t);
        }
    };

  } // namespace details

  template <AaUInt n, typename T, AaUInt m>
  class Interpolator
  {
    public:
      typedef details::InterpolationHelper<T, m, n> Helper;

    public:
      const V<T,      m> * m_data;
      /***/ V<AaUInt, n>   m_dims;

    public:
      template <class U>
      inline static
      void Resample (const Signal<n, T, m> * s1,
                     /***/ Signal<n, U, m> * s2)
      {
        Interpolator interpolator (s1);
        const uvec3 & d2 = s2->dims ();
        vec3 ratio = vec3 (s1->dims ()) / d2;
        typedef GridIterator<n> Iterator;
        for (Iterator it = Iterator::Begin (d2); it != Iterator::End (d2);)
        {
          uvec3 v2 = *(it++);
          vec3 v1 = 0.5f + (v2 * ratio);
          //(*s2) [v2] = interpolator (v1);
          (*s2) [v2] = (*s1) [uvec3 (v1)];
        }
      }

      template <class U>
      inline static
      Signal<n, U, m> Resample (const Signal<n, T, m> * s1,
                                const V<AaUInt, n>    & d2)
      {
        Signal<n, U, m> s2 (d2);
        Interpolator::Resample (s1, &s2);
        return s2;
      }

    public:
      Interpolator (const Signal<n, T, m> * s) :
        m_data (s->begin ()),
        m_dims (s->dims ())
      {
      }

      V<float, m> operator() (const V<T, n> & p) const
      {
        return Helper::Interpolate (m_data, m_dims, p - 0.5f);
      }
  };

  template <typename T, AaUInt m>
    inline static
      Interpolator<1, T, m> Linear (const Signal<1, T, m> * s)
      {
        return Interpolator<1, T, m> (s);
      }

  template <typename T, AaUInt m>
    inline static
      Interpolator<2, T, m> Bilinear (const Signal<2, T, m> * s)
      {
        return Interpolator<2, T, m> (s);
      }

  template <typename T, AaUInt m>
    inline static
      Interpolator<3, T, m> Trilinear (const Signal<3, T, m> * s)
      {
        return Interpolator<3, T, m> (s);
      }

} // namespace Aa

#endif // AA_INTERPOLATOR__H

