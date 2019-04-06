#include <stdio.h>
#include <assert.h>
#include <string>
#include <utility>
#include <memory>
#include <vector>
#include <unordered_map>

#include <llvm/ADT/STLExtras.h>

enum class Token {
  Eof = 0,
  Def,
  Extern,
  Identifier,
  Number,
  Lparen,
  Rparen,
  Comma,
  BinaryOp,

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
  } else if (token == Token::Lparen) {
    return "lparen";
  } else if (token == Token::Rparen) {
    return "rparen";
  } else if (token == Token::Comma) {
    return "comma";
  } else if (token == Token::BinaryOp) {
    return "binary_op";
  } else if (token == Token::Unknown) {
    return "unknown";
  } else {
    return "invalid";
  }
}

std::string to_string (const std::pair<Token, std::string>& p) {
  return "[" + to_string(p.first) + "] " + p.second;
}

using TokenPair = std::pair<Token, std::string>;

class Lexer {
  public:
    Lexer (char* ptr, uint32_t len) :
      ptr_(ptr), ptr_bak_(ptr),
      len_(len), len_bak_(len) {}

    TokenPair get_token_and_forward ();
    TokenPair get_token ();

  private:
    void forward (int32_t step = 1);
    void backward (int32_t step = 1) { forward(-step); };
    char* get_next_char ();
    void save ();
    void restore ();

  private:
    char*     ptr_;
    char*     ptr_bak_;
    uint32_t  len_;
    uint32_t  len_bak_;
};

void Lexer::forward (int32_t step) {
    ptr_ += step;
    len_ -= step;
}

char* Lexer::get_next_char () {
  if (len_ <= 0) {
    return nullptr;
  } else if (*ptr_ == EOF) {
    return nullptr;
  } else {
    char* ret = ptr_;
    forward();
    return ret;
  }
}

void Lexer::save () {
  ptr_bak_ = ptr_;
  len_bak_ = len_;
}

void Lexer::restore () {
  ptr_ = ptr_bak_;
  len_ = len_bak_;
}

