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

using crow::json::wvalue;
namespace pascal {

// Recursively convert any ASTNode to JSON
static wvalue node_to_wvalue(const ASTNode *node) {
  if (!node)
    return wvalue::empty_object(); // {} :contentReference[oaicite:5]{index=5}

  wvalue obj = wvalue::empty_object(); // initialize object
                                       // :contentReference[oaicite:6]{index=6}
  obj["kind"] = static_cast<int>(
      node->kind);          // enum as int :contentReference[oaicite:7]{index=7}
  obj["line"] = node->line; // numeric :contentReference[oaicite:8]{index=8}
  obj["column"] = node->column; // numeric :contentReference[oaicite:9]{index=9}

  switch (node->kind) {

  case NodeKind::TypeDefinition: {
    auto td = static_cast<const TypeDefinition *>(node);
    obj["name"] = td->name; // string :contentReference[oaicite:13]{index=13}
    obj["type"] = node_to_wvalue(td->type.get());
    break;
  }

  case NodeKind::VarSection: {
    auto vs = static_cast<const VarSection *>(node);
    wvalue::list decls; // alias for std::vector<wvalue>
                        // :contentReference[oaicite:12]{index=12}
    for (const auto &d : vs->declarations)
      decls.push_back(node_to_wvalue(&d));
    obj["declarations"] = std::move(decls);
    break;
  }

  case NodeKind::Program: {
    auto p = static_cast<const Program *>(node);
    obj["name"] = p->name; // string :contentReference[oaicite:10]{index=10}
    obj["block"] = node_to_wvalue(p->block.get());
    break;
  }
  case NodeKind::Block: {
    auto b = static_cast<const Block *>(node);
    // declarations array
    wvalue::list decls; // alias for std::vector<wvalue>
                        // :contentReference[oaicite:11]{index=11}
    for (auto &d : b->declarations)
      decls.push_back(node_to_wvalue(d.get()));
    obj["declarations"] = std::move(decls);

    // statements array
    wvalue::list stmts;
    for (auto &s : b->statements)
      stmts.push_back(node_to_wvalue(s.get()));
    obj["statements"] = std::move(stmts);
    break;
  }
  case NodeKind::VarDecl: {
    auto vd = static_cast<const VarDecl *>(node);
    obj["names"] = node_to_wvalue(&vd->names);
    obj["type"] = node_to_wvalue(vd->type.get());
    break;
  }
  case NodeKind::ConstDecl: {
    auto cd = static_cast<const ConstDecl *>(node);
    obj["name"] = cd->name;
    obj["value"] = node_to_wvalue(cd->value.get());
    break;
  }
  case NodeKind::IdentifierList: {
    auto il = static_cast<const IdentifierList *>(node);
    wvalue::list ids;
    for (auto &id : il->identifiers)
      ids.push_back(id);
    obj["identifiers"] = std::move(ids);
    break;
  }
  case NodeKind::TypeDecl: {
    auto td = static_cast<const TypeDecl *>(node);
    wvalue::list defs;
    for (auto &def : td->definitions) {
      auto def_obj = node_to_wvalue(&def);
      defs.push_back(std::move(def_obj));
    }
    obj["definitions"] = std::move(defs);
    break;
  }
  case NodeKind::ProcedureDecl: {
    auto pd = static_cast<const ProcedureDecl *>(node);
    obj["name"] = pd->name;
    wvalue::list pars;
    for (auto &p : pd->params)
      pars.push_back(node_to_wvalue(p.get()));
    obj["params"] = std::move(pars);
    obj["body"] = node_to_wvalue(pd->body.get());
    break;
  }
  case NodeKind::FunctionDecl: {
    auto fd = static_cast<const FunctionDecl *>(node);
    obj["name"] = fd->name;
    wvalue::list fpars;
    for (auto &p : fd->params)
      fpars.push_back(node_to_wvalue(p.get()));
    obj["params"] = std::move(fpars);
    obj["returnType"] = node_to_wvalue(fd->returnType.get());
    obj["body"] = node_to_wvalue(fd->body.get());
    break;
  }
  case NodeKind::ParamDecl: {
    auto pd = static_cast<const ParamDecl *>(node);
    wvalue::list names;
    for (auto &n : pd->names)
      names.push_back(n);
    obj["names"] = std::move(names);
    obj["type"] = node_to_wvalue(pd->type.get());
    break;
  }
  case NodeKind::CompoundStmt: {
    auto cs = static_cast<const CompoundStmt *>(node);
    wvalue::list ss;
    for (auto &s : cs->statements)
      ss.push_back(node_to_wvalue(s.get()));
    obj["statements"] = std::move(ss);
    break;
  }
  case NodeKind::AssignStmt: {
    auto as = static_cast<const AssignStmt *>(node);
    obj["target"] = node_to_wvalue(as->target.get());
    obj["value"] = node_to_wvalue(as->value.get());
    break;
  }
  case NodeKind::ProcCall: {
    auto pc = static_cast<const ProcCall *>(node);
    obj["name"] = pc->name;
    wvalue::list args;
    for (auto &a : pc->args)
      args.push_back(node_to_wvalue(a.get()));
    obj["args"] = std::move(args);
    break;
  }
  case NodeKind::IfStmt: {
    auto iff = static_cast<const IfStmt *>(node);
    obj["condition"] = node_to_wvalue(iff->condition.get());
    obj["thenBranch"] = node_to_wvalue(iff->thenBranch.get());
    if (iff->elseBranch)
      obj["elseBranch"] = node_to_wvalue(iff->elseBranch.get());
    break;
  }
  case NodeKind::WhileStmt: {
    auto ws = static_cast<const WhileStmt *>(node);
    obj["condition"] = node_to_wvalue(ws->condition.get());
    obj["body"] = node_to_wvalue(ws->body.get());
    break;
  }
  case NodeKind::ForStmt: {
    auto fs = static_cast<const ForStmt *>(node);
    obj["init"] = node_to_wvalue(fs->init.get());
    obj["downto"] = fs->downto;
    obj["limit"] = node_to_wvalue(fs->limit.get());
    obj["body"] = node_to_wvalue(fs->body.get());
    break;
  }
  case NodeKind::RepeatStmt: {
    auto rs = static_cast<const RepeatStmt *>(node);
    wvalue::list body;
    for (auto &s : rs->body)
      body.push_back(node_to_wvalue(s.get()));
    obj["body"] = std::move(body);
    obj["condition"] = node_to_wvalue(rs->condition.get());
    break;
  }
  case NodeKind::CaseStmt: {
    auto cs = static_cast<const CaseStmt *>(node);
    obj["expr"] = node_to_wvalue(cs->expr.get());
    wvalue::list cases;
    for (auto &c : cs->cases)
      cases.push_back(node_to_wvalue(c.get()));
    obj["cases"] = std::move(cases);
    break;
  }
  case NodeKind::WithStmt: {
    auto ws = static_cast<const WithStmt *>(node);
    obj["record"] = node_to_wvalue(ws->recordExpr.get());
    obj["body"] = node_to_wvalue(ws->body.get());
    break;
  }
  case NodeKind::BinaryExpr: {
    auto be = static_cast<const BinaryExpr *>(node);
    obj["left"] = node_to_wvalue(be->left.get());
    obj["op"] = be->op;
    obj["right"] = node_to_wvalue(be->right.get());
    break;
  }
  case NodeKind::UnaryExpr: {
    auto ue = static_cast<const UnaryExpr *>(node);
    obj["op"] = ue->op;
    obj["operand"] = node_to_wvalue(ue->operand.get());
    break;
  }
  case NodeKind::LiteralExpr: {
    auto le = static_cast<const LiteralExpr *>(node);
    obj["value"] = le->value;
    break;
  }
  case NodeKind::VariableExpr: {
    auto ve = static_cast<const VariableExpr *>(node);
    obj["name"] = ve->name;
    wvalue::list sels;
    for (auto &s : ve->selectors) {
      wvalue sel = wvalue::empty_object();
      sel["kind"] = static_cast<int>(s.kind);
      if (s.kind == VariableExpr::Selector::Kind::Field)
        sel["field"] = s.field;
      else if (s.kind == VariableExpr::Selector::Kind::Index)
        sel["index"] = node_to_wvalue(s.index.get());
      sels.push_back(std::move(sel));
    }
    obj["selectors"] = std::move(sels);
    break;
  }
  case NodeKind::Range: {
    auto rg = static_cast<const Range *>(node);
    obj["start"] = rg->start;
    obj["end"] = rg->end;
    break;
  }
  case NodeKind::SimpleTypeSpec: {
    auto ts = static_cast<const SimpleTypeSpec *>(node);
    obj["basic"] = static_cast<int>(ts->basic);
    obj["name"] = ts->name;
    break;
  }
  case NodeKind::ArrayTypeSpec: {
    auto at = static_cast<const ArrayTypeSpec *>(node);
    wvalue::list ranges;
    for (auto &r : at->ranges)
      ranges.push_back(node_to_wvalue(&r));
    obj["ranges"] = std::move(ranges);
    obj["elementType"] = node_to_wvalue(at->elementType.get());
    break;
  }
  case NodeKind::RecordTypeSpec: {
    auto rt = static_cast<const RecordTypeSpec *>(node);
    wvalue::list fields;
    for (auto &f : rt->fields)
      fields.push_back(node_to_wvalue(f.get()));
    obj["fields"] = std::move(fields);
    break;
  }
  case NodeKind::PointerTypeSpec: {
    auto pt = static_cast<const PointerTypeSpec *>(node);
    obj["refType"] = node_to_wvalue(pt->refType.get());
    break;
  }
  case NodeKind::CaseLabel: {
    auto cl = static_cast<const CaseLabel *>(node);
    wvalue::list consts;
    for (auto &c : cl->constants)
      consts.push_back(node_to_wvalue(c.get()));
    obj["constants"] = std::move(consts);
    obj["stmt"] = node_to_wvalue(cl->stmt.get());
    break;
  }
  case NodeKind::NewExpr: {
    auto ne = static_cast<const NewExpr *>(node);
    obj["variable"] = node_to_wvalue(ne->variable.get());
    break;
  }
  case NodeKind::DisposeExpr: {
    auto de = static_cast<const DisposeExpr *>(node);
    obj["variable"] = node_to_wvalue(de->variable.get());
    break;
  }
  default:
    break;
  }
  return obj;
}

// Public API: convert entire AST to JSON
inline wvalue ast_to_wvalue(const AST &ast) {
  if (!ast.valid || !ast.root)
    return wvalue::empty_object();
  return node_to_wvalue(ast.root.get());
}
} // namespace pascal

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
