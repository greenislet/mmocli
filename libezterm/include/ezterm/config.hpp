#pragma once

#if defined(_WIN32)
# define EZTERM_SYSTEM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
# include "TargetConditionals.h"
# if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#  define EZTERM_SYSTEM_IOS
# elif TARGET_OS_MAC
#  define EZTERM_SYSTEM_MACOS
# else
#  error This Apple operating system is not supported by EZTERM library
# endif
#elif defined(__unix__)
# if defined(__ANDROID__)
#  define EZTERM_SYSTEM_ANDROID
# elif defined(__linux__)
#  define EZTERM_SYSTEM_LINUX
# elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#  define EZTERM_SYSTEM_FREEBSD
# else
#  error This UNIX operating system is not supported by EZTERM library
# endif
#else
# error This operating system is not supported by EZTERM library
# endif

#if !defined(EZTERM_STATIC)
# if defined(EZTERM_SYSTEM_WINDOWS)
#  define EZTERM_SYMBOL_EXPORT __declspec(dllexport)
#  define EZTERM_SYMBOL_IMPORT __declspec(dllimport)
#  ifdef _MSC_VER
#   pragma warning(disable: 4251)
#  endif
# else // Linux, FreeBSD, Mac OS X
#  if __GNUC__ >= 4
#   define EZTERM_SYMBOL_EXPORT __attribute__ ((__visibility__ ("default")))
#   define EZTERM_SYMBOL_IMPORT __attribute__ ((__visibility__ ("default")))
#  else
#   define EZTERM_SYMBOL_EXPORT
#   define EZTERM_SYMBOL_IMPORT
#  endif
# endif
#else
# define EZTERM_SYMBOL_EXPORT
# define EZTERM_SYMBOL_IMPORT
#endif

#if defined(EZTERM_BUILDING_THE_LIB)
# define EZTERM_API EZTERM_SYMBOL_EXPORT
#else
# define EZTERM_API EZTERM_SYMBOL_IMPORT
#endif
