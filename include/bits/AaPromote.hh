#ifndef __AA_PROMOTE__
#define __AA_PROMOTE__

namespace Aa
{

  template <class A, class B> struct Promote;
  //template <> struct Promote<bool,  bool>    {typedef bool   type;};
  //template <> struct Promote<bool,  int>     {typedef int    type;};
  //template <> struct Promote<bool,  float>   {typedef float  type;};
  //template <> struct Promote<bool,  double>  {typedef double type;};
  //template <> struct Promote<int,   bool>    {typedef int    type;};
  template <> struct Promote<int,   int>     {typedef int    type;};
  template <> struct Promote<int,   float>   {typedef float  type;};
  template <> struct Promote<int,   double>  {typedef double type;};
  //template <> struct Promote<float, bool>    {typedef float  type;};
  template <> struct Promote<float, int>     {typedef float  type;};
  template <> struct Promote<float, float>   {typedef float  type;};
  template <> struct Promote<float, double>  {typedef double type;};
  //template <> struct Promote<double, bool>   {typedef double type;};
  template <> struct Promote<double, int>    {typedef double type;};
  template <> struct Promote<double, float>  {typedef double type;};
  template <> struct Promote<double, double> {typedef double type;};

#define AA_PROMOTE(T, U) typename Promote<T, U>::type
}

#endif // __AA_PROMOTE__

