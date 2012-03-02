#ifndef __AA_COLOR__
#define __AA_COLOR__

//#include <PaniniVector.hh>

namespace Aa
{
  typedef unsigned char  uint8;
  typedef unsigned short uint16;
  typedef unsigned long  uint32;

////////////////////////////////////////////////////////////////////////////////
// ColorType<T> ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  class ColorType {};

  template <>
  class ColorType<uint8>
  {
    public:
      inline static uint8 Min () {return 0x00;}
      inline static uint8 Max () {return 0xFF;}
      inline static uint8 Bar (uint8 u8) {return Max () ^ u8;}
  };

  template <>
  class ColorType<uint16>
  {
    public:
      inline static uint16 Min () {return 0x0000;}
      inline static uint16 Max () {return 0xFFFF;}
      inline static uint16 Bar (uint16 u16) {return Max () ^ u16;}
  };

  template <>
  class ColorType<uint32>
  {
    public:
      inline static uint32 Min () {return 0x00000000;}
      inline static uint32 Max () {return 0xFFFFFFFF;}
      inline static uint32 Bar (uint32 u32) {return Max () ^ u32;}
  };

  template <>
  class ColorType<float>
  {
    public:
      inline static float Min () {return 0.0f;}
      inline static float Max () {return 1.0f;}
      inline static float Bar (float f) {return Max () - f;}
  };

  template <>
  class ColorType<double>
  {
    public:
      inline static double Min () {return 0.0;}
      inline static double Max () {return 1.0;}
      inline static double Bar (double d) {return Max () - d;}
  };

////////////////////////////////////////////////////////////////////////////////
// AA_DECLARE_COLOR ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_DECLARE_COLOR(id, n)\
template <typename T>\
class id\
{\
  public:\
    enum {NumComponents = n};\
    typedef T Type;\
    typedef V<T, n> Pixel;\
};

  AA_DECLARE_COLOR (RGB,   3);
  AA_DECLARE_COLOR (RGBA,  4);
  AA_DECLARE_COLOR (HSV,   3);
  AA_DECLARE_COLOR (YCbCr, 3);
  AA_DECLARE_COLOR (CMY,   3);
  AA_DECLARE_COLOR (CMYK,  4);
  AA_DECLARE_COLOR (Mono,  1);

  typedef RGB<uint8>   RGB8;
  typedef RGB<float>   RGBf;
  typedef RGBA<uint8>  RGBA8;
  typedef RGBA<float>  RGBAf;
  typedef HSV<uint8>   HSV8;
  typedef HSV<float>   HSVf;
  typedef CMY<uint8>   CMY8;
  typedef CMYK<uint8>  CMYK8;
  typedef YCbCr<uint8> YCbCr8;
  typedef Mono<uint8>  Mono8;

////////////////////////////////////////////////////////////////////////////////
// ColorConv ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class K1, class K2>
  class ColorConv
  {
    public:
      typedef typename K1::Pixel SrcPixel;
      typedef typename K2::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel &) const;
  };

  // Conversion via un espace intermédiaire.
  template <class K1, class K2, class K3>
  class ColorConv3
  {
    public:
      typedef typename K1::Pixel SrcPixel;
      typedef typename K3::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & src) const
      {
        ColorConv <K1, K2> c1;
        ColorConv <K2, K3> c2;
        return c2 (c1 (src));
      }
  };

////////////////////////////////////////////////////////////////////////////////
// RGB & RGBA //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  class ColorConv < RGB<T>, RGBA<T> >
  {
    public:
      typedef typename RGB <T>::Pixel SrcPixel;
      typedef typename RGBA<T>::Pixel DstPixel;

    private:
      T m_alpha;

    public:
      ColorConv (const T & alpha = T ()) : m_alpha (alpha) {}
      DstPixel operator() (const SrcPixel & rgb) const
      {
        return DstPixel (rgb, m_alpha);
      }
  };

  template <typename T>
  class ColorConv < RGBA<T>, RGB<T> >
  {
    public:
      typedef typename RGBA<T>::Pixel SrcPixel;
      typedef typename RGB <T>::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & rgba) const
      {
        return rgba;
      }
  };

////////////////////////////////////////////////////////////////////////////////
// RGB & HSV (float) ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <>
  class ColorConv<RGBf, HSVf>
  {
    public:
      typedef RGBf::Pixel SrcPixel;
      typedef HSVf::Pixel DstPixel;

    private:
      inline float offset0 (const SrcPixel & rgb) const
      {
        return (rgb [1] > rgb [2] ? 0.0f : 6.0f);
      }
      inline float offset1 (const SrcPixel & rgb) const {return 2.0f;}
      inline float offset2 (const SrcPixel & rgb) const {return 4.0f;}
      inline float hue (const SrcPixel & rgb, unsigned int m, float d) const
      {
        switch (m)
        {
          case 0:  return offset0 (rgb) + (rgb [1] - rgb [2]) / d;
          case 1:  return offset1 (rgb) + (rgb [2] - rgb [0]) / d;
          case 2:  return offset2 (rgb) + (rgb [0] - rgb [1]) / d;
          default: return 0.0f;
        }
      }

    public:
      DstPixel operator() (const SrcPixel & rgb) const
      {
        unsigned int min = rgb.min ();
        unsigned int max = rgb.max ();

        float m = rgb [max];
        float d = m - rgb [min];

        if (d > 0.0f) return vec (hue (rgb, max, d) / 6.0f, d / m, m);
        else          return vec (0.0f,                      0.0f, m);
      }
  };

  template <>
  class ColorConv<HSVf, RGBf>
  {
    public:
      typedef HSVf::Pixel SrcPixel;
      typedef RGBf::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & cmy) const
      {
        return vec (0.0f, 0.0f, 0.0f); // TODO
      }
  };

