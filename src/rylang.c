#include "common.h"
// git checkout tags/day16
// git log --diff-filter=A -- resolve.c
// git blame file.c


// ensure optimisations are off
// start (f11), run to line (ctrl-r), resume (f8), into (f5), over (f6), out (f7)
// show disassembly view

RL_CHECK RL_INTERNAL RL_STATUS
rl__compile_file(char const* file_path)
{
  // NOTE(Ryan): sys-free
  char const* file_contents = rl_read_file(file_path);
  if (file_contents == RL_ERROR_STATE) {
    rl_set_error(
                  "Unable to read file %s. Error: %s", 
                  file_path, 
                  rl_get_error()
                );
    return RL_FAILURE;
  } else {
    char const* stream = file_contents;
    char const* line_start = stream;  
    RL_LexToken cur_token = {0};
    rl_lex_get_next_token(stream, &cur_token);

    /* lex --> stream, builtins
       parse -->
       sym -->
       gen --> 
    */
  }

}

// order independent declarations --> int a = b; int b = 1;

int
main(int argc, char* argv[argc + 1])
{
  if (argc < 2) {
    // stderr --> error and usage
    fprintf(stderr, "Usage: %s <rylang-source-file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (rl__compile_file(argv[1]) == RL_FAILURE) {
    fprintf(stderr, "Compilation failed: %s\n", rl_get_error());
    return EXIT_FAILURE;
  }

  puts("Compilation succeeded\n");

  return EXIT_SUCCESS;
}

// r-value read only, print default output and inspect in debugger watch
// inline semantics, make macros uppercase to ensure used properly, use single compiler version, use compiler extensions where appropriate
// assume modern os
