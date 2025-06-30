# Pascal Compiler

This project implements a simple Pascal compiler written in C++23. The goal
is to support core Pascal syntax and several extensions such as floating point
numbers, unsigned integers, `longint`, dynamic memory handling, strings, arrays,
structures, and pointers.

## Compiling

Use the provided `Makefile` with a C++23 compiler. Set `CXX` to `g++`
or `clang++` as needed (g++ is the default).
```bash
make        # builds the compiler
make tests  # runs example tests
make clean  # removes build artefacts
```

The resulting binary is placed in `build/compiler`.

## Using the Compiler

Once built, invoke the compiler with a Pascal source file:

```bash
./build/compiler my_program.pas
```

The compiler currently outputs a placeholder message and can be extended to
generate assembly.

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
