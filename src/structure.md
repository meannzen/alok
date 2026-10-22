codecrafters-shell-c/
├── CMakeLists.txt
├── your_program.sh
├── codecrafters.yml
└── src/
    ├── main.c              # entry: REPL loop only (prompt → read → parse → execute)
    │
    ├── common.h            # shared includes, bool/size_t, small macros
    ├── memory.c / .h       # reallocate, GROW_ARRAY, FREE_ARRAY
    │
    ├── lexer/
    │   ├── lexer.c / .h    # chars → tokens (struct lexer *, no globals)
    │   └── token.h         # enum token_type, struct token
    │
    ├── parser/
    │   ├── ast.h           # struct command, redirection, pipeline, command_list
    │   ├── parser.c / .h   # tokens → AST, returns status + error message
    │   └── ast.c           # free_*, helpers (free_pipeline ...)
    │
    ├── expand/
    │   └── expand.c / .h   # quote removal, $VAR, ~, (later: globbing)
    │
    ├── exec/
    │   ├── executor.c / .h # run pipeline: fork, pipe, dup2, execvp, wait
    │   ├── redirect.c / .h # open files, apply redirects with dup2
    │   └── path.c / .h     # PATH search (executable_path)
    │
    ├── builtins/
    │   ├── builtins.c / .h # table: name → function, is_builtin(), run_builtin()
    │   ├── cmd_exit.c
    │   ├── cmd_echo.c
    │   ├── cmd_type.c
    │   ├── cmd_cd.c
    │   └── cmd_pwd.c
    │
    ├── shell/
    │   ├── shell.c / .h    # struct shell state: last exit status, env, cwd, jobs
    │   └── input.c / .h    # read line (getline now, readline/history later)
    │
    └── debug/
        └── debug.c / .h    # print_pipeline, print_tokens
