#ifndef p_ast_hpp
#define p_ast_hpp

#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>

extern std::vector< std::map<std::string, int> >var_stack;


//A parent class Value, classes like number, operaor, id, will be the kids
class Value{
public:
    virtual ~Value()
    {}
    
    //Virtual functions will be implemented in kid classes
    virtual void Print(std::ostream &dst) const=0;
    virtual int get_value_number(){std::cout<<"Wrong if you see me"<<std::endl; return 2;}
    virtual std::string get_value(){ return "Wrong if you see me";}
    virtual int toArm(){std::cout<<"Wrong if you see me"<<std::endl; return 1;}
};


//Expression
class Expr : public Value{
private:
    std::vector<Value*> entries;
    int bracket;
    bool single;
public:
    Expr():single(false){}
    
    void Add(Value* op1, Value* oper, Value* op2){
        entries.push_back(op1);
        entries.push_back(oper);
        entries.push_back(op2);
        single = false;
    }
    
    void Add(Value* op1){
        entries.push_back(op1);
        single = true;
    }
    
    //Return the numerical results
    int get_value_number(){
        if(single){
            return entries[0]->get_value_number();
        }else{
            if(entries[1]->get_value() == "+"){
                int result = entries[0]->get_value_number() + entries[2]->get_value_number();
                return result;
            }else if(entries[1]->get_value() == "-"){
                int result = entries[0]->get_value_number() - entries[2]->get_value_number();
                return result;
            }else if(entries[1]->get_value() == "*"){
                int result = entries[0]->get_value_number() * entries[2]->get_value_number();
                return result;
            }else{
                int result = entries[0]->get_value_number() / entries[2]->get_value_number();
                return result;
            }
        }
    }
    
    void set_bracket(int bkt){bracket = bkt;}
    
    int toArm(){
        //Return the numerical results
        if(single){
            return entries[0]->get_value_number();
        }else{
            if(entries[1]->get_value() == "+"){
                int result = entries[0]->get_value_number() + entries[2]->get_value_number();
                return result;
            }else if(entries[1]->get_value() == "-"){
                int result = entries[0]->get_value_number() - entries[2]->get_value_number();
                return result;
            }else if(entries[1]->get_value() == "*"){
                int result = entries[0]->get_value_number() * entries[2]->get_value_number();
                return result;
            }else{
                int result = entries[0]->get_value_number() / entries[2]->get_value_number();
                return result;
            }
        }
    }
    
    virtual void Print(std::ostream &dst) const{
        int flag = 0;
        if(bracket == 1 && flag == 0){
            dst<<"(";
            flag = 1;
        }
        for(unsigned int i=0; i<entries.size(); i++){
            entries[i]->Print(dst);
        }
        if(flag == 1){
            dst<<")";
        }
    }
};


//Argument
class Argument {
private:
    Expr* expression;
public:
    Argument(Expr* e): expression(e){}
    void Print(std::ostream &dst){
        expression->Print(dst);
    }
};

//Argument_list
class Argument_list{
private:
    std::vector<Argument*> arglist;
public:
    Argument_list(Argument* a){arglist.push_back(a);}
    
    void Add(Argument* a){arglist.push_back(a);}
    
    void Print(std::ostream &dst){
        for(unsigned int i=0; i<arglist.size(); i++){
            arglist[i]->Print(dst);
            if(i != arglist.size()-1){dst<<",";}
        }
    }
};

//ID_list
class Id_list : public Value{
private:
    std::vector<std::string> idlist;
    std::string type;
public:
    void Add(std::string id){
        idlist.push_back(id);
    }

    void Add_type(std::string tp){type = tp;}
    
    std::string get_id(int i){ return idlist[i];}
    
    int get_size(){ return idlist.size(); }
    
    std::string get_type(){return type;}
    
    virtual void Print(std::ostream &dst) const{
        for(unsigned int i=0; i<idlist.size(); i++){
            if(i != idlist.size()-1){
                dst<<idlist[i]<<", ";
            }
        }
    }
};

