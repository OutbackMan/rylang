#if !defined(LEXER_H)
#define LEXER_H

#include "common.h"

typedef enum {
  LAST_CHAR = 127,
  INT,
  NAME
} LEX_TOKEN_TYPE;

typedef struct {
  LEX_TOKEN_TYPE type; // will store ascii if just a single entity
  char const* start;
  char const* end;
  union {
    int val;
    char const* name;
  };
} LexToken;

extern LexToken lex_token;

void
lex_next_token(char const* stream);

typedef struct {
  size_t len;
  char const* str;
} LexInternStr;

extern LexInternStr* lex_intern_str_table;

#endif
