//===-- main.cpp ------------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "A.h"

int main() {
  //% self.expect("expr foo(3)", DATA_TYPES_DISPLAYED_CORRECTLY, substrs = ["5"])
  return 0;
}
