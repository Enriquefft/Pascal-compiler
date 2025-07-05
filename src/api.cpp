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

auto main(int /*argc*/, char * /*argv*/[]) -> int {
  crow::App<crow::CORSHandler> app;
  auto &cors = app.get_middleware<crow::CORSHandler>();
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
  CROW_ROUTE(app, "/compile")
      .methods(crow::HTTPMethod::Post)([](const crow::request &req) {
        const std::string &code = req.body;

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

          std::cout << "AST valid: " << ast.valid << std::endl;
          std::cout << "AST root: " << (ast.root ? "not null" : "null")
                    << std::endl;

          result["ast"] = pascal::ast_to_wvalue(ast);

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
        } catch (const std::exception &e) {
          error = e.what();
        }

        // Fill tokens regardless of success
        for (unsigned int i = 0; i < tokens.size(); ++i) {
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
