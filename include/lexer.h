#if !defined(LEXER_H)
#define LEXER_H

#include "common.h"

#define PRINT_TOKEN_NAME() \
  printf("%.*s", (size_t)(token.start - token.end), token.start)

typedef enum {
  EOF = 0,
  // reserve first 128 for one char tokens
  LAST_CHAR = 127,
  INT,
  NAME,
  FLOAT,
  STR,
  INC,
  INC_ASSIGN
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

// in C all string literals are interned 
extern LexInternStr* lex_intern_str_table;

#endif
