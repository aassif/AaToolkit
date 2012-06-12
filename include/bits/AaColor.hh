#ifndef __AA_COLOR__
#define __AA_COLOR__

namespace Aa
{
////////////////////////////////////////////////////////////////////////////////
// ColorMath<T> ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  class ColorMath {};

  template <>
  class ColorMath<AaUInt8>
  {
    public:
      inline static AaUInt8 Min () {return 0x00;}
      inline static AaUInt8 Max () {return 0xFF;}
      inline static AaUInt8 Not (AaUInt8 u8) {return Max () ^ u8;}
  };

  template <>
  class ColorMath<AaUInt16>
  {
    public:
      inline static AaUInt16 Min () {return 0x0000;}
      inline static AaUInt16 Max () {return 0xFFFF;}
      inline static AaUInt16 Not (AaUInt16 u16) {return Max () ^ u16;}
  };

  template <>
  class ColorMath<AaUInt32>
  {
    public:
      inline static AaUInt32 Min () {return 0x00000000;}
      inline static AaUInt32 Max () {return 0xFFFFFFFF;}
      inline static AaUInt32 Not (AaUInt32 u32) {return Max () ^ u32;}
  };

  template <>
  class ColorMath<float>
  {
    public:
      inline static float Min () {return 0.0f;}
      inline static float Max () {return 1.0f;}
      inline static float Not (float f) {return Max () - f;}
  };

  template <>
  class ColorMath<double>
  {
    public:
      inline static double Min () {return 0.0;}
      inline static double Max () {return 1.0;}
      inline static double Not (double d) {return Max () - d;}
  };

////////////////////////////////////////////////////////////////////////////////
// AA_DEFINE_COLOR /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_DEFINE_COLOR(id, n)\
template <typename T>\
class id\
{\
  public:\
    enum {NumComponents = n};\
    typedef T Type;\
    typedef V<T, n> Pixel;\
};

  AA_DEFINE_COLOR (RGB,   3)
  AA_DEFINE_COLOR (RGBA,  4)
  AA_DEFINE_COLOR (HSV,   3)
  AA_DEFINE_COLOR (YCbCr, 3)
  AA_DEFINE_COLOR (CMY,   3)
  AA_DEFINE_COLOR (CMYK,  4)
  AA_DEFINE_COLOR (Mono,  1)

  typedef RGB<AaUInt8>    RGB8;
  //typedef RGB<AaUInt16>   RGB16;
  typedef RGB<float>      RGBf;
  typedef RGBA<AaUInt8>   RGBA8;
  //typedef RGBA<AaUInt16>  RGBA16;
  typedef RGBA<float>     RGBAf;
  typedef HSV<AaUInt8>    HSV8;
  typedef HSV<float>      HSVf;
  typedef CMY<AaUInt8>    CMY8;
  typedef CMYK<AaUInt8>   CMYK8;
  typedef YCbCr<AaUInt8>  YCbCr8;
  //typedef YCbCr<AaUInt16> YCbCr16;
  typedef Mono<AaUInt8>   Mono8;
  typedef Mono<AaUInt16>  Mono16;

////////////////////////////////////////////////////////////////////////////////
// RuntimeColor ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  typedef enum
  {
    RUNTIME_DEFAULT =  0,
    RUNTIME_MONO8   =  1,
    RUNTIME_MONO16  =  2,
    //RUNTIME_MONOf   =  3,
    //RUNTIME_MONOd   =  4,
    RUNTIME_RGB8    =  5,
    //RUNTIME_RGB16   =  6,
    //RUNTIME_RGBf    =  7,
    //RUNTIME_RGBd    =  8,
    RUNTIME_RGBA8   =  9,
    //RUNTIME_RGBA16  = 10,
    //RUNTIME_RGBAf   = 11,
    //RUNTIME_RGBAd   = 12,
    RUNTIME_YCbCr8  = 13,
    //RUNTIME_YCbCr16 = 14,
    //RUNTIME_YCbCrf  = 15,
    //RUNTIME_YCbCrd  = 16,
    RUNTIME_ERROR   = 17
  }
  RuntimeEnum;

