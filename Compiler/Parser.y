%code requires{
    #include <stdio.h>
    #include <iostream>
    #include "p_ast.hpp"
    
    int yylex(void); //recognizes tokens from the input stream and returns them to the parser.
    void yyerror(const char *);
    extern Func_body_list *g_ast; // A way of getting the AST out
}

%union{
    Value *cvalue;
    Func_body *func_body;
    Func_body_list *func_body_list;
    Expr *cexpr;
    Optr *optr;
    Statement *statement;
    Statement_list *statement_list;
    Var *var;
    Id_list *id_list;
    Conditions *conditions;
    Argument_list *argument_list;
    Argument *argument;
    double number;
    const char* str;
}

%{
    std::vector<std::string> Var::value;
    std::vector<std::string> Var::type;
    std::vector< std::map<std::string, int> > var_stack;
    std::vector<bool> Statement::whether_fill;
    int Func_body_list::func_call_flag;
%}

%token COMMA LCURLY RCURLY FUN_ENTRY INCLUDE COMMENT SEMICN  EQUOP LBKT RBKT EOLINE
%token <str> ID
%token <str> RETURN0
%token <str> EXIT0
%token <str> IF
%token <str> WHILE
%token <str> ADDOP
%token <str> MUNISOP
%token <str> MULTIOP
%token <str> DIVOP
%token <str> DEQUOP
%token <str> GT
%token <str> LT
%token <str> GE
%token <str> LE
%token <str> DATA_TYPE
%token <str> ANY_STRING
%token <str> FUNC_NAME
%token <number> NUM
%type <cexpr> EXPR
%type <cvalue> FACTOR
%type <statement> ARITHMETIC
%type <func_body> FUN_BODY
%type <statement_list> STATEMENT_list
%type <conditions> CONDITION
%type <func_body_list> FUN_BODY_list
%type <var> DATA_VAR
%type <var> DATA_DECLARE
%type <id_list> ID_list
%type <func_body> FUNC_CALL
%type <argument_list> ARGUMENT_list
%type <argument> ARGUMENT
%left ADDOP MUNISOP
%left MULTIOP DIVOP


%%
StPoint : INCLUDE_list FUN
| FUN
;

INCLUDE_list : INCLUDE
| INCLUDE_list INCLUDE
;

FUN : DATA_TYPE FUN_ENTRY LCURLY RCURLY { std::cout << "int main(){NOTHING}" << std::endl;}
| DATA_TYPE FUN_ENTRY LCURLY FUN_BODY_list RETURN0 SEMICN RCURLY {Var* v = new Var(); $4->set_return($5); $4->debug_declare(v); $4->toArm(v->Calculate_space()); g_ast = $4;}
| DATA_TYPE FUN_ENTRY LCURLY FUN_BODY_list EXIT0 SEMICN RCURLY {Var* v = new Var(); $4->set_return($5); $4->debug_declare(v); $4->toArm(v->Calculate_space()); g_ast = $4;}
;

FUN_BODY_list : FUN_BODY_list FUN_BODY { $1->Add($2); $$ = $1;}
| FUN_BODY {$$ = new Func_body_list(); $$->Add($1); }
;


FUN_BODY : STATEMENT_list{$$ = $1;}
| IF LBKT CONDITION RBKT LCURLY STATEMENT_list RCURLY {$3->Add_statement($6, $1); $$ = $3;}
| WHILE LBKT CONDITION RBKT LCURLY STATEMENT_list RCURLY {$3->Add_statement($6, $1); $$ = $3;}
| FUNC_CALL {Func_body_list* f = new Func_body_list(); f->set_func_flag(); $$ = $1;}
;

FUNC_CALL : FUNC_NAME LBKT ARGUMENT_list RBKT SEMICN {$$ = new Func_call($1); $$->Add_arg_list($3, true);}
;

ARGUMENT_list : ARGUMENT_list COMMA ARGUMENT {$1->Add($3); $$ = $1;}
| ARGUMENT {$$ = new Argument_list($1);}
;

ARGUMENT : EXPR {$$ = new Argument($1);}
;

CONDITION: ID DEQUOP EXPR {Var* v = new Var($1, true); $$ = new Conditions(); $$->Add_condition(v, $2, $3); }
| ID GT EXPR {Var* v = new Var($1, true); $$ = new Conditions(); $$->Add_condition(v, $2, $3); }
| ID LT EXPR {Var* v = new Var($1, true); $$ = new Conditions(); $$->Add_condition(v, $2, $3); }
| ID GE EXPR {Var* v = new Var($1, true); $$ = new Conditions(); $$->Add_condition(v, $2, $3); }
| ID LE EXPR {Var* v = new Var($1, true); $$ = new Conditions(); $$->Add_condition(v, $2, $3); }
;

STATEMENT_list : STATEMENT_list ARITHMETIC {$1->Add($2); $$ = $1; }
| ARITHMETIC {$$ = new Statement_list(); $$->Add($1);}
;

ARITHMETIC: DATA_VAR EQUOP EXPR SEMICN { $$ = new Statement(); $$->Add($1, $3); }
| DATA_DECLARE {$$ = new Statement(); $$->Add($1);}
;

DATA_DECLARE : DATA_VAR SEMICN {$1->Add_semi(); $$ = $1;}
| DATA_VAR COMMA ID_list SEMICN {$3->Add_type($1->get_type()); $1->Add_id($3); $1->Add_semi(); $$ = $1;}
;

ID_list : ID_list COMMA ID {$1->Add($3); $$ = $1;}
| ID { $$ = new Id_list(); $$->Add($1); }
;

DATA_VAR: DATA_TYPE ID {$$ = new Var($2); $$->Add_type($1);}
| ID {$$ = new Var($1, true); }
;

EXPR: EXPR ADDOP EXPR { $$ = new Expr(); Optr* a = new Optr($2); $$->Add($1, a, $3);}
| EXPR MUNISOP EXPR { $$ = new Expr(); Optr* a = new Optr($2); $$->Add($1, a, $3);}
| EXPR MULTIOP EXPR { $$ = new Expr(); Optr* a = new Optr($2); $$->Add($1, a, $3);}
| EXPR DIVOP EXPR { $$ = new Expr(); Optr* a = new Optr($2); $$->Add($1, a, $3);}
| FACTOR {$$ = new Expr(); $$->Add($1);}
;

FACTOR: LBKT EXPR RBKT {$2->set_bracket(1); $$ = $2; }
| NUM { $$ = new Number($1);}
| ID {$$ = new Var($1, true); }
;

%%

void  yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
    exit(1);
}
