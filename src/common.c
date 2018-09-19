#include "common.h"

void
fatal(
  char* file_name, 
  int line_num, 
  char* fnc_sig, 
  char* fmt_str, 
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
  size_t new_cap = CLAMP_MIN(2 * BUF_CAP(content), MAX(desired_len, 16));
  size_t new_size = offsetof(Buf, content) + new_cap * elem_size;
  Buf* new_buf = NULL;
  if (content != NULL) {
    xrealloc(BUF__HEADER(content), new_size); 
  } else {
	new_buf = xmalloc(desired_len);
	new_buf->len = 0;
  }
  new_buf->cap = new_cap;
  return new_buf->content;
}

byte* 
read_file(char* path)
{
  FILE* file = fopen(path, "r");
  if (file == NULL) {
	fprintf(stderr, "Unable to open file.\nErrno %s\n", strerror(errno));
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  byte* buf = xmalloc(file_size + 1);
  if (fread(buf, file_size, 1, file) != 1) {
    fclose(file);
	free(buf);
	return NULL;
  }
  fclose(file);
  buf[file_size] = 0;
  return buf;
}

bool
write_file(
  char* path, 
  char* buf, 
  size_t len
)
{
  FILE* file = fopen(path, "w");
  if (file == NULL) {
	fprintf(stderr, "Unable to open file.\nErrno %s\n", strerror(errno));
    return false;
  }
  
  fwrite(buf, 1, len, file);
  fclose(file);
  return true;
}
