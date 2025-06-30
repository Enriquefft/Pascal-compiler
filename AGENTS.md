# Pascal Compiler Agent instructions

This project implements a simple Pascal compiler written in C++23. The goal
is to support core Pascal syntax and several extensions such as floating point
numbers, unsigned integers, `longint`, dynamic memory handling, strings, arrays,
structures, and pointers.

## Workflow

1. **Format** all C++ files with `clang-format` if available:
   ```bash
   clang-format -i src/**/*.cpp include/**/*.hpp
   ```
2. Build the compiler:
   ```bash
   make compiler
   ```
3. Run tests with:
   ```bash
   make tests
   ```
   If tests fail due to missing dependencies or environment restrictions, mention this in the PR description.
4. Make small, focused commits using the prefixes `feat:`, `fix:`, `style:` or `chore:`.
5. Pull request summaries must describe key changes and cite updated files. Include test output in the testing section.

## Development Guidelines

- Only modify header files (`*.hpp`) when extending the compiler API. Source
  files under `src/` can remain stub implementations.
- Ensure headers declare all types and functions necessary to support the full
  Pascal grammar described in `README.md`.

## Testing
- Tests rely on hard‑coded expectations for tokens, AST validity, generated
  assembly, and execution output.
- For each tests, there must exist a hardcoded: token list, ast, asm & expected output, for example:
```cpp
TEST(ExpressionTests, Expr2) {

    auto input_str = "begin b := a + 1; end.";

        // Example ast, incorrect/incomplete, but for demonstration purposes
  std::vector<Token> expected_tokens_t = std::vector<Token>{
      {"begin", pascal::TokenType::Begin},
      {"b", pascal::TokenType::Identifier},
      {":", pascal::TokenType::Colon},
      {"=", pascal::TokenType::Assign},
      {"a", pascal::TokenType::Identifier},
      {"+", pascal::TokenType::Plus},
      {"1", pascal::TokenType::Number},
      {";", pascal::TokenType::Semicolon},
      {"end", pascal::TokenType::End},
      {".", pascal::TokenType::Dot} {"begin", pascal::TokenType::Begin},
      {"b", pascal::TokenType::Identifier},
      {":", pascal::TokenType::Colon},
      {"=", pascal::TokenType::Assign},
      {"a", pascal::TokenType::Identifier},
      {"+", pascal::TokenType::Plus},
      {"1", pascal::TokenType::Number},
      {";", pascal::TokenType::Semicolon},
      {"end", pascal::TokenType::End},
      {".", pascal::TokenType::Dot}};

    AST expected_ast = {
        // Example ast, incorrect/incomplete, but for demonstration purposes
        std::make_unique<pascal::Program>("test_program",
                                      std::make_unique<pascal::Block>(
                                          std::vector<std::unique_ptr<pascal::Declaration>>{},
                                          std::vector<std::unique_ptr<pascal::Statement>>{
                                              std::make_unique<pascal::AssignStmt>(
                                                  "b",
                                                  std::make_unique<pascal::BinaryExpr>(
                                                      std::make_unique<pascal::VariableExpr>("a"),
                                                      "+",
                                                      std::make_unique<pascal::LiteralExpr>("1")))})};

        // Example ast, incorrect/incomplete, but for demonstration purposes
    std::string expected_asm = R"(
        section .text
        global _start
        _start:
            ; begin
            mov eax, 1          ; Load 1 into eax
            mov [b], eax        ; Assign to b
            ; end
            ret
        )";


    std::string expected_output = ""; // No output since there is no writeln



    run_full(input_str, expected_tokens_t, expected_ast, expected_asm, expected_output);

}
```
- Each one of this hardcoded expected outputs, will be manually created & typed, no automated function for this.


## Code Style

- Use clear comments and keep lines under 80 characters when possible.
- Organize sources under `src` and headers under `include`.
- Remove unused code and keep the repository clean of build artefacts.

## Build System

