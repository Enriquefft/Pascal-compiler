#!/bin/sh
# Simple test runner for the C++ Pascal compiler
set -e

TEST_BIN=../build/tests

if [ ! -x "$TEST_BIN" ]; then
  echo "Tests not built" >&2
  exit 1
fi

"$TEST_BIN"
