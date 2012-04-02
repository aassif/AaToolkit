#ifndef __AA_BUFFER__
#define __AA_BUFFER__

#include <sstream>

namespace Aa
{
  namespace details
  {
    inline static
    void CheckRange (unsigned int i,
                     unsigned int d) throw (std::out_of_range)
    {
#ifdef AA_BUFFER_DEBUG
      std::cout << "CheckRange (i = " << i << ", d = " << d << ")\n";
#endif
      if (i >= d)
      {
        std::ostringstream oss;
        oss << "CheckRange: " << i << " n'appartient pas à l'intervalle [0, " << (d-1) << ']';
        throw std::out_of_range (oss.str ());
      }
    }
  }

  template <class T, class A = ADM<T> >
  class Buffer : protected SmartPointer<T, A>
  {
    protected:
      unsigned int m_size;

    public:
      Buffer (unsigned int n = 0) throw (std::bad_alloc);
      // Gestion du buffer.
      void alloc (unsigned int n = 0);
      void disconnect ();
      // Validité du pointeur?
      inline operator bool () const {return SmartPointer<T, A>::operator bool ();}
      // Dimensions.
      inline unsigned int size () const {return m_size;}
      // Pseudo-itérateurs.
      inline /***/ T * begin ();
      inline const T * begin () const;
      inline /***/ T * end   ();
      inline const T * end   () const;
      // Accès à un élément.
      inline /***/ T & operator[] (unsigned int) /***/ throw (std::out_of_range);
      inline const T & operator[] (unsigned int) const throw (std::out_of_range);
  };

  template <class T, class A>
  Buffer<T, A>::Buffer (unsigned int n) throw (std::bad_alloc) :
    SmartPointer<T, A> (),
    m_size (0)
  {
#ifdef AA_BUFFER_DEBUG
    std::cout << this << " Buffer::Buffer (n = " << n << ")\n";
#endif
    alloc (n);
  }

  template <class T, class A>
  void Buffer<T, A>::alloc (unsigned int n)
  {
#ifdef AA_BUFFER_DEBUG
    std::cout << this << " Buffer::alloc (n = " << n << ")\n";
#endif
    SmartPointer<T, A>::disconnect ();
    SmartPointer<T, A>::connect (n != 0 ? A::Alloc (n) : NULL);
    m_size = n;
  }

  template <class T, class A>
  void Buffer<T, A>::disconnect ()
  {
#ifdef AA_BUFFER_DEBUG
    std::cout << this << " Buffer::disconnect ()\n";
#endif
    SmartPointer<T, A>::disconnect ();
    m_size = 0;
  }

  //------------------------------------------------------------------------------
  // Pseudo-itérateurs.
  //------------------------------------------------------------------------------

  template <class T, class A>
  T * Buffer<T, A>::begin ()
  {
    return SmartPointer<T, A>::data ();
  }

  template <class T, class A>
  const T * Buffer<T, A>::begin () const
  {
    return SmartPointer<T, A>::data ();
  }

  template <class T, class A>
  T * Buffer<T, A>::end ()
  {
    return begin () + m_size;
  }

  template <class T, class A>
  const T * Buffer<T, A>::end () const
  {
    return begin () + m_size;
  }

////////////////////////////////////////////////////////////////////////////////
// Accès à un élément. /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, class A>
  T & Buffer<T, A>::operator[] (unsigned int i)
    throw (std::out_of_range)
  {
#ifdef AA_BUFFER_DEBUG
    std::cout << this << " Buffer::operator[] (i = " << i << ")\n";
#endif
    details::CheckRange (i, m_size);
    return this->begin () [i];
  }

  template <class T, class A>
  const T & Buffer<T, A>::operator[] (unsigned int i) const
    throw (std::out_of_range)
  {
#ifdef AA_BUFFER_DEBUG
    std::cout << this << " Buffer::operator[] (i = " << i << ")\n";
#endif
    details::CheckRange (i, m_size);
    return this->begin () [i];
  }
}

#endif // __AA_BUFFER__

