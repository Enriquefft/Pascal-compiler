# Agent Instructions

This repository contains a Pascal compiler implemented in **C++23**. A simple **Makefile** builds sources and runs tests.

## Workflow

1. **Format** all C++ files with `clang-format` if available:
   ```bash
   clang-format -i src/**/*.cpp include/**/*.hpp
   ```
2. Build the compiler:
   ```bash
   make compiler
   ```
3. Run tests with:
   ```bash
   make tests
   ```
   If tests fail due to missing dependencies or environment restrictions, mention this in the PR description.
4. Make small, focused commits using the prefixes `feat:`, `fix:`, `style:` or `chore:`.
5. Pull request summaries must describe key changes and cite updated files. Include test output in the testing section.

## Development Guidelines

- Only modify header files (`*.hpp`) when extending the compiler API. Source
  files under `src/` can remain stub implementations.
- Ensure headers declare all types and functions necessary to support the full
  Pascal grammar described in `README.md`.
- Tests rely on hard‑coded expectations for tokens, AST validity, generated
  assembly, and execution output. Keep the helper utilities under `tests/`
  working.

## Code Style

- Use clear comments and keep lines under 80 characters when possible.
- Organize sources under `src` and headers under `include`.
- Remove unused code and keep the repository clean of build artefacts.

## Build System

- Only the provided **Makefile** should be used. It defines the `all`,
  `compiler`, `tests`, and `clean` rules.
- Place compiled binaries in the `build/` directory.

## AGENTS.md Inheritance

- This file governs the entire repository.
- Nested `AGENTS.md` files may override these instructions for their
  directory subtree.
