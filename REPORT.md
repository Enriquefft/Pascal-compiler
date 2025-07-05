# Pascal Compiler Project Report

## Introduction

This repository implements a Pascal compiler written in modern C++23. It aims to support the core language plus a range of extensions including floating point numbers, unsigned integers, dynamic memory, records, arrays, strings, and pointer types. The project can be built as a command line tool or as an HTTP API service. A separate Next.js application serves as the web frontend.

This document provides an extended overview of the codebase. It covers repository structure, the compilation pipeline, API features, frontend integration, and the test strategy used during development.

---

## Repository Organization

The root `AGENTS.md` summarises the layout of important folders:

- `include/` contains all header files for parser, scanner, visitors, executor, token definitions and utilities.
- `src/` hosts the corresponding C++ source files for each module along with `main.cpp` and `api.cpp`.
- `tests/` holds a large GoogleTest suite where each test file contains manually typed expectations for tokens, ASTs, assembly, and program output.
- Additional directories like `libs/`, `main.asm`, `runasm.sh` and build helpers (`Makefile`, `Dockerfile`, `flake.*`) support development.

These guidelines are explicitly described in the agent instructions: "Tests rely on hard‑coded expectations for tokens, AST validity, generated assembly, and execution output" and the list of subdirectories under the Project Structure Overview【F:AGENTS.md†L1-L71】.

---

## Building and Running

The README explains how to compile the project using the provided Makefile:

```bash
make        # builds the compiler
make tests  # runs example tests
make clean  # removes build artefacts
```

After building, the compiler binary resides under `build/compiler`【F:README.md†L9-L17】. A `Dockerfile` allows running the API service by building an image and exposing port 443. The container serves a `/compile` endpoint that returns a JSON object shaped like `CompilationResult`【F:README.md†L20-L33】.

---

## Compiler Pipeline

The compilation process consists of five main stages.

### Stage 1: Tokenization

The lexer reads the source text and produces tokens. Keywords such as `program`, `var`, `procedure`, etc., are matched by comparing strings against a table of keywords inside `lexer.cpp`【F:src/scanner/lexer.cpp†L7-L38】. Tokens are stored with their type, lexeme, line, and column numbers.

### Stage 2: Parsing

A recursive descent parser transforms the token list into an Abstract Syntax Tree (AST). To simplify backtracking, the parser can freeze and reset its internal state. Calls to `freeze()` push the current token index onto a stack while `reset()` pops it to revert to a previous position【F:include/parser/parser.hpp†L46-L61】. This mechanism is used when repeatedly attempting declarations such as variable or type definitions in loops【F:src/parser/parser.cpp†L177-L208】【F:src/parser/parser.cpp†L244-L276】.

The grammar implemented in the parser roughly follows the Pascal EBNF provided in the README, supporting statements like assignments, `if`, `while`, and `for` along with types for arrays, records, and pointers【F:README.md†L52-L152】.

#### Grammar & Parsing

The project’s `README.md` lists an extended EBNF grammar for the language. At the top level a `program` consists of the `program` keyword, its identifier, an optional identifier list, and a terminating `.`【F:README.md†L52-L55】. Each program contains a `block` whose first portion is a `declaration_part` made of optional `const`, `type`, and `var` sections followed by zero or more routine declarations. After these come a `begin`/`end` wrapped `statement_list` where the executable code resides.

**Declarations** – The parser iterates over `const_section`, `type_section`, and `var_section` using the freeze/reset helpers so that failed attempts roll back cleanly. Variable declarations are read with `parseVarDecl` in a loop until the upcoming token is no longer part of the same section【F:src/parser/parser.cpp†L179-L199】. The same strategy handles custom type definitions for arrays, records, and pointers【F:src/parser/parser.cpp†L201-L221】.

**Routine Declarations** – Procedures and functions are parsed with optional parameter lists. After the identifier, `parseDeclaration` optionally reads parameters inside parentheses and for functions expects a return type after a colon before parsing the body block. These flows implement the `procedure_decl` and `function_decl` rules【F:README.md†L70-L72】【F:src/parser/parser.cpp†L224-L282】.

**Statements** – `parseStatement` covers every alternative in the grammar’s `statement` rule. Starting with `begin` to create `CompoundStmt`, it then checks sequentially for `if`, `while`, `repeat`, `for`, `case`, `with`, dynamic memory procedures (`new`/`dispose`), explicit procedure calls, and finally assignments when an identifier is followed by `:=`【F:README.md†L97-L147】【F:src/parser/parser.cpp†L388-L548】.

**Expressions** – Expressions are parsed recursively with precedence. The helper `is_op` lists recognized binary operators like `+`, `-`, `div`, `mod`, `and`, and comparison operators【F:src/parser/parser.cpp†L289-L296】. `parseExpression` first builds a primary node for a literal, variable, or parenthesised subexpression, then consumes an operator and recurses for the right-hand side if one is present. Selectors for fields, array indices, and pointer dereferences are parsed by `parseVariable`【F:src/parser/parser.cpp†L298-L317】.

**Type Specifications** – `parseTypeSpec` recognises pointer types with leading `^`, arrays with optional `[low..high]` ranges, record types made of nested variable declarations, and predefined simple types such as `integer`, `real`, `unsigned`, `longint`, and `string`【F:README.md†L78-L95】【F:src/parser/parser.cpp†L551-L625】.

The top-level function `parseProgram` ties everything together by expecting a `program` header, delegating to `parseBlock`, and ensuring the final dot is present as required by the grammar【F:src/parser/parser.cpp†L27-L56】. Together with the freeze/reset stack this design enables robust handling of Pascal syntax while remaining straightforward to extend.

### Stage 3: AST Validation

