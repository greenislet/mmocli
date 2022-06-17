#pragma once

#if defined(_WIN32)
# define MMOCLI_SYSTEM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
# include "TargetConditionals.h"
# if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#  define MMOCLI_SYSTEM_IOS
# elif TARGET_OS_MAC
#  define MMOCLI_SYSTEM_MACOS
# else
#  error This Apple operating system is not supported by MMOCLI library
# endif
#elif defined(__unix__)
# if defined(__ANDROID__)
#  define MMOCLI_SYSTEM_ANDROID
# elif defined(__linux__)
#  define MMOCLI_SYSTEM_LINUX
# elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#  define MMOCLI_SYSTEM_FREEBSD
# else
#  error This UNIX operating system is not supported by MMOCLI library
# endif
#else
# error This operating system is not supported by MMOCLI library
# endif

#if !defined(MMOCLI_STATIC)
# if defined(MMOCLI_SYSTEM_WINDOWS)
#  define MMOCLI_SYMBOL_EXPORT __declspec(dllexport)
#  define MMOCLI_SYMBOL_IMPORT __declspec(dllimport)
#  ifdef _MSC_VER
#   pragma warning(disable: 4251)
#  endif
# else // Linux, FreeBSD, Mac OS X
#  if __GNUC__ >= 4
#   define MMOCLI_SYMBOL_EXPORT __attribute__ ((__visibility__ ("default")))
#   define MMOCLI_SYMBOL_IMPORT __attribute__ ((__visibility__ ("default")))
#  else
#   define MMOCLI_SYMBOL_EXPORT
#   define MMOCLI_SYMBOL_IMPORT
#  endif
# endif
#else
# define MMOCLI_SYMBOL_EXPORT
# define MMOCLI_SYMBOL_IMPORT
#endif

#if defined(MMOCLI_BUILDING_THE_LIB)
# define MMOCLI_API MMOCLI_SYMBOL_EXPORT
#else
# define MMOCLI_API MMOCLI_SYMBOL_IMPORT
#endif
