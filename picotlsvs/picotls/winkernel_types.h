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

/* Terminates the system on a fatal error (bug check 0x139 KERNEL_SECURITY_CHECK_FAILURE). Used as the replacement of abort(3):
 * unlike DbgBreakPoint() or DbgRaiseAssertionFailure(), it can neither be continued from a debugger nor be caught by SEH, and it
 * is kept in release builds. A function-like macro named abort() is not defined here, because it would break the declaration of
 * abort in <stdlib.h> if that header is included afterwards; each source file maps abort() to this macro after its includes. */
/* __fastfail and the FAST_FAIL_* codes are declared by wdm.h */
#ifndef FAST_FAIL_FATAL_APP_EXIT
#define FAST_FAIL_FATAL_APP_EXIT 7
#endif
#define PTLS_WINKERNEL_ABORT() __fastfail(FAST_FAIL_FATAL_APP_EXIT)

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

/* normally provided by vcruntime.h (via stddef.h); the guards are the same as the ones used there */
#ifndef _INTPTR_T_DEFINED
#define _INTPTR_T_DEFINED
typedef LONG_PTR intptr_t;
#endif
#ifndef _UINTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
typedef ULONG_PTR uintptr_t;
#endif

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
