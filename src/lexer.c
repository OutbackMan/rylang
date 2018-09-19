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
  default:
  }

  lex_token.end = stream;
}
