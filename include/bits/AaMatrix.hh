#ifndef __AA_MATRIX__
#define __AA_MATRIX__

namespace Aa
{
  namespace details
  {

#define AA_MP(T, U, m, n) M<AA_PROMOTE(T, U), m, n>

////////////////////////////////////////////////////////////////////////////////
// AA_VECTOR_ID (T, m, k) //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_VECTOR_ID(T, m, k) VectorId<m>::template Id<T> (k)

    template <unsigned int m>
    struct VectorId
    {
      template <class T>
      inline static
      V<T, m> Id (unsigned int k)
      {
        return V<T, m> (AA_VECTOR_ID (T, m-1, k), k == m-1 ? 1 : 0);
      }
    };

    template <>
    struct VectorId<1>
    {
      template <class T>
      inline static
      V<T, 1> Id (unsigned int k)
      {
        return V<T, 1> (k == 0 ? 1 : 0);
      }
    };

////////////////////////////////////////////////////////////////////////////////
// AA_MATRIX_ID (T, m, n) //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_MATRIX_ID(T, m, n) MatrixId<m, n>::template Id<T> ()

    template <unsigned int m, unsigned int n>
    struct MatrixId
    {
      template <class T>
      inline static
      V<V<T, m>, n> Id ()
      {
        return V<V<T, m>, n> (AA_MATRIX_ID (T, m, n-1), AA_VECTOR_ID (T, m, n-1));
      }
    };

    template <unsigned int m>
    struct MatrixId<m, 1>
    {
      template <class T>
      inline static
      V<V<T, m>, 1> Id ()
      {
        return V<V<T, m>, 1> (AA_VECTOR_ID (T, m, 0));
      }
    };
  }

  template <class T, unsigned int m, unsigned int n>
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

      template <class U, unsigned int p, unsigned int q>
      inline
      M (const M<U, p, q> & u) :
        Parent (Id ())
      {
        const unsigned int cols = (n < q ? n : q);
        const unsigned int rows = (m < p ? m : p);

        for (unsigned int c = 0; c < cols; ++c)
          for (unsigned int r = 0; r < rows; ++r)
            (*this) [c][r] = static_cast<T> (u [c][r]);
      }

      inline explicit
      M (const T * data) :
        Parent ()
      {
        std::copy (data, data + m * n, &((*this)[0][0]));
      }

      inline
      Column col (unsigned int k) const
      {
        return (*this) [k];
      }

      inline
      Row row (unsigned int k) const
      {
        Row row;

        for (unsigned int c = 0; c < n; ++c)
          row [c] = (*this) [c][k];

        return row;
      }

      inline
      M<T, n, m> transpose () const
      {
        M<T, n, m> matrix;

        for (unsigned int r = 0; r < m; ++r)
          matrix [r] = this->row (r);

        return matrix;
      }
  };

////////////////////////////////////////////////////////////////////////////////
// Produit matriciel. //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, class U, unsigned int m, unsigned int n>
  AA_TOOLKIT_INLINE
  AA_VP(T,U,m) operator* (const M<T, m, n> & m1, const V<U, n> & v1)
  {
    AA_VP(T,U,m) v2;

    for (unsigned int i = 0; i < m; ++i)
      v2 [i] = DotProd (m1.row (i), v1);

    return v2;
  }

  template <class T, class U, unsigned int m, unsigned int n, unsigned int p>
  AA_TOOLKIT_INLINE
  AA_MP(T,U,m,n) operator* (const M<T, m, n> & m1, const M<U, n, p> & m2)
  {
    AA_MP(T,U,m,n) m3;

    for (unsigned int i = 0; i < p; ++i)
      m3 [i] = m1 * m2 [i];

    return m3;
  }

////////////////////////////////////////////////////////////////////////////////
// Raccourcis. /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, unsigned int m>
  AA_TOOLKIT_INLINE
  M<T, m, 1> mat (const V<T, m> & c1)
  {
    return M<T, m, 1> (vec (c1));
  }

  template <class T, unsigned int m>
  AA_TOOLKIT_INLINE
  M<T, m, 2> mat (const V<T, m> & c1, const V<T, m> & c2)
  {
    return M<T, m, 2> (vec (c1, c2));
  }

  template <class T, unsigned int m>
  AA_TOOLKIT_INLINE
  M<T, m, 3> mat (const V<T, m> & c1, const V<T, m> & c2, const V<T, m> & c3)
  {
    return M<T, m, 3> (vec (c1, c2, c3));
  }

  template <class T, unsigned int m>
  AA_TOOLKIT_INLINE
  M<T, m, 4> mat (const V<T, m> & c1, const V<T, m> & c2, const V<T, m> & c3, const V<T, m> & c4)
  {
    return M<T, m, 4> (vec (c1, c2, c3, c4));
  }

