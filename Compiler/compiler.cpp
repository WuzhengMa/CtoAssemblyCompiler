#include "p_ast.hpp"
#include "Parser.tab.hpp"
#include<iostream>
using namespace std;
Func_body_list *g_ast;

int main(){
    
    
    yyparse();
    
    Func_body_list *ast = g_ast;
    ast->Print(cout);
    
    
    return 0;
}

