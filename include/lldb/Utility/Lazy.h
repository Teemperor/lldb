//===-- Lazy.h --------------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_Lazy_h_
#define liblldb_Lazy_h_

#include <type_traits>

#include "lldb/lldb-private-enumerations.h"

namespace lldb_private {

/// Contains a member variable value that will be lazily calculcated when it's
/// used for the first time.
///
/// The class takes a value type T that should be stored, and a member function
/// that will be called to calculate the actual value on the first read.
template <typename T, typename Class, T (Class::*Update)()> class LazyMember {
  T m_value;
  bool m_needs_update;

  static_assert(std::is_trivial<T>::value, "Only trivial types are supported.");

public:
  LazyMember() { reset(); }

  /// Requests that the member should be recalculated on the next read.
  void reset() { m_needs_update = true; }

  /// Reads the value of this LazyMember. Potentially recalculates the value
  /// before returning.
  const T &get(Class &instance) {
    if (m_needs_update)
      set((instance.*Update)());
    assert(!m_needs_update);
    return m_value;
  }

  /// Explicitly sets the value of this instance to a certain value, which will
  /// also prevent that any future reads recalculate it.
  void set(const T &value) {
    m_value = value;
    m_needs_update = false;
  }
};

/// Convenience struct for LazyMember with a bool.
template <typename Class, bool (Class::*Update)()>
class LazyBoolMember : public LazyMember<bool, Class, Update> {};
} // namespace lldb_private

#endif // liblldb_Lazy_h_
