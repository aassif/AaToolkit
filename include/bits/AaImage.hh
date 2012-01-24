#ifndef __AA_IMAGE__
#define __AA_IMAGE__

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
          std::vector<FileFormat *> m_formats;

        private:
          FileFormatManager ();

        public:
          // Chargement.
          static Im<n, K> Read (std::istream &) throw (ParseError);
          static Im<n, K> Read (const std::string &) throw (FileNotFound, ParseError);
          // Enregistrement.
          void Write (const Im<n, K> &, std::ostream &) throw ();
          void Write (const Im<n, K> &, const std::string &) throw (FileNotFound);
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
      const Pixel & operator() (unsigned int x, unsigned int y) const;
      /***/ Pixel & operator() (unsigned int x, unsigned int y) /***/;
  };

  template <unsigned int n, class K>
  Im<n, K>::Im (const V<unsigned int, n> & d) : Signal<n, Type, NumComponents> (d)
  {
#ifdef AA_IMAGE_DEBUG
    std::cout << this << ' '
              << "Im<" << n << ">::Im (" << "d = " << d << ")" << std::endl;
#endif
  }

  template <unsigned int n, class K>
  const typename Im<n, K>::Pixel & Im<n, K>::operator() (unsigned int x, unsigned int y) const
  {
    return this->operator[] (vec (x, y));
  }

  template <unsigned int n, class K>
  typename Im<n, K>::Pixel & Im<n, K>::operator() (unsigned int x, unsigned int y)
  {
    return this->operator[] (vec (x, y));
  }

  typedef Im<2, Mono8> Im256;
  typedef Im<2, RGB8> ImRGB;
  typedef Im<2, RGBA8> ImRGBA;
  typedef Im<2, YCbCr8> ImYCbCr;

  template <class K>
  class ImPalette : public Im256
  {
    public:
      typedef Im<1, K> Palette;

    private:
      Palette m_palette;

    public:
      ImPalette (unsigned int w, unsigned int h) :
        Im256 (vec (w, h)),
        m_palette (256)
      {
      }
      ImPalette (unsigned int w, unsigned int h, const Palette & p) :
        Im256 (vec (w, h)),
        m_palette (p)
      {
      }
      const Palette & palette () const {return m_palette;}
      /***/ Palette & palette () /***/ {return m_palette;}
  };

  typedef ImPalette<RGB8> ImPalRGB;
  typedef ImPalette<RGBA8> ImPalRGBA;
}

#endif // __AA_IMAGE__

