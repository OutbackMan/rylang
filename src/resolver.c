typedef enum {
  SYM_UNRESOLVED,
  SYM_RESOLVING,
  SYM_RESOLVED
} SYM_STATE;

typedef struct {
  SYM_STATE state;
  char const* name; 
} Sym;

Sym* sym_table;
