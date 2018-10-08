#include "lexer.h"
#include "common.h"

LexToken lex_token;

void
lex_next_token(char* stream)
{
  // allows for asm jump tables
  lex_token.start = stream;
  switch (*stream) {
    case '1' ... '9': {
      lex_token.type = INT;
      u64 val = 0;
      while (isdigit(*stream)) {
        val *= 10;
      val += *stream++ - '0';
      }
      lex_token.val = val;
    } break;
    case 'A' ... 'Z':
    case 'a' ... 'z':
    case '_': {
      lex_token.type = NAME;
        while (isalnum(*stream) || *stream == '_') {
          stream++;		
        } 
      } break;
    default: {
    } break;
  }
  lex_token.end = stream;
}

LexInternStr* lex_intern_str_table;

char* 
str_intern_range(char* tok_start, char* tok_end)
{
  size_t tok_len = tok_end - tok_start;	
  for (size_t str_intern_i = 0; str_intern_i < BUF_LEN(lex_intern_str_table); \
        ++str_intern_i) {
    if (tok_len == lex_intern_str_table[str_intern_i].len && \
         strncmp(lex_intern_str_table[str_intern_i], tok_start, tok_len)) {
      return lex_intern_str_table[str_intern_i].str;
    }
  }

  char* intern_tok = xmalloc(tok_len + 1);
  intern_tok[tok_len] = '\0';
  BUF_PUSH(lex_intern_str_table, ((LexInternStr){tok_len, intern_tok}));
}