  template <RuntimeEnum E> struct RuntimeColor;

#define AA_DEFINE_RUNTIME_COLOR(E, K)\
  template <> struct RuntimeColor<E> {typedef K ColorSpace;};

  AA_DEFINE_RUNTIME_COLOR (RUNTIME_MONO8,  Mono8)
  AA_DEFINE_RUNTIME_COLOR (RUNTIME_MONO16, Mono16)
  AA_DEFINE_RUNTIME_COLOR (RUNTIME_RGB8,   RGB8)
  AA_DEFINE_RUNTIME_COLOR (RUNTIME_RGBA8,  RGBA8)
  AA_DEFINE_RUNTIME_COLOR (RUNTIME_YCbCr8, YCbCr8)

#define AA_RUNTIME_COLOR(E) RuntimeColor<E>::ColorSpace
#define AA_RUNTIME_PIXEL(E) AA_RUNTIME_COLOR(E)::Pixel

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
      inline float offset1 (const SrcPixel &) const {return 2.0f;}
      inline float offset2 (const SrcPixel &) const {return 4.0f;}
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

        float m =     rgb [max];
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
      DstPixel operator() (const SrcPixel & hsv) const
      {
        float h = hsv [0];
        float s = hsv [1];
        float v = hsv [2];

        float i, f = std::modf (h * 6.0, &i);

        float l = v * (1.0f - s);
        float m = v * (1.0f - f * s);
        float n = v * (1.0f - (1.0f - f) * s);

        switch ((int) std::floor (i))
        {
          default:
          case  0: return vec (v, n, l);
          case  1: return vec (m, v, l);
          case  2: return vec (l, v, n);
          case  3: return vec (l, m, v);
          case  4: return vec (n, l, v);
          case  5: return vec (v, l, m);
        }
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
        T c = ColorMath<T>::Not (rgb [0]);
        T m = ColorMath<T>::Not (rgb [1]);
        T y = ColorMath<T>::Not (rgb [2]);
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
        T r = ColorMath<T>::Not (cmy [0]);
        T g = ColorMath<T>::Not (cmy [0]);
        T b = ColorMath<T>::Not (cmy [0]);
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
#if 0
      typedef typename RGB <T>::Pixel SrcPixel;
      typedef typename CMYK<T>::Pixel DstPixel;
#endif

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
#if 0
      typedef typename CMYK<T>::Pixel SrcPixel;
      typedef typename RGB <T>::Pixel DstPixel;
#endif

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
      static AaUInt8 Round (float f)
      {
        if (f <=   0.0) return   0;
        if (f >= 255.0) return 255;
        return static_cast<AaUInt8> (f + 0.5);
      }

    public:
      YCbCr8::Pixel operator() (const RGB8::Pixel & rgb) const
      {
        AaUInt8 R = rgb [0];
        AaUInt8 G = rgb [1];
        AaUInt8 B = rgb [2];
        // JPEG Y'CbCr.
        return vec (Round ( 0.299f  * R +  0.587f  * G +  0.114f  * B),
                    Round (-0.1687f * R + -0.3313f * G +  0.5f    * B + 128.0f),
                    Round ( 0.5f    * R + -0.4187f * G + -0.0813f * B + 128.0f));
      }
  };

  template <>
  class ColorConv<YCbCr8, RGB8>
  {
    private:
      static AaUInt8 Round (float f)
      {
        if (f <=   0.0) return   0;
        if (f >= 255.0) return 255;
        return static_cast<AaUInt8> (f + 0.5);
      }

    public:
      RGB8::Pixel operator() (const YCbCr8::Pixel & ycc) const
      {
        AaUInt8 Y  = ycc [0];
        float   Cb = ycc [1] - 128.0f;
        float   Cr = ycc [2] - 128.0f;
        return vec (Round (Y                  +  1.402f   * Cr),
                    Round (Y + -0.34414f * Cb + -0.71414f * Cr),
                    Round (Y +  1.772f   * Cb));
      }
  };
}

#endif // __AA_COLOR__

