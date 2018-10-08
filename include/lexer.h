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

typedef struct {
  size_t len;
  char* str;
} LexInternStr;

extern LexInternStr* lex_intern_str_table;

#endif
