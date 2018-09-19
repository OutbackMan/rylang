#include "common.h"

// ensure optimisations are off
// start (f11), run to line (ctrl-r), resume (f8), into (f5), over (f6), out (f7)
// show disassembly view

#if 0
void
test_lexer(void)
{
  char* stream = "12345asdav_123(";	
  lex_token.type = INT;
  while (lex_token.type != 500) {
	lex_next_token(stream);
  }
}
#endif

int
main(int argc, char* UNUSED argv[argc + 1])
{
  return EXIT_SUCCESS;
}

// r-value read only, print default output and inspect in debugger watch
// inline semantics, make macros uppercase to ensure used properly, use single compiler version, use compiler extensions where appropriate
// assume modern os