//A parent class Function_body, classes like statement will be the kids
class Func_body{
public:
    virtual ~Func_body(){}
    
    //Virtual functions will be implemented in kid classes
    virtual void Print(std::ostream &dst) const=0;
    virtual std::vector<std::string> get_value_list(){std::vector<std::string> foo; return foo;}
    virtual std::vector<std::string> get_type_list(){ std::vector<std::string> foo; return foo;}
    virtual void Add_arg_list(Argument_list* foo, bool bool_foo){
        std::cout<<bool_foo;
        foo->Print(std::cout);
    }
    virtual void toArm(int pairs){std::cout<<pairs<<"lol"<<std::endl;}
    
};

//Variable
class Var : public Value, public Func_body{
private:
    static std::vector<std::string> value;
    std::string current_value;
    static std::vector<std::string> type;
    std::string current_type;
    bool semicn;
    bool defined;
public:
    
    Var(){}
    
    Var(std::string _value){
        semicn = false;
        value.push_back(_value);
        current_value = _value;
        
    }
    
    //Add the type for a variable
    void Add_type(std::string _type){
        type.push_back(_type);
        current_type = _type;
    }
    
    //For those variables which has either been divided before or caused undefined problems
    Var(std::string _value, bool df){
        semicn = false;
        defined = df;
        value.push_back(_value);
        current_value = _value;
        type.push_back("NotaType");
        
    }
    
    //Calculate the spaces need to be allowcated
    int Calculate_space(){
        if(value.size() == 0){
            std::cout<<"No variable declared"<<std::endl;
        }else if(value.size() == 1){
            return 1*8;
        }
        
        int duplicate = 0;
        for(unsigned int i=0; i<value.size(); i++){
            if(type[i] == "NotaType")
                duplicate++;
        }
        int pairs = (value.size()-duplicate) / 2 * 8;
        if((value.size()-duplicate)%2 != 0){
            pairs+=8;
        }
        
        return pairs;
        
    }
    
    //Return the numerical results of variables
    int get_value_number(){
        typedef std::vector< std::map<std::string, int> >  vec_map;
        typedef std::map<std::string, int> map_v;
        vec_map::const_iterator vit;
        for(vit=var_stack.begin(); vit!=var_stack.end(); ++vit){
            map_v::const_iterator it = vit->begin();
            if(current_value == it->first){
                return it->second;
            }
        }
        return 0;
    }
    
    void Add_semi(){semicn = true; }
    
    //Add a lists of variables of same data_type e.g: int a, b, c;
    void Add_id(Id_list* idlist){
        for(int i=0; i<idlist->get_size(); i++){
            value.push_back(idlist->get_id(i));
            type.push_back(idlist->get_type());
        }
    }
    
    std::vector<std::string> get_value_list() {return value;}
    std::vector<std::string> get_type_list() {return type;}
    
    std::string get_value(){
        return current_value;
    }
    
    std::string get_type(){
        return current_type;
    }
    
    virtual void Print(std::ostream &dst) const{
        if(semicn){
            dst<<current_type;
            for(unsigned int i=0; i<value.size(); i++){
                dst<<" "<<value[i];
                if(i != value.size()-1){
                    dst<<",";
                }
            }
            dst<<";";
        }else{
            dst<<current_type<<" "<<current_value;
        }
    }
};

//STATEMENT
class Statement : public Func_body{
private:
    typedef std::map<std::string, Value*> map_s;
    std::map<std::string, Value*> stment;
    Var* variable;
    Value* expression;
    bool declaration;
    static std::vector<bool> whether_fill;
    std::map<std::string, int> variables_stack;   //variables in the stack, stored in forms of "a, 3", "b, 5"
    int index;       //Index used in the stack, indexes are in times of 4
public:
    Statement(): declaration(false){}
    
    void Add(Var* var, Value* expr){
        std::string temp = var->get_value();
        stment[temp] = expr;
        expression = expr;
        variable = var;
    }
    
    void Add(Var* var){
        variable = var;
        declaration = true;
    }
    
