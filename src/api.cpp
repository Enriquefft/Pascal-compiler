#include <crow/app.h>
#include <crow/common.h>

#include "executor/executor.hpp"
#include "parser/parser.hpp"
#include "parser/validator.hpp"
#include "scanner/lexer.hpp"
#include "visitors/codegen.hpp"

#include <string>

// Port used for the API server.
constexpr uint16_t PORT = 443;

namespace {

// Convert a token type enum to a string representation used in the JSON
// response.  The strings mirror the enum names.
std::string tokenTypeToString(pascal::TokenType type) {
  using pascal::TokenType;
  switch (type) {
  case TokenType::Plus:
    return "Plus";
  case TokenType::Minus:
    return "Minus";
  case TokenType::Star:
    return "Star";
  case TokenType::Slash:
    return "Slash";
  case TokenType::Comma:
    return "Comma";
  case TokenType::Semicolon:
    return "Semicolon";
  case TokenType::Colon:
    return "Colon";
  case TokenType::Dot:
    return "Dot";
  case TokenType::LeftParen:
    return "LeftParen";
  case TokenType::RightParen:
    return "RightParen";
  case TokenType::LeftBracket:
    return "LeftBracket";
  case TokenType::RightBracket:
    return "RightBracket";
  case TokenType::Caret:
    return "Caret";
  case TokenType::Assign:
    return "Assign";
  case TokenType::Equal:
    return "Equal";
  case TokenType::NotEqual:
    return "NotEqual";
  case TokenType::Less:
    return "Less";
  case TokenType::LessEqual:
    return "LessEqual";
  case TokenType::Greater:
    return "Greater";
  case TokenType::GreaterEqual:
    return "GreaterEqual";
  case TokenType::Range:
    return "Range";
  case TokenType::Identifier:
    return "Identifier";
  case TokenType::Number:
    return "Number";
  case TokenType::String:
    return "String";
  case TokenType::Program:
    return "Program";
  case TokenType::Var:
    return "Var";
  case TokenType::Const:
    return "Const";
  case TokenType::Type:
    return "Type";
  case TokenType::Procedure:
    return "Procedure";
  case TokenType::Function:
    return "Function";
  case TokenType::Begin:
    return "Begin";
  case TokenType::End:
    return "End";
  case TokenType::If:
    return "If";
  case TokenType::Then:
    return "Then";
  case TokenType::Else:
    return "Else";
  case TokenType::While:
    return "While";
  case TokenType::Do:
    return "Do";
  case TokenType::For:
    return "For";
  case TokenType::To:
    return "To";
  case TokenType::Downto:
    return "Downto";
  case TokenType::Repeat:
    return "Repeat";
  case TokenType::Until:
    return "Until";
  case TokenType::Case:
    return "Case";
  case TokenType::Of:
    return "Of";
  case TokenType::With:
    return "With";
  case TokenType::Record:
    return "Record";
  case TokenType::Array:
    return "Array";
  case TokenType::New:
    return "New";
  case TokenType::Dispose:
    return "Dispose";
  case TokenType::Not:
    return "Not";
  case TokenType::Div:
    return "Div";
  case TokenType::Mod:
    return "Mod";
  case TokenType::And:
    return "And";
  case TokenType::Or:
    return "Or";
  case TokenType::EndOfFile:
    return "EndOfFile";
  default:
    return "Unknown";
  }
}

} // namespace

auto main(int /*argc*/, char* /*argv*/[]) -> int {
  crow::SimpleApp app;

  // Health check endpoint used by CI or clients.
  CROW_ROUTE(app, "/health")([] { return "Hello world"; });

  // Compilation endpoint.  Expects the Pascal source code in the request body
  // and returns the tokens, AST validity, generated assembly, and execution
  // output as JSON matching the TypeScript interface provided in the project
  // description.
  CROW_ROUTE(app, "/compile").methods(crow::HTTPMethod::Post)(
      [](const crow::request& req) {
        const std::string& code = req.body;

        // Run the compiler pipeline.
        pascal::Lexer lexer(code);
        std::vector<pascal::Token> tokens = lexer.scanTokens();

        pascal::Parser parser(tokens);
        pascal::AST ast = parser.parse();

        pascal::ASTValidator validator;
        bool ast_ok = validator.validate(ast);

        pascal::CodeGenerator codegen;
        std::string asm_code = codegen.generate(ast);

        pascal::Executor exec;
        std::string output = exec.run(asm_code);

        // Build JSON response.
        crow::json::wvalue result;

        // Tokens
        for (std::size_t i = 0; i < tokens.size(); ++i) {
          result["tokens"][i]["token_name"] = tokenTypeToString(tokens[i].type);
          result["tokens"][i]["token_content"] = tokens[i].lexeme;
        }

        // AST (only validity flag and null root for now)
        if (ast.root) {
          // When a full AST is implemented, serialization can be expanded here.
          result["ast"]["root"] = crow::json::wvalue();
        } else {
          result["ast"]["root"] = nullptr;
        }
        result["ast"]["valid"] = ast_ok && ast.valid;

        result["asm"] = asm_code;
        result["output"] = output;

        return result;
      });

  app.port(PORT).multithreaded().run();
}
