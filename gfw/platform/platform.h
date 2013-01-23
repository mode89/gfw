#ifndef __GFW_PLATFORM_PLATFORM_H__
#define __GFW_PLATFORM_PLATFORM_H__

#include "common\platform.h"

#if PLATFORM_WIN32
#include "gfw\platform\win\platform.h"
#else
#error Undefined platform
#endif // Determine platform

#endif // __GFW_PLATFORM_PLATFORM_H__
