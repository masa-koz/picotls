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

#else
#define ssize_t int
#include <quic_platform.h>


#define assert(X) CXPLAT_DBG_ASSERT(X)

#define abort() DbgBreakPoint()

#define fprintf(output, ...) DbgPrint(__VA_ARGS__)

#define perror(...) DbgPrint(__VA_ARGS__)

#define malloc(SIZE) CXPLAT_ALLOC_PAGED(SIZE, 'ocip')

#define free(PTR) CXPLAT_FREE(PTR, 'ocip')

#define _aligned_malloc(SIZE, ALIGNMENT) CXPLAT_ALLOC_PAGED(SIZE, 'ocip')

#define _aligned_free(PTR) CXPLAT_FREE(PTR, 'ocip')

#define UINT64_C(c) c##UL

#endif

#ifndef __attribute__
#define __attribute__(X)
#endif


#ifdef __cplusplus
extern "C" {
#endif
struct timezone {
    int tz_minuteswest; /* minutes west of Greenwich */
    int tz_dsttime;     /* type of DST correction */
};

int wintimeofday(struct timeval *tv, struct timezone *tz);

#ifndef strcasecmp
#define strcasecmp _stricmp
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WINCOMPAT_H */