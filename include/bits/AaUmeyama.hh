#ifndef AA_UMEYAMA__H
#define AA_UMEYAMA__H

#include <vector>
#include <Eigen/Geometry>

namespace Aa
{

  inline static
  mat4 Umeyama (const std::vector<vec3> & src,
                const std::vector<vec3> & dst,
                bool scale = true)
  {
    typedef Eigen::Matrix<float, 3, Eigen::Dynamic> InputMatrix;
    typedef Eigen::Matrix4f                        OutputMatrix;

    InputMatrix eigen_src (3, src.size ());
    for (AaUInt i = 0; i < src.size (); ++i)
    {
      eigen_src (0, i) = src [i][0];
      eigen_src (1, i) = src [i][1];
      eigen_src (2, i) = src [i][2];
    }

    InputMatrix eigen_dst (3, dst.size ());
    for (AaUInt i = 0; i < dst.size (); ++i)
    {
      eigen_dst (0, i) = dst [i][0];
      eigen_dst (1, i) = dst [i][1];
      eigen_dst (2, i) = dst [i][2];
    }

    OutputMatrix m =
      Eigen::umeyama (eigen_src, eigen_dst, scale);

#if 0
    return mat (vec (m (0, 0), m (0, 1), m (0, 2), m (0, 3)),
                vec (m (1, 0), m (1, 1), m (1, 2), m (1, 3)),
                vec (m (2, 0), m (2, 1), m (2, 2), m (2, 3)),
                vec (m (3, 0), m (3, 1), m (3, 2), m (3, 3)));
#else
    return mat (vec (m (0, 0), m (1, 0), m (2, 0), m (3, 0)),
                vec (m (0, 1), m (1, 1), m (2, 1), m (3, 1)),
                vec (m (0, 2), m (1, 2), m (2, 2), m (3, 2)),
                vec (m (0, 3), m (1, 3), m (2, 3), m (3, 3)));
#endif
  }

} // namespace Aa

#endif // AA_UMEYAMA__H