    int get_stack_size(){return whether_fill.size();}
    
    //To arm
    void toArm(int pairs){
        std::ofstream outfile;
        outfile.open("Myfirstarm.s", std::ios::app);
        index = pairs/4;
        var_stack.resize(index);
        whether_fill.resize(index, false);
        if(variable->get_type() == "int"){
            variables_stack[variable->get_value()] = expression->toArm();
            for(int i=index-1; i>=0; i--){
                if(!whether_fill[i]){
                    var_stack[i] = variables_stack;
                    outfile<<"mov r3, #"<<variables_stack[variable->get_value()]<<std::endl;
                    outfile<<"str r3, [fp, #-"<<(i+1)*4<<"]"<<std::endl;
                    whether_fill[i] = true;
                    break;
                }
            }
        }
        outfile.close();
    }
    
    virtual void Print(std::ostream &dst) const{
        if(declaration){
            variable->Print(dst);
        }else{
            map_s::const_iterator it;
            for(it = stment.begin(); it != stment.end(); ++it){
                if(it->first == variable->get_value()){
                    variable->Print(dst);
                    dst<<"=";
                    it->second->Print(dst);
                }
            }
        }
    }
};

//STATEMENT_list
class Statement_list : public Func_body {
private:
    std::vector<Statement*> st_list;
    
public:
    Statement_list(){}
    void Add(Statement* statement){
        st_list.push_back(statement);
    }
    
    void toArm(int pairs){
        for(unsigned int i = 0; i<st_list.size(); i++){
            st_list[i]->toArm(pairs);
        }
    }
    
    virtual void Print(std::ostream &dst) const{
        for(unsigned int i = 0; i<st_list.size(); i++){
            st_list[i]->Print(dst);
            dst<<";";
        }
    }
};

//Function call
class Func_call : public Func_body{
private:
    std::string func_name;
    Argument_list* arg_list;
    bool have_arg;
public:
    Func_call(std::string name): func_name(name), have_arg(false){}
    
    void Add_arg_list(Argument_list* al, bool have){
        arg_list = al;
        have_arg = have;
    }
    
    virtual void Print(std::ostream &dst) const{
        dst<<func_name<<"(";
        if(have_arg){
            arg_list->Print(dst);
        }
        dst<<");";
    }
    
};

//Conditional expression e.g if and while loops
class Conditions :public Func_body {
private:
    Value* EXPR1;
    Value* EXPR2;
    Statement_list* CON_BODY;
    std::string CON_type;
    std::string oper;
public:
    void Add_condition(Value* ex1, std::string op, Value* ex2){
        EXPR1 = ex1;
        oper = op;
        EXPR2 = ex2;
    }
    
    void Add_statement(Statement_list* st, std::string type){
        CON_BODY = st;
        CON_type = type;
    }
    
    void toArm(int pairs){
        if(CON_type == "if"){
            std::ofstream outfile;
            outfile.open("Myfirstarm.s", std::ios::app);
            
            std::string var_name = EXPR1->get_value();
            int index = 0;
            typedef std::vector< std::map<std::string, int> >  vec_map;
            typedef std::map<std::string, int> map_v;
            vec_map::const_iterator vit;
            for(vit=var_stack.begin(); vit!=var_stack.end(); ++vit){
                map_v::const_iterator it = vit->begin();
                index++;
                if(var_name == it->first){
                    break;
                }
            }
            int compare_num = EXPR2->toArm();
            
            outfile<<"ldr r3, [fp, #-"<<index*4<<"]"<<std::endl;
            outfile<<"cmp r3, #"<<compare_num<<std::endl;
            if(oper == "=="){
                outfile<<"bne .L2"<<std::endl;
            }else if(oper == "!="){
                outfile<<"beq .L2"<<std::endl;
            }else if(oper == ">"){
                outfile<<"ble .L2"<<std::endl;
            }else if(oper == "<"){
                outfile<<"bge .L2"<<std::endl;
            }else if(oper == ">="){
                outfile<<"blt .L2"<<std::endl;
            }else if(oper == "<="){
                outfile<<"bgt .L2"<<std::endl;
            }
            outfile.close();
            CON_BODY->toArm(pairs);
            
            outfile.open("Myfirstarm.s", std::ios::app);
            outfile<<".L2"<<std::endl;
            outfile.close();
        }
    }
    
