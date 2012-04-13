#ifndef __AA_TOOLKIT__
#define __AA_TOOLKIT__

#define AA_TOOLKIT_INLINE inline

#ifdef _WIN32
  #ifdef AaToolkit_EXPORTS // Merci CMake :)
    #define AA_TOOLKIT_API    __declspec(dllexport)
    #define AA_TOOLKIT_EXTERN
  #else
    #define AA_TOOLKIT_API    __declspec(dllimport)
    #define AA_TOOLKIT_EXTERN extern
  #endif
#else
  #define AA_TOOLKIT_API
  #define AA_TOOLKIT_EXTERN
#endif

#if defined (_WIN32) && ! defined (__MINGW32__)
  #define NAN 0.0
  #define rint(a) floor((a)<0 ? (a)-0.5 : (a)+0.5)
  #define log2(a) (log((double)a)/log((double)2.0))
  #define cbrt(a) pow(abs((a)),1.0/3.0)
#endif

namespace Aa
{
#if 1 // __USE_MISC
  typedef unsigned int   uint;
  typedef unsigned short ushort;
  typedef unsigned long  ulong;
#endif

  typedef unsigned char  uint8;
  typedef unsigned short uint16;
  typedef unsigned long  uint32;
}

#endif // __AA_TOOLKIT__

