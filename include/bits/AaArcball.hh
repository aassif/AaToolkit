#ifndef AA_ARCBALL__H
#define AA_ARCBALL__H

namespace Aa
{

////////////////////////////////////////////////////////////////////////////////
// Aa::Arcball /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  class Arcball
  {
    private:
      vec2  m_center;
      float m_radius;
      vec3  m_position;

    private:
      vec3 map (const vec2 & point) const
      {
        vec2 p = vec2 (+1.0f, -1.0f) * (point - m_center) / m_radius;
        float d2 = p.length2 ();
        return (d2 > 1.0f) ? vec3 (p / std::sqrt (d2), 0.0f) : vec3 (p, std::sqrt (1.0f - d2));
      }

    public:
      Arcball () :
        m_center (),
        m_radius (0.0f),
        m_position ({0, 0, 1})
      {
      }

      void resize (const uvec2 & d)
      {
        m_center   = 0.5f * d;
        m_radius   = 0.5f * std::max (d[0], d[1]);
        //m_position = {0, 0, 1};
      }

      void init (const vec2 & p)
      {
        m_position = this->map (p);
      }

      quat operator() (const vec2 & p) const
      {
        vec3 p1 = m_position;
        vec3 p2 = this->map (p);
        return vec4 (CrossProd (p1, p2), 0.5f * DotProd (p1, p2));
      }
  };

} // namespace Aa

#endif // AA_ARCBALL__H