    virtual void Print(std::ostream &dst) const{
        
        if(CON_type == "if"){
            dst<<"if(";
        }else if(CON_type == "while"){
            dst<<"while(";
        }
        EXPR1->Print(dst);
        dst<<oper;
        EXPR2->Print(dst);
        dst<<"){";
        CON_BODY->Print(dst);
        dst<<"}";
    }
};

//The toplevel class, the top level class
class Func_body_list : public Func_body{
private:
    std::vector<Func_body*> fc_list;
    static int func_call_flag;
    Var* variable;
    std::string return_value;
    
public:
    
    void Add(Func_body* f){
        fc_list.push_back(f);
    }
    
    void set_func_flag(){func_call_flag = 1;}
    
    void set_return(std::string rt){return_value = rt;}
    
    //debug, so far only multi-declaration and undefination errors will be recongnized
    void debug_declare(Var* var){
        variable = var;
        std::vector<std::string> value = var->get_value_list();
        std::vector<std::string> type = var->get_type_list();
        bool redefine_error = false;
        for(unsigned int i=0; i<value.size(); i++){
            if(!redefine_error){
                for(unsigned int k=0; k<value.size(); k++){
                    if(i != k){
                        if(value[i] == value[k]){
                            if(type[i] == type[k]){
                                std::cout<<"Error: Variable name '"<<value[k]<<"' already declared"<<std::endl;
                                redefine_error = true;
                                break;
                            }
                        }
                    }
                }
            }
            if(type[i] == "NotaType"){
                bool notdefine_error = true;
                for(unsigned int j=0; j<value.size(); j++){
                    if(i != j){
                        if(value[j] == value[i]){
                            notdefine_error = false;
                            break;
                        }
                    }
                }
                if(notdefine_error){
                std::cout<<"Error: Variable name '"<<value[i]<<"' not defined"<<std::endl;
                }
            }
            
        }
    }
    
    void toArm(int pairs){
        if(func_call_flag != 1){
            std::ofstream outfile;
            outfile.open("Myfirstarm.s", std::ios::app);
            
            outfile<<"str fp, [sp, #-4]!"<<std::endl;
            outfile<<"add fp, sp, #0"<<std::endl;
            outfile<<"sub sp, sp, #"<<variable->Calculate_space()<<std::endl;
            outfile.close();
            for(unsigned int i = 0; i<fc_list.size(); i++){
                fc_list[i]->toArm(pairs);
            }
            outfile.open("Myfirstarm.s", std::ios::app);
            outfile<<"mov r3, #0"<<std::endl;   //return 0;
            outfile<<"mov r0, r3"<<std::endl;
            outfile<<"add sp, fp, #0"<<std::endl;
            outfile<<"ldmfd sp!, {fp}"<<std::endl;
            outfile<<"bx lr"<<std::endl;
            outfile.close();
            
        }
    }
    
    virtual void Print(std::ostream &dst) const{
        dst<<"int main(){"<<std::endl;
        for(unsigned int i = 0; i<fc_list.size(); i++){
            fc_list[i]->Print(dst);
        }
        dst<<return_value<<std::endl;
        dst<<"}";
    }
};


//NUMBER
class Number : public Value{
private:
    double value;
    
public:
    Number(double _value) : value(_value){}
    
    int get_value_number(){
        return value;
    }
    
    virtual void Print(std::ostream &dst) const{
        dst<<value;
    }
};

//OPERATOR
class Optr : public Value{
private:
    std::string op;
public:
    Optr(std::string _value) : op(_value){}
    std::string get_value(){ return op;}
    
    virtual void Print(std::ostream &dst) const{
        dst<<op;
    }
};

#endif

