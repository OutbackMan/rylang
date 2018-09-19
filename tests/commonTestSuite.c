#include "helpers.h"

TEST_STATUS test_common_buf_empty_cap_0(void) 
{	
  int* buf = NULL;
  return (BUF_CAP(buf) == 0) ? PASSED : FAILED;
}

TEST_STATUS test_common_buf_empty_len_0(void) 
{	
  int* buf = NULL;
  return (BUF_LEN(buf) == 0) ? PASSED : FAILED;
}

TEST_STATUS test_common_buf_push_10_elems_len_10(void) 
{	
  int* buf = NULL;
  for (size_t i = 0; i < 10; ++i) {
    BUF_PUSH(buf, i);
  }

  TEST_STATUS has_passed = ((BUF_LEN(buf) == 10) ? PASSED : FAILED);

  BUF_FREE(buf);

  return has_passed;
}

TEST_STATUS test_common_buf_push_10_elems_right_order(void) 
{	
  int* buf = NULL;
  for (size_t i = 0; i < 10; ++i) {
    BUF_PUSH(buf, i);
  }

  for (size_t i = 0; i < BUF_LEN(buf); ++i) {
    if (buf[i] != i) return FAILED;
  }

  BUF_FREE(buf);

  return PASSED;
}

TEST_STATUS test_common_buf_free_sets_to_null(void)
{
  int* buf = NULL;
  for (size_t i = 0; i < 10; ++i) {
    BUF_PUSH(buf, i);
  }

  BUF_FREE(buf);

  return (buf == NULL) ? PASSED : FAILED;
}

#define COMMON_TESTS_XX(EVAL) \
  EVAL(test_common_buf_empty_cap_0) \
  EVAL(test_common_buf_empty_len_0) \
  EVAL(test_common_buf_push_10_elems_len_10) \
  EVAL(test_common_buf_push_10_elems_right_order) \
  EVAL(test_common_buf_free_sets_to_null) 

TEST_STATUS tests_commonTestSuite(TEST_STATUS argc, char* argv[argc + 1])
{
  COMMON_TESTS_XX(INVOKE_SELECTED_TEST); 

  return EXIT_SUCCESS;
}
