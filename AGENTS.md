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
