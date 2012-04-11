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

