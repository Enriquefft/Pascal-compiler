## Testing Development guidelines
- Tests rely on hard‑coded expectations for tokens, AST validity, generated
  assembly, and execution output.
- For each tests, there must exist a hardcoded: token list, ast, asm & expected output, for example:
```cpp
TEST(VarDeclTests, Decl1) {
  std::string input_str = "var a: integer;";
  std::vector<Token> expected_tokens = {
      {TT::Var, "var"},     {TT::Identifier, "a"},
      {TT::Colon, ":"},     {TT::Identifier, "integer"},
      {TT::Semicolon, ";"}, {TT::EndOfFile, ""}};
  AST expected_ast{};
  expected_ast.root = std::make_unique<pascal::Program>(
      "test", std::make_unique<pascal::Block>(
                  std::vector<std::unique_ptr<pascal::Declaration>>{
                      std::make_unique<pascal::VarDecl>(
                          std::vector<std::string>{"a"},
                          std::make_unique<pascal::SimpleTypeSpec>(
                              pascal::BasicType::Integer, "integer"))},
                  std::vector<std::unique_ptr<pascal::Statement>>{}));
  expected_ast.valid = true;
  std::string expected_asm = "section .bss\n"
                             "a:    resq    1\n\n"
                             "section .text\n"
                             "global main\n"
                             "main:\n"
                             "    ret\n";
  std::string expected_output = "";
  run_full(input_str, expected_tokens, expected_ast, expected_asm,
           expected_output);
}

}
```
- Each one of this hardcoded expected outputs, will be manually created & typed, no automated function for this.

### Running tests

You can build and run the entire test suite with:

```bash
make tests
```

To compile and run a single test file, pass the desired filename (from the
`tests/` directory) using the `FILE` variable:

```bash
make tests FILE=print_tests.cpp
```

## Project Structure Overview
- `include/`: Header files organized into subdirectories mirroring the `src/` tree.
  - `parser/`: AST nodes (`ast.hpp`), recursive descent parser (`parser.hpp`), and
    semantic validator (`validator.hpp`).
  - `scanner/`: Lexical analyzer definitions in `lexer.hpp`.
  - `visitors/`: Code generation and runtime memory helpers.
  - `executor/`: Assembly execution helpers.
  - `token/`: Token type definitions used by the scanner and parser.
  - `utils.hpp`: Miscellaneous helpers shared across modules.
- `src/`: C++ sources implementing the compiler.
  - `parser/`, `scanner/`, `visitors/`, `executor/` correspond to their header
    counterparts.
  - `main.cpp`: Command line entry point building the compiler binary.
  - `api.cpp`: HTTP API entry point.
  - `utils.cpp`: Miscellaneous helper implementations.
- `tests/`: C++ test suites using GoogleTest. Each `*_tests.cpp` file contains
  large hardcoded expectations for tokens, ASTs, assembly, and runtime output.
  Helper headers such as `test_utils.hpp` and the `run_tests.sh` script also
  live here. Use `make tests` (or run the script directly) to build and execute
  all tests.
  - Available suites:
    - `array_tests.cpp`: array declarations and indexing
    - `control_tests.cpp`: `if`, `while`, and `for` statements
    - `dynamic_tests.cpp`: heap allocation and `dispose` semantics
    - `expression_tests.cpp`: expression precedence and operators
    - `float_tests.cpp`: floating‑point literals and operations
    - `function_tests.cpp`: procedures and functions with parameters
    - `invalid_code_tests.cpp`: intentionally bad code for failure cases
    - `long_int_tests.cpp`: 64‑bit `longint` handling
    - `pointer_tests.cpp`: pointer types and dereferencing
    - `print_tests.cpp`: `writeln` and other I/O routines
    - `string_tests.cpp`: string literals and assignments
    - `struct_tests.cpp`: `record` types and field access
    - `unsigned_tests.cpp`: unsigned integer support
    - `validator_tests.cpp`: semantic validation helper routines
    - `var_decl_tests.cpp`: variable declarations and basic types
- `libs/`: Third‑party dependencies (currently only `spdlog`).
- `test`: Simple Pascal program used temporarily for manual experimentation;
  this is a single file, not a directory.
- `main.asm`, `runasm.sh`: Example assembly program and helper script.
- `Makefile`, `Dockerfile`, `flake.*`, `vcpkg.json`: Build helpers and dependency
  configurations.

Most implementation files exceed one thousand lines (e.g. `include/parser/ast.hpp` and
`src/visitors/codegen.cpp`), so navigating by directory is often faster than
opening files blindly.
