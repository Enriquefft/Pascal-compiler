#include "parser/parser.hpp"
#include <cstdlib>

namespace pascal {

Parser::Parser(const std::vector<Token> &tokens) : m_tokens(tokens) {}

const Token &Parser::advance() {
  if (!isAtEnd())
    ++m_current;
  return m_tokens[m_current - 1];
}

const Token &Parser::peek() const { return m_tokens[m_current]; }

bool Parser::match(TokenType type) {
  if (peek().type == type) {
    advance();
    return true;
  }
  return false;
}

bool Parser::isAtEnd() const { return peek().type == TokenType::EndOfFile; }

std::unique_ptr<Program> Parser::parseProgram() {
  auto block = parseBlock();
  return std::make_unique<Program>("test", std::move(block));
}

AST Parser::parse() {
  AST ast{};
  ast.root = parseProgram();
  ast.valid = ast.root != nullptr;
  return ast;
}

std::unique_ptr<Block> Parser::parseBlock() {
  std::vector<std::unique_ptr<Declaration>> decls;
  std::vector<std::unique_ptr<Statement>> stmts;

  while (!isAtEnd()) {
    if (peek().type == TokenType::Var || peek().type == TokenType::Function ||
        peek().type == TokenType::Procedure) {
      auto decl = parseDeclaration();
      if (decl)
        decls.push_back(std::move(decl));
    } else {
      break;
    }
  }

  while (!isAtEnd() && peek().type != TokenType::End &&
         peek().type != TokenType::Dot) {
    auto stmt = parseStatement();
    if (stmt)
      stmts.push_back(std::move(stmt));
  }

  if (match(TokenType::End)) {
    match(TokenType::Dot);
  }

  return std::make_unique<Block>(std::move(decls), std::move(stmts));
}

std::unique_ptr<Declaration> Parser::parseDeclaration() {
  if (match(TokenType::Var)) {
    std::vector<std::string> names;
    if (peek().type == TokenType::Identifier) {
      names.push_back(advance().lexeme);
    }
    match(TokenType::Colon);
    auto type = parseTypeSpec();
    match(TokenType::Semicolon);
    return std::make_unique<VarDecl>(std::move(names), std::move(type));
  }

  if (match(TokenType::Function)) {
    std::string name;
    if (peek().type == TokenType::Identifier)
      name = advance().lexeme;
    std::vector<std::unique_ptr<ParamDecl>> params;
    if (match(TokenType::LeftParen)) {
      while (peek().type != TokenType::RightParen && !isAtEnd()) {
        std::vector<std::string> pnames;
        if (peek().type == TokenType::Identifier)
          pnames.push_back(advance().lexeme);
        match(TokenType::Colon);
        auto ptype = parseTypeSpec();
        params.push_back(
            std::make_unique<ParamDecl>(std::move(pnames), std::move(ptype)));
        match(TokenType::Semicolon);
      }
      match(TokenType::RightParen);
    }
    match(TokenType::Colon);
    auto ret = parseTypeSpec();
    match(TokenType::Semicolon);
    auto body = parseBlock();
    match(TokenType::Semicolon);
    return std::make_unique<FunctionDecl>(std::move(name), std::move(params),
                                         std::move(ret), std::move(body));
  }

  if (match(TokenType::Procedure)) {
    std::string name;
    if (peek().type == TokenType::Identifier)
      name = advance().lexeme;
    std::vector<std::unique_ptr<ParamDecl>> params;
    if (match(TokenType::LeftParen)) {
      while (peek().type != TokenType::RightParen && !isAtEnd()) {
        std::vector<std::string> pnames;
        if (peek().type == TokenType::Identifier)
          pnames.push_back(advance().lexeme);
        match(TokenType::Colon);
        auto ptype = parseTypeSpec();
        params.push_back(
            std::make_unique<ParamDecl>(std::move(pnames), std::move(ptype)));
        match(TokenType::Semicolon);
      }
      match(TokenType::RightParen);
    }
    match(TokenType::Semicolon);
    auto body = parseBlock();
    match(TokenType::Semicolon);
    return std::make_unique<ProcedureDecl>(std::move(name), std::move(params),
                                           std::move(body));
  }

  // Unknown declaration - skip token
  advance();
  return nullptr;
}

static bool is_op(TokenType t) {
  return t == TokenType::Plus || t == TokenType::Minus || t == TokenType::Star ||
         t == TokenType::Slash || t == TokenType::Equal || t == TokenType::Less ||
         t == TokenType::Greater || t == TokenType::LessEqual ||
         t == TokenType::GreaterEqual || t == TokenType::Div ||
         t == TokenType::Mod || t == TokenType::And || t == TokenType::Or;
}

std::unique_ptr<Expression> Parser::parseExpression() {
  // parse literal or variable
  std::unique_ptr<Expression> left;
  if (match(TokenType::Number)) {
    std::string num = m_tokens[m_current - 1].lexeme;
    if (match(TokenType::Dot)) {
      num += ".";
      if (peek().type == TokenType::Number) {
        num += advance().lexeme;
      }
    }
    left = std::make_unique<LiteralExpr>(num);
  } else if (match(TokenType::Identifier)) {
    std::string id = m_tokens[m_current - 1].lexeme;
    if (id == "'") {
      std::string val = "'";
      while (!isAtEnd() && peek().lexeme != "'")
        val += advance().lexeme;
      match(TokenType::Identifier); // closing quote
      val += "'";
      left = std::make_unique<LiteralExpr>(val);
    } else {
      std::vector<VariableExpr::Selector> sels;
      while (match(TokenType::Caret)) {
        sels.emplace_back("", VariableExpr::Selector::Kind::Pointer);
      }
      while (match(TokenType::LeftBracket)) {
        auto idx = parseExpression();
        match(TokenType::RightBracket);
        sels.emplace_back(std::move(idx));
      }
      left = std::make_unique<VariableExpr>(id, std::move(sels));
    }
  } else if (match(TokenType::LeftParen)) {
    left = parseExpression();
    match(TokenType::RightParen);
  } else {
    left = std::make_unique<LiteralExpr>("0");
  }

  if (is_op(peek().type) || (peek().type == TokenType::Less &&
                              m_current + 1 < m_tokens.size() &&
                              m_tokens[m_current + 1].type == TokenType::Greater)) {
    std::string op;
    if (match(TokenType::Less) && match(TokenType::Greater)) {
      op = "<>";
    } else {
      op = advance().lexeme;
    }
    auto right = parseExpression();
    return std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
  }

  return left;
}

std::unique_ptr<Statement> Parser::parseStatement() {
  if (match(TokenType::Begin)) {
    std::vector<std::unique_ptr<Statement>> stmts;
    while (!isAtEnd() && peek().type != TokenType::End) {
      auto st = parseStatement();
      if (st)
        stmts.push_back(std::move(st));
    }
    match(TokenType::End);
    match(TokenType::Semicolon);
    return std::make_unique<CompoundStmt>(std::move(stmts));
  }

  if (match(TokenType::If)) {
    auto cond = parseExpression();
    match(TokenType::Then);
    auto thenBranch = parseStatement();
    std::unique_ptr<Statement> elseBranch;
    if (match(TokenType::Else)) {
      elseBranch = parseStatement();
    }
    return std::make_unique<IfStmt>(std::move(cond), std::move(thenBranch),
                                    std::move(elseBranch));
  }

  if (match(TokenType::While)) {
    auto cond = parseExpression();
    match(TokenType::Do);
    auto body = parseStatement();
    return std::make_unique<WhileStmt>(std::move(cond), std::move(body));
  }

  if (match(TokenType::Repeat)) {
    std::vector<std::unique_ptr<Statement>> body;
    while (!isAtEnd() && peek().type != TokenType::Until) {
      auto st = parseStatement();
      if (st)
        body.push_back(std::move(st));
    }
    match(TokenType::Until);
    auto cond = parseExpression();
    match(TokenType::Semicolon);
    return std::make_unique<RepeatStmt>(std::move(body), std::move(cond));
  }

  if (match(TokenType::For)) {
    auto init = std::make_unique<AssignStmt>();
    if (peek().type == TokenType::Identifier) {
      auto var = parseExpression();
      match(TokenType::Colon);
      match(TokenType::Assign);
      auto val = parseExpression();
      init = std::make_unique<AssignStmt>(std::move(var), std::move(val));
    }
    bool downto = false;
    if (match(TokenType::Downto))
      downto = true;
    else
      match(TokenType::To);
    auto limit = parseExpression();
    match(TokenType::Do);
    auto body = parseStatement();
    return std::make_unique<ForStmt>(std::move(init), downto, std::move(limit),
                                     std::move(body));
  }

  if (match(TokenType::Case)) {
    auto expr = parseExpression();
    match(TokenType::Of);
    std::vector<std::unique_ptr<CaseLabel>> cases;
    while (!isAtEnd() && peek().type != TokenType::End) {
      std::vector<std::unique_ptr<Expression>> consts;
      consts.push_back(parseExpression());
      match(TokenType::Colon);
      auto stmt = parseStatement();
      cases.push_back(
          std::make_unique<CaseLabel>(std::move(consts), std::move(stmt)));
    }
    match(TokenType::End);
    match(TokenType::Semicolon);
    return std::make_unique<CaseStmt>(std::move(expr), std::move(cases));
  }

  if (match(TokenType::New) || match(TokenType::Dispose)) {
    std::string name = m_tokens[m_current - 1].lexeme;
    match(TokenType::LeftParen);
    std::vector<std::unique_ptr<Expression>> args;
    if (peek().type != TokenType::RightParen) {
      args.push_back(parseExpression());
    }
    match(TokenType::RightParen);
    match(TokenType::Semicolon);
    return std::make_unique<ProcCall>(name, std::move(args));
  }

  if (peek().type == TokenType::Identifier) {
    auto var = parseExpression();
    match(TokenType::Colon);
    match(TokenType::Assign);
    auto val = parseExpression();
    match(TokenType::Semicolon);
    return std::make_unique<AssignStmt>(std::move(var), std::move(val));
  }

  // unknown statement - consume token
  advance();
  return nullptr;
}

std::unique_ptr<TypeSpec> Parser::parseTypeSpec() {
  if (match(TokenType::Caret)) {
    auto ref = parseTypeSpec();
    return std::make_unique<PointerTypeSpec>(std::move(ref));
  }

  if (match(TokenType::Array)) {
    std::vector<Range> ranges;
    if (match(TokenType::LeftBracket)) {
      if (match(TokenType::Number)) {
        int start = std::atoi(m_tokens[m_current - 1].lexeme.c_str());
        match(TokenType::Dot);
        match(TokenType::Dot);
        int end = 0;
        if (match(TokenType::Number))
          end = std::atoi(m_tokens[m_current - 1].lexeme.c_str());
        ranges.emplace_back(start, end);
      }
      match(TokenType::RightBracket);
    }
    match(TokenType::Of);
    auto elem = parseTypeSpec();
    return std::make_unique<ArrayTypeSpec>(std::move(ranges), std::move(elem));
  }

  if (match(TokenType::Identifier)) {
    std::string name = m_tokens[m_current - 1].lexeme;
    BasicType bt = BasicType::Integer;
    if (name == "integer")
      bt = BasicType::Integer;
    else if (name == "real")
      bt = BasicType::Real;
    else if (name == "unsigned")
      bt = BasicType::UnsignedInt;
    else if (name == "longint")
      bt = BasicType::LongInt;
    else if (name == "string")
      bt = BasicType::String;
    return std::make_unique<SimpleTypeSpec>(bt, name);
  }

  return std::make_unique<SimpleTypeSpec>(BasicType::Integer, "integer");
}

} // namespace pascal
