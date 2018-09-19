#if !defined(HELPERS_H)
#define HELPERS_H

#include "common.h"

typedef enum {
  PASSED,
  FAILED
} TEST_STATUS;

#define INVOKE_SELECTED_TEST(test) \
  if (strcmp(argv[1], STRINGIFY(test)) == 0) return test();

#endif
