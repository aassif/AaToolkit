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

#endif // __AA_TOOLKIT__

