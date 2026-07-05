#include "lexer.h"
#include <stdbool.h>

struct alok_lexer {
  const char *start;
  const char *current;
};

static struct alok_lexer lex;

static bool is_end_at() { return *lex.current == '\0'; }

static char peak() { return *lex.current; }

static char advance() {
  if (is_end_at())
    return '\0';
  return *lex.current++;
}

static bool is_digit(char c) { return (c >= '0' && c <= '9'); }

static bool is_word_char(char c) {
  if (c == '\0' || c == ' ' || c == '\t' || c == '\r' || c == '\n')
    return false;
  if (c == '|' || c == '&' || c == ';' || c == '<' || c == '>')
    return false;
  return true;
}

static void skip_white_space() {
  while (!is_end_at()) {
    char c = peak();
    if (c == ' ' || c == '\r' || c == '\n' || c == '\t') {
      advance();
    } else {
      break;
    }
  }
}

static struct alok_token parse_word() {
  struct alok_token token;
  token.type = STRING;
  token.value.start = lex.current;
  token.value.len = 0;

  while (!is_end_at()) {
    char c = peak();
    if (c == '\'') {
      advance();
      while (!is_end_at() && peak() != '\'') {
        token.value.len++;
        advance();
      }

      if (!is_end_at())
        advance();
    } else if (c == '"') {
      advance();
      while (!is_end_at() && peak() != '"') {

        if (peak() == '\\') {
          advance();
          if (!is_end_at()) {
            token.value.len++;
            advance();
          }
        } else {

          token.value.len++;
          advance();
        }
      }
      if (!is_end_at())
        advance();
    } else if (is_word_char(c)) {
      token.value.len++;
      advance();
    } else {
      break;
    }
  }

  return token;
}

void init_alok_lexer(const char *input) {
  lex.start = input;
  lex.current = input;
}

struct alok_token lexer() {
  struct alok_token token;
  token.value.fd = -1;

  skip_white_space();
  lex.start = lex.current;

  if (is_end_at()) {
    token.type = EOF;
    return token;
  }

  if (is_digit(peak())) {
    const char *saved_position = lex.current;
    int fd_accumulator = 0;

    while (!is_end_at() && is_digit(peak())) {
      fd_accumulator = (fd_accumulator * 10) + (advance() - '0');
    }

    char next_c = peak();
    if (next_c == '>') {
      advance();
      token.value.fd = fd_accumulator;
      if (peak() == '>') {
        advance();
        token.type = REDIRECT_APPEND;
      } else {
        token.type = REDIRECT_OUT;
      }
      return token;
    } else if (next_c == '<') {
      advance();
      token.value.fd = fd_accumulator;
      token.type = REDIRECT_IN;
      return token;
    }

    lex.current = saved_position;
  }

  char c = advance();
  switch (c) {
  case '|':
    token.type = PIPE;
    return token;
  case '&':
    token.type = BACKGROUND;
    return token;
  case ';':
    token.type = SEMICOLON;
    return token;
  case '>':
    token.value.fd = 1;
    if (peak() == '>') {
      advance();
      token.type = REDIRECT_APPEND;
    } else {
      token.type = REDIRECT_OUT;
    }
    return token;
  case '<':
    token.value.fd = 0;
    token.type = REDIRECT_IN;
    return token;
  default:
    lex.current--;
    return parse_word();
  }
}
