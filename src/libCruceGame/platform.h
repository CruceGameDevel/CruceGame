/**
 * @file platform.h
 * @brief Contains platform specific definitions
 */

#ifndef PLATFORM_H
#define PLATFORM_H 

#ifdef _MSC_VER
#ifdef LIBCRUCEGAME_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#else
#define EXPORT
#endif


#endif