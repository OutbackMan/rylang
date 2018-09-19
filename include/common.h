#if !defined(COMMON_H)
#define COMMON_H

// timing --> consider is application batch or long-running
// errors --> only handle errors appropriate/relevent to domain
// const --> assumes knowledge of api. if want optimisation, use restrict
// macros --> give lazy evaluation. use appripriately.
// globals --> simplify code
// cmake --> recreate build folder to recreate error
// workflow --> write feature_test() for main() and later formalise into test

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
#define NON_NULL _Nonnull
#define NULLABLE _Nullable

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
typedef char byte;

#define FATAL(...) \
  fatal(__FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
PRINTF_FMT(4, 5) NO_RETURN void 
fatal(
  char* NON_NULL file_name, 
  int line_num, 
  char* NON_NULL fnc_sig, 
  char* NON_NULL fmt_str, 
  ...
);

#define MIN(val1, val2) (val1 < val2 ? val1 : val2)
#define CLAMP_MAX(clamp, val) MIN(clamp, val)
#define MAX(val1, val2) (val1 > val2 ? val1 : val2)
#define CLAMP_MIN(clamp, val) MAX(clamp, val)

RAW_MEM void* NON_NULL
xmalloc(size_t num_bytes);

RAW_MEM void* NON_NULL
xcalloc(size_t num_items, size_t item_size);

void* NON_NULL
xrealloc(void* NON_NULL orig_mem, size_t mem_size_change);

typedef struct {
  size_t cap;
  size_t len;
  byte content[];
} Buf;

#define BUF__HEADER(buf) \
  ((Buf* )((byte* )buf - offsetof(Buf, content)))

#define BUF_CAP(buf) \
  ((buf != NULL) ? BUF__HEADER(buf)->cap : 0)

#define BUF_LEN(buf) \
  ((buf != NULL) ? BUF__HEADER(buf)->len : 0)

#define BUF__FIT(buf, amount) \
  (amount <= BUF_CAP(buf) ? 0 : (buf = buf__grow(buf, amount, sizeof(*buf))))

#define BUF_PUSH(buf, elem) \
  (BUF__FIT(buf, BUF_LEN(buf) + 1), (buf)[BUF__HEADER(buf)->len++] = elem)

#define BUF_FREE(buf) \
  ((buf != NULL) ? (free(BUF__HEADER(buf)), (buf = NULL)) : 0)

byte* NON_NULL
buf__grow(void* NULLABLE content, size_t desired_len, size_t elem_size);

byte* NULLABLE 
read_file(char* NON_NULL path);

bool
write_file(
  char* NON_NULL path, 
  char* NON_NULL buf, 
  size_t len
);

#endif
