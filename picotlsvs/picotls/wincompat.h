#ifndef WINCOMPAT_H
#define WINCOMPAT_H

#ifndef _KERNEL_MODE
#include <stdint.h>
#define ssize_t int
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <malloc.h>

#ifndef gettimeofday
#define gettimeofday wintimeofday
#endif

#else /* _KERNEL_MODE */

/* Windows kernel-mode support; self-contained, does not depend on msquic or any other platform layer */
#include "winkernel_types.h"

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef SSIZE_T ssize_t;
#endif

/* pool tag ('pico' in a memory dump) and flags used for allocations made by picotls; may be overridden by the build */
#ifndef PTLS_WINKERNEL_POOL_TAG
#define PTLS_WINKERNEL_POOL_TAG 'ocip'
#endif
#ifndef PTLS_WINKERNEL_POOL_FLAGS
#define PTLS_WINKERNEL_POOL_FLAGS POOL_FLAG_PAGED
#endif

static __inline void *ptls_winkernel_malloc(size_t size)
{
    return ExAllocatePool2(PTLS_WINKERNEL_POOL_FLAGS, size != 0 ? size : 1, PTLS_WINKERNEL_POOL_TAG);
}

static __inline void ptls_winkernel_free(void *p)
{
    if (p != NULL)
        ExFreePoolWithTag(p, PTLS_WINKERNEL_POOL_TAG);
}

/* Aligned allocation: over-allocate and store the original pointer right before the aligned address. `alignment` must be a
 * power of two. Memory obtained from this function MUST be released by ptls_winkernel_aligned_free. */
static __inline void *ptls_winkernel_aligned_malloc(size_t size, size_t alignment)
{
    UINT8 *raw;
    ULONG_PTR aligned;

    if (alignment < sizeof(void *))
        alignment = sizeof(void *);
    if ((alignment & (alignment - 1)) != 0 || size > SIZE_MAX - alignment - sizeof(void *))
        return NULL;
    if ((raw = (UINT8 *)ptls_winkernel_malloc(size + alignment - 1 + sizeof(void *))) == NULL)
        return NULL;
    aligned = ((ULONG_PTR)raw + sizeof(void *) + alignment - 1) & ~(ULONG_PTR)(alignment - 1);
    ((void **)aligned)[-1] = raw;
    return (void *)aligned;
}

static __inline void ptls_winkernel_aligned_free(void *p)
{
    if (p != NULL)
        ptls_winkernel_free(((void **)p)[-1]);
}

static __inline char *ptls_winkernel_strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *p;
    if ((p = (char *)ptls_winkernel_malloc(len)) != NULL)
        memcpy(p, s, len);
    return p;
}

/* milliseconds since the UNIX epoch (KeQuerySystemTimePrecise returns 100ns units since 1601-01-01) */
static __inline uint64_t ptls_winkernel_time_epoch_ms(void)
{
    LARGE_INTEGER now;
    KeQuerySystemTimePrecise(&now);
    return (uint64_t)(now.QuadPart - 116444736000000000LL) / 10000;
}

#define assert(X) NT_ASSERT(X)
#define abort() DbgBreakPoint()
#define fprintf(output, ...) DbgPrint(__VA_ARGS__)
#define perror(...) DbgPrint(__VA_ARGS__)
#define malloc(SIZE) ptls_winkernel_malloc(SIZE)
#define free(PTR) ptls_winkernel_free(PTR)
#define _aligned_malloc(SIZE, ALIGNMENT) ptls_winkernel_aligned_malloc((SIZE), (ALIGNMENT))
#define _aligned_free(PTR) ptls_winkernel_aligned_free(PTR)
#define strdup(S) ptls_winkernel_strdup(S)

#endif /* _KERNEL_MODE */

#ifndef __attribute__
#define __attribute__(X)
#endif


#ifdef __cplusplus
extern "C" {
#endif
#ifndef _KERNEL_MODE
struct timezone {
    int tz_minuteswest; /* minutes west of Greenwich */
    int tz_dsttime;     /* type of DST correction */
};

int wintimeofday(struct timeval *tv, struct timezone *tz);
#endif

#ifndef strcasecmp
#define strcasecmp _stricmp
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WINCOMPAT_H */