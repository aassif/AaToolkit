#ifndef AA_EIGEN_3x3__H
#define AA_EIGEN_3x3__H

extern "C"
{
  #include <dsyevc3.h> // Eigenvalues (Cardano)
  #include <dsyevh3.h> // Eigensystem (Hybrid)
}

namespace Aa
{
  namespace Eigen3x3
  {

    inline
    dvec3 Values (const dmat3 & m)
    {
      dvec3 values;
      double A [3][3] =
      {
        m[0][0], m[1][0], m[2][0],
        m[0][1], m[1][1], m[2][1],
        m[0][2], m[1][2], m[2][2]
      };
      int r = dsyevc3 (A, &(values[0]));
      return values;
    }

    class System
    {
      public:
        dvec3 values;
        dmat3 vectors;

      public:
        inline
        System (const dmat3 & m) :
          values (),
          vectors ()
        {
          double A [3][3] =
          {
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2]
          };

          double Q [3][3];
          int r = dsyevh3 (A, Q, &(values[0]));

          vectors[0] = vec (Q[0][0], Q[1][0], Q[2][0]);
          vectors[1] = vec (Q[0][1], Q[1][1], Q[2][1]);
          vectors[2] = vec (Q[0][2], Q[1][2], Q[2][2]);
        }
    };

  } // namespace Eigen3x3
} // namespace Aa

#endif // AA_EIGEN_3x3__H

