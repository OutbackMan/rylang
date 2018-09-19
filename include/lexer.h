#if !defined(LEXER_H)
#define LEXER_H

#include "common.h"

typedef enum {
  LAST_CHAR = 127,
  INT,
  NAME
} LEX_TOKEN_TYPE;

typedef struct {
  LEX_TOKEN_TYPE type;
  char* start;
  char* end;
  union {
    u64 val;
  };
} LexToken;

extern LexToken lex_token;

void
lex_next_token(char* stream);

#if 0
typedef struct {
  size_t len;
  char* str;
} InternStr;

extern InternStr* intern_str_table;

char* str_intern_range(char* start, char* end)
{
  size_t len = end - start;	
  for (size_t intern_str_table_i = 0; intern_str_table_i < BUF_LEN(intern_str_table); ++intern_str_table_i) {
    if (len == intern_str_table[i]  	  
  }
}
#endif

#endif
