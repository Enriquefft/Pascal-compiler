#include "test_common.hpp"

TEST(InvalidCodeTests, TypeDeclMissingName) {
  std::string input_str = "type = integer;";
  std::vector<Token> expected_tokens = {{TT::Type, "type"},
                                        {TT::Equal, "="},
                                        {TT::Identifier, "integer"},
                                        {TT::Semicolon, ";"},
                                        {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  decls.emplace_back(std::make_unique<pascal::TypeDecl>(
      "", std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                                   "integer")));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "TypeDecl missing name");
}

TEST(InvalidCodeTests, ProcedureMissingName) {
  std::string input_str = "procedure ; begin end;";
  std::vector<Token> expected_tokens = {
      {TT::Procedure, "procedure"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},         {TT::End, "end"},
      {TT::Semicolon, ";"},         {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::ParamDecl>> params;
  std::vector<std::unique_ptr<pascal::Statement>> body_stmts;
  body_stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(
      std::vector<std::unique_ptr<pascal::Statement>>{}));
  auto body_block = std::make_unique<pascal::Block>(
      std::vector<std::unique_ptr<pascal::Declaration>>{},
      std::move(body_stmts));
  decls.emplace_back(std::make_unique<pascal::ProcedureDecl>(
      "", std::move(params), std::move(body_block)));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "ProcedureDecl missing name");
}

TEST(InvalidCodeTests, FunctionMissingName) {
  std::string input_str = "function : integer; begin end;";
  std::vector<Token> expected_tokens = {
      {TT::Function, "function"},  {TT::Colon, ":"},
      {TT::Identifier, "integer"}, {TT::Semicolon, ";"},
      {TT::Begin, "begin"},        {TT::End, "end"},
      {TT::Semicolon, ";"},        {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::ParamDecl>> params;
  std::vector<std::unique_ptr<pascal::Statement>> body_stmts;
  body_stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(
      std::vector<std::unique_ptr<pascal::Statement>>{}));
  auto body_block = std::make_unique<pascal::Block>(
      std::vector<std::unique_ptr<pascal::Declaration>>{},
      std::move(body_stmts));
  decls.emplace_back(std::make_unique<pascal::FunctionDecl>(
      "", std::move(params),
      std::make_unique<pascal::SimpleTypeSpec>(pascal::BasicType::Integer,
                                               "integer"),
      std::move(body_block)));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "FunctionDecl missing name");
}

