#ifndef __AA_TOOLKIT__
#define __AA_TOOLKIT__

#define AA_TOOLKIT_INLINE inline

#if defined( _WIN32 ) && defined( AA_TOOLKIT_BUILD_SHARED )
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

typedef   signed char  AaByte;
typedef unsigned char  AaUByte;
typedef   signed int   AaInt;
typedef unsigned int   AaUInt;
typedef   signed short AaShort;
typedef unsigned short AaUShort;
typedef   signed long  AaLong;
typedef unsigned long  AaULong;
//typedef float          AaFloat;
//typedef double         AaDouble;

typedef   signed char  AaInt8;
typedef unsigned char  AaUInt8;
typedef   signed short AaInt16;
typedef unsigned short AaUInt16;
typedef   signed long  AaInt32;
typedef unsigned long  AaUInt32;

#endif // __AA_TOOLKIT__

