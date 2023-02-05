#ifndef __SNAIL_UTIL_H__
#define __SNAIL_UTIL_H_

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
namespace snail
{
	pid_t GetThreadId();
	uint32_t GetFiberId();
}

#endif