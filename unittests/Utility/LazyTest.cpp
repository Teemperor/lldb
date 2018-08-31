//===-- StreamTest.cpp ------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "lldb/Utility/Lazy.h"
#include "gtest/gtest.h"

using namespace lldb_private;

namespace {
class LazyClass {
  bool UpdateFoo() {
    ++m_updates_called;
    return m_foo_value;
  }

public:
  LazyBoolMember<LazyClass, &LazyClass::UpdateFoo> m_foo;
  bool getFoo() { return m_foo.get(*this); }
  bool m_foo_value = true;
  int m_updates_called = 0;
};
} // namespace

TEST(LazyTest, UpdateCount) {
  LazyClass l;
  EXPECT_EQ(0, l.m_updates_called);

  l.getFoo();
  EXPECT_EQ(1, l.m_updates_called);
  l.getFoo();
  EXPECT_EQ(1, l.m_updates_called);
  l.getFoo();
  EXPECT_EQ(1, l.m_updates_called);
}

TEST(LazyTest, Value) {
  {
    LazyClass l1;
    EXPECT_EQ(l1.m_foo_value, l1.getFoo());
  }

  {
    LazyClass l2;
    bool old_value = l2.m_foo_value;
    l2.m_foo_value = !l2.m_foo_value;
    EXPECT_NE(old_value, l2.getFoo());
  }
}

TEST(LazyTest, Reset) {
  LazyClass l;
  EXPECT_EQ(0, l.m_updates_called);
  l.m_foo.reset();
  EXPECT_EQ(0, l.m_updates_called);

  l.getFoo();
  EXPECT_EQ(1, l.m_updates_called);

  l.m_foo.reset();
  EXPECT_EQ(1, l.m_updates_called);

  l.getFoo();
  EXPECT_EQ(2, l.m_updates_called);
  l.getFoo();
  EXPECT_EQ(2, l.m_updates_called);
}
