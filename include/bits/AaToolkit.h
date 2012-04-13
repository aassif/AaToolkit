#ifndef __AA_TOOLKIT__
#define __AA_TOOLKIT__

#ifdef _WIN32
  #ifdef AaToolkit_EXPORTS // Merci CMake :)
    #define AA_TOOLKIT_API    __declspec(dllexport)
    #define AA_TOOLKIT_EXTERN
  #else
    #define AA_TOOLKIT_API    __declspec(dllimport)
    #define AA_TOOLKIT_EXTERN extern
  #endif
  #ifndef __MINGW32__
    #define rint(a) floor((a)<0 ? (a)-0.5 : (a)+0.5)
  //#define NAN std::numeric_limits<double>::infinity()
  #define NAN 0.0
    #define log2(a) (log((double)a)/log((double)2.0))
    #define cbrt(a) pow(abs((a)),1.0/3.0)
  #endif
#else
  #define AA_TOOLKIT_API
  #define AA_TOOLKIT_EXTERN
#endif

namespace Aa
{
  typedef unsigned int   uint;

  typedef unsigned char  uint8;
  typedef unsigned short uint16;
  typedef unsigned long  uint32;
}

#endif // __AA_TOOLKIT__

