//===-- CompletionRequest.h -------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLDB_UTILITY_COMPLETIONREQUEST_H
#define LLDB_UTILITY_COMPLETIONREQUEST_H

#include <lldb/Utility/Args.h>
#include <lldb/Utility/StringList.h>
#include <llvm/ADT/StringRef.h>

namespace lldb_private {

//----------------------------------------------------------------------
/// @class ArgCompletionRequest ArgCompletionRequest.h
///   "lldb/Utility/ArgCompletionRequest.h"
///
/// Wraps a request to complete a command line invocation.
///
//----------------------------------------------------------------------
class CompletionRequest {
public:
  CompletionRequest(llvm::StringRef command, unsigned raw_cursor_pos,
                       Args &parsed_line, int cursor_index,
                       int cursor_char_position, int match_start_point,
                       int max_return_elements, bool word_complete,
                       StringList &matches);

  llvm::StringRef GetRawLine() const { return m_command; }

  unsigned GetRawCursorPos() const { return m_raw_cursor_pos; }

  Args &GetParsedLine() { return m_parsed_line; }

  void SetCursorIndex(int i) { m_cursor_index = i; }
  int GetCursorIndex() const { return m_cursor_index; }

  void SetCursorCharPosition(int i) { m_cursor_char_position = i; }
  int GetCursorCharPosition() const { return m_cursor_char_position; }

  int GetMatchStartPoint() const { return m_match_start_point; }

  int GetMaxReturnElements() const { return m_max_return_elements; }

  bool GetWordComplete() { return m_word_complete; }

  void SetWordComplete(bool v) { m_word_complete = v; }

  StringList &GetMatches() { return *m_matches; }

private:
  // The raw command line we are supposed to complete.
  llvm::StringRef m_command;
  // The cursor position in m_command.
  unsigned m_raw_cursor_pos;
  // The command line parsed as arguments.
  Args m_parsed_line;
  // The index of the argument in which the completion cursor is.
  int m_cursor_index;
  // The cursor position in the argument indexed by m_cursor_index.
  int m_cursor_char_position;
  int m_match_start_point;
  int m_max_return_elements;
  bool m_word_complete;
  StringList *m_matches;
};

} // namespace lldb_private

#endif // LLDB_UTILITY_COMPLETIONREQUEST_H
