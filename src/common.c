#include "common.h"

static char global_error_msg[256];

void
rl_set_error(char const* fmt_str, ...)
{
}

char const* 
rl_get_error(void)
{
    
}

void
fatal(
  char const* file_name, 
  int line_num, 
  char const* fnc_sig, 
  char const* fmt_str, 
  ...
)
{
  va_list args;
  va_start(args, fmt_str);

  fprintf(stderr, "%s:%d:%s\n[RYLANG FATAL]: ", file_name, line_num, fnc_sig);
  vfprintf(stderr, fmt_str, args);
  fprintf(stderr, "\n");
  fflush(stderr);
  va_end(args);

  exit(EXIT_FAILURE);
}

void*
xmalloc(size_t num_bytes)
{
  void* mem = malloc(num_bytes);
  if (mem == NULL) {
    FATAL(
	    "Failed to allocate %zd bytes.\nErrno %d: %s", 
	    num_bytes, 
	    errno, 
	    strerror(errno)
	  );
  } else {
    return mem;	  
  }
}

void*
xcalloc(size_t num_items, size_t item_size)
{
  void* mem = calloc(num_items, item_size);
  if (mem == NULL) {
    FATAL(
	    "Failed to allocate %zd items of size %zd.\nErrno %d: %s", 
	    num_items,
	    item_size, 
	    errno, 
	    strerror(errno)
	  );
  } else {
    return mem;	  
  }
}

void*
xrealloc(void* orig_mem, size_t mem_size_change)
{
  void* mem = realloc(orig_mem, mem_size_change);
  if (mem == NULL) {
    FATAL(
	    "Failed to reallocate %zd bytes.\nErrno %d: %s", 
	    mem_size_change,
	    errno, 
	    strerror(errno)
	  );
  } else {
    return mem;	  
  }
}

byte* 
buf__grow(void* content, size_t desired_len, size_t elem_size)
{
  // overflow checking -- rearrange expression 
  assert(BUF_CAP(content) < SIZE_MAX / 2);
  size_t new_cap = CLAMP_MIN(2 * BUF_CAP(content), MAX(desired_len, 16));
  // implicit predicate
  assert(new_cap > desired_len);
  assert(new_cap < (SIZE_MAX - offsetof(Buf, content)) / elem_size);
  size_t new_size = offsetof(Buf, content) + new_cap * elem_size;
  Buf* new_buf = NULL;
  if (content != NULL) {
    new_buf = xrealloc(BUF__HEADER(content), new_size); 
  } else {
	  new_buf = xmalloc(new_size);
	  new_buf->len = 0;
  }
  new_buf->cap = new_cap;
  return new_buf->content;
}
