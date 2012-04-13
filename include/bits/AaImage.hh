#ifndef __AA_IMAGE__
#define __AA_IMAGE__

//#define AA_IMAGE_DEBUG

#include <vector>
//#include "PaniniExcept.hh"
//#include "PaniniSignal.hh"
//#include "PaniniColor.hh"

namespace Aa
{
  template <unsigned int n, class K>
  class Im : public Signal<n, typename K::Type, K::NumComponents>
  {
    public:
      typedef K ColorSpace;
      typedef typename K::Type  Type;
      typedef typename K::Pixel Pixel;
      enum {NumComponents = K::NumComponents};

    public:
      class FileFormat
      {
        public:
          // Name.
          virtual std::string name () const = 0;
          // Extensions.
          virtual std::string extensions () const = 0;
          // Test.
          virtual bool test (std::istream &) const = 0;
          // Chargement.
          virtual Im<n, K> read (std::istream &) const throw (ParseError) = 0;
          // Enregistrement.
          virtual void write (const Im<n, K> &, std::ostream &) const = 0;
      };

      class FileFormatManager
      {
        private:
          static std::vector<FileFormat *> m_formats;

        private:
          FileFormatManager ();

        public:
          // Chargement.
          static Im<n, K> Read (std::istream &) throw (ParseError);
          static Im<n, K> Read (const std::string &) throw (FileNotFound, ParseError);
          // Enregistrement.
          static void Write (const Im<n, K> &, std::ostream &) throw ();
          static void Write (const Im<n, K> &, const std::string &) throw (FileNotFound);
      };

    public:
      Im (const V<unsigned int, n> &);
#ifdef AA_IMAGE_DEBUG
      Im (const Im & im) : Signal<m, Type, NumComponents> (im)
      {
        std::cout << this << " Im<" << n << ">::Im (im = " << &im << ")" << std::endl;
      }
      virtual ~Im ()
      {
        std::cout << this << " Im::~Im ()" << std::endl;
      }
      Im & operator= (const Im & im)
      {
        std::cout << this << " Im::op= (im = " << &im << ")" << std::endl;
        Signal<n, Type, NumComponents>::operator= (im);
        return *this;
      }
#endif
  };

  template <unsigned int n, class K>
  Im<n, K>::Im (const V<unsigned int, n> & d) : Signal<n, Type, NumComponents> (d)
  {
#ifdef AA_IMAGE_DEBUG
    std::cout << this << ' '
              << "Im<" << n << ">::Im (" << "d = " << d << ")" << std::endl;
#endif
  }

  typedef Im<2, Mono8>  Im256;
  typedef Im<2, RGB8>   ImRGB;
  typedef Im<2, RGBA8>  ImRGBA;
  typedef Im<2, YCbCr8> ImYCbCr;

  template <unsigned int n, class K>
  class ImPalette : public Im<n, Mono8>
  {
    public:
      typedef Im<1, K> Palette;

    private:
      Palette m_palette;

    public:
      ImPalette (const V<unsigned int, n> & d) :
        Im<n, Mono8> (d),
        m_palette (256)
      {
      }
      ImPalette (const V<unsigned int, n> & d, const Palette & p) :
        Im<n, Mono8> (d),
        m_palette (p)
      {
      }
      const Palette & palette () const {return m_palette;}
      /***/ Palette & palette () /***/ {return m_palette;}
  };

  typedef ImPalette<2, RGB8>  ImPalRGB;
  typedef ImPalette<2, RGBA8> ImPalRGBA;
}

#endif // __AA_IMAGE__

