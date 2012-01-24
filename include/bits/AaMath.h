#ifndef __AA_MATH__
#define __AA_MATH__

#include <iostream>
#include <stdexcept>

namespace Aa
{
  namespace Math
  {
#if 0
    template <class X>
    inline const X & MIN (const X & x1, const X & x2) {return (x1 < x2 ? x1 : x2);}

    template <class X>
    inline const X & MAX (const X & x1, const X & x2) {return (x1 < x2 ? x2 : x1);}
#endif

    class pR3;
    class vR3;

    class AA_TOOLKIT_API div_by_zero : public std::runtime_error
    {
      public:
        div_by_zero ();
        virtual ~div_by_zero () throw ();
    };

////////////////////////////////////////////////////////////////////////////////
// pR3 : 3d point //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class AA_TOOLKIT_API pR3
    {
      public:
        double x, y, z;

      public:
        static pR3 Barycenter (const pR3 & p1,            const pR3 & p2)            throw ();
        static pR3 Barycenter (const pR3 & p1, double w1, const pR3 & p2, double w2) throw (div_by_zero);

        static pR3 Barycenter (const pR3 & p1,            const pR3 & p2,            const pR3 & p3)            throw ();
        static pR3 Barycenter (const pR3 & p1, double w1, const pR3 & p2, double w2, const pR3 & p3, double w3) throw (div_by_zero);

      public:
        pR3 ();
        pR3 (double, double, double);
        // default copy ctor.
        // default dtor.
      
        pR3 & operator+= (const vR3 &);
        pR3 & operator*= (double);
    };
    
    pR3 operator+ (const pR3 &, const vR3 &);
    pR3 operator+ (const vR3 &, const pR3 &);
    //pR3 operator+ (const pR3 &, const pR3 &); // ugly!
    
    vR3 operator- (const pR3 &, const pR3 &);
    
    pR3 operator* (const pR3 &, double);
    pR3 operator* (double, const pR3 &);
    
    pR3 operator/ (const pR3 &, double) throw (div_by_zero);
    
    bool operator== (const pR3 &, const pR3 &);
    bool operator!= (const pR3 &, const pR3 &);
    
    std::istream & operator>> (std::istream &, pR3 &);
    std::ostream & operator<< (std::ostream &, const pR3 &);

////////////////////////////////////////////////////////////////////////////////
// BaryHelper //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class AA_TOOLKIT_API BaryHelper
    {
      private:
        pR3 m_point;
        double m_weight;

      public:
        BaryHelper ();
        void reset ();
        void feed (const pR3 & p, double w = 1.0);
        pR3 get () const throw (div_by_zero);
    };
    
  ////////////////////////////////////////////////////////////////////////////////
  // vR3 : 3d vector /////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

    class AA_TOOLKIT_API vR3
    {
      public:
        double x, y, z;

      public:
        static vR3     Normalize (const vR3 &);
        static double DotProduct (const vR3 &, const vR3 &);
        static vR3  CrossProduct (const vR3 &, const vR3 &);

      public:
        vR3 ();
        vR3 (double, double, double);
        vR3 (const pR3 &, const pR3 &);
        // default copy ctor.
        // default dtor.
      
        vR3 & operator*= (double);
        vR3 & operator/= (double) throw (div_by_zero);
      
        vR3 & operator+= (const vR3 &);
    };
    
    // Scale.
    vR3 operator* (const vR3 &, double);
    vR3 operator* (double, const vR3 &);
    
    vR3 operator/ (const vR3 &, double) throw (div_by_zero);
    
    // Addition.
    vR3 operator+ (const vR3 &, const vR3 &);
    vR3 operator- (const vR3 &, const vR3 &);
    
    // Dot product.
    double operator% (const vR3 &, const vR3 &);
    double operator% (const vR3 &, const pR3 &);
    double operator% (const pR3 &, const vR3 &);
    
    // Cross product.
    vR3 operator^ (const vR3 &, const vR3 &);
    
    // Square length.
    double operator! (const vR3 &);
    
    pR3 FromPolar (double theta, double phi, double rho);
    void ToPolar (const pR3 &, double * theta, double * phi, double * rho);
    
    bool operator== (const vR3 &, const vR3 &);
    bool operator!= (const vR3 &, const vR3 &);
    
    std::istream & operator>> (std::istream &, vR3 &);
    std::ostream & operator<< (std::ostream &, const vR3 &);

////////////////////////////////////////////////////////////////////////////////
// Box : 3d axis-aligned bounding box //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class AA_TOOLKIT_API Box
    {
      private:
        pR3 m_pos;
        vR3 m_dim;
    
      public:
        Box ();
        Box (const pR3 &, const vR3 &);
        Box (double, double, double,
             double, double, double);
    
        // Translation.
        void  translate  (const vR3 &);
        Box & operator+= (const vR3 &);
        Box   operator+  (const vR3 &) const;
    
        // Union.
        void  unite      (const Box &);
        Box & operator|= (const Box &);
        Box   operator|  (const Box &) const;
    
        // Intersection.
        void  intersect  (const Box &);
        Box & operator&= (const Box &);
        Box   operator&  (const Box &) const;
        
        // Data accessors.
        const pR3 & pos () const;
        const vR3 & dim () const;
    
        // Volume.
        bool isEmpty () const;
        double volume () const;
    
        // Equality test.
        bool equals (double, double, double,
                     double, double, double) const;
        bool equals     (const Box &) const;
        bool operator== (const Box &) const;
        bool operator!= (const Box &) const;
    
        // Inclusion test.
        bool contains (const Box &) const;
        bool contains (double, double, double,
                       double, double, double) const;
    
        // I/O.
        void read  (std::istream &);
        void write (std::ostream &) const;

      public:
        static Box Center (double, double, double);
    };
    
    std::istream & operator >> (std::istream &, Box &);
    std::ostream & operator << (std::ostream &, const Box &);
    
////////////////////////////////////////////////////////////////////////////////
// BoxHelper : helps computing bounding boxes //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class AA_TOOLKIT_API BoxHelper
    {
      private:
        bool m_okay;
        Box m_box;
    
      public:
        BoxHelper ();
        virtual ~BoxHelper ();
        void reset ();
        void feed (const pR3 &);
        const Box & get () const;
    };  

////////////////////////////////////////////////////////////////////////////////
// OBB : 3d oriented bounding box //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class AA_TOOLKIT_API OBB
    {
      private:
        pR3 m_pos;
        vR3 m_axes [3]; // half-extent.

      public:
        OBB ();
        OBB (const pR3 &, const vR3 [3]);
    };

  } // namespace Math
} // namespace Aa

#endif // __AA_MATH__

