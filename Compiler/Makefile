CPPFLAGS += -W -Wall -g

all:result

result:Parser.tab.cpp lex.yy.cpp compiler.cpp
	g++ $(CPPFLAGS) compiler.cpp Parser.tab.cpp lex.yy.cpp -o result

lex.yy.cpp:Lexer.l Parser.tab.hpp
	flex -o lex.yy.cpp Lexer.l

Parser.tab.cpp Parser.tab.hpp:Parser.y
	bison -d Parser.y -o Parser.tab.cpp

clean:
	rm result lex.yy.cpp Parser.tab.cpp Parser.tab.hpp