////////////////////////////////////////////////////////////////////////////////
// RGB & CMY ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  class ColorConv < RGB<T>, CMY<T> >
  {
    public:
      typedef typename RGB<T>::Pixel SrcPixel;
      typedef typename CMY<T>::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & rgb) const
      {
        T c = ColorType<T>::Bar (rgb [0]);
        T m = ColorType<T>::Bar (rgb [1]);
        T y = ColorType<T>::Bar (rgb [2]);
        return vec<T> (c, m, y);
      }
  };

  template <typename T>
  class ColorConv < CMY<T>, RGB<T> >
  {
    public:
      typedef typename CMY<T>::Pixel SrcPixel;
      typedef typename RGB<T>::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & cmy) const
      {
        T r = ColorType<T>::Bar (cmy [0]);
        T g = ColorType<T>::Bar (cmy [0]);
        T b = ColorType<T>::Bar (cmy [0]);
        return vec<T> (r, g, b);
      }
  };

////////////////////////////////////////////////////////////////////////////////
// CMY & CMYK //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  class ColorConv < CMY<T>, CMYK<T> >
  {
    public:
      typedef typename CMY <T>::Pixel SrcPixel;
      typedef typename CMYK<T>::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & cmy) const
      {
        const T & c = cmy [0]; T          k = c;
        const T & m = cmy [1]; if (m < k) k = m;
        const T & y = cmy [2]; if (y < k) k = y;
        return vec<T> (c - k, m - k, y - k, k); // FIXME
      }
  };

  template <typename T>
  class ColorConv < CMYK<T>, CMY<T> >
  {
    public:
      typedef typename CMYK<T>::Pixel SrcPixel;
      typedef typename CMY <T>::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & cmyk) const
      {
        const T & c = cmyk [0];
        const T & m = cmyk [1];
        const T & y = cmyk [2];
        const T & k = cmyk [3];
        return vec<T> (c + k, m + k, y + k); // FIXME
      }
  };

////////////////////////////////////////////////////////////////////////////////
// RGB & CMYK //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  class ColorConv < RGB<T>, CMYK<T> > :
    public ColorConv3 < RGB<T>, CMY<T>, CMYK<T> >
  {
    public:
      typedef typename RGB <T>::Pixel SrcPixel;
      typedef typename CMYK<T>::Pixel DstPixel;

    public:
#if 0
      DstPixel operator() (const SrcPixel & rgb) const
      {
        ColorConv < RGB<T>, CMY <T> > c1;
        ColorConv < CMY<T>, CMYK<T> > c2;
        return c2 (c1 (rgb));
      }
#endif
  };

  template <typename T>
  class ColorConv < CMYK<T>, RGB<T> > :
    public ColorConv3 < CMYK<T>, CMY<T>, RGB<T> >
  {
    public:
      typedef typename CMYK<T>::Pixel SrcPixel;
      typedef typename RGB <T>::Pixel DstPixel;

    public:
#if 0
      DstPixel operator() (const SrcPixel & cmyk) const
      {
        ColorConv < CMYK<T>, CMY<T> > c1;
        ColorConv < CMY <T>, RGB<T> > c2;
        return c2 (c1 (cmyk));
      }
#endif
  };

////////////////////////////////////////////////////////////////////////////////
// RGB & Mono /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  class ColorConv < RGB<T>, Mono<T> >
  {
    public:
      typedef typename RGB <T>::Pixel SrcPixel;
      typedef typename Mono<T>::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & rgb) const
      {
        const T & R = rgb [0];
        const T & G = rgb [1];
        const T & B = rgb [2];
        // CCIR 601.
        return static_cast<T> (0.299 * R + 0.587 * G + 0.114 * B);
      }
  };

  template <typename T>
  class ColorConv < Mono<T>, RGB<T> >
  {
    public:
      typedef typename Mono<T>::Pixel SrcPixel;
      typedef typename RGB <T>::Pixel DstPixel;

    public:
      DstPixel operator() (const SrcPixel & mono) const
      {
        return vec (mono, mono, mono);
      }
  };

////////////////////////////////////////////////////////////////////////////////
// RGB & YCbCr (8 bits) ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <>
  class ColorConv<RGB8, YCbCr8>
  {
    private:
      static uint8 Round (float f)
      {
        if (f <=   0.0) return   0;
        if (f >= 255.0) return 255;
        return static_cast<uint8> (f + 0.5);
      }

    public:
      YCbCr8::Pixel operator() (const RGB8::Pixel & rgb) const
      {
        uint8 R = rgb [0];
        uint8 G = rgb [1];
        uint8 B = rgb [2];
        // JPEG Y'CbCr.
        return vec (Round ( 0.299  * R +  0.587  * G +  0.114  * B),
                    Round (-0.1687 * R + -0.3313 * G +  0.5    * B + 128.0),
                    Round ( 0.5    * R + -0.4187 * G + -0.0813 * B + 128.0));
      }
  };

  template <>
  class ColorConv<YCbCr8, RGB8>
  {
    private:
      static uint8 Round (float f)
      {
        if (f <=   0.0) return   0;
        if (f >= 255.0) return 255;
        return static_cast<uint8> (f + 0.5);
      }

    public:
      RGB8::Pixel operator() (const YCbCr8::Pixel & ycc) const
      {
        uint8 Y  = ycc [0];
        float Cb = ycc [1] - 128.0;
        float Cr = ycc [2] - 128.0;
        return vec (Round (Y                 +  1.402   * Cr),
                    Round (Y + -0.34414 * Cb + -0.71414 * Cr),
                    Round (Y +  1.772   * Cb));
      }
  };
}

#endif // __AA_COLOR__

