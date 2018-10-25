#if !defined(AST_H)
#define AST_H

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
    VarDecl var_decl;
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

typedef struct Expr Expr;
struct Expr {
  EXPR_TYPE type;
  union {
    u64 int_val;
    double float_val;
    char const* str_val;
    char const* name;
    struct {
      Type* cast_type;
      Expr* cast_expr;
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
