#include "parser/parser.hpp"
#include <string>

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

  if (!match(TokenType::Program)) {
    return nullptr; // No program declaration found
  }

  std::string program_name = parseIdentifier();

  IdentifierList id_list;

  // possible identifier list
  if (match(TokenType::LeftParen)) {
    id_list = parseIdentifierList();

    if (!match(TokenType::RightParen)) {
      throw std::runtime_error(
          "Expected right parenthesis after identifier list");
    }
  }
  if (!match(TokenType::Semicolon)) {
    throw std::runtime_error("Expected semicolon after program name");
  }

  auto block = parseBlock();

  if (!match(TokenType::Dot)) {
    throw std::runtime_error("Expected dot at the end of program");
  }

  auto prog = std::make_unique<Program>(program_name, std::move(block));
  if (!m_tokens.empty()) {
    prog->line = m_tokens.front().line;
    prog->column = m_tokens.front().column;
  }
  return prog;
}

IdentifierList Parser::parseIdentifierList() {
  std::vector<std::string> ids;
  if (peek().type == TokenType::Identifier) {
    ids.push_back(parseIdentifier());
  }
  while (match(TokenType::Comma)) {
    if (peek().type == TokenType::Identifier) {
      ids.push_back(parseIdentifier());
    } else {
      throw std::runtime_error("Expected identifier after comma");
    }
  }
  return IdentifierList(std::move(ids));
}

