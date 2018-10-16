#include "lexer.h"
#include "common.h"

LexToken lex_token;

void
lex_next_token(char const* stream)
{
  // allows for asm jump tables
  lex_token.start = stream;
  switch (*stream) {
    case '1' ... '9': {
      lex_token.type = INT;
      int val = 0;
      while (isdigit(*stream)) {
        int digit = *stream++ - '0';
        if (val > INT_MAX - digit / 10) {
          syntax_error("Integer literal overflow");
          while (isdigit(*stream)) {
            stream++; 
          }
        }
        val = val * 10 + digit;
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
      lex_token.name = str_intern_range(lex_token.start, stream);
      } break;
    default: {
      lex_token.type = *stream++;
    } break;
  }
  lex_token.end = stream;
}

LexInternStr* _intern_str_table;

char const*
str_intern_range(char const* tok_start, char const* tok_end)
{
  size_t tok_len = tok_end - tok_start;	
  for (size_t intern_i = 0; intern_i < BUF_LEN(_intern_str_table); ++intern_i) {
    if (tok_len == _intern_str_table[intern_i].len && \
         strncmp(_intern_str_table[intern_i].str, tok_start, tok_len)) {
      return _intern_str_table[_intern_i].str;
    }
  }

  char* intern_tok = xmalloc(tok_len + 1);
  intern_tok[tok_len] = '\0';
  BUF_PUSH(_intern_str_table, ((LexInternStr){tok_len, intern_tok}));
  return intern_tok;
}

char const*
str_intern(char const* str)
{
  return str_intern_range(str, str + strlen(str));
}

// PARSING

char const* keyword_if;
char const* keyword_while;

void init_keywords()
{
  keyword_if = str_intern("if");
  keyword_while = str_intern("while");
}

inline bool is_cur_token_type(LEX_TOKEN_TYPE token_type)
{
  return cur_token.type == token_type;
}

inline bool is_cur_token_a_name(char const* name)
{
  cur_token.type == NAME && cur_token.name == name;
}

bool match_token(LEX_TOKEN_TYPE token_type)
{
  if (is_cur_token_type(token_type)) {
    lex_next_token(); 
    return true;
  } else {
    return false; 
  }
}

bool expect_token(LEX_TOKEN_TYPE token_type)
{
  if (is_cur_token_type(token_type)) {
    lex_next_token(); 
    return true;
  } else {
    FATAL("expected token: %s, got %s", token_type_str(token_type), cur_token.type);
  }
}

// pointer will be overwritten next call
char const* token_type_str(LEX_TOKEN_TYPE token_type)
{
  static char buf[256];
  switch (token_type) {
    case INT: {
      sprintf(buf, "integer");   
    } break;
    case NAME: {
      sprintf(buf, "name");   
    } break;
    default: {
      if (token_type < 128 && isprint(token_type)) {
        sprintf(buf, "%c", token_type); 
      } else {
        sprintf(buf, "<ASCII %d>", token_type);   
      }
    } break;
  }
  return buf;
}

/*
 * expr3 = INT | '(' expr ')' 
 * expr2 = [-]expr3 
 * expr1 = expr2 ([*\] expr2)*
 * expr0 = expr1 ([+-] expr1)*
 * expr = expr0
 * START FROM LOWEST PRECENDENCE
 */
int parse_expr3()
{
  if (is_token(TOKEN_INT)) {
    next_token();
    return cur_token.val;
  } else if (match_token('(')) {
    int val = parse_expr();
    expect_token(')');
    return val;
  } else {
    FATAL("expected integer or ), got %s", token_type_name(cur_token.type)); 
  }
}

int parse_expr2()
{
  if (match_token('-')) {
    return -parse_expr2(); // allow for chaining of unary
  } else {
    return parse_expr3(); 
  }
}

int parse_expr1()
{
  int val = parse_expr2();
  while (is_token('*') || is_token('/')) { // lookahead
    char op = cur_token.type;
    next_token(); // consume
    int rval = parse_expr2(); // parse recursively
    if (op == '*') {
      val *= rval;
    } else {
      assert(op == '/');
      assert(rval != 0); // express edge cases even if don't handle
      val /= rval;
    }
  }
  return val;
}

int parse_expr0()
{
  int val = parse_expr1(); // handle higher precedence first. left fold
  while (is_token('+') || is_token('-')) { // lookahead
    char op = cur_token.type; 
    next_token();
    int rval = parse_expr1();
    if (op == '+') { // dispatch on type to operator
      val += rval;
    } else {
      assert(op == '-');
      val -= rval;
    }
  }
  return val;
}

int parse_expr()
{
  return parse_expr0();
}