- Only the provided **Makefile** should be used. It defines the `all`,
  `compiler`, `tests`, and `clean` rules.
- Place compiled binaries in the `build/` directory.

## AGENTS.md Inheritance

- This file governs the entire repository.
- Nested `AGENTS.md` files may override these instructions for their
  directory subtree.

## Pascal Grammar

```ebnf
program         = 'program' identifier [ '(' identifier_list ')' ] ';' block '.' ;

block           = declaration_part 'begin' statement_list 'end' ;

declaration_part= [ const_section ]
                [ type_section ]
                [ var_section ]
                { routine_decl } ;

const_section   = 'const' const_def { const_def } ;
const_def       = identifier '=' constant ';' ;

type_section    = 'type' type_def { type_def } ;
type_def        = identifier '=' type_spec ';' ;

var_section     = 'var' var_decl { var_decl } ;
var_decl        = identifier_list ':' type_spec ';' ;

routine_decl    = procedure_decl | function_decl ;
procedure_decl  = 'procedure' identifier param_list? ';' block ';' ;
function_decl   = 'function' identifier param_list? ':' type_spec ';' block ';' ;

param_list      = '(' param_decl { ';' param_decl } ')' ;
param_decl      = identifier_list ':' type_spec ;
identifier_list = identifier { ',' identifier } ;

type_spec       = simple_type
                | array_type
                | record_type
                | pointer_type ;

simple_type     = 'integer'
                | 'longint'
                | 'unsigned'
                | 'real'
                | 'string'
                | identifier ;

array_type      = 'array' { '[' range ']' } 'of' type_spec ;
range           = number '..' number ;

record_type     = 'record' var_decl { var_decl } 'end' ;

pointer_type    = '^' identifier ;

statement_list  = statement { ';' statement } ;

statement       = assignment
                | procedure_call
                | compound
                | if_stmt
                | while_stmt
                | for_stmt
                | repeat_stmt
                | case_stmt
                | with_stmt ;

compound        = 'begin' statement_list 'end' ;

assignment      = variable ':=' expression ;

procedure_call  = identifier '(' expression_list? ')' ;

if_stmt         = 'if' expression 'then' statement [ 'else' statement ] ;

while_stmt      = 'while' expression 'do' statement ;

for_stmt        = 'for' assignment ( 'to' | 'downto' ) expression 'do' statement ;

repeat_stmt     = 'repeat' statement_list 'until' expression ;

case_stmt       = 'case' expression 'of' case_list 'end' ;
case_list       = case_label ':' statement { ';' case_label ':' statement } ;
case_label      = constant { ',' constant } ;

with_stmt       = 'with' variable 'do' statement ;

expression_list = expression { ',' expression } ;

expression      = simple_expression [ relop simple_expression ] ;
simple_expression= [ addop ] term { addop term } ;
term            = factor { mulop factor } ;

factor          = variable
                | number
                | string
                | '(' expression ')'
                | 'not' factor
                | 'new' '(' variable ')'
                | 'dispose' '(' variable ')' ;

variable        = identifier { selector } ;
selector        = '.' identifier
                | '[' expression ']'
                | '^' ;

addop           = '+' | '-' | 'or' ;
mulop           = '*' | '/' | 'div' | 'mod' | 'and' ;
relop           = '=' | '<>' | '<' | '<=' | '>' | '>=' ;

constant        = number | string | identifier ;
```

## Extensions

- **float** – support for real numbers with decimal points.
- **unsigned int** – ability to declare unsigned integer types.
- **longint** – 32‑bit signed integers.
- **dynamic memory** – `new` and `dispose` for pointer management.
- **strings** – native string type handling.
- **arrays** – fixed length array declarations and indexing.
- **struct** – record types to group fields.
- **pointers** – pointer types and dereferencing.

## Example

```
program Hello;
begin
  writeln('Hello, world!');
end.
```

The resulting (example) assembly might look like:

```
section .text
  global _start
_start:
  ; print message
```