TokenPair Lexer::get_token_and_forward () {
  char last_char = ' ';

  while (isspace(last_char)) {
    auto* c = get_next_char();
    if (c) {
      last_char = *c;
    } else {
      return std::make_pair(Token::Eof, "");
    }
  }

  if (isalpha(last_char)) { // [a-zA-Z][a-zA-Z0-9]*
    std::string identifier_str(1, last_char);
    auto* c = get_next_char();
    while (c && isalnum(*c)) {
      identifier_str += *c;
      c = get_next_char();
    }
    if (c) {
      backward();
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
    auto* c = get_next_char();
    while (c && (isdigit(*c) || *c == '.')) {
      num_str += *c;
      c = get_next_char();
    }
    if (c) {
      backward();
    }
    return std::make_pair(Token::Number, num_str);
  }

  if (last_char == '#') {
    auto* c = get_next_char();
    while (c && *c != '\n' && *c != '\r') {
      c = get_next_char();
    }

    if (c) {
      backward();
      return get_token_and_forward();
    } else {
      return std::make_pair(Token::Eof, "");
    }
  }

  switch (last_char) {
    case '(':
      return std::make_pair(Token::Lparen, "(");
    case ')':
      return std::make_pair(Token::Rparen, ")");
    case ',':
      return std::make_pair(Token::Comma, ",");
    case '+':
      return std::make_pair(Token::BinaryOp, "+");
    case '-':
      return std::make_pair(Token::BinaryOp, "-");
    case '*':
      return std::make_pair(Token::BinaryOp, "*");
    case '/':
      return std::make_pair(Token::BinaryOp, "/");
    case '<':
      return std::make_pair(Token::BinaryOp, "<");
  }

  return std::make_pair(Token::Unknown, std::string(1, last_char));
}

TokenPair Lexer::get_token () {
  save();
  auto ret = get_token_and_forward();
  restore();
  return std::move(ret);
}

class ExprAST {
  public:
    virtual ~ExprAST() {}

    virtual void print () = 0;
};

std::unique_ptr<ExprAST> LogError (const char* msg) {
  fprintf(stderr, "LogError: %s\n", msg);
  return nullptr;
}

class NumberExprAST : public ExprAST {
  public:
    NumberExprAST (double val) : val_(val) {}

    void print () override {
      printf("[num]%.1f", val_);
    }

  private:
    double val_;
};

class VariableExprAST : public ExprAST {
  public:
    VariableExprAST (const std::string& name) : name_(name) {}

    void print () override {
      printf("[var]%s", name_.c_str());
    }

  private:
    std::string name_;
};

class BinaryExprAST : public ExprAST {
  public:
    BinaryExprAST (const std::string op,
                   std::unique_ptr<ExprAST> lhs,
                   std::unique_ptr<ExprAST> rhs) :
      op_(op),
      lhs_(std::move(lhs)),
      rhs_(std::move(rhs))
    {}

    void print () override {
      printf("[bop]%s(", op_.c_str());
      lhs_->print();
      printf(", ");
      rhs_->print();
      printf(")");
    }

  private:
    std::string op_;
    std::unique_ptr<ExprAST> lhs_, rhs_;
};

class CallExprAST : public ExprAST {
  public:
    CallExprAST (const std::string& func_name,
                 std::vector<std::unique_ptr<ExprAST>> args) :
      func_name_(func_name), args_(std::move(args)) {}

    void print () override {
      printf("[call]%s(", func_name_.c_str());
      if (!args_.empty()) {
        args_[0]->print();
      }
      for (uint32_t i = 1; i < args_.size(); i++) {
        printf(", ");
        args_[i]->print();
      }
      printf(")");
    }

  private:
    std::string func_name_;
    std::vector<std::unique_ptr<ExprAST>> args_;
};

class PrototypeAST : public ExprAST {
  public:
    PrototypeAST (const std::string& func_name,
                 std::vector<std::string> args) :
      func_name_(func_name), args_(std::move(args)) {}

    const std::string& get_name () const { return func_name_; }

    void print () override {
      printf("[proto]%s(", func_name_.c_str());
      if (!args_.empty()) {
        printf("%s", args_[0].c_str());
      }
      for (uint32_t i = 1; i < args_.size(); i++) {
        printf(", %s", args_[i].c_str());
      }
      printf(")");
    }

  private:
    std::string func_name_;
    std::vector<std::string> args_;
};

std::unique_ptr<PrototypeAST> LogErrorP (const char* msg) {
  fprintf(stderr, "LogErrorP: %s\n", msg);
  return nullptr;
}

class FunctionAST : public ExprAST {
  public:
    FunctionAST (std::unique_ptr<PrototypeAST> proto,
                 std::unique_ptr<ExprAST> body):
      proto_(std::move(proto)), body_(std::move(body)) {}

    void print () override {
      printf("[func]( ");
      proto_->print();
      printf(" ){ ");
      body_->print();
      printf(" }");
    }

  private:
    std::unique_ptr<PrototypeAST> proto_;
    std::unique_ptr<ExprAST> body_;
};

class Parser {

  public:
    Parser (Lexer& lexer) :
      lexer_(lexer) {
        binary_op_pri_tbl_["*"] = 40;
        binary_op_pri_tbl_["/"] = 40;
        binary_op_pri_tbl_["+"] = 20;
        binary_op_pri_tbl_["-"] = 20;
        binary_op_pri_tbl_["<"] = 10;
      }

    std::unique_ptr<ExprAST> parse_number_expr ();
    std::unique_ptr<ExprAST> parse_paren_expr ();
    std::unique_ptr<ExprAST> parse_identifier_expr ();

    std::unique_ptr<ExprAST> parse_primary ();

    std::unique_ptr<ExprAST> parse_expression_l (std::unique_ptr<ExprAST> lhs, uint32_t min_precedence);
    std::unique_ptr<ExprAST> parse_expression ();
    std::unique_ptr<FunctionAST> parse_top_level_expression ();

    std::unique_ptr<PrototypeAST> parse_prototype ();
    std::unique_ptr<PrototypeAST> parse_extern ();
    std::unique_ptr<FunctionAST> parse_definition ();

    std::unique_ptr<ExprAST> parse ();

  private:
    uint32_t precedence (const std::string& val) {
      return binary_op_pri_tbl_[val];
    }

  private:
    Lexer&  lexer_;
    std::unordered_map<std::string, uint32_t> binary_op_pri_tbl_;
};

#if 1
// number_expr ::= number
std::unique_ptr<ExprAST> Parser::parse_number_expr () {
  auto cur = lexer_.get_token_and_forward();
  assert(cur.first == Token::Number);
  double val = std::stod(cur.second);
  auto result = std::make_unique<NumberExprAST>(val);
  return std::move(result);
}

// paren_expr ::= '(' expression ')'
std::unique_ptr<ExprAST> Parser::parse_paren_expr () {
  auto cur = lexer_.get_token_and_forward();
  assert(cur.first == Token::Lparen);
  auto v = parse_expression();
  if (!v) {
    return nullptr;
  }

  auto last = lexer_.get_token_and_forward();
  if (last.first != Token::Rparen) {
    return LogError("expected ')'");
  }

  return v;
}

// identifier_expr
//  ::= identifier
//  ::= identifier '(' expression* ')'
std::unique_ptr<ExprAST> Parser::parse_identifier_expr () {
  auto cur = lexer_.get_token_and_forward();
  assert(cur.first == Token::Identifier);
  std::string identifier_name = cur.second;

  auto v = lexer_.get_token();
  if (v.first != Token::Lparen) {
    return std::make_unique<VariableExprAST>(identifier_name);
  } else {
    lexer_.get_token_and_forward(); // drop '('
    // call function
    auto next = lexer_.get_token();
    std::vector<std::unique_ptr<ExprAST>> args;
    if (next.first != Token::Rparen) {
      while (1) {
        if (auto arg = parse_expression()) {
          args.emplace_back(std::move(arg));
        } else {
          return nullptr;
        }

        auto end = lexer_.get_token_and_forward();
        if (end.first == Token::Rparen) {
          break;
        }

        if (end.first != Token::Comma) {
          return LogError("Expected ')' or ',' in argument list");
        }
      }
    } else {
      lexer_.get_token_and_forward(); // drop ')'
    }
    return std::make_unique<CallExprAST>(identifier_name, std::move(args));
  }
}

// primary
//  ::= identifier_expr
//  ::= number_expr
//  ::= paren_expr
std::unique_ptr<ExprAST> Parser::parse_primary () {
  auto cur = lexer_.get_token();
  switch (cur.first) {
    case Token::Identifier:
      return parse_identifier_expr();
    case Token::Number:
      return parse_number_expr();
    case Token::Lparen:
      return parse_paren_expr();
    default:
      printf("token: %s\n", to_string(cur).c_str());
      return LogError("unknown token when expecting an expression");
  }
}

// https://en.wikipedia.org/wiki/Operator-precedence_parser
std::unique_ptr<ExprAST> Parser::parse_expression_l (std::unique_ptr<ExprAST> lhs, uint32_t min_precedence) {
  auto lookahead = lexer_.get_token();
  while (lookahead.first == Token::BinaryOp &&
         precedence(lookahead.second) >= min_precedence) {
    lexer_.get_token_and_forward();
    auto op = std::move(lookahead);
    auto rhs = parse_primary();
    if (!rhs) {
      return nullptr;
    }
    lookahead = std::move(lexer_.get_token());
    // lhs op_l (rhs op_h ...)
    if (lookahead.first == Token::BinaryOp &&
        precedence(lookahead.second) > precedence(op.second)) {
      rhs = std::move(parse_expression_l(std::move(rhs), precedence(lookahead.second)));
      if (!rhs) {
        return nullptr;
      }
      lookahead = std::move(lexer_.get_token());
    }
    // (lhs op_h rhs) op_l ...
    lhs = std::make_unique<BinaryExprAST>(op.second, std::move(lhs), std::move(rhs));
  }
  return std::move(lhs);
}

// Operator-precedence parser
// expression
//  ::= primary binary_op_rhs
std::unique_ptr<ExprAST> Parser::parse_expression () {
  auto lhs = parse_primary();
  if (!lhs) {
    return nullptr;
  }
  return parse_expression_l(std::move(lhs), 0);
}

// prototype
//  ::= func_name '(' arg* ')'
std::unique_ptr<PrototypeAST> Parser::parse_prototype () {
  auto cur = lexer_.get_token_and_forward();
  assert(cur.first == Token::Identifier);

  auto func_name = cur.second;

  auto lparen = lexer_.get_token_and_forward();
  if (lparen.first != Token::Lparen) {
    return LogErrorP("Expected '(' in prototype");
  }

  std::vector<std::string> args;
  auto lookahead = lexer_.get_token_and_forward();
  while (lookahead.first == Token::Identifier) {
    args.emplace_back(lookahead.second);
    lookahead = lexer_.get_token_and_forward();
  }
  //auto rparen = lexer_.get_token_and_forward();
  if (lookahead.first != Token::Rparen) {
    return LogErrorP("Expected ')' in prototype");
  }

  return std::make_unique<PrototypeAST>(func_name, std::move(args));
}

// function
//  ::= 'def' prototype body
std::unique_ptr<FunctionAST> Parser::parse_definition () {
  auto cur = lexer_.get_token_and_forward();
  assert(cur.first == Token::Def);

  auto proto = parse_prototype();
  if (!proto) {
    return nullptr;
  }

  auto body = parse_expression();
  if (body) {
    return std::make_unique<FunctionAST>(std::move(proto), std::move(body));
  } else {
    return nullptr;
  }
}

// extern
//  ::= 'extern' prototype
std::unique_ptr<PrototypeAST> Parser::parse_extern () {
  auto cur = lexer_.get_token_and_forward();
  assert(cur.first == Token::Extern);
  return parse_prototype();
}

// expression wrap to function
std::unique_ptr<FunctionAST> Parser::parse_top_level_expression () {
  auto expr = parse_expression();
  if (expr) {
    auto proto = std::make_unique<PrototypeAST>("anonymous", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
  } else {
    return nullptr;
  }
}

std::unique_ptr<ExprAST> Parser::parse () {
  auto cur = lexer_.get_token();
  switch (cur.first) {
    case Token::Eof:
      return nullptr;
    case Token::Def:
      return parse_definition();
    case Token::Extern:
      return parse_extern();
    default:
      return parse_expression();
  }
}
#endif

int main () {

#if 0
  std::string code = 
    std::string("# Compute the x'th fibonacci number. \n") +
    std::string("  def fib(x) \n") +
    std::string("    if x < 3 then \n") +
    std::string("      1 \n") +
    std::string("    else \n") +
    std::string("      fib(x-1)+fib(x-2) \n") +
    std::string("# This expression will compute the 40th number. \n") +
    std::string("  fib(40)\n");
#else
  std::string code = 
    std::string("  def foo(x y) x+y\n");
#endif

  printf("code: %s\n", code.c_str());

  char* ptr = const_cast<char*>(code.data());
  uint32_t len = code.size();

  printf("lex result:\n");
  // print lexer
  {
    Lexer lexer(ptr, len);
    while (1) {
      auto p = lexer.get_token_and_forward();
      printf("%s\n", to_string(p).c_str());
      Token token = p.first;
      if (token == Token::Eof || token == Token::Invalid) {
        break;
      }
    }
  }
  printf("\n");

  printf("parse result:\n");
  // parse
  {
    Lexer lexer(ptr, len);
    Parser parser(lexer);
    auto res = parser.parse();
    res->print();
  }
  printf("\n");

  return 0;
}
