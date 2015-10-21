%option noyywrap #Just exit whenever we run out of input symbols

%{
    #include <iostream>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string>
    #include "Parser.tab.hpp"
%}

%%

[0-9]+                  { yylval.number = atoi(yytext);
                          return NUM;
                        }

[0-9]+"."[0.9]+         { yylval.number = atof(yytext);
                          return NUM;
                        }

["][^"]*["]             { yylval.str=strdup(yytext);
                          return ANY_STRING;
                        }

"printf"                { yylval.str = strdup(yytext);
                          return FUNC_NAME;
                        }

"if"                    { yylval.str = strdup(yytext);
                          return IF;
                        }

"while"                 { yylval.str = strdup(yytext);
                          return WHILE;
                        }

"int"                   { yylval.str = strdup(yytext);
                          return DATA_TYPE;
                        }

"double"                { yylval.str = strdup(yytext);
                          return DATA_TYPE;
                        }

"main()"*               { return FUN_ENTRY; }

"!="                    { yylval.str = strdup(yytext);
                          return DEQUOP;
                        }

"=="                    { yylval.str = strdup(yytext);
                          return DEQUOP;
                        }

">"                     { yylval.str = strdup(yytext);
                          return GT;
                        }

"<"                     { yylval.str = strdup(yytext);
                          return LT;
                        }

">="                    { yylval.str = strdup(yytext);
                          return GE;
                        }

"<="                    { yylval.str = strdup(yytext);
                          return LE;
                        }

"="                     { return EQUOP; }

","                     { return COMMA; }

"+"                     { yylval.str=strdup(yytext);
                          return ADDOP;
                        }

"-"                     { yylval.str=strdup(yytext);
                          return MUNISOP;
                        }

"*"                     { yylval.str=strdup(yytext);
                          return MULTIOP;
                        }
    
"/"                     { yylval.str=strdup(yytext);
                          return DIVOP;
                        }
    
"("                     { return LBKT; }

")"                     { return RBKT; }

"{"                     { return LCURLY; }

"}"                     { return RCURLY; }

";"                     { return SEMICN; }

\n                      {}

[a-zA-Z][a-zA-Z_0-9]*   { yylval.str=strdup(yytext);
                          return ID;
                        }

"#include".*            { return INCLUDE; }

"return 0"              { yylval.str=strdup(yytext);
                          return RETURN0;
                        }

"exit(0)"               { yylval.str=strdup(yytext);
                          return EXIT0;
                        }

%%
    
    
