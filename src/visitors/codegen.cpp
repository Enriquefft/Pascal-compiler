#include "visitors/codegen.hpp"
#include "parser/ast.hpp"
#include <sstream>

namespace pascal {

std::string CodeGenerator::generate(const AST &ast) {
    m_output.clear();
    if (ast.root)
        ast.root->accept(*this);
    return m_output;
}

void CodeGenerator::visitProgram(const Program &node) {
    emit("section .bss\n");
    if (node.block) {
        for (const auto &decl : node.block->declarations) {
            if (decl)
                decl->accept(*this);
        }
    }
    emit("\nsection .text\n");
    emit("global main\n");
    emit("main:\n");
    if (node.block) {
        for (const auto &stmt : node.block->statements) {
            if (stmt)
                stmt->accept(*this);
        }
    }
    emit("    ret\n");
}

void CodeGenerator::visitBlock(const Block &node) {
    for (const auto &stmt : node.statements) {
        if (stmt)
            stmt->accept(*this);
    }
}

void CodeGenerator::visitVarDecl(const VarDecl &node) {
    for (const auto &n : node.names) {
        std::ostringstream oss;
        oss << n << ":    resq    1\n";
        emit(oss.str());
    }
}

void CodeGenerator::visitCompoundStmt(const CompoundStmt &node) {
    for (const auto &s : node.statements) {
        if (s)
            s->accept(*this);
    }
}

void CodeGenerator::genExpr(const Expression *expr) {
    if (!expr)
        return;
    switch (expr->kind) {
    case NodeKind::LiteralExpr: {
        const auto *lit = static_cast<const LiteralExpr *>(expr);
        emit("    mov    rax, " + lit->value + "\n");
        break;
    }
    case NodeKind::VariableExpr: {
        const auto *var = static_cast<const VariableExpr *>(expr);
        emit("    mov    rax, [" + var->name + "]\n");
        break;
    }
    case NodeKind::BinaryExpr: {
        const auto *bin = static_cast<const BinaryExpr *>(expr);
        genExpr(bin->left.get());
        emit("    push   rax\n");
        genExpr(bin->right.get());
        emit("    mov    rbx, rax\n");
        emit("    pop    rax\n");
        if (bin->op == "+")
            emit("    add    rax, rbx\n");
        else if (bin->op == "-")
            emit("    sub    rax, rbx\n");
        else if (bin->op == "*")
            emit("    imul   rax, rbx\n");
        break;
    }
    default:
        break;
    }
}

void CodeGenerator::visitAssignStmt(const AssignStmt &node) {
    const auto *var = dynamic_cast<const VariableExpr *>(node.target.get());
    if (!var)
        return;

    if (node.value->kind == NodeKind::LiteralExpr) {
        const auto *lit = static_cast<const LiteralExpr *>(node.value.get());
        emit("    mov    qword [" + var->name + "], " + lit->value + "\n");
    } else {
        genExpr(node.value.get());
        emit("    mov    [" + var->name + "], rax\n");
    }
}

void CodeGenerator::emit(const std::string &text) {
    m_output += text;
}

} // namespace pascal
