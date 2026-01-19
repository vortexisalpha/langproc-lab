CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include

all : bin/print_canonical bin/eval_expr

BISON = /usr/local/opt/bison/bin/bison
FLEX = /usr/local/opt/flex/bin/flex

HEADERS := include/ast.hpp \
           include/ast/ast_expression.hpp \
           include/ast/ast_primitives.hpp \
           include/ast/ast_operators.hpp \
           include/ast/ast_unary.hpp \
           include/ast/ast_functions.hpp

src/maths_parser.tab.cpp src/maths_parser.tab.hpp : src/maths_parser.y
	$(BISON) -v -d src/maths_parser.y -o src/maths_parser.tab.cpp

src/maths_lexer.yy.cpp : src/maths_lexer.flex src/maths_parser.tab.hpp
	$(FLEX) -o src/maths_lexer.yy.cpp  src/maths_lexer.flex

src/print_canonical.o : src/print_canonical.cpp $(HEADERS)
	g++ $(CPPFLAGS) -c -o $@ $<

src/eval_expr.o : src/eval_expr.cpp $(HEADERS)
	g++ $(CPPFLAGS) -c -o $@ $<

src/maths_parser.tab.o : src/maths_parser.tab.cpp $(HEADERS)
	g++ $(CPPFLAGS) -c -o $@ $<

src/maths_lexer.yy.o : src/maths_lexer.yy.cpp $(HEADERS)
	g++ $(CPPFLAGS) -c -o $@ $<

bin/print_canonical : src/print_canonical.o src/maths_parser.tab.o src/maths_lexer.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/print_canonical $^

bin/eval_expr : src/eval_expr.o src/maths_parser.tab.o src/maths_lexer.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/eval_expr $^


clean :
	rm -f src/*.o
	rm -f bin/*
	rm -f src/*.tab.cpp
	rm -f src/*.yy.cpp
	rm -f src/maths_parser.output
	rm -f src/maths_parser.tab.hpp
