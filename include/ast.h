#if !defined(AST_H)
#define AST_H

// create nodes of ast

typedef enum {
  NONE,
  PAREN,
  NAME,
  FUNC,
  ARRAY,
  POINTER
} TYPESPEC;

typedef struct {
  TYPESPEC spec;
  union {
  };
} Type;

typedef enum {
  NONE, // allow for 0 intialisation
  ENUM,
  STRUCT,
  UNION,
  VAR,
  CONST,
  TYPEDEF,
  FUNC
} DECL_TYPE;

typedef struct {
  char const* name;
  Type* type;
} EnumItem;

typedef struct {
  char const* name[];
  Type* type;
} AggregateItem;

typedef struct {
  char const* name;
  Type* type;
} FuncParam;

typedef struct {
  FuncParam* params;
  Type* return_type;
} FuncDecl;

typedef struct {
  DECL_TYPE type;
  char const* name;
  union {
    EnumItem* enum_items;
    AggregateItem* aggregate_items;
    struct { 
      // canonicalise for var, const, typedef
      Type* type;
      Expr* expr;
    };
    FuncDecl func_decl;
  }
} Decl;

typedef enum {
  NONE,
  INT,
  FLOAT,
  STR,
  NAME,
  CALL,
  CAST,
  INDEX,
  FIELD,
  COMPOUND,
  UNARY,
  BINARY,
  TERNARY
} EXPR_TYPE;

// have a tree data structure when a struct contains non-linear pointer chains
typedef struct Expr Expr;
struct Expr {
  EXPR_TYPE type;
  TOKEN_TYPE op;
  union {
    u64 int_val;
    double float_val;
    char const* str_val;
    char const* name;
    struct {
      Type* cast_type;
      Expr* operand;
    };
    struct {
      // unary
      Expr* operand;
      Type* type;
    };
    struct {
      // binary
      Expr* left_expr;
      Expr* right_expr;
    };
    struct {
      // ternary
      Expr* cond;
      Expr* then_expr;
      Expr* else_expr;
    };
  };
};

Expr* create_expr_type(EXPR_TYPE type)
{
  Expr* expr = xcalloc(1, sizeof(Expr));
  expr->type = type;
  return expr;
}

Expr* expr_int(u64 int_val)
{
  Expr* expr = create_expr_type(INT);
  expr->int_val = int_val;
  return expr;
}

Expr* expr_str(char const* str_val);
Expr* expr_cast(Type* type, Expr* operand);
Expr* expr_unary(TOKEN_TYPE op, Expr* expr);

void print_expr(EXPR_TYPE type)
{
  switch (type) {
    case INT: {
    
    } break;
  }
}

typedef enum {
  NONE,
  DO,
  IF,
  SWITCH,
  WHILE,
  FOR,
  BREAK,
  CONTINUE,
  RETURN,
  ASSIGN,
  AUTO_ASSIGN,
  EXPR
} STATEMENT_TYPE;

typedef struct {
  STATEMENT_TYPE type;
  union {
    struct {
      IfClause* then_clause;
      IfClause* elseif_clauses;
    };
    struct {
      Type* type;
      char const* name;
      Expr* expr;
    };
    struct {
      Expr* lhs; 
      Expr* rhs; 
    };
  };
} Statement;

#endif
