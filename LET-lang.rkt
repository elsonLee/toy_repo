#lang eopl

(define scanner-spec-1
  '((white-sp (whitespace) skip)
    (comment ("%" (arbno (not #\newline))) skip)
    (identifier (letter (arbno (or letter digit))) symbol)
    (number (digit (arbno digit)) number)))

(define grammar-1
  '(
    (program    (expression) a-program)
    (program    (bool-exp) b-program)
    (expression ("print" "(" expression ")") print-exp)
    (expression (number) const-exp)
    (expression ("minus" "(" expression ")") minus-exp)
    (expression ("-" "(" expression "," expression ")") diff-exp)
    (expression ("+" "(" expression "," expression ")") plus-exp)
    (expression ("*" "(" expression "," expression ")") times-exp)
    (expression ("/" "(" expression "," expression ")") quotient-exp)
    (bool-exp   ("zero?" expression) zero?-exp)
    (bool-exp   ("equal?" "(" expression "," expression ")") equal?-exp)
    (bool-exp   ("greater?" "(" expression "," expression ")") greater?-exp)
    (bool-exp   ("less?" "(" expression "," expression ")") less?-exp)
    (expression ("null?" expression) null?-exp)
    (expression ("cons" "(" expression "," expression ")") cons-exp)
    (expression ("car" expression) car-exp)
    (expression ("cdr" expression) cdr-exp)
    (expression ("emptylist") emptylist-exp)
    (expression ("list" "(" (separated-list expression ",") ")") list-exp)
    (expression ("if" bool-exp "then" expression "else" expression) if-exp)
    (expression ("cond" (arbno bool-exp "==>" expression) "end") cond-exp)
    (expression (identifier) var-exp)
    (expression ("let" (arbno identifier "=" expression) "in" expression) let-exp)
    (expression ("let*" (arbno identifier "=" expression) "in" expression) let*-exp)
    (expression ("unpack" (arbno identifier) "=" expression "in" expression) unpack-exp)
    ))

(define scan&parse
  (sllgen:make-string-parser scanner-spec-1 grammar-1))

(define report-no-binding-found
  (lambda (search-var)
    (eopl:error 'apply-env "No binding for ~s" search-var)))

(define report-invalid-env
  (lambda (env)
    (eopl:error 'apply-env "Bad environment: ~s" env)))

(define report-expval-extractor-error
  (lambda (val)
    (eopl:error 'expval->xxx "Bad converstion: ~s" val)))

(define report-divide-zero
  (lambda ()
    (eopl:error 'quotient-exp "Divide zero")))

(define report-is-not-list
  (lambda (var)
    (eopl:error 'apply-env "Is not list ~s" var)))

(define report-no-cond-meet
  (lambda ()
    (eopl:error 'cond-expr "No cond condition meet")))

(define report-convert-to-bool-error
  (lambda (var)
    (eopl:error "Cannot convert to bool-exp ~s" var)))

(define empty-env
  (lambda () (list 'empty-env)))

(define extend-env
  (lambda (var val env)
    (list 'extend-env var val env)))

(define extend-env-with-multi-var
  (lambda (vars exps env)
    (define func
      (lambda (vars exps env ret-env)
        (let ((len (length vars)))
          (if (= len 0)
              ret-env
              (let ((var (list-ref vars 0))
                    (val (value-of (list-ref exps 0) env))
                    (var-tail (list-tail vars 1))
                    (exp-tail (list-tail exps 1)))
               (func var-tail exp-tail env (extend-env var val ret-env)))))))
    (func vars exps env env))) 

(define apply-env
  (lambda (env search-var)
    (cond
      ((eqv? (car env) 'empty-env)
       (report-no-binding-found search-var))
      ((eqv? (car env) 'extend-env)
       (let ((saved-var (cadr env))
             (saved-val (caddr env))
             (saved-env (cadddr env)))
         (if (eqv? search-var saved-var)
             saved-val
             (apply-env saved-env search-var))))
      (else
       (report-invalid-env env)))))

(define-datatype program program?
  (a-program (exp1 expression?))
  (b-program (exp1 bool-exp?)))

(define-datatype bool-exp bool-exp?
  (zero?-exp    (exp1 expression?))
  (equal?-exp   (exp1 expression?) (exp2 expression?))
  (greater?-exp (exp1 expression?) (exp2 expression?))
  (less?-exp    (exp1 expression?) (exp2 expression?)))

(define-datatype expression expression?
  (print-exp    (exp1 expression?))
  (const-exp    (num number?))
  (var-exp      (var symbol?))
  (minus-exp    (exp1 expression?))
  (diff-exp     (exp1 expression?) (exp2 expression?))
  (plus-exp     (exp1 expression?) (exp2 expression?))
  (times-exp    (exp1 expression?) (exp2 expression?))
  (quotient-exp (exp1 expression?) (exp2 expression?))
  (cons-exp     (exp1 expression?) (exp2 expression?))
  (car-exp      (exp1 expression?))
  (cdr-exp      (exp1 expression?))
  (null?-exp    (exp1 expression?))
  (emptylist-exp)
  (list-exp     (exps (list-of expression?)))
  (if-exp       (exp1 bool-exp?) (exp2 expression?) (exp3 expression?))
  (cond-exp     (exps1 (list-of bool-exp?)) (exps2 (list-of expression?)))
  (let-exp      (vars (list-of symbol?)) (exps1 (list-of expression?)) (body expression?))
  (let*-exp     (vars (list-of symbol?)) (exps1 (list-of expression?)) (body expression?))
  (unpack-exp   (vars (list-of symbol?)) (exp1 expression?) (body expression?)))

(define-datatype expval expval?
  (num-val  (num number?))
  (bool-val (bool boolean?))
  (cons-val (exp1 expval?)
            (exp2 expval?))
  (emptylist))

(define is-null?
  (lambda (val)
    (cases expval val
      (emptylist () #t)
      (else #f))))

(define expval->car
  (lambda (val)
    (cases expval val
      (cons-val (exp1 exp2) exp1)
      (else (report-is-not-list val)))))

(define expval->cdr
  (lambda (val)
    (cases expval val
      (cons-val (exp1 exp2) exp2)
      (else (report-is-not-list val)))))

(define expval->num
  (lambda (val)
    (cases expval val
      (num-val (num) num)
      (else (report-expval-extractor-error 'num val)))))

(define expval->bool
  (lambda (val)
    (cases expval val
      (bool-val (bool) bool)
      (else (report-expval-extractor-error 'bool val)))))

(define expval->val
  (lambda (val)
    (cases expval val
      (bool-val (bool) bool)
      (num-val (num) num)
      (emptylist () `())
      (else (report-expval-extractor-error 'bool val)))))

(define run
  (lambda (string)
    (value-of-program (scan&parse string))))

(define value-of-program
  (lambda (pgm)
    (cases program pgm
      (a-program (exp1)
        (value-of exp1 (init-env)))
      (b-program (exp1)
        (value-of-bool-exp exp1 (init-env))))))

(define value-of-bool-exp
  (lambda (exp env)
    (cases bool-exp exp
     (zero?-exp (exp1)
       (let ((val1 (value-of exp1 env)))
             (let ((num1 (expval->num val1)))
               (if (zero? num1)
                 (bool-val #t)
                 (bool-val #f)))))
     (equal?-exp (exp1 exp2)
       (let ((val1 (value-of exp1 env))
             (val2 (value-of exp2 env)))
          (let ((num1 (expval->num val1))
                (num2 (expval->num val2)))
            (if (= num1 num2)
                (bool-val #t)
                (bool-val #f)))))
      (greater?-exp (exp1 exp2)
        (let ((val1 (value-of exp1 env))
              (val2 (value-of exp2 env)))
          (let ((num1 (expval->num val1))
                (num2 (expval->num val2)))
            (if (> num1 num2)
                (bool-val #t)
                (bool-val #f)))))
      (less?-exp (exp1 exp2)
        (let ((val1 (value-of exp1 env))
              (val2 (value-of exp2 env)))
          (let ((num1 (expval->num val1))
                (num2 (expval->num val2)))
            (if (< num1 num2)
                (bool-val #t)
                (bool-val #f))))))))

(define value-of
  (lambda (exp env)
    (cases expression exp
      (print-exp (exp1)
        (let ((val1 (expval->val (value-of exp1 env))))
          (display val1)
          (newline)
          (num-val 1)))
      (const-exp (num)
        (num-val num))
      (var-exp (var)
        (apply-env env var))
      (minus-exp (exp1)
        (let ((val1 (value-of exp1 env)))
          (let ((num1 (expval->num val1)))
            (num-val (- 0 num1)))))
      (diff-exp (exp1 exp2)
        (let ((val1 (value-of exp1 env))
              (val2 (value-of exp2 env)))
          (let ((num1 (expval->num val1))
                (num2 (expval->num val2)))
            (num-val (- num1 num2)))))
      (plus-exp (exp1 exp2)
        (let ((val1 (value-of exp1 env))
              (val2 (value-of exp2 env)))
          (let ((num1 (expval->num val1))
                (num2 (expval->num val2)))
            (num-val (+ num1 num2)))))
      (times-exp (exp1 exp2)
        (let ((val1 (value-of exp1 env))
              (val2 (value-of exp2 env)))
          (let ((num1 (expval->num val1))
                (num2 (expval->num val2)))
            (num-val (* num1 num2)))))
      (quotient-exp (exp1 exp2)
        (let ((val1 (value-of exp1 env))
              (val2 (value-of exp2 env)))
          (let ((num1 (expval->num val1))
                (num2 (expval->num val2)))
            (if (zero? num2)
                (report-divide-zero)
                (num-val (/ num1 num2))))))
      (cons-exp (exp1 exp2)
        (let ((val1 (value-of exp1 env))
              (val2 (value-of exp2 env)))
          (cons-val val1 val2)))
      (null?-exp (exp1)
        (let ((val1 (value-of exp1 env)))
            (is-null? val1)))   
      (emptylist-exp ()
        (emptylist))
      ; TODO: opt
      (car-exp (exp1)
        (let ((val1 (value-of exp1 env)))
          (expval->car val1)))
      (cdr-exp (exp1)
        (let ((val1 (value-of exp1 env)))
          (expval->cdr val1)))
      (list-exp (exps)
        (let ((len (length exps)))
          (cond ((= len 0) (emptylist))
                ((> len 0)
                 (let ((val1 (value-of (list-ref exps 0) env)))
                   (cons-val val1 (value-of (list-exp (list-tail exps 1)) env)))))))
      (if-exp (exp1 exp2 exp3)
        (let ((val1 (value-of-bool-exp exp1 env)))
          (if (expval->bool val1)
              (value-of exp2 env)
              (value-of exp3 env))))
      (cond-exp (exps1 exps2)
        (if (= (length exps1) 0)
            (report-no-cond-meet)
            (let ((val1 (value-of-bool-exp (list-ref exps1 0) env)))
              (if (expval->bool val1)
                  (value-of (list-ref exps2 0) env)
                  (value-of (cond-exp (list-tail exps1 1) (list-tail exps2 1)) env)))))
      (let-exp (vars1 exps1 body)
        (value-of body (extend-env-with-multi-var vars1 exps1 env)))
      (let*-exp (vars1 exps1 body)
        (let ((len (length vars1)))
          (if (= len 0)
              (value-of body env)
              (let ((var (list-ref vars1 0))
                    (val (value-of (list-ref exps1 0) env))
                    (var-tail (list-tail vars1 1))
                    (exp-tail (list-tail exps1 1)))
                (value-of (let*-exp var-tail exp-tail body) (extend-env var val env))))))
      (unpack-exp (vars exp1 body)
        (let ((vals (value-of exp1 env)))
          (define extend-new-env
            (lambda (vars vals ret-env)
              (if (= (length vars) 0)
                  ret-env
                  (let ((var (list-ref vars 0))
                        (var-tail (list-tail vars 1))
                        (val (expval->car vals))
                        (val-cdr (expval->cdr vals)))
                    (extend-new-env var-tail val-cdr (extend-env var val ret-env))))))
          (value-of body (extend-new-env vars vals env)))))))

(define init-env
  (lambda ()
    (extend-env 'i (num-val 1)
      (extend-env 'v (num-val 5)
        (extend-env 'x (num-val 10)
          (empty-env))))))

; test
(value-of-program (scan&parse "print(print(-(55, -(x,11))))"))
(value-of-program (scan&parse "minus(-(minus(5), 9))"))
(value-of-program (scan&parse "+(1,2)"))
(value-of-program (scan&parse "print(-(3,2))"))
(value-of-program (scan&parse "*(3,2)"))
(value-of-program (scan&parse "/(3,2)"))
(value-of-program (scan&parse "less?(+(1,2),*(3,2))"))
(value-of-program (scan&parse "null? emptylist"))
(value-of-program (scan&parse "cons (1, emptylist)"))
(value-of-program (scan&parse "let x = 4 in cons(x, cons(cons(-(x,1),emptylist),emptylist))"))
(value-of-program (scan&parse "list(1, 2, 3, 4)"))
(value-of-program (scan&parse "list()"))
(value-of-program (scan&parse "cond zero? 1 ==> 2 zero? 2 ==> 3 zero? -(34, 34) ==> +(1, 6) end"))
(value-of-program (scan&parse "if zero? 0 then 3 else 2"))
(value-of-program (scan&parse "let x = 30 in let x = -(x,1) y = -(x,2) in -(x,y)"))
(value-of-program (scan&parse "let x = 30 in let* x = -(x,1) y = -(x,2) in -(x,y)"))
(value-of-program (scan&parse "car cons(23,cons(45,emptylist))"))
(value-of-program (scan&parse "let u = 7 in unpack x y = cons(u, cons(3, emptylist)) in -(x,y)"))
