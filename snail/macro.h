#ifndef __SNAIL_MACRO_H__
#define __SNAIL_MACRO_H__

#include <string.h>
#include <assert.h>

#include "util.h"


#if defined __GNUC__ || defined __llvm__
#   define SNAIL_LICKLY(x)      __builtin_expect(!!(x), 1)
#   define SNAIL_UNLICKLY(x)    __builtin_expect(!!(x), 0)
#else
#   define SNAIL_LICKLY(x)      (x)
#   define SNAIL_UNLICKLY(x)    (x)
#endif

#define SNAIL_ASSERT(x) \
    if(SNAIL_UNLICKLY(!(x))) \
    {   \
        SNAIL_LOG_ERROR(SNAIL_LOG_ROOT()) << "ASSERTION:: " #x \
            << "\n backtrace:\n" \
            << snail::BacktraceToString(100, 2, "   "); \
            assert(x); \
    } 

#define SNAIL_ASSERT2(x, w) \
    if(SNAIL_UNLICKLY(!(x))) \
    {   \
        SNAIL_LOG_ERROR(SNAIL_LOG_ROOT()) << "ASSERTION:: " #x \
            << "\n" << w \
            << "\n backtrace:\n" \
            << snail::BacktraceToString(100, 2, "   "); \
            assert(x); \
    } 


#endif