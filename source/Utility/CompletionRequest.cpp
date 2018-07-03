//===-- CompletionRequest.cpp -----------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "lldb/Utility/CompletionRequest.h"

using namespace lldb;
using namespace lldb_private;

CompletionRequest::CompletionRequest(llvm::StringRef command,
                                     unsigned raw_cursor_pos, Args &parsed_line,
                                     int cursor_index, int cursor_char_position,
                                     int match_start_point,
                                     int max_return_elements,
                                     bool word_complete, StringList &matches)
    : m_command(command), m_raw_cursor_pos(raw_cursor_pos),
      m_parsed_line(parsed_line), m_cursor_index(cursor_index),
      m_cursor_char_position(cursor_char_position),
      m_match_start_point(match_start_point),
      m_max_return_elements(max_return_elements),
      m_word_complete(word_complete), m_matches(&matches) {}

CompletionRequest::CompletionRequest(llvm::StringRef command_line, unsigned raw_cursor_pos, int match_start_point, int max_return_elements, StringList &matches)
  : m_command(command), m_raw_cursor_pos(raw_cursor_pos),
    m_match_start_point(match_start_point),
    m_max_return_elements(max_return_elements),
    m_matches(matches) {

  // We parse the argument up to the cursor, so the last argument in
  // parsed_line is the one containing the cursor, and the cursor is after the
  // last character.
  m_parsed_line = Args(command_line);
  m_partial_parsed_line = Args(command_line.substr(0, raw_cursor_pos));

  m_cursor_index = m_partial_parsed_line.GetArgumentCount() - 1;

  if (m_cursor_index == -1)
    m_cursor_char_position = 0;
  else
    m_cursor_char_position =
        strlen(m_partial_parsed_line.GetArgumentAtIndex(m_cursor_index));

  if (cursor > current_line && cursor[-1] == ' ') {
    // We are just after a space.  If we are in an argument, then we will
    // continue parsing, but if we are between arguments, then we have to
    // complete whatever the next element would be. We can distinguish the two
    // cases because if we are in an argument (e.g. because the space is
    // protected by a quote) then the space will also be in the parsed
    // argument...

    const char *current_elem =
        m_partial_parsed_line.GetArgumentAtIndex(m_cursor_index);
    if (m_cursor_char_position == 0 ||
        current_elem[m_cursor_char_position - 1] != ' ') {
        m_parsed_line.InsertArgumentAtIndex(m_cursor_index + 1, llvm::StringRef(),
                                          '\0');
        m_cursor_index++;
        m_cursor_char_position = 0;
      }
  }

  matches.Clear();
}
