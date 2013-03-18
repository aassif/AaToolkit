#ifndef AA_SIGNAL__H
#define AA_SIGNAL__H

namespace Aa
{
  template <AaUInt n, class T, AaUInt m = 1>
  class Signal : public Table<n, V<T, m>, ADB<V<T, m> > >
  {
    public:
      typedef T          Type;
      typedef V<T, m>    Pixel;
      typedef ADB<Pixel> Allocator;
      enum {NumComponents = m};

    public:
      Signal (const V<AaUInt, n> &);
#ifdef AA_SIGNAL_DEBUG
      Signal (const Signal & s) : Table<n, Pixel, Allocator> (s)
      {
        std::cout << this << " Signal<" << n << ">::Signal (s = " << &s << ")" << std::endl;
      }
      virtual ~Signal ()
      {
        std::cout << this << " Signal<" << n << ">::~Signal ()" << std::endl;
      }
      Signal & operator= (const Signal & s)
      {
        std::cout << this << " Signal<" << n << ">::op= (s = " << &s << ")" << std::endl;
        Table<n, Pixel, Allocator>::operator= (s);
        return *this;
      }
#endif
  };

  template <AaUInt n, class T, AaUInt m>
  Signal<n, T, m>::Signal (const V<AaUInt, n> & d) : Table<n, Pixel, Allocator> (d)
  {
#ifdef AA_SIGNAL_DEBUG
    std::cout << this << " Signal<" << n << ">::Signal (d = " << d << ")\n";
#endif
  }
}

#endif // AA_SIGNAL__H