std::string Parser::parseIdentifier() {
  // TODO: Handle empty identifiers
  // TODO: Handle identifiers with special characters
  // TODO: Handle reserved keywords as identifiers
  if (peek().type == TokenType::Identifier) {
    std::string id = advance().lexeme;
    if (id.empty()) {
      throw std::runtime_error("Expected identifier, found empty string");
    }
    return id;
  }
  throw std::runtime_error("Expected identifier, found " + peek().lexeme);
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
  std::size_t startLine = peek().line;
  std::size_t startCol = peek().column;

  while (!isAtEnd()) {
    if (peek().type == TokenType::Var || peek().type == TokenType::Function ||
        peek().type == TokenType::Procedure || peek().type == TokenType::Type) {
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

  if (!match(TokenType::End)) {
    throw std::runtime_error("Expected 'end' or '.' at the end of block");
  }

  auto blk = std::make_unique<Block>(std::move(decls), std::move(stmts));
  blk->line = startLine;
  blk->column = startCol;
  return blk;
}

std::unique_ptr<Declaration> Parser::parseDeclaration() {
  if (match(TokenType::Var)) {
    Token startTok = m_tokens[m_current - 1];
    std::vector<std::string> names;
    if (peek().type == TokenType::Identifier) {
      names.push_back(advance().lexeme);
    }
    match(TokenType::Colon);
    auto type = parseTypeSpec();
    match(TokenType::Semicolon);
    auto node = std::make_unique<VarDecl>(std::move(names), std::move(type));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (match(TokenType::Type)) {
    Token startTok = m_tokens[m_current - 1];
    std::string name;
    if (peek().type == TokenType::Identifier)
      name = advance().lexeme;
    match(TokenType::Equal);
    auto type = parseTypeSpec();
    match(TokenType::Semicolon);
    auto node = std::make_unique<TypeDecl>(std::move(name), std::move(type));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (match(TokenType::Function)) {
    Token startTok = m_tokens[m_current - 1];
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
    auto node = std::make_unique<FunctionDecl>(
        std::move(name), std::move(params), std::move(ret), std::move(body));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (match(TokenType::Procedure)) {
    Token startTok = m_tokens[m_current - 1];
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
    auto node = std::make_unique<ProcedureDecl>(
        std::move(name), std::move(params), std::move(body));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  // Unknown declaration - skip token
  advance();
  return nullptr;
}

static bool is_op(TokenType t) {
  return t == TokenType::Plus || t == TokenType::Minus ||
         t == TokenType::Star || t == TokenType::Slash ||
         t == TokenType::Equal || t == TokenType::Less ||
         t == TokenType::Greater || t == TokenType::LessEqual ||
         t == TokenType::GreaterEqual || t == TokenType::Div ||
         t == TokenType::Mod || t == TokenType::And || t == TokenType::Or;
}

std::unique_ptr<VariableExpr> Parser::parseVariable(std::string name) {
  std::vector<VariableExpr::Selector> sels;
  std::size_t line = m_tokens[m_current - 1].line;
  std::size_t col = m_tokens[m_current - 1].column;
  while (true) {
    if (match(TokenType::Caret)) {
      sels.emplace_back("", VariableExpr::Selector::Kind::Pointer);
    } else if (match(TokenType::LeftBracket)) {
      auto idx = parseExpression();
      match(TokenType::RightBracket);
      sels.emplace_back(std::move(idx));
    } else if (match(TokenType::Dot)) {
      std::string field;
      if (peek().type == TokenType::Identifier)
        field = advance().lexeme;
      sels.emplace_back(field, VariableExpr::Selector::Kind::Field);
    } else {
      break;
    }
  }
  auto node = std::make_unique<VariableExpr>(std::move(name), std::move(sels));
  node->line = line;
  node->column = col;
  return node;
}

std::unique_ptr<Expression> Parser::parseExpression() {
  // parse literal or variable
  std::unique_ptr<Expression> left;
  Token startTok = peek();
  if (match(TokenType::Number)) {
    std::string num = m_tokens[m_current - 1].lexeme;
    if (match(TokenType::Dot)) {
      num += ".";
      if (peek().type == TokenType::Number) {
        num += advance().lexeme;
      }
    }
    auto lit = std::make_unique<LiteralExpr>(num);
    lit->line = startTok.line;
    lit->column = startTok.column;
    left = std::move(lit);
  } else if (match(TokenType::Identifier)) {
    std::string id = m_tokens[m_current - 1].lexeme;
    if (id == "'") {
      std::string val = "'";
      while (!isAtEnd() && peek().lexeme != "'")
        val += advance().lexeme;
      match(TokenType::Identifier); // closing quote
      val += "'";
      auto lit = std::make_unique<LiteralExpr>(val);
      lit->line = startTok.line;
      lit->column = startTok.column;
      left = std::move(lit);
    } else {
      left = parseVariable(id);
    }
  } else if (match(TokenType::LeftParen)) {
    left = parseExpression();
    match(TokenType::RightParen);
  } else {
    auto lit = std::make_unique<LiteralExpr>("0");
    lit->line = startTok.line;
    lit->column = startTok.column;
    left = std::move(lit);
  }

  if (is_op(peek().type) ||
      (peek().type == TokenType::Less && m_current + 1 < m_tokens.size() &&
       m_tokens[m_current + 1].type == TokenType::Greater)) {
    std::string op;
    if (peek().type == TokenType::Less && m_current + 1 < m_tokens.size() &&
        m_tokens[m_current + 1].type == TokenType::Greater) {
      advance();
      advance();
      op = "<>";
    } else {
      op = advance().lexeme;
    }
    auto right = parseExpression();
    auto node =
        std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  return left;
}

std::unique_ptr<Statement> Parser::parseStatement() {
  Token startTok = peek();
  if (match(TokenType::Begin)) {
    std::vector<std::unique_ptr<Statement>> stmts;
    while (!isAtEnd() && peek().type != TokenType::End) {
      auto st = parseStatement();
      if (st)
        stmts.push_back(std::move(st));
    }
    match(TokenType::End);
    match(TokenType::Semicolon);
    auto node = std::make_unique<CompoundStmt>(std::move(stmts));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (match(TokenType::If)) {
    auto cond = parseExpression();
    match(TokenType::Then);
    auto thenBranch = parseStatement();
    std::unique_ptr<Statement> elseBranch;
    if (match(TokenType::Else)) {
      elseBranch = parseStatement();
    }
    auto node = std::make_unique<IfStmt>(std::move(cond), std::move(thenBranch),
                                         std::move(elseBranch));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (match(TokenType::While)) {
    auto cond = parseExpression();
    match(TokenType::Do);
    auto body = parseStatement();
    auto node = std::make_unique<WhileStmt>(std::move(cond), std::move(body));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
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
    auto node = std::make_unique<RepeatStmt>(std::move(body), std::move(cond));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
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
    auto node = std::make_unique<ForStmt>(std::move(init), downto,
                                          std::move(limit), std::move(body));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
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
    auto node = std::make_unique<CaseStmt>(std::move(expr), std::move(cases));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (match(TokenType::With)) {
    auto recordVar = parseExpression();
    match(TokenType::Do);
    auto body = parseStatement();
    auto node =
        std::make_unique<WithStmt>(std::move(recordVar), std::move(body));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
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
    auto node = std::make_unique<ProcCall>(name, std::move(args));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (peek().type == TokenType::Identifier) {
    std::string id = advance().lexeme;
    if (peek().type == TokenType::LeftParen) {
      match(TokenType::LeftParen);
      std::vector<std::unique_ptr<Expression>> args;
      if (peek().type != TokenType::RightParen) {
        args.push_back(parseExpression());
        while (match(TokenType::Comma)) {
          args.push_back(parseExpression());
        }
      }
      match(TokenType::RightParen);
      match(TokenType::Semicolon);
      auto node = std::make_unique<ProcCall>(id, std::move(args));
      node->line = startTok.line;
      node->column = startTok.column;
      return node;
    }

    auto var = parseVariable(id);
    match(TokenType::Colon);
    match(TokenType::Assign);
    auto val = parseExpression();
    match(TokenType::Semicolon);
    auto node = std::make_unique<AssignStmt>(std::move(var), std::move(val));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  // unknown statement - consume token
  advance();
  return nullptr;
}

std::unique_ptr<TypeSpec> Parser::parseTypeSpec() {
  Token startTok = peek();
  if (match(TokenType::Caret)) {
    auto ref = parseTypeSpec();
    auto node = std::make_unique<PointerTypeSpec>(std::move(ref));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (match(TokenType::Array)) {
    std::vector<Range> ranges;
    if (match(TokenType::LeftBracket)) {
      if (match(TokenType::Number)) {
        int start = std::stoi(m_tokens[m_current - 1].lexeme);
        match(TokenType::Dot);
        match(TokenType::Dot);
        int end = 0;
        if (match(TokenType::Number))
          end = std::stoi(m_tokens[m_current - 1].lexeme);
        ranges.emplace_back(start, end);
      }
      match(TokenType::RightBracket);
    }
    match(TokenType::Of);
    auto elem = parseTypeSpec();
    auto node =
        std::make_unique<ArrayTypeSpec>(std::move(ranges), std::move(elem));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }

  if (match(TokenType::Record)) {
    std::vector<std::unique_ptr<VarDecl>> fields;
    while (!isAtEnd() && peek().type != TokenType::End) {
      std::vector<std::string> names;
      if (peek().type == TokenType::Identifier)
        names.push_back(advance().lexeme);
      match(TokenType::Colon);
      auto ftype = parseTypeSpec();
      match(TokenType::Semicolon);
      fields.emplace_back(
          std::make_unique<VarDecl>(std::move(names), std::move(ftype)));
    }
    match(TokenType::End);
    auto node = std::make_unique<RecordTypeSpec>(std::move(fields));
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
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
    auto node = std::make_unique<SimpleTypeSpec>(bt, name);
    node->line = startTok.line;
    node->column = startTok.column;
    return node;
  }
  auto node = std::make_unique<SimpleTypeSpec>(BasicType::Integer, "integer");
  node->line = startTok.line;
  node->column = startTok.column;
  return node;
}

} // namespace pascal