////////////////////////////////////////////////////////////////////////////////
// Transformations. ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  AA_TOOLKIT_INLINE
  M<T, 4, 4> Translation (const V<T, 3> & t)
  {
    return mat (vec<T> (1,    0,    0,    0),
                vec<T> (0,    1,    0,    0),
                vec<T> (0,    0,    1,    0),
                vec<T> (t[0], t[1], t[2], 1));
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M<T, 4, 4> Rotation (const T & angle, const V<T, 3> & axis)
    throw (div_by_zero)
  {
    V<T, 3> v = axis.normalize ();
    const T & x = v [0], y = v [1], z = v [2];
    T c = std::cos (angle), s = std::sin (angle);
    return mat (vec<T> (x*x*(1-c)+c,   x*y*(1-c)+z*s, x*z*(1-c)-y*s, 0),
                vec<T> (y*x*(1-c)-z*s, y*y*(1-c)+c,   y*z*(1-c)+x*s, 0),
                vec<T> (z*x*(1-c)+y*s, z*y*(1-c)-x*s, z*z*(1-c)+c,   0),
                vec<T> (0,             0,             0,             1));
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M<T, 4, 4> Scale (const V<T, 3> & s)
  {
    return mat (vec<T> (s[0], 0,    0,    0),
                vec<T> (0,    s[1], 0,    0),
                vec<T> (0,    0,    s[2], 0),
                vec<T> (0,    0,    0,    1));
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M<T, 4, 4> Scale (const T & t)
  {
    return Scale (V<T, 3> (t));
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M<T, 4, 4> Transform (const M<T, 3, 3> & r1, const V<T, 3> & t1,
                        const M<T, 3, 3> & r2, const V<T, 3> & t2)
  {
    return Translation (t2) * M<T, 4, 4> (r2) * M<T, 4, 4> (r1.transpose ()) * Translation (-t1);
  }

  template <class T>
  AA_TOOLKIT_INLINE
  M<T, 4, 4> Transform (const M<T, 4, 4> & m1, const M<T, 4, 4> & m2)
  {
    return Transform (M<T, 3, 3> (m1), (const V<T, 3> &) (m1 [3]),
                      M<T, 3, 3> (m2), (const V<T, 3> &) (m2 [3]));
  }

////////////////////////////////////////////////////////////////////////////////
// MatrixFunctor<T, k> /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, unsigned int k = 1>
  class MatrixFunctor
  {
    private:
      M<T, 4, 4> m_matrix;

    public:
      inline
      MatrixFunctor (const M<T, 4, 4> & matrix) :
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

  typedef M<float,  2, 2>  mat2;
  typedef M<float,  2, 3>  mat2x3;
  typedef M<float,  2, 4>  mat2x4;

  typedef M<float,  3, 2>  mat3x2;
  typedef M<float,  3, 3>  mat3;
  typedef M<float,  3, 4>  mat3x4;

  typedef M<float,  4, 2>  mat4x2;
  typedef M<float,  4, 3>  mat4x3;
  typedef M<float,  4, 4>  mat4;

  typedef M<double, 2, 2> dmat2;
  typedef M<double, 2, 3> dmat2x3;
  typedef M<double, 2, 4> dmat2x4;

  typedef M<double, 3, 2> dmat3x2;
  typedef M<double, 3, 3> dmat3;
  typedef M<double, 3, 4> dmat3x4;

  typedef M<double, 4, 2> dmat4x2;
  typedef M<double, 4, 3> dmat4x3;
  typedef M<double, 4, 4> dmat4;

  typedef M<int,    2, 2> imat2;
  typedef M<int,    2, 3> imat2x3;
  typedef M<int,    2, 4> imat2x4;

  typedef M<int,    3, 2> imat3x2;
  typedef M<int,    3, 3> imat3;
  typedef M<int,    3, 4> imat3x4;

  typedef M<int,    4, 2> imat4x2;
  typedef M<int,    4, 3> imat4x3;
  typedef M<int,    4, 4> imat4;

} // namespace Aa

#endif // __AA_MATRIX__

