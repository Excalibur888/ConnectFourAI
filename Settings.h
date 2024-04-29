#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); } }

#ifdef _WIN32
#  define INLINE inline
#else
#  define INLINE static inline
#endif

#endif
