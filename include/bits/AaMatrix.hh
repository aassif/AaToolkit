#ifndef AA_MATRIX__H
#define AA_MATRIX__H

namespace Aa
{
  namespace details
  {

#define AA_MP(T, U, m, n) M<AA_PROMOTE(T, U), m, n>

////////////////////////////////////////////////////////////////////////////////
// AA_VECTOR_ID (T, m, k) //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_VECTOR_ID(T, m, k) VectorId<m>::template Id<T, k> ()

    template <AaUInt m>
    struct VectorId
    {
      template <class T, AaUInt k>
      inline static
      V<T, m> Id ()
      {
        return V<T, m> (AA_VECTOR_ID (T, m-1, k), k == m-1 ? 1 : 0);
      }
    };

    template <>
    struct VectorId<1>
    {
      template <class T, AaUInt k>
      inline static
      V<T, 1> Id ()
      {
        return V<T, 1> (k == 0 ? 1 : 0);
      }
    };

////////////////////////////////////////////////////////////////////////////////
// AA_MATRIX_ID (T, m, n) //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_MATRIX_ID(T, m, n) MatrixId<m, n>::template Id<T> ()

    template <AaUInt m, AaUInt n>
    struct MatrixId
    {
      template <class T>
      inline static
      V<V<T, m>, n> Id ()
      {
        return V<V<T, m>, n> (AA_MATRIX_ID (T, m, n-1), AA_VECTOR_ID (T, m, n-1));
      }
    };

    template <AaUInt m>
    struct MatrixId<m, 1>
    {
      template <class T>
      inline static
      V<V<T, m>, 1> Id ()
      {
        return V<V<T, m>, 1> (AA_VECTOR_ID (T, m, 0));
      }
    };

  } // namespace details

  template <class T, AaUInt m, AaUInt n>
  class M : public V<V<T, m>, n>
  {
    public:
      typedef V<T,      m> Column;
      typedef V<T,      n> Row;
      typedef V<Column, n> Parent;

    public:
      inline static
      Parent Id ()
      {
        return details::AA_MATRIX_ID (T, m, n);
      }

    public:
      inline
      M () :
        Parent (Id ())
      {
      }

      inline
      M (const Parent & p) :
        Parent (p)
      {
      }

      inline
      M (const M<T, m, n-1> & p, const Column & c) :
        Parent (p, c)
      {
      }

      template <class U, AaUInt p, AaUInt q>
      inline
      M (const M<U, p, q> & u) :
        Parent (Id ())
      {
        const AaUInt cols = (n < q ? n : q);
        const AaUInt rows = (m < p ? m : p);

        for (AaUInt c = 0; c < cols; ++c)
          for (AaUInt r = 0; r < rows; ++r)
            (*this) [c][r] = static_cast<T> (u [c][r]);
      }

      inline explicit
      M (const T * data) :
        Parent ()
      {
        std::copy (data, data + m * n, &((*this)[0][0]));
      }

      inline
      Column col (AaUInt k) const
      {
        return (*this) [k];
      }

      inline
      Row row (AaUInt k) const
      {
        Row row;

        for (AaUInt c = 0; c < n; ++c)
          row [c] = (*this) [c][k];

        return row;
      }

      inline
      M<T, n, m> transpose () const
      {
        M<T, n, m> matrix;

        for (AaUInt r = 0; r < m; ++r)
          matrix [r] = this->row (r);

        return matrix;
      }

      inline
      M<T, n-1, m-1> sub (AaUInt i, AaUInt j) const
      {
        V<AaUInt, m-1> v;
        for (AaUInt k = 0; k < m-1; ++k)
          v [k] = (k < j ? k : k+1);

        M<T, n-1, m-1> matrix;

        for (AaUInt k = 0; k < n-1; ++k)
          matrix [k] = this->col (k < i ? k : k+1) (v);

        return matrix;
      }
  };

////////////////////////////////////////////////////////////////////////////////
// Produit matriciel. //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, class U, AaUInt m, AaUInt n>
  AA_TOOLKIT_INLINE
  AA_MP(T,U,m,n) operator* (const M<T, m, n> & m1, U u)
  {
    AA_MP(T,U,m,n) m2;

    for (AaUInt i = 0; i < m; ++i)
      m2 [i] = m1 [i] * u;

    return m2;
  }

  template <class T, class U, AaUInt m, AaUInt n>
  AA_TOOLKIT_INLINE
  AA_MP(T,U,m,n) operator* (T t, const M<U, m, n> & m1)
  {
    return m1 * t;
  }

  template <class T, class U, AaUInt m, AaUInt n>
  AA_TOOLKIT_INLINE
  AA_VP(T,U,m) operator* (const M<T, m, n> & m1, const V<U, n> & v1)
  {
    AA_VP(T,U,m) v2;

    for (AaUInt i = 0; i < m; ++i)
      v2 [i] = DotProd (m1.row (i), v1);

    return v2;
  }

