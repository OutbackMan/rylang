#if !defined(RESOLVE_H)
#define RESOLVE_H

typedef enum {
  SYM_UNRESOLVED,
  SYM_RESOLVING,
  SYM_RESOLVED
} SYM_STATE;

typedef struct {
  char const* name;  
  SYM_STATE state;
  Decl* decl;
} Sym;

Sym* sym_table;

#endif
