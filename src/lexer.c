#include "lexer.h"
#include "common.h"

LexToken lex_token;

// all invalid numbers will be 0 initialised
#define INVALID_CH_TO_DIGIT 0
static unsigned char ch_to_digit[128] = {
  ['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, ['4'] = 4, 
  ['5'] = 5, ['6'] = 6, ['7'] = 7, ['8'] = 8, ['9'] = 9,
  ['a'] = ['A'] = 10, ['b'] = ['B'] = 11, ['c'] = ['C'] = 12,
  ['d'] = ['D'] = 13, ['e'] = ['E'] = 14, ['f'] = ['F'] = 15
};

void scan_int()
{
  int base = 10;
  LEX_TOKEN_SUB_TYPE sub_type = NONE;
  if (*stream++ == '0') {
    if (tolower(*stream) == 'x') {
      base = 16;
      sub_type = HEX;
      stream++;
    else if (isdigit(*stream)) {
      base = 8; 
      sub_type = OCTAL;
      stream++;
    } else if (tolower(*stream) == 'b') {
      base = 2;
      sub_type = BINARY;
      stream++;
    } else {
      token.type = INT;
      token.val = 0;
      syntax_error("Invalid integer suffix %c", *stream);       
    } 
  }

  if (base == 10 && *stream == '.') {
    lex_token.type = FLOAT;
  }
  
  lex_token.type = INT;
  u64 val = 0;

  while (true) {
    u64 digit = ch_to_digit[*stream]; // more capable than *stream++ - '0';
    if (stream != '0' && digit == INVALID_CH_TO_DIGIT) {
      break;
    }
    if (digit >= base) {
      syntax_error("Digit outside of base range");
    }
    if (val > (UINT64_MAX - digit) / base) {
      syntax_error("Integer literal overflow");
      while (isdigit(*stream)) {
        stream++; 
      }
    }
    val = val * base + digit;
  }

  lex_token.val = val;
}

// aim for LL(1) at token level, NOT character level
void
lex_next_token(char const* stream)
{
  // allows for asm jump tables
  lex_token.start = stream;
  switch (*stream) {
    case ' ': case '\t': case '\r': case '\n': {
      while (isspace(*stream)) {
        stream++;
      }
      lex_next_token();
    } break;
    case "'": {
      token.type = CHAR;          
      scan_char();
    } break;
    case "\"": {
      token.type = STRING;          
      scan_str();
    } break;
    case '0' ... '9': {
      while (isdigit(*stream)) {
        stream++; 
      } 
      if (*stream == '.' || tolower(*stream) == 'e') {
        // bookmarked/recorded string here 
        stream = token.start;
        token.type = FLOAT;
        token.float_val = parse_float();
      } else {
        stream = token.start;
        token.type = INT;
        token.int_val = parse_int();
      }
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
    // there is no lexing distinction between unary minus and binary, only parsing
    case '+': {
      token.type = *stream++; // handle single char case
      if (*stream == '+') {
        token.type = OP_INC; 
        stream++;
      } else if (*stream == '=') {
        token.type = OP_INC_ASSIGN; 
        stream++;
      }
    } break;
    default: {
      lex_token.type = *stream++;
    } break;
  }
  lex_token.end = stream;
}

void scan_str(void)
{
  assert(*stream == '"');
  char* buf = NULL;

  token.type = STR;
  stream++;
  char* val = stream;
  while (*stream != '"') {
    if (*stream == '\n') {
      syntax_error("String literal cannot contain new line"); 
    }
    if (*stream == '\\') {
      val = escape_to_ch[*stream];
    }
    BUF_PUSH(buf, *val);
    stream++; 
    val++;
  }
  BUF_PUSH(buf, '\0');
  token.str_val = buf;
}

static char escape_to_ch[128] = {
  ['a'] = '\a',
  ['b'] = '\b',
  ['t'] = '\t',
  ['r'] = '\r',
  ['n'] = '\n',
};

void scan_char(void)
{
  assert(*stream == '\'');
  ++stream;

  char val;
  if (*stream == '\\') {
    ++stream;
    val = escape_to_ch[*stream];
  }

  if (*stream != '\'') {
    syntax_error("Expected closing ' for char literal"); 
  } else {
    ++stream; 
  }
}

double scan_float()
{
  char const* start = stream;
  while (isdigit(*stream)) {
    stream++;  
  }
  if (stream == '.') {
    stream++;
  }
  while (isdigit(*stream)) {
    stream++;  
  }
  // principle of parsing -> find, handle, consume
  if (tolower(*stream) == 'e') {
    stream++; 
    if (*stream == '+' || *stream == '-') {
      stream++; 
    }
    if (!isdigit(*stream)) {
      syntax_error("Expected digit after float literal exponent"); 
    }
    while (isdigit(*stream)) {
      stream++; 
    }
  }
  char const* end = stream;
  double val = strtod(start, NULL);
  if (val == HUGE_VAL || val == -HUGE_VAL) {
    syntax_error("float literal overflow"); 
  }
  return val;
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

// to avoid having to store extra data, have explicit 'markers' that signify information
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
