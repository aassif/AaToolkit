#ifndef __AA_PROMOTE__
#define __AA_PROMOTE__

namespace Aa
{

  template <class A, class B> struct Promote;

#define AA_DEFINE_PROMOTE(A, B, C)\
  template <> struct Promote<A, B> {typedef C Type;};

  //AA_DEFINE_PROMOTE (bool,  bool,    bool)
  //AA_DEFINE_PROMOTE (bool,  int,     int)
  //AA_DEFINE_PROMOTE (bool,  float,   float)
  //AA_DEFINE_PROMOTE (bool,  double,  double)

  //AA_DEFINE_PROMOTE (int,   bool,    int)
  AA_DEFINE_PROMOTE (int,   int,     int)
  AA_DEFINE_PROMOTE (int,   float,   float)
  AA_DEFINE_PROMOTE (int,   double,  double)

  //AA_DEFINE_PROMOTE (float, bool,    float)
  AA_DEFINE_PROMOTE (float, int,     float)
  AA_DEFINE_PROMOTE (float, float,   float)
  AA_DEFINE_PROMOTE (float, double,  double)

  //AA_DEFINE_PROMOTE (double, bool,   double)
  AA_DEFINE_PROMOTE (double, int,    double)
  AA_DEFINE_PROMOTE (double, float,  double)
  AA_DEFINE_PROMOTE (double, double, double)

#define AA_PROMOTE(T, U) typename Promote<T, U>::Type

}

#endif // __AA_PROMOTE__