Once parsing succeeds, `ASTValidator` traverses the tree to ensure semantic correctness. The validator manages nested scopes via `pushScope()` and `popScope()` functions and checks that identifiers have been declared before use【F:src/parser/validator.cpp†L390-L422】. Validation results include an error message with line and column numbers if a problem is found.

Validation begins in `ASTValidator::validate` where the error state is reset and a global scope is opened before visiting the `Program` node【F:src/parser/validator.cpp†L4-L16】.

Errors are recorded through `setError`, storing the message and the offending location the first time it is called【F:src/parser/validator.cpp†L19-L26】.

Scopes are tracked using a vector of sets: blocks push a new set with `pushScope`, declarations insert names via `declare`, and `visitVariableExpr` ensures each identifier is found using `isDeclared`【F:src/parser/validator.cpp†L402-L419】【F:src/parser/validator.cpp†L322-L333】.

The final outcome is returned as a `Result` struct containing the success flag, message, line and column numbers【F:include/parser/validator.hpp†L11-L18】.

### Stage 4: Code Generation

`CodeGenerator` is the `NodeVisitor` responsible for producing NASM syntax.
The `generate` method resets all internal collections, invokes `collectVars` to
gather globals, strings, pointer sizes and required runtime helpers, then visits
the AST to emit instructions【F:src/visitors/codegen.cpp†L35-L50】.

### Output Sections & Runtime Symbols

`visitProgram` writes a `.data` section for string literals and a format string
used by `printf`. Variable storage is reserved in `.bss`. The `.text` section
includes `extern` directives for any helpers detected during collection
(`malloc`, `free`, `puts`, `printf`). It then emits all routine bodies and the
`main` label【F:src/visitors/codegen.cpp†L53-L97】.

### Parameter Mapping & Functions

Procedures and functions assign the first six parameters to registers `rdi`
through `r9`. `visitProcedureDecl` and `visitFunctionDecl` fill `m_paramMap`,
emit the body, and restore the previous context so nested declarations work
properly【F:src/visitors/codegen.cpp†L122-L156】.

### Expression Evaluation

`genExpr` covers literals, variables, and binary operations. Floating literals
are converted to hexadecimal with `floatToHex`; array indexing computes offsets
when the selector is `[index]`; and binary expressions either constant fold or
emit arithmetic instructions after evaluating operands【F:src/visitors/codegen.cpp†L165-L268】.

### Assignments and Built‑ins

`visitAssignStmt` stores values into variables, dereferenced pointers or array
slots. String literals go through `addString` which deduplicates them in the
data section. `visitProcCall` recognizes the built‑in `new`, `dispose` and
`writeln` routines, generating calls to `malloc`, `free` or either `puts` or
`printf` respectively【F:src/visitors/codegen.cpp†L275-L469】.

### Control Flow

`makeLabel` generates unique labels for jumps. Conditional and loop statements
emit comparisons and branches reflecting `if`, `while`, `for` and `repeat`
structure, sometimes optimized when numeric constants are present【F:src/visitors/codegen.cpp†L482-L688】.

`collectVars` runs before code generation to compute storage requirements and
set flags for needed runtime functions. It traverses every AST node, populating
maps for variable sizes, pointer types and string constants【F:src/visitors/codegen.cpp†L766-L955】.

### Stage 5: Execution

The executor interface currently acts as a stub. `Executor::run` accepts assembly as input and would execute it, returning the program output, but the function body is empty in this repository【F:src/executor/executor.cpp†L1-L6】.

---

## API Server

The `src/api.cpp` file implements an HTTP service using Crow. The server exposes two routes: `/health` for a basic check and `/compile` to compile submitted Pascal code. The handler performs lexing, parsing, validation, code generation, and execution in sequence. Tokens, AST validity, assembly text, and program output are encoded as JSON. If an error occurs, the response also includes the offending line and column【F:src/api.cpp†L10-L135】.

---

## Frontend Application

A companion repository named `Pascal-compiler-app` provides a web-based interface written in Next.js. The central `PascalCompiler` component hosts a text area, preset program selector, and compile button. It communicates with the API using the `compilePascal` helper function which POSTs the source code to `${env.NEXT_PUBLIC_API_URL}/compile` and parses the JSON result【F:Pascal-compiler-app/src/components/pascal-compiler.tsx†L1-L66】【F:Pascal-compiler-app/src/lib/compiler-api.ts†L15-L31】.

The frontend displays tokens, AST as a tree visualization, generated assembly, and the program output. If compilation fails, an error card shows the message along with line and column information.

---

## Testing Strategy

The project relies on extensive GoogleTest suites located under `tests/`. According to `AGENTS.md`, each test manually specifies the expected tokens, AST structure, generated assembly, and output. This approach allows deterministic comparison but requires significant boilerplate for every test case. Tests can be run with `make tests` or for individual files via `make tests FILE=name.cpp`【F:AGENTS.md†L40-L59】.

Running the full test suite in the provided environment leads to several failures, producing assertions and an AddressSanitizer error as shown in the truncated logs:

```
Expected equality of these values:
  tokens[i].lexeme
    Which is: "."
  expected_tokens[i].lexeme
    Which is: "end"
...
==7993==ABORTING
make: *** [Makefile:78: tests] Error 1
```
【c64057†L1-L72】

---

## Conclusion

This codebase demonstrates how to build a simple Pascal compiler and expose it via both a command line interface and an HTTP API. The compiler features a multi-stage pipeline: tokenization, recursive descent parsing with backtracking assistance, AST validation with scoped symbol tracking, and x86 assembly generation. A Next.js frontend enables users to experiment with Pascal snippets in the browser. Although the executor is currently a stub and tests fail in this environment, the project lays a solid foundation for further enhancements such as full runtime integration, richer type checking, and extended language support.