  template <class T, class U, AaUInt m, AaUInt n, AaUInt p>
  AA_TOOLKIT_INLINE
  AA_MP(T,U,m,n) operator* (const M<T, m, n> & m1, const M<U, n, p> & m2)
  {
    AA_MP(T,U,m,n) m3;

    for (AaUInt i = 0; i < p; ++i)
      m3 [i] = m1 * m2 [i];

    return m3;
  }

////////////////////////////////////////////////////////////////////////////////
// Raccourcis. /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, AaUInt m>
  AA_TOOLKIT_INLINE
  M<T, m, 1> mat (const V<T, m> & c1)
  {
    return M<T, m, 1> (vec (c1));
  }

  template <class T, AaUInt m>
  AA_TOOLKIT_INLINE
  M<T, m, 2> mat (const V<T, m> & c1, const V<T, m> & c2)
  {
    return M<T, m, 2> (vec (c1, c2));
  }

  template <class T, AaUInt m>
  AA_TOOLKIT_INLINE
  M<T, m, 3> mat (const V<T, m> & c1, const V<T, m> & c2, const V<T, m> & c3)
  {
    return M<T, m, 3> (vec (c1, c2, c3));
  }

  template <class T, AaUInt m>
  AA_TOOLKIT_INLINE
  M<T, m, 4> mat (const V<T, m> & c1, const V<T, m> & c2, const V<T, m> & c3, const V<T, m> & c4)
  {
    return M<T, m, 4> (vec (c1, c2, c3, c4));
  }

////////////////////////////////////////////////////////////////////////////////
// Aa::M2<T, m> ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, AaUInt m>
  class M2 : public M<T, m, m>
  {
    public:
      inline
      M2 () :
        M<T, m, m> ()
      {
      }

      inline
      M2 (const M<T, m, m> & matrix) :
        M<T, m, m> (matrix)
      {
      }

      template <class U, AaUInt p, AaUInt q>
      inline
      M2 (const M<U, p, q> & u) :
        M<T, m, m> (u)
      {
      }

      inline explicit
      M2 (const T * data) :
        M<T, m, m> (data)
      {
      }

      inline
      T cofactor (AaUInt i, AaUInt j) const
      {
        M2<T, m-1> matrix = this->sub (i, j);
        return (((i+j) & 1) ? -1 : +1) * matrix.det ();
      }

      inline
      T det () const
      {
        T d = 0;
        for (AaUInt i = 0; i < m; ++i)
          d += (*this)[0][i] * this->cofactor (0, i);
        return d;
      }

      inline
      M2<T, m> adj () const
      {
        M2<T, m> a;
        for (AaUInt i = 0; i < m; ++i)
          for (AaUInt j = 0; j < m; ++j)
            a [i][j] = this->cofactor (j, i);
        return a;
      }

      inline
      M2<double, m> inv () const throw (div_by_zero)
      {
        T d = this->det ();
        if (d == 0) throw div_by_zero ();
        return (1.0 / d) * this->adj ();
      }
  };

  template <class T>
  class M2<T, 1> : public M<T, 1, 1>
  {
    public:
      inline
      M2 (const M<T, 1, 1> & matrix = M<T, 1, 1> ()) :
        M<T, 1, 1> (matrix)
      {
      }

      inline T det () const {return (*this) [0][0];};