TEST(InvalidCodeTests, ParamMissingName) {
  std::string input_str = "procedure p(:integer); begin end;";
  std::vector<Token> expected_tokens = {{TT::Procedure, "procedure"},
                                        {TT::Identifier, "p"},
                                        {TT::LeftParen, "("},
                                        {TT::Colon, ":"},
                                        {TT::Identifier, "integer"},
                                        {TT::RightParen, ")"},
                                        {TT::Semicolon, ";"},
                                        {TT::Begin, "begin"},
                                        {TT::End, "end"},
                                        {TT::Semicolon, ";"},
                                        {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::ParamDecl>> params;
  params.emplace_back(std::make_unique<pascal::ParamDecl>(
      std::vector<std::string>{}, std::make_unique<pascal::SimpleTypeSpec>(
                                      pascal::BasicType::Integer, "integer")));
  std::vector<std::unique_ptr<pascal::Statement>> body_stmts;
  body_stmts.emplace_back(std::make_unique<pascal::CompoundStmt>(
      std::vector<std::unique_ptr<pascal::Statement>>{}));
  auto body_block = std::make_unique<pascal::Block>(
      std::vector<std::unique_ptr<pascal::Declaration>>{},
      std::move(body_stmts));
  decls.emplace_back(std::make_unique<pascal::ProcedureDecl>(
      "p", std::move(params), std::move(body_block)));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "ParamDecl missing names");
}

TEST(InvalidCodeTests, CaseLabelMissingStmt) {
  std::string input_str = "case a of 1: ; end;";
  std::vector<Token> expected_tokens = {
      {TT::Case, "case"}, {TT::Identifier, "a"}, {TT::Of, "of"},
      {TT::Number, "1"},  {TT::Colon, ":"},      {TT::Semicolon, ";"},
      {TT::End, "end"},   {TT::Semicolon, ";"},  {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  {
    std::vector<std::unique_ptr<pascal::Expression>> consts;
    consts.emplace_back(std::make_unique<pascal::LiteralExpr>("1"));
    std::vector<std::unique_ptr<pascal::CaseLabel>> labels;
    labels.emplace_back(
        std::make_unique<pascal::CaseLabel>(std::move(consts), nullptr));
    stmts.emplace_back(std::make_unique<pascal::CaseStmt>(
        std::make_unique<pascal::VariableExpr>("a"), std::move(labels)));
  }
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "CaseLabel missing statement");
}

TEST(InvalidCodeTests, EmptyRecord) {
  std::string input_str = "type R = record end;";
  std::vector<Token> expected_tokens = {
      {TT::Type, "type"},     {TT::Identifier, "R"}, {TT::Equal, "="},
      {TT::Record, "record"}, {TT::End, "end"},      {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  decls.emplace_back(std::make_unique<pascal::TypeDecl>(
      "R", std::make_unique<pascal::RecordTypeSpec>(
               std::vector<std::unique_ptr<pascal::VarDecl>>{})));
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "RecordTypeSpec with no fields");
}

TEST(InvalidCodeTests, ForMissingBody) {
  std::string input_str = "for i:=1 to 10 do ;";
  std::vector<Token> expected_tokens = {
      {TT::For, "for"},   {TT::Identifier, "i"}, {TT::Colon, ":"},
      {TT::Assign, "="},  {TT::Number, "1"},     {TT::To, "to"},
      {TT::Number, "10"}, {TT::Do, "do"},        {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::ForStmt>(
      std::make_unique<pascal::AssignStmt>(
          std::make_unique<pascal::VariableExpr>("i"),
          std::make_unique<pascal::LiteralExpr>("1")),
      false, std::make_unique<pascal::LiteralExpr>("10"), nullptr));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "ForStmt missing body");
}

TEST(InvalidCodeTests, WhileMissingBody) {
  std::string input_str = "while a>0 do ;";
  std::vector<Token> expected_tokens = {
      {TT::While, "while"}, {TT::Identifier, "a"}, {TT::Greater, ">"},
      {TT::Number, "0"},    {TT::Do, "do"},        {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::WhileStmt>(
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("a"), ">",
          std::make_unique<pascal::LiteralExpr>("0")),
      nullptr));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "WhileStmt missing body");
}

TEST(InvalidCodeTests, IfMissingThenBranch) {
  std::string input_str = "if a>0 then ;";
  std::vector<Token> expected_tokens = {
      {TT::If, "if"},     {TT::Identifier, "a"}, {TT::Greater, ">"},
      {TT::Number, "0"},  {TT::Then, "then"},    {TT::Semicolon, ";"},
      {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::IfStmt>(
      std::make_unique<pascal::BinaryExpr>(
          std::make_unique<pascal::VariableExpr>("a"), ">",
          std::make_unique<pascal::LiteralExpr>("0")),
      nullptr, nullptr));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "IfStmt missing then branch");
}

TEST(InvalidCodeTests, WithMissingBody) {
  std::string input_str = "with a do ;";
  std::vector<Token> expected_tokens = {{TT::With, "with"},
                                        {TT::Identifier, "a"},
                                        {TT::Do, "do"},
                                        {TT::Semicolon, ";"},
                                        {TT::EndOfFile, ""}};

  AST expected_ast{};
  std::vector<std::unique_ptr<pascal::Declaration>> decls;
  std::vector<std::unique_ptr<pascal::Statement>> stmts;
  stmts.emplace_back(std::make_unique<pascal::WithStmt>(
      std::make_unique<pascal::VariableExpr>("a"), nullptr));
  auto block =
      std::make_unique<pascal::Block>(std::move(decls), std::move(stmts));
  expected_ast.root =
      std::make_unique<pascal::Program>("test", std::move(block));
  expected_ast.valid = true;

  test_utils::run_validation_fail(input_str, expected_tokens, expected_ast, "",
                                  "", "WithStmt missing body");
}

