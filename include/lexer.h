#if !defined(LEXER_H)
#define LEXER_H

#include "common.h"

typedef enum {
  EOF = 0,
  LAST_CHAR = 127,
  INT,
  NAME,
  FLOAT,
  STR,
} LEX_TOKEN_TYPE;

typedef enum {
  NONE, // match null terminator
  BIN,
  OCT,
  HEX,
  CHAR
} LEX_TOKEN_SUB_TYPE;

typedef struct {
  LEX_TOKEN_TYPE type; // will store ascii if just a single entity
  LEX_TOKEN_SUB_TYPE sub_type;
  char const* start;
  char const* end;
  union {
    u64 int_val;
    double float_val;
    char const* name;
    char const* str_val;
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
