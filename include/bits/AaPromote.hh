#ifndef __AA_PROMOTE__
#define __AA_PROMOTE__

namespace Aa
{

  template <class A, class B> struct Promote;

#define AA_DEFINE_PROMOTE(A, B, C)\
  template <> struct Promote<A, B> {typedef C Type;};

  //AA_DEFINE_PROMOTE (bool,    bool,    AaInt)
  //AA_DEFINE_PROMOTE (bool,    AaInt8,  AaInt)
  //AA_DEFINE_PROMOTE (bool,    AaUInt8, AaInt)
  //AA_DEFINE_PROMOTE (bool,    AaInt,   AaInt)
  //AA_DEFINE_PROMOTE (bool,    AaUInt,  AaUInt)
  //AA_DEFINE_PROMOTE (bool,    float,   float)
  //AA_DEFINE_PROMOTE (bool,    double,  double)

  //AA_DEFINE_PROMOTE (AaInt8,  bool,    AaInt)
  AA_DEFINE_PROMOTE (AaInt8,  AaInt8,  AaInt)
  AA_DEFINE_PROMOTE (AaInt8,  AaUInt8, AaInt)
  AA_DEFINE_PROMOTE (AaInt8,  AaInt,   AaInt)
  AA_DEFINE_PROMOTE (AaInt8,  AaUInt,  AaUInt)
  AA_DEFINE_PROMOTE (AaInt8,  float,   float)
  AA_DEFINE_PROMOTE (AaInt8,  double,  double)

  //AA_DEFINE_PROMOTE (AaUInt8, bool,    AaInt)
  AA_DEFINE_PROMOTE (AaUInt8, AaInt8,  AaInt)
  AA_DEFINE_PROMOTE (AaUInt8, AaUInt8, AaInt)
  AA_DEFINE_PROMOTE (AaUInt8, AaInt,   AaInt)
  AA_DEFINE_PROMOTE (AaUInt8, AaUInt,  AaUInt)
  AA_DEFINE_PROMOTE (AaUInt8, float,   float)
  AA_DEFINE_PROMOTE (AaUInt8, double,  double)

  //AA_DEFINE_PROMOTE (AaInt,   bool,    AaInt)
  AA_DEFINE_PROMOTE (AaInt,   AaInt8,  AaInt)
  AA_DEFINE_PROMOTE (AaInt,   AaUInt8, AaInt)
  AA_DEFINE_PROMOTE (AaInt,   AaInt,   AaInt)
  AA_DEFINE_PROMOTE (AaInt,   AaUInt,  AaUInt)
  AA_DEFINE_PROMOTE (AaInt,   float,   float)
  AA_DEFINE_PROMOTE (AaInt,   double,  double)

  //AA_DEFINE_PROMOTE (AaUInt,  bool,    AaUInt)
  AA_DEFINE_PROMOTE (AaUInt,  AaInt8,  AaUInt)
  AA_DEFINE_PROMOTE (AaUInt,  AaUInt8, AaUInt)
  AA_DEFINE_PROMOTE (AaUInt,  AaInt,   AaUInt)
  AA_DEFINE_PROMOTE (AaUInt,  AaUInt,  AaUInt)
  AA_DEFINE_PROMOTE (AaUInt,  float,   float)
  AA_DEFINE_PROMOTE (AaUInt,  double,  double)

  //AA_DEFINE_PROMOTE (float,   bool,    float)
  AA_DEFINE_PROMOTE (float,   AaInt8,  float)
  AA_DEFINE_PROMOTE (float,   AaUInt8, float)
  AA_DEFINE_PROMOTE (float,   AaInt,   float)
  AA_DEFINE_PROMOTE (float,   AaUInt,  float)
  AA_DEFINE_PROMOTE (float,   float,   float)
  AA_DEFINE_PROMOTE (float,   double,  double)

  //AA_DEFINE_PROMOTE (double,  bool,    double)
  AA_DEFINE_PROMOTE (double,  AaInt8,  double)
  AA_DEFINE_PROMOTE (double,  AaUInt8, double)
  AA_DEFINE_PROMOTE (double,  AaInt,   double)
  AA_DEFINE_PROMOTE (double,  AaUInt,  double)
  AA_DEFINE_PROMOTE (double,  float,   double)
  AA_DEFINE_PROMOTE (double,  double,  double)

#define AA_PROMOTE(T, U) typename Promote<T, U>::Type

}

#endif // __AA_PROMOTE__