      inline
      M2<double, 1> inv () const throw (div_by_zero)
      {
        T d = this->det ();
        if (d == 0) throw div_by_zero ();
        return M2<double, 1> (1.0 / d);
      }
  };

////////////////////////////////////////////////////////////////////////////////
// Transformations. ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Translation (const V<T, 3> & t)
  {
    return mat ({1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                V<T, 4> (t, 1));
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Rotation (T angle, const V<T, 3> & axis)
    throw (div_by_zero)
  {
    V<T, 3> v = axis.normalize ();
    T x = v [0], y = v [1], z = v [2];
    T c = std::cos (angle), s = std::sin (angle);
    return mat (vec<T> (x*x*(1-c)+c,   x*y*(1-c)+z*s, x*z*(1-c)-y*s, 0),
                vec<T> (y*x*(1-c)-z*s, y*y*(1-c)+c,   y*z*(1-c)+x*s, 0),
                vec<T> (z*x*(1-c)+y*s, z*y*(1-c)-x*s, z*z*(1-c)+c,   0),
                vec<T> (0,             0,             0,             1));
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Scale (const V<T, 3> & s)
  {
    return mat (vec<T> (s[0], 0,    0,    0),
                vec<T> (0,    s[1], 0,    0),
                vec<T> (0,    0,    s[2], 0),
                vec<T> (0,    0,    0,    1));
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Scale (T t)
  {
    return Scale (V<T, 3> (t));
  }

////////////////////////////////////////////////////////////////////////////////
// Perspective<T> //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Perspective (T vertical, T aspect, T n, T f)
  {
    T v = std::tan (0.5 * vertical);
    T h = v * aspect;

    return mat (vec<T> (1.0/h,     0,              0,  0),
                vec<T> (    0, 1.0/v,              0,  0),
                vec<T> (    0,     0,   -(f+n)/(f-n), -1),
                vec<T> (    0,     0, -(2*f*n)/(f-n),  0));
  }

////////////////////////////////////////////////////////////////////////////////
// Ortho<T> ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Ortho (T left, T right, T bottom, T top, T n, T f)
  {
    T l = left;
    T r = right;
    T b = bottom;
    T t = top;

    return mat (vec<T> (  +2.0/(r-l),            0,            0, 0),
                vec<T> (           0,   +2.0/(t-b),            0, 0),
                vec<T> (           0,            0,   -2.0/(f-n), 0),
                vec<T> (-(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1));
  }

////////////////////////////////////////////////////////////////////////////////
// Transform<T> ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Transform (const M2<T, 3> & r1, const V<T, 3> & t1,
                      const M2<T, 3> & r2, const V<T, 3> & t2)
  {
    return Translation (t2) * M2<T, 4> (r2) * M2<T, 4> (r1.inv ()) * Translation (-t1);
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Transform (const M2<T, 4> & m1,
                      const M2<T, 4> & m2)
  {
#if 0
    return Transform (M2<T, 3> (m1), (const V<T, 3> &) (m1 [3]),
                      M2<T, 3> (m2), (const V<T, 3> &) (m2 [3]));
#else
    return m2 * m1.inv ();
#endif
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Transform (const M<T, 4, 4> & m1,
                      const M<T, 4, 4> & m2)
  {
    return Transform (M2<T, 4> (m2), M2<T, 4> (m1));
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> Transform (const V<T, 3> & v1,
                      const V<T, 3> & v2)
  {
    try
    {
      V<T, 3> w1 = v1.normalize ();
      V<T, 3> w2 = v2.normalize ();
      float dot = DotProd (w1, w2);

      V<T, 3> v3 = CrossProd (w1, w2);
      float d3 = v3.length ();

      if (d3 != 0)
      {
        float a = std::atan2 (d3, dot);
        return Rotation<T> (dot >= 0 ? a : M_PI - a, v3);
      }
      else
        return Scale<T> (dot >= 0 ? +1 : -1);
    }
    catch (div_by_zero)
    {
      return M2<T, 4> ();
    }
  }

////////////////////////////////////////////////////////////////////////////////
// LookAt<T> ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 4> LookAt (const V<T, 3> & eye,
                   const V<T, 3> & center,
                   const V<T, 3> & up)
  {
    V<T, 3> v1 = (center - eye).normalize ();
    V<T, 3> v2 = up.normalize ();
    V<T, 3> v3 = CrossProd (v1, v2).normalize ();
    V<T, 3> v4 = CrossProd (v3, v1).normalize ();
    M2<T, 3> m3 = mat (v3, v4, -v1).transpose ();
    return M<T, 3, 4> (m3, m3 * -eye);
  }

////////////////////////////////////////////////////////////////////////////////
// NormalMatrix<T> /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  M2<T, 3> NormalMatrix (const M<T, 4, 4> & m)
  {
    return M2<T, 3> (m).inv ().transpose ();
  }

////////////////////////////////////////////////////////////////////////////////
// MatrixFunctor<T, k> /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, AaUInt k = 1>
  class MatrixFunctor
  {
    private:
      M2<T, 4> m_matrix;

    public:
      inline
      MatrixFunctor (const M2<T, 4> & matrix) :
        m_matrix (matrix)
      {
      }

      inline
      V<T, 3> operator() (const V<T, 3> & v)
      {
        return m_matrix * V<T, 4> (v, k);
      }
  };

////////////////////////////////////////////////////////////////////////////////
// Typedefs (GLSL). ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  typedef M2<float,  2>     mat2;
  typedef M <float,  2, 3>  mat3x2;
  typedef M <float,  2, 4>  mat4x2;

  typedef M <float,  3, 2>  mat2x3;
  typedef M2<float,  3>     mat3;
  typedef M <float,  3, 4>  mat4x3;

  typedef M <float,  4, 2>  mat2x4;
  typedef M <float,  4, 3>  mat3x4;
  typedef M2<float,  4>     mat4;

  typedef M2<double, 2>    dmat2;
  typedef M <double, 2, 3> dmat3x2;
  typedef M <double, 2, 4> dmat4x2;

  typedef M <double, 3, 2> dmat2x3;
  typedef M2<double, 3>    dmat3;
  typedef M <double, 3, 4> dmat4x3;

  typedef M <double, 4, 2> dmat2x4;
  typedef M <double, 4, 3> dmat3x4;
  typedef M2<double, 4>    dmat4;

  typedef M2<int,    2>    imat2;
  typedef M <int,    2, 3> imat3x2;
  typedef M <int,    2, 4> imat4x2;

  typedef M <int,    3, 2> imat2x3;
  typedef M2<int,    3>    imat3;
  typedef M <int,    3, 4> imat4x3;

  typedef M <int,    4, 2> imat2x4;
  typedef M <int,    4, 3> imat3x4;
  typedef M2<int,    4>    imat4;

} // namespace Aa

#endif // AA_MATRIX__H

