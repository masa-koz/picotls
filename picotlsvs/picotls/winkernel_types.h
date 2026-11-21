/*
 * Minimal Windows kernel-mode platform definitions for picotls and cifra.
 *
 * This header replaces the former dependency on msquic's "quic_platform.h". It only pulls in the WDK headers and provides the
 * C99 <stdint.h>/<inttypes.h> subset that is not available in kernel-mode builds. It deliberately does NOT redefine any libc
 * function (malloc, free, abort, assert, ...) so that it can be safely included from third-party headers such as those of
 * cifra; those replacements live in wincompat.h.
 */
#ifndef PTLS_WINKERNEL_TYPES_H
#define PTLS_WINKERNEL_TYPES_H

#ifndef _KERNEL_MODE
#error "winkernel_types.h is only for Windows kernel-mode builds"
#endif

/* ntifs.h is a superset of ntddk.h / wdm.h; including it first keeps the include order valid regardless of what the consumer
 * includes later */
#include <ntifs.h>
#include <ntstrsafe.h>
#include <ws2def.h>
#include <ws2ipdef.h>
#include <ip2string.h>

#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

/* <stdint.h> substitutes; identical typedefs are allowed to be repeated, so this coexists with other kernel headers (e.g.,
 * msquic_winkernel.h) that define the same types */
typedef INT8 int8_t;
typedef INT16 int16_t;
typedef INT32 int32_t;
typedef INT64 int64_t;
typedef UINT8 uint8_t;
typedef UINT16 uint16_t;
typedef UINT32 uint32_t;
typedef UINT64 uint64_t;

#ifndef INT8_MIN
#define INT8_MIN (-127i8 - 1)
#endif
#ifndef INT16_MIN
#define INT16_MIN (-32767i16 - 1)
#endif
#ifndef INT32_MIN
#define INT32_MIN (-2147483647i32 - 1)
#endif
#ifndef INT64_MIN
#define INT64_MIN (-9223372036854775807i64 - 1)
#endif
#ifndef INT8_MAX
#define INT8_MAX 127i8
#endif
#ifndef INT16_MAX
#define INT16_MAX 32767i16
#endif
#ifndef INT32_MAX
#define INT32_MAX 2147483647i32
#endif
#ifndef INT64_MAX
#define INT64_MAX 9223372036854775807i64
#endif
#ifndef UINT8_MAX
#define UINT8_MAX 0xffui8
#endif
#ifndef UINT16_MAX
#define UINT16_MAX 0xffffui16
#endif
#ifndef UINT32_MAX
#define UINT32_MAX 0xffffffffui32
#endif
#ifndef UINT64_MAX
#define UINT64_MAX 0xffffffffffffffffui64
#endif
#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX UINT64_MAX
#else
#define SIZE_MAX UINT32_MAX
#endif
#endif

#ifndef INT64_C
#define INT64_C(c) c##LL
#endif
#ifndef UINT64_C
#define UINT64_C(c) c##ULL
#endif

/* <inttypes.h> substitutes */
#ifndef PRId64
#define PRId64 "lld"
#endif
#ifndef PRIu64
#define PRIu64 "llu"
#endif
#ifndef PRIx64
#define PRIx64 "llx"
#endif

#endif /* PTLS_WINKERNEL_TYPES_H */
