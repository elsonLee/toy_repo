#include <stdio.h>
#include <string>
#include <utility>
#include <memory>
#include <vector>

enum class Token {
  Eof = 0,
  Def,
  Extern,
  Identifier,
  Number,

  Unknown,
  Invalid,
};

std::string to_string (const Token& token) {
  if (token == Token::Eof) {
    return "eof";
  } else if (token == Token::Def) {
    return "def";
  } else if (token == Token::Extern) {
    return "extern";
  } else if (token == Token::Identifier) {
    return "identifier";
  } else if (token == Token::Number) {
    return "number";
  } else if (token == Token::Unknown) {
    return "unknown";
  } else {
    return "invalid";
  }
}

std::string to_string (const std::pair<Token, std::string>& p) {
  return "[" + to_string(p.first) + "] " + p.second;
}

void back_off (char*& ptr, uint32_t& len) {
    ptr -= 1;
    len += 1;
}

char* get_char (char*& ptr, uint32_t& len) {
  if (len <= 0) {
    return nullptr;
  } else if (*ptr == EOF) {
    return nullptr;
  } else {
    char* ret = ptr;
    ptr += 1;
    len -= 1;
    return ret;
  }
}

std::pair<Token, std::string> get_token (char*& ptr, uint32_t& len) {
  char last_char = ' ';

  while (isspace(last_char)) {
    auto* c = get_char(ptr, len);
    if (c) {
      last_char = *c;
    } else {
      return std::make_pair(Token::Eof, "");
    }
  }

  if (isalpha(last_char)) { // [a-zA-Z][a-zA-Z0-9]*
    std::string identifier_str(1, last_char);
    auto* c = get_char(ptr, len);
    while (c && isalnum(*c)) {
      identifier_str += *c;
      c = get_char(ptr, len);
    }
    if (c) {
      back_off(ptr, len);
    }

    if (identifier_str == "def") {
      return std::make_pair(Token::Def, "def");
    } else if (identifier_str == "extern") {
      return std::make_pair(Token::Extern, "extern");
    } else {
      return std::make_pair(Token::Identifier, identifier_str);
    }
  }

  if (isdigit(last_char) || last_char == '.') {  // [0-9.]+
    std::string num_str(1, last_char);
    auto* c = get_char(ptr, len);
    while (c && (isdigit(*c) || *c == '.')) {
      num_str += *c;
      c = get_char(ptr, len);
    }
    if (c) {
      back_off(ptr, len);
    }

    return std::make_pair(Token::Number, num_str);
  }

  if (last_char == '#') {
    auto* c = get_char(ptr, len);
    while (c && *c != '\n' && *c != '\r') {
      c = get_char(ptr, len);
    }

    if (c) {
      back_off(ptr, len);
      return get_token(ptr, len);
    } else {
      return std::make_pair(Token::Eof, "");
    }
  }

  return std::make_pair(Token::Unknown, std::string(1, last_char));
}

class ExprAST {
  public:
    virtual ~ExprAST() {}
};

class NumberExprAST : public ExprAST {
  public:
    NumberExprAST (double val) : val_(val) {}

  private:
    double val_;
};

class VariableExprAST : public ExprAST {
  public:
    VariableExprAST (const std::string& name) : name_(name) {}

  private:
    std::string name_;
};

class BinaryExprAST : public ExprAST {
  public:
    BinaryExprAST (const char op,
                   std::unique_ptr<ExprAST> lhs,
                   std::unique_ptr<ExprAST> rhs) :
      op_(op),
      lhs_(std::move(lhs)),
      rhs_(std::move(rhs))
  {}

  private:
    char op_;
    std::unique_ptr<ExprAST> lhs_, rhs_;
};

class CallExprAST : public ExprAST {
  public:
    CallExprAST (const std::string& callee,
                 std::vector<std::unique_ptr<ExprAST>> args) :
      callee_(callee), args_(std::move(args)) {}

  private:
    std::string callee_;
    std::vector<std::unique_ptr<ExprAST>> args_;
};

class PrototypeAST {
  public:
    PrototypeAST (const std::string& name,
                 std::vector<std::string> args) :
      name_(name), args_(std::move(args)) {}

    const std::string& get_name () const { return name_; }

  private:
    std::string name_;
    std::vector<std::string> args_;
};

class FunctionAST {
  public:
    FunctionAST (std::unique_ptr<PrototypeAST> proto,
                 std::unique_ptr<ExprAST> body):
      proto_(std::move(proto)), body_(std::move(body)) {}

  private:
    std::unique_ptr<PrototypeAST> proto_;
    std::unique_ptr<ExprAST> body_;
};

int main () {

  std::string code = 
    std::string("# Compute the x'th fibonacci number. \n") +
    std::string("  def fib(x) \n") +
    std::string("    if x < 3 then \n") +
    std::string("      1 \n") +
    std::string("    else \n") +
    std::string("      fib(x-1)+fib(x-2) \n") +
    std::string("# This expression will compute the 40th number. \n") +
    std::string("  fib(40)");

  printf("code: %s\n\n", code.c_str());

  char* ptr = const_cast<char*>(code.data());
  uint32_t len = code.size();
  while (1) {
    auto p = get_token(ptr, len);
    printf("%s\n", to_string(p).c_str());
    Token token = p.first;
    if (token == Token::Eof || token == Token::Invalid) {
      break;
    }
  }

  return 0;
}
