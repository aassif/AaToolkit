#ifndef AA_GEOMETRY__H
#define AA_GEOMETRY__H

namespace Aa
{
  template <class T, unsigned int m>
  class Geometry
  {
    public:
      typedef V<T, m>     Point;
      typedef V<T, m>     Vector;
      typedef V<Point, 2> Segment;
      typedef V<Point, 3> Triangle;

////////////////////////////////////////////////////////////////////////////////
// Aa::Geometry<T, m>::Plane ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

      class Plane
      {
        public:
          enum Side
          {
            NEGATIVE = -1,
            ZERO     =  0,
            POSITIVE = +1
          };

        public:
          Point  point;
          Vector normal;

        public:
          Plane (const Point & p, const Vector & n) :
            point (p),
            normal (n)
          {
          }

          Side side (const Point & p) const
          {
            T dot = DotProd (normal, p - point);
            if (dot < 0) return NEGATIVE;
            if (dot > 0) return POSITIVE;
            /**********/ return ZERO;
          }
      };

////////////////////////////////////////////////////////////////////////////////
// Aa::Geometry<T, m>::PlaneSegment ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

      class PlaneSegment
      {
        public:
          class Intersection
          {
            public:
              int         count;
              V<Point, 2> points;

            public:
              Intersection ()                  : count (0), points ()  {}
              Intersection (const Point   & p) : count (1), points (p) {}
              Intersection (const Segment & s) : count (2), points (s) {}
          };

#if 0
        private:
          static inline
          Point Intersect (const Plane & plane,
                           const Point & p0,
                           const Point & p1)
          {
            const Point & p2 = plane.point;
            T dot1 = DotProd (plane.normal, p1 - p0);
            T dot2 = DotProd (plane.normal, p2 - p0);
            return p0 + dot2 * (p1 - p0) / dot1;
          }
#endif

        public:
          Intersection Intersect (const Plane   & plane,
                                  const Segment & segment)
          {
#if 1
            typedef Intersection I;

            const Point & p0 = segment [0];
            const Point & p1 = segment [1];
            const Point & p2 = plane.point;

            Vector dir = p1 - p0;
            T dot1 = DotProd (plane.normal, dir);
            T dot2 = DotProd (plane.normal, p2 - p0);

            if (dot1 != 0)
            {
              double u = (double) dot2 / dot1;
              return (u >= 0.0 && u <= 1.0) ? I (p0 + u * dir) : I ();
            }
            else
            {
              return dot2 == 0 ? I (segment) : I ();
            }
#else
            typedef        Plane P;
            typedef Intersection I;

            const Point & p0 = segment [0];
            const Point & p1 = segment [1];

            typename P::Side side0 = plane.side (p0);
            typename P::Side side1 = plane.side (p1);

            switch (side0)
            {
              case P::NEGATIVE :
                switch (side1)
                {
                  case P::NEGATIVE : return I ();
                  case P::ZERO     : return I (p1);
                  case P::POSITIVE : return Intersect (plane, p0, p1);
                }

              case P::ZERO :
                switch (side1)
                {
                  case P::NEGATIVE : return I (p0);
                  case P::ZERO     : return I (segment);
                  case P::POSITIVE : return I (p0);
                }

              case P::POSITIVE :
                switch (side1)
                {
                  case P::NEGATIVE : return Intersect (plane, p0, p1);
                  case P::ZERO     : return I (p1);
                  case P::POSITIVE : return I ();
                }
            }
#endif
          }
      };

////////////////////////////////////////////////////////////////////////////////
// Aa::Geometry<T, m>::PlaneTriangle ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

      class PlaneTriangle
      {
        public:
          class Intersection
          {
            public:
              int         count;
              V<Point, 3> points;

            public:
              Intersection ()                   : count (0), points ()  {}
              Intersection (const Point    & p) : count (1), points (p) {}
              Intersection (const Segment  & s) : count (2), points (s) {}
              Intersection (const Triangle & t) : count (3), points (t) {}
          };

