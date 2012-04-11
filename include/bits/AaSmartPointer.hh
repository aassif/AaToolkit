#ifndef __PANINI_SMARTPOINTER__
#define __PANINI_SMARTPOINTER__

#include <cstdlib>
#include <stdexcept>
#include <iostream>

namespace Aa
{
////////////////////////////////////////////////////////////////////////////////
// ADB : allocateur brut. //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  class ADB
  {
    public:
      static inline T * New (const T &) throw (std::bad_alloc);
      static inline T * Alloc (unsigned int n) throw (std::bad_alloc);
      static inline void Free (T *);
  };

  // Code.

  template <class T>
  T * ADB<T>::New (const T & t) throw (std::bad_alloc)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADB::New (" << t << ")\n";
#endif
    T * data = (T *) malloc (1 * sizeof (T));
    if (data == NULL) throw std::bad_alloc ();
    *data = t;
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "  > " << data << std::endl;
#endif
    return data;
  }

  template <class T>
  T * ADB<T>::Alloc (unsigned int n) throw (std::bad_alloc)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADB::Alloc (" << n << " * " << sizeof (T) << ")\n";
#endif
    T * data = (T *) malloc (n * sizeof (T));
    if (data == NULL) throw std::bad_alloc ();
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "  > " << data << std::endl;
#endif
    return data;
  }

  template <class T>
  void ADB<T>::Free (T * data)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADB::Free (" << data << ")\n";
#endif
    if (data != NULL) free (data);
  }

////////////////////////////////////////////////////////////////////////////////
// ADS : allocateur simple. ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  class ADS
  {
    public:
      static inline T * New (const T &) throw (std::bad_alloc);
      static inline T * Alloc (unsigned int n) throw (std::bad_alloc);
      static inline void Free (T *);
  };

  // Code.

  template <class T>
  T * ADS<T>::New (const T & t) throw (std::bad_alloc)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADS::New (" << t << ")\n";
#endif
    T * data = new T (t);
    if (data == NULL) throw std::bad_alloc (); // nothrow?
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "  > " << data << std::endl;
#endif
    return data;
  }

  template <class T>
  T * ADS<T>::Alloc (unsigned int n) throw (std::bad_alloc)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADS::Alloc (" << n << " * " << sizeof (T) << ")\n";
#endif
    throw std::bad_alloc ();
    return NULL;
  }

  template <class T>
  void ADS<T>::Free (T * data)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADS::Free (" << data << ")\n";
#endif
    if (data != NULL) delete data;
  }

////////////////////////////////////////////////////////////////////////////////
// ADM : allocateur multiple. //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T>
  class ADM
  {
    public:
      static inline T * New (const T &) throw (std::bad_alloc);
      static inline T * Alloc (unsigned int n) throw (std::bad_alloc);
      static inline void Free (T *);
  };

  // Code.

  template <class T>
  T * ADM<T>::New (const T & t) throw (std::bad_alloc)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADM::New (" << t << ")\n";
#endif
    T * data = new T [1];
    if (data == NULL) throw std::bad_alloc (); // nothrow?
    *data = t;
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "  > " << data << std::endl;
#endif
    return data;
  }

  template <class T>
  T * ADM<T>::Alloc (unsigned int n) throw (std::bad_alloc)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADM::Alloc (" << n << " * " << sizeof (T) << ")\n";
#endif
    T * data = new T [n];
    if (data == NULL) throw std::bad_alloc (); // nothrow?
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "  > " << data << std::endl;
#endif
    return data;
  }

  template <class T>
  void ADM<T>::Free (T * data)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << "ADM::Free (" << data << ")\n";
#endif
    if (data != NULL) delete[] data;
  }

////////////////////////////////////////////////////////////////////////////////
// SPD<T, A> : pointeur dupliqué. //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if 0
  template <class T, class A = ADS<T> >
  class SPD
  {
    private:
      //T * m_data;
  };
#endif

////////////////////////////////////////////////////////////////////////////////
// SmartPointer<T, A> : pointeur partagé. ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  template <class T, class A = ADS<T> >
  class SmartPointer
  {
    private:
      T   * m_data;
      int * m_refs;

    protected:
      // Gestion de la connexion.
      void connect (T *);
      void connect (const SmartPointer &);
      void disconnect ();
      // Accès au pointeur.
      inline       T * data ()       {return m_data;}
      inline const T * data () const {return m_data;}

    public:
      SmartPointer ();
      SmartPointer (const T &);
      SmartPointer (const SmartPointer &);
      virtual ~SmartPointer ();
      SmartPointer & operator= (const SmartPointer &);
      // Validité du pointeur?
      inline operator bool () const {return m_data != NULL;}
      // Opérateurs d'accès aux données.
      inline T & operator*  () const {return *m_data;}
      inline T * operator-> () const {return  m_data;}
  };

  // Code.

  template <class T, class A>
  void SmartPointer<T, A>::connect (T * data)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << this << " SmartPointer::connect (data = " << data << ')' << std::endl;
#endif

    if (data != NULL)
    {
      m_data = data;
      m_refs = ADS<int>::New (1);
    }
    else
    {
      m_data = NULL;
      m_refs = NULL;
    }
  }

  template <class T, class A>
  void SmartPointer<T, A>::connect (const SmartPointer<T, A> & sp)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << this << " SmartPointer::connect (sp = " << &sp << ')' << std::endl;
#endif

    if (sp.m_data != NULL)
    {
      m_data = sp.m_data;
      m_refs = sp.m_refs; ++(*m_refs);
#ifdef AA_SMARTPOINTER_DEBUG
      std::cout << "  > refs = " << *m_refs << std::endl;
#endif
    }
    else
    {
      m_data = NULL;
      m_refs = NULL;
    }
  }

  template <class T, class A>
  void SmartPointer<T, A>::disconnect ()
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << this << " SmartPointer::disconnect () " << std::endl;
#endif

    if (m_data != NULL)
    {
      if (--(*m_refs) == 0)
      {
        A::Free (m_data);
        ADS<int>::Free (m_refs);
      }
      m_data = NULL;
      m_refs = NULL;
    }
  }

  template <class T, class A>
  SmartPointer<T, A>::SmartPointer () :
    m_data (NULL), m_refs (NULL)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << this << " SmartPointer::SmartPointer ()\n";
#endif
  }

  template <class T, class A>
  SmartPointer<T, A>::SmartPointer (const T & t) :
    m_data (NULL), m_refs (NULL)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << this << " SmartPointer::SmartPointer (t = " << t << ")\n";
#endif
    //disconnect (); // inutile.
    connect (A::New (t));
  }

  template <class T, class A>
  SmartPointer<T, A>::SmartPointer (const SmartPointer<T, A> & sp) :
    m_data (NULL), m_refs (NULL)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << this << " SmartPointer::SmartPointer (sp = " << &sp << ")\n";
#endif
    //disconnect (); // inutile.
    connect (sp);
  }

  template <class T, class A>
  SmartPointer<T, A>::~SmartPointer ()
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << this << " SmartPointer::~SmartPointer ()\n";
#endif
    disconnect ();
  }

  template <class T, class A>
  SmartPointer<T, A> & SmartPointer<T, A>::operator= (const SmartPointer<T, A> & sp)
  {
#ifdef AA_SMARTPOINTER_DEBUG
    std::cout << this << " SmartPointer::op= (" << &sp << ")\n";
#endif
    if (sp.m_data != m_data)
    {
      disconnect ();
      connect (sp);
    }
    return *this;
  }
}

#endif // __PANINI_SMARTPOINTER__

