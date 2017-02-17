#ifndef AA_CAMERA__H
#define AA_CAMERA__H

namespace Aa
{
  class Camera
  {
    public:
      enum View
      {
        LEFT   = 1,
        RIGHT  = 2,
        BOTTOM = 3,
        TOP    = 4,
        BACK   = 5,
        FRONT  = 6
      };

      inline
      static quat View (enum View view)
      {
#if 0
        static const vec3 X = vec<float> (1.0f, 0.0f, 0.0f);
        static const vec3 Y = vec<float> (0.0f, 1.0f, 0.0f);
        static const vec3 Z = vec<float> (0.0f, 0.0f, 1.0f);

        static const quat LEFT   = mat3 (Rotation<float> (M_PI/2,  Y));
        static const quat RIGHT  = mat3 (Rotation<float> (M_PI/2, -Y));

        static const quat BOTTOM = mat3 (Rotation<float> (M_PI/2, -X));
        static const quat TOP    = mat3 (Rotation<float> (M_PI/2,  X));

        static const quat BACK   = mat3 (Rotation<float> (M_PI,    Y));
        static const quat FRONT  = mat3 ();
#endif

        switch (view)
        {
          case LEFT:   return vec<float> (0, +1, 0, 1);
          case RIGHT:  return vec<float> (0, -1, 0, 1);

          case BOTTOM: return vec<float> (-1, 0, 0, 1);
          case TOP:    return vec<float> (+1, 0, 0, 1);

          case BACK:   return vec<float> (0, 1, 0, 0);
          case FRONT:  return vec<float> (0, 0, 0, 1);
        }

        return quat ();
      }

    protected:
      uvec2   m_size;
      mat3    m_camera_orientation;
      vec3    m_camera_position;
      float   m_camera_distance;
      vec3    m_scene_center;
      float   m_scene_radius;
      Arcball m_arcball;
      mat3    m_arcball_orientation;
      vec3    m_arcball_target;

    public:
      inline
      Camera () :
        m_size                (0u, 0u),
        m_camera_orientation  (),
        m_camera_position     (vec<float> (0,  0, +1)),
        m_camera_distance     (1.0f),
        m_scene_center        (vec<float> (0,  0,  0)),
        m_scene_radius        (1.0f),
        m_arcball             (),
        m_arcball_orientation (),
        m_arcball_target      ()
      {
      }

      inline
      void set_scene_sphere (const vec3 & c, float r)
      {
        m_scene_center = c;
        m_scene_radius = r;
      }

      inline
      void set_scene_box (const vec3 & min, const vec3 & max)
      {
        vec3 d = 0.5f * (max - min);
        this->set_scene_sphere (min + d, d.length ());
      }

      inline
      void set_orientation (const quat & q)
      {
        m_camera_orientation = q.matrix ();
      }

      inline
      void set_orientation (enum View view)
      {
        m_camera_orientation = View (view).matrix ();
      }

      inline
      void fit_scene ()
      {
        m_camera_distance = 2.0f * m_scene_radius;
        m_camera_position = m_scene_center + m_camera_distance * m_camera_orientation[2];
      }

      inline
      void zoom (float f)
      {
        vec3 target = this->target ();
        m_camera_distance *= f;
        m_camera_position = target + m_camera_distance * m_camera_orientation[2];
      }

      inline
      vec3 target () const
      {
        return m_camera_position - m_camera_distance * m_camera_orientation [2];
      }

      inline
      vec2 range () const
      {
        float r = m_scene_radius;
        float d = m_camera_distance;
        return vec2 (d > r ? d - r : 0.01f * r, d + r);
      }

      inline
      mat4 modelview () const
      {
        mat3 m = m_camera_orientation.transpose ();
        return mat4x3 (m, m * -m_camera_position);
      }

      virtual mat4 projection () const = 0;

      inline
      void resize (const uvec2 & d)
      {
        m_size = d;
        m_arcball.resize (d);
      }

      inline
      void click (const vec2 & p)
      {
        m_arcball.init (p);
        m_arcball_target      = this->target ();
        m_arcball_orientation = m_camera_orientation;
      }

      inline
      void move (const vec2 & p)
      {
        quat q = m_arcball (p);
        m_camera_orientation = m_arcball_orientation * q.matrix ();
        m_camera_position    = m_arcball_target + m_camera_distance * m_camera_orientation [2];
      }
  };

  class OrthoCamera : public Camera
  {
    public:
      OrthoCamera () :
        Camera ()
      {
      }

      virtual mat4 projection () const
      {
        float w = m_camera_distance * m_size [0] / m_size [1];
        float h = m_camera_distance * 1.0f;
        vec2  r = this->range ();
        return Ortho (-0.5f * w, +0.5f * w, -0.5f * h, +0.5f * h, r [0], r [1]);
      }
  };

  class PerspectiveCamera : public Camera
  {
    private:
      float m_vertical_fov;

    public:
      PerspectiveCamera () :
        Camera (),
        m_vertical_fov (45.0f * M_PI / 180.0f)
      {
      }

      virtual Aa::mat4 projection () const
      {
        int  w = m_size [0];
        int  h = m_size [1];
        vec2 r = this->range  ();
        return Perspective (m_vertical_fov, (float) w / h, r [0], r [1]);
      }
  };
}

#endif // AA_CAMERA__H