        private:
          static inline
          Point Intersect (const Plane   & plane,
                           const Segment & segment)
          {
#if 1
            const Point  & p0 = segment [0];
            const Point  & p1 = segment [1];
            Vector         d  = p1 - p0;

            const Point  & p2 = plane.point;
            const Vector & n  = plane.normal;

            T dot1 = DotProd (n, d);
            T dot2 = DotProd (n, p2 - p0);

            return p0 + dot2 * d / dot1;
#else
            typename PlaneSegment::Intersection intersection =
              PlaneSegment::Intersect (plane, segment);

            return intersection.points [0];
#endif
          }

          static inline
          Segment Intersect (const Plane   & plane,
                             const Segment & s0,
                             const Segment & s1)
          {
            Point p0 = Intersect (plane, s0);
            Point p1 = Intersect (plane, s1);
            return Segment (p0, p1);
          }

          static inline
          Segment Intersect (const Plane   & plane,
                             const Point   & point,
                             const Segment & segment)
          {
            return Segment (point, Intersect (plane, segment));
          }

        public:
          static inline
          Intersection Intersect (const Plane    & plane,
                                  const Triangle & triangle)
          {
            typedef         Plane P;
            typedef       Segment S;
            typedef  Intersection I;

            const Point & p0 = triangle [0];
            const Point & p1 = triangle [1];
            const Point & p2 = triangle [2];

            typename P::Side side0 = plane.side (p0);
            typename P::Side side1 = plane.side (p1);
            typename P::Side side2 = plane.side (p2);

            switch (side0)
            {
              case P::NEGATIVE :
                switch (side1)
                {
                  case P::NEGATIVE :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* --- */ return I ();
                      case P::ZERO     : /* --0 */ return I (p2);
                      case P::POSITIVE : /* --+ */ return I (Intersect (plane, S (p2, p0), S (p2, p1)));
                    }

                  case P::ZERO :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* -0- */ return I (p1);
                      case P::ZERO     : /* -00 */ return I (S (p1, p2));
                      case P::POSITIVE : /* -0+ */ return I (Intersect (plane, p1,         S (p0, p2)));
                    }

                  case P::POSITIVE :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* -+- */ return I (Intersect (plane, S (p1, p0), S (p1, p2)));
                      case P::ZERO     : /* -+0 */ return I (Intersect (plane, p2,         S (p0, p1)));
                      case P::POSITIVE : /* -++ */ return I (Intersect (plane, S (p0, p1), S (p0, p2)));
                    }
                }

              case P::ZERO :
                switch (side1)
                {
                  case P::NEGATIVE :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* 0-- */ return I (p0);
                      case P::ZERO     : /* 0-0 */ return I (S (p0, p2));
                      case P::POSITIVE : /* 0-+ */ return I (Intersect (plane, p0,         S (p1, p2)));
                    }

                  case P::ZERO :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* 00- */ return I (S (p0, p1));
                      case P::ZERO     : /* 000 */ return I (triangle);
                      case P::POSITIVE : /* 00+ */ return I (S (p0, p1));
                    }

                  case P::POSITIVE :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* 0+- */ return I (Intersect (plane, p0,         S (p1, p2)));
                      case P::ZERO     : /* 0+0 */ return I (S (p0, p2));
                      case P::POSITIVE : /* 0++ */ return I (p0);
                    }
                }

              case P::POSITIVE :
                switch (side1)
                {
                  case P::NEGATIVE :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* +-- */ return I (Intersect (plane, S (p0, p1), S (p0, p2)));
                      case P::ZERO     : /* +-0 */ return I (Intersect (plane, p2,         S (p0, p1)));
                      case P::POSITIVE : /* +-+ */ return I (Intersect (plane, S (p1, p0), S (p1, p2)));
                    }

                  case P::ZERO :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* +0- */ return I (Intersect (plane, p1, S (p0, p2)));
                      case P::ZERO     : /* +00 */ return I (S (p1, p2));
                      case P::POSITIVE : /* +0+ */ return I (p1);
                    }

                  case P::POSITIVE :
                    switch (side2)
                    {
                      case P::NEGATIVE : /* ++- */ return I (Intersect (plane, S (p2, p0), S (p2, p1)));
                      case P::ZERO     : /* ++0 */ return I (p2);
                      case P::POSITIVE : /* +++ */ return I ();
                    }
                }
            }

            return I ();
          }
      };
  };

}

#endif // AA_GEOMETRY__H

