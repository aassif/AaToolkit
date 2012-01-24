#ifndef __AA_SIGNAL__
#define __AA_SIGNAL__

//#include "PaniniTable.hh"

namespace Aa
{
  template <unsigned int n, class T, unsigned int m = 1>
  class Signal : public Table<n, V<T, m>, ADB<V<T, m> > >
  {
    public:
      typedef V<T, m> Elem;

    public:
      Signal (const V<unsigned int, n> &);
#ifdef AA_SIGNAL_DEBUG
      Signal (const Signal & s) : Table<n, V<T, m>, ADB<V<T, m> > > (s)
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
        Table<n, V<T, m>, ADB<V<T, m> > >::operator= (s);
        return *this;
      }
#endif
  };

  template <unsigned int n, class T, unsigned int m>
  Signal<n, T, m>::Signal (const V<unsigned int, n> & d) : Table<n, V<T, m>, ADB<V<T, m> > > (d)
  {
#ifdef AA_SIGNAL_DEBUG
    std::cout << this << " Signal<" << n << ">::Signal (d = " << d << ")\n";
#endif
  }
}

#endif // __AA_SIGNAL__
