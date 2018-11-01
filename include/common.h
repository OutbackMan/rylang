#if !defined(COMMON_H)
#define COMMON_H

// timing --> consider is application batch or long-running
// errors --> only handle errors appropriate/relevent to domain
// const --> assumes knowledge of api. if want optimisation, use restrict. Only use for chars as dictated by clib
// macros --> give lazy evaluation. use appripriately. (, ) parameter expansion
// globals --> simplify code
// cmake --> recreate build folder to recreate error
// workflow --> write feature_test() for main() and later formalise into test.
//   inspect values in debugger for expected
//   check for memory leaks with top/process explorer
// js --> saucelabs


// arena allocators speed up allocations, segregates memory lifetimes, linearises memory (cache prefetches for traversals)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define INTERNAL static
#define PERSIST static

#define FALLTHROUGH_INTENDED
#define NO_DEFAULT_CASE

#define TO_STRING(val) #val
#define STRINGIFY(val) TO_STRING(val)

#define COMPILER "clang-"STRINGIFY(__clang_major__)"."\
  STRINGIFY(__clang_minor__)"."STRINGIFY(__clang_patch_level__)

#define PURE __attribute__((pure))
#define CONST __attribute__((const))
#define HOT __attribute__((hot))
#define COLD __attribute__((cold))
#define CHECK __attribute__((warn_unused_result))
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#define UNUSED __attribute__((unused))
#define LIKELY(val) __builtin_expect(!!(val), 1)
#define UNLIKELY(val) __builtin_expect(!!(val), 0)
#define RAW_MEM __attribute__((malloc))
#define NO_RETURN __attribute__((noreturn))
#define CLEANUP(fnc) __attribute__((__cleanup__(fnc)))
#define PRINTF_FMT(fmt_str_index, args_index) \
  __attribute__((format(printf, fmt_str_index, args_index)))
#define NO_NULL_RET __attribute__((returns_nonnull))

typedef unsigned int uint;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintmax_t umax;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef unsigned char byte;

#define FATAL(...) \
  fatal(__FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
PRINTF_FMT(4, 5) NO_RETURN void 
fatal(
  char const* file_name, 
  int line_num, 
  char const* fnc_sig, 
  char const* fmt_str, 
  ...
);

#define MIN(val1, val2) ((val1) < (val2) ? (val1) : (val2))
#define CLAMP_MAX(clamp, val) MIN(clamp, val)
#define MAX(val1, val2) ((val1) > (val2) ? (val1) : (val2))
#define CLAMP_MIN(clamp, val) MAX(clamp, val)

RAW_MEM NO_NULL_RET void*
xmalloc(size_t num_bytes);

RAW_MEM NO_NULL_RET void*
xcalloc(size_t num_items, size_t item_size);

NO_NULL_RET void*
xrealloc(void* orig_mem, size_t mem_size_change);

typedef struct {
  size_t cap;
  size_t len;
  byte content[];
} Buf;

NO_NULL_RET byte* 
buf__grow(void* content, size_t desired_len, size_t elem_size);

#define BUF__HEADER(b) \
  ((Buf* )((byte* )(b) - offsetof(Buf, content)))

#define BUF_CAP(b) \
  (((b) != NULL) ? BUF__HEADER(b)->cap : 0)

#define BUF_LEN(b) \
  (((b) != NULL) ? BUF__HEADER(b)->len : 0)

#define BUF__FIT(b, amount) \
  ((amount) <= BUF_CAP(b) ? 0 : ((b) = (typeof(b))buf__grow((b), amount, sizeof(*(b)))))

// __VA_ARGS__ to avoid preprocessor comma interpretation
#define BUF_PUSH(b, ...) \ 
  (BUF__FIT((b), BUF_LEN(b) + 1), (b)[BUF__HEADER(b)->len++] = (__VA_ARGS__))

#define BUF_FREE(b) \
  (((b) != NULL) ? (free(BUF__HEADER(b)), (b) = NULL) : (void)(0))

#define BUF_END(b) \
  ((b) + BUF_LEN(b))

/*
 * for (int* it = buf; it != BUF_END(buf); ++it) {
 *  int elem = it->content;
 * }
 */

#endif
