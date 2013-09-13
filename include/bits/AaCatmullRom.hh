#ifndef AA_CATMULL_ROM__H
#define AA_CATMULL_ROM__H

#include <vector>

namespace Aa
{

  inline
  std::vector<vec3> Extend (const std::vector<vec3> & p)
  {
    const AaUInt n = p.size ();
    if (n < 2) return p;

    std::vector<vec3> q;
    q.reserve (n + 2);
    q.push_back (2 * p[0] - p[1]);
    q.insert (q.end (), p.begin (), p.end ());
    q.push_back (2 * p[n-1] - p[n-2]);
    return q;
  }

  inline
  vec3 CatmullRom (const vec3 * p, float t)
  {
    static const mat4 M =
      mat (vec ( 0.0f, -0.5f, +1.0f, -0.5f),
           vec (+1.0f,  0.0f, -2.5f, +1.5f),
           vec ( 0.0f, +0.5f, +2.0f, -1.5f),
           vec ( 0.0f,  0.0f, -0.5f, +0.5f));

    vec4 u = vec (1.0f, t, t * t, t * t * t);

    vec4 m = M.transpose () * u;
    return m[0]*p[0] + m[1]*p[1] + m[2]*p[2] + m[3]*p[3];
  }

  inline
  std::vector<vec3> CatmullRom (const std::vector<vec3> & p, AaUInt k = 3)
  {
    const AaUInt n = p.size ();

    std::vector<vec3> points;
    points.push_back (p[1]);
    for (AaUInt i = 0; i < n-3; ++i)
    {
      for (AaUInt j = 1; j <= k; ++j)
        points.push_back (CatmullRom (&(p[i]), (float) j / (k+1)));
      points.push_back (p[i+2]);
    }

    return points;
  }

}

#endif // AA_CATMULL_ROM__H

