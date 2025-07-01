#include <crow/app.h>
#include <crow/common.h>
#include <crow/middlewares/cors.h>

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
  crow::App<crow::CORSHandler> app;
  auto& cors = app.get_middleware<crow::CORSHandler>();
  cors.global()
      .methods("GET"_method, "POST"_method, "OPTIONS"_method)
      .headers("Content-Type");

  // Health check endpoint used by CI or clients.
  CROW_ROUTE(app, "/health")([] { return "Hello world"; });

  // Compilation endpoint. Expects the Pascal source code in the request body
  // and returns a JSON object that matches the following TypeScript type:
  //
  // export type CompilationResult = {
  //   tokens?: { token_name: string; token_content: string }[];
  //   ast?: AST;
  //   asm?: string;
  //   output?: string;
  //   error?: string;
  //   line?: number;
  //   column?: number;
  // };
  CROW_ROUTE(app, "/compile").methods(crow::HTTPMethod::Post)(
      [](const crow::request& req) {
        const std::string& code = req.body;

        crow::json::wvalue result;
        std::vector<pascal::Token> tokens;
        pascal::AST ast{};
        std::string asm_code;
        std::string output;
        std::string error;
        std::size_t errLine = 0;
        std::size_t errColumn = 0;

        try {
          // Lexing
          pascal::Lexer lexer(code);
          tokens = lexer.scanTokens();

          // Parsing
          pascal::Parser parser(tokens);
          ast = parser.parse();
          if (!ast.valid || !ast.root) {
            error = "Parsing failed";
          }

          // Validation
          if (error.empty()) {
            pascal::ASTValidator validator;
            auto valRes = validator.validate(ast);
            if (!valRes.success) {
              error = valRes.message;
              errLine = valRes.line;
              errColumn = valRes.column;
            }
          }

          // Code generation and execution
          if (error.empty()) {
            pascal::CodeGenerator codegen;
            asm_code = codegen.generate(ast);

            pascal::Executor exec;
            output = exec.run(asm_code);
          }
        } catch (const std::exception& e) {
          error = e.what();
        }

        // Fill tokens regardless of success
        for (std::size_t i = 0; i < tokens.size(); ++i) {
          result["tokens"][i]["token_name"] = tokenTypeToString(tokens[i].type);
          result["tokens"][i]["token_content"] = tokens[i].lexeme;
        }

        // AST validity flag only
        result["ast"]["valid"] = ast.valid;

        if (!asm_code.empty())
          result["asm"] = asm_code;
        if (!output.empty())
          result["output"] = output;
        if (!error.empty()) {
          result["error"] = error;
          if (errLine != 0)
            result["line"] = static_cast<int>(errLine);
          if (errColumn != 0)
            result["column"] = static_cast<int>(errColumn);
        }

        return result;
      });

  app.port(PORT).multithreaded().run();
}
