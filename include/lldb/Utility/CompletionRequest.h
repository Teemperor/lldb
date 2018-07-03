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

#include "lldb/Utility/Args.h"
#include "lldb/Utility/StringList.h"
#include "llvm/ADT/StringRef.h"

namespace lldb_private {

//----------------------------------------------------------------------
/// @class CompletionRequest CompletionRequest.h
///   "lldb/Utility/ArgCompletionRequest.h"
///
/// Contains all information necessary to complete an incomplete command
/// for the user. Will be filled with the generated completions by the different
/// completions functions.
///
//----------------------------------------------------------------------
class CompletionRequest {
public:
  //----------------------------------------------------------------------
  /// Constructs a completion request.
  ///
  /// See the respective members of this class for documentation for the
  /// parameters.
  // TODO: Infer the parsed_line and the cursor positions from the other
  // arguments.
  //----------------------------------------------------------------------
  CompletionRequest(llvm::StringRef command, unsigned raw_cursor_pos,
                    Args &parsed_line, int cursor_index,
                    int cursor_char_position, int match_start_point,
                    int max_return_elements, bool word_complete,
                    StringList &matches);
  CompletionRequest(llvm::StringRef command_line, unsigned raw_cursor_pos,
                    int match_start_point, int max_return_elements,
                    StringList &matches);

  llvm::StringRef GetRawLine() const { return m_command; }

  unsigned GetRawCursorPos() const { return m_raw_cursor_pos; }

  const Args &GetParsedLine() const { return m_parsed_line; }

  Args &GetParsedLine() { return m_parsed_line; }

  void SetCursorIndex(int i) { m_cursor_index = i; }
  int GetCursorIndex() const { return m_cursor_index; }

  void SetCursorCharPosition(int pos) { m_cursor_char_position = pos; }
  int GetCursorCharPosition() const { return m_cursor_char_position; }

  int GetMatchStartPoint() const { return m_match_start_point; }

  int GetMaxReturnElements() const { return m_max_return_elements; }

  bool GetWordComplete() { return m_word_complete; }

  void SetWordComplete(bool v) { m_word_complete = v; }

  /// The array of matches returned.
  StringList &GetMatches() { return *m_matches; }

  llvm::StringRef GetCursorArgument() const {
    return GetParsedLine().GetArgumentAtIndex(GetCursorIndex());
  }

  llvm::StringRef GetCursorArgumentPrefix() const {
    return GetCursorArgument().substr(0, GetCursorCharPosition());
  }

private:
  /// The raw command line we are supposed to complete.
  llvm::StringRef m_command;
  /// The cursor position in m_command.
  unsigned m_raw_cursor_pos;
  /// The command line parsed as arguments.
  Args m_parsed_line;
  /// The index of the argument in which the completion cursor is.
  int m_cursor_index;
  /// The cursor position in the argument indexed by m_cursor_index.
  int m_cursor_char_position;
  /// If there is a match that is expensive
  /// to compute, these are here to allow you to compute the completions in
  /// batches.  Start the completion from \amatch_start_point, and return
  /// \amatch_return_elements elements.
  // FIXME: These two values are not implemented.
  int m_match_start_point;
  int m_max_return_elements;
  /// \btrue if this is a complete option value (a space will be inserted
  /// after the completion.)  \bfalse otherwise.
  bool m_word_complete;
  // We don't own the list.
  StringList *m_matches;
};

CompletionRequest::CompletionRequest(llvm::StringRef command_line, unsigned raw_cursor_pos, int match_start_point, int max_return_elements, StringList &matches)
    : m_command(command), m_raw_cursor_pos(raw_cursor_pos),
      m_match_start_point(match_start_point),
      m_max_return_elements(max_return_elements),
      m_matches(matches) {

    // We parse the argument up to the cursor, so the last argument in
    // parsed_line is the one containing the cursor, and the cursor is after the
    // last character.
    Args parsed_line(command_line);
    Args partial_parsed_line(command_line.substr(0, raw_cursor_pos));

    int num_args = partial_parsed_line.GetArgumentCount();
    int cursor_index = partial_parsed_line.GetArgumentCount() - 1;
    int cursor_char_position;

    if (cursor_index == -1)
      cursor_char_position = 0;
    else
      cursor_char_position =
          strlen(partial_parsed_line.GetArgumentAtIndex(cursor_index));

    if (cursor > current_line && cursor[-1] == ' ') {
      // We are just after a space.  If we are in an argument, then we will
      // continue parsing, but if we are between arguments, then we have to
      // complete whatever the next element would be. We can distinguish the two
      // cases because if we are in an argument (e.g. because the space is
      // protected by a quote) then the space will also be in the parsed
      // argument...

      const char *current_elem =
          partial_parsed_line.GetArgumentAtIndex(cursor_index);
      if (cursor_char_position == 0 ||
          current_elem[cursor_char_position - 1] != ' ') {
        parsed_line.InsertArgumentAtIndex(cursor_index + 1, llvm::StringRef(),
                                          '\0');
        cursor_index++;
        cursor_char_position = 0;
      }
    }

    matches.Clear();
}

} // namespace lldb_private

#endif // LLDB_UTILITY_COMPLETIONREQUEST_H
