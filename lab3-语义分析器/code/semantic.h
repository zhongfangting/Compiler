#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstdio>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include "lexical_analysis.hpp"
using namespace std;

#define is_print 1

vector<string> table_column={"i","p","+","-","*","/","%","(",")","#"};
set<int> lexical_value= {33,34,35,36,45,48,49,50,51,52,70};;  //表达式字符的内码值
vector<string> table_row={"S","E","A","T", "B", "F"};  //产生式的左部
//vector<string> Grammars_right={"S","p=E","ExT","T","TyF","F","i","p","(E)"}; //产生式的右部

//四元式
struct Quater{
    string op;
    string left_exp;
    string right_exp;
    string result;
    Quater(string o="", string exp1="", string exp2="", string res="")
            :op(o),left_exp(exp1),right_exp(exp2),result(res){}
    
    void clear(){
        op="";
        left_exp="";
        right_exp="";
        result="";
    }

    string get_expression(){
        string expression=op+" "+left_exp+" "+right_exp+" "+result;
        return expression;
    }
    
};

//将队列转为字符串
string queue_to_string(queue<string> queue){
    string str="";
    while(!queue.empty()){
        str += queue.front();
        queue.pop();
    }
    return str;
}

//将栈转为字符串
string stack_to_string(stack<string> stack,int type){
    vector<string> temp;
    string str="";
    //if(type==1) str="(";
    while(!stack.empty()){
        temp.emplace_back(stack.top());
        stack.pop();
    }

    for(int i=temp.size()-1; i>=0; i--){
        str += temp[i];
        if(type==1){
            str +=" ";
           // if(i == 0) str += ")";
        }
    }

    return str;
}

//得到列号
int get_column(string str){
    for(int i=0;i<table_column.size();i++){
        if(table_column[i] == str)
            return i;
    }
    return -1;
}

//得到行号
int get_row(string str){
    for(int i=0;i<table_row.size();i++){
        if(table_row[i] == str)
            return i;
    }
    return -1;
}

//将词法分析后的序列转为语法分析的输入字符
string token_convert(int value){
    if(value == 33) return "p";  //返回标识符
    else if(value == 34) return "i"; //返回常数
    else if(value == 35) return "(";
    else if(value == 36) return ")";
    else if(value == 45) return "#";  //当识别为;时，返回"#"作为算术表达式的结束符号
    else if(value == 48) return "+";  
    else if(value == 49) return "-";
    else if(value == 50) return "*";
    else if(value == 51) return "/";
    else if(value == 52) return "%";
    else if(value == 70) return "="; 
}

//将LL1分析表取出的内容分离开，方便逆序压栈
vector<string> operate_generator(string str,string ch){
    vector<string> generator;
    for(int i=0; i<str.size(); i++){
        if(str[i] == '{'){
            //++i;
            string value="";
            while(++i){
                if(str[i] == '}') break;
                value += str[i];
            }
            if(value == "PUSH") value = value+"("+ch+")";
            generator.push_back(value);
        }
        else{
            generator.push_back(string(1,str[i]));
        }
    }
    return generator;
}

//取出PUSH(p)的p
string operate_push(string str){
    string value="";
    for(int i=str.find("(")+1;i<str.size();i++){
        if(str[i] == ')') break;
        value += str[i];
    }
    return value;
}

//LL1语义分析
class LL1_Semantic{
protected:
    ofstream outfile;  //输出文件
    vector<vector<string>> LL1Table;  //LR分析表
public:
    LL1_Semantic(){
        outfile.open("LL1_output.txt");
        LL1Table={
            {"","p{PUSH}=E{GEQ(=)}","","","","","","","",""},
            {"TA","TA","","","","","","TA","",""},
            {"","","+T{GEQ(+)}A","-T{GEQ(-)}A","","","","","e","e"},
            {"FB","FB","","","","","","FB","",""},
            {"","","e","e","*F{GEQ(*)}B","/F{GEQ(/)}B","%F{GEQ(%)}B","","e","e"},
            {"i{PUSH}","p{PUSH}","","","","","","(E)","",""}
        };
    }

    void analysis(queue<string> expression,queue<string> input){
        if(is_print){
            cout<<"*********************************************************** Output of LL1 Semantic Analyzer:***********************************************************"<<endl;
            cout<<"expression:"<<queue_to_string(expression)<<"----------"<<queue_to_string(input)<<endl;
        }

        outfile<<"*********************************************************** Output of LL1 Semantic Analyzer:***********************************************************"<<endl;
        outfile<<"expression:"<<queue_to_string(expression)<<"----------"<<queue_to_string(input)<<endl;

        expression.push("#");
        int t_i=1; //t的下标

        stack<string> SYN; //LL1语法分析栈
        stack<string> SEM; //语义分析栈
        vector<Quater> quaters; //四元式  

        string cur_symbol="";   //当前字符
        Quater cur_quater;     //当前四元式
        string operation="Start";    //操作

        SYN.push("#");
        SYN.push("S");  //S是开始符号

        if(is_print){
            cout << setiosflags(ios::left);
            cout << setw(45) << "SYN Analysis stack" << setw(25) << "Current character"         
                 << setw(25) << "Remaining sequence" << setw(30) << "Operation" << setw(22)
                 << "SEM Analysis stack" << setw(14) << "Quater" << endl;
            cout << setw(45) << stack_to_string(SYN,0) << setw(25) << cur_symbol 
                 << setw(25) << queue_to_string(input) << setw(30) << operation << setw(22)
                 << stack_to_string(SEM,0) << setw(14) <<cur_quater.get_expression() << endl;
        }
        outfile << setiosflags(ios::left);
        outfile << setw(45) << "SYN Analysis stack" << setw(25) << "Current character"         
                << setw(25) << "Remaining sequence" << setw(30) << "Operation" << setw(22)
                << "SEM Analysis stack" << setw(14) << "Quater" << endl;
        outfile << setw(45) << stack_to_string(SYN,0) << setw(25) << cur_symbol 
                << setw(25) << queue_to_string(input) << setw(30) << operation << setw(22)
                << stack_to_string(SEM,0) << setw(14) <<cur_quater.get_expression() << endl;
 
        while(!SYN.empty()){
            // cout<<"aa"<<endl;
            cur_quater.clear();  //清空当前四元式
            string syn_str=SYN.top();  //取出语法分析栈栈顶字符串
            // 取出当前字符
            if(cur_symbol.empty()){
                if(input.empty()){
                    if(is_print){
                        cout << "error: input is invalid!" << endl;
                    }
                    outfile << "error: input is invalid!" << endl <<endl;
                    return;
                }
                cur_symbol=input.front();
                input.pop();
            }
            string syn_content=stack_to_string(SYN,0);  //语法分析栈当前内容
            string cur_str=cur_symbol;  //保存当前字符
            SYN.pop();
            // cout<<syn_str<<" " <<cur_symbol<<" "<<cur_str<<endl;
            // cout<<syn_content<<endl;

            // 当前栈顶字符匹配上当前读入的字符
            if(syn_str == cur_symbol){
                operation="match:"+syn_str+"="+cur_symbol;
                cur_symbol.clear();
                expression.pop();
            }
            // 如果栈顶字符含PUSH，那么语义栈push
            else if(syn_str.find("PUSH") != syn_str.npos){
                string push_symbol=operate_push(syn_str);
                SEM.push(push_symbol);
                operation="push to SEM:"+push_symbol;
            }
            else if(syn_str.find("GEQ") != syn_str.npos){
                string op;  //运算符
                op.push_back(syn_str[4]); //syn_str类似GEQ(op),syn_str[5]=op

                if(op == "="){
                    string exp1=SEM.top();
                    SEM.pop();
                    string exp2="_";
                    string result=SEM.top();
                    SEM.pop();
                    cur_quater=Quater(op,exp1,exp2,result);
                }
                else{
                    string exp2=SEM.top();
                    SEM.pop();
                    string exp1=SEM.top();
                    SEM.pop();
                    string result="t"+to_string(t_i);  //生成结果ti
                    SEM.push(result);
                    cur_quater=Quater(op,exp1,exp2,result);
                    t_i++;
                }

                quaters.push_back(cur_quater);
                operation="generate quaternion";
            }
            //如果是非终结符，逆序push进SYN
            else if(get_row(syn_str) != -1){
               // cout<<"aa"<<endl;
                int row=get_row(syn_str);  //LL1分析表的行号
                int column=get_column(cur_symbol); //LL1分析表的列号
                string table_content=LL1Table[row][column];
                
                if(table_content == ""){
                    if(is_print){
                        cout<<"error: search table failed!"<<endl<<endl;
                    }
                    outfile<<"error: search table failed!"<<endl<<endl;
                    return;
                }
                operation="select:"+syn_str+"->"+table_content;
                
                if(table_content != "e"){
                    vector<string> generator;
                    string ch="";
                    if(table_content.find("PUSH") != table_content.npos){
                        ch=expression.front();
                    }
                    generator=operate_generator(table_content,ch);
                   
                    for(int i=generator.size()-1; i>=0; i--){
                        SYN.push(generator[i]);
                    }
                }
            }
            else{
                if(is_print){
                    cout<<"error:matching error!"<<endl<<endl;
                }
                outfile<<"error:matching error!"<<endl<<endl;
                return;
            }

            cout<<syn_content<<endl;

            if(is_print){
                cout << setw(45) << syn_content << setw(25) << cur_str 
                     << setw(25) << queue_to_string(input) << setw(30) << operation << setw(22)
                     << stack_to_string(SEM,1) << setw(14) <<cur_quater.get_expression() << endl;
            }
            outfile  << setw(45) << syn_content << setw(25) << cur_str 
                     << setw(25) << queue_to_string(input) << setw(30) << operation << setw(22)
                     << stack_to_string(SEM,1) << setw(14) <<cur_quater.get_expression() << endl;
        }
        
        if(is_print){
            cout << "Right!" << endl; 
            cout <<"The quaternions are:"<<endl;
        }
        outfile << "Right!" << endl; 
        outfile <<"The quaternions are:"<<endl;

        for(int i=0; i<quaters.size(); i++){
            if(quaters[i].op == "="){
                if(is_print){
                    cout << "(" << i+1 << ") " << quaters[i].result << " = "
                         << quaters[i].left_exp << endl;
                }
                outfile << "(" << i+1 << ") " <<"("<<quaters[i].get_expression()<<")"<<"----->"<< quaters[i].result << " = "
                        << quaters[i].left_exp << endl;
            }
            else{
                if(is_print){
                    cout << "(" << i+1 << ") " << quaters[i].result << " = "
                         << quaters[i].left_exp <<" "<<quaters[i].op<<" "<<quaters[i].right_exp<< endl;
                }
                outfile << "(" << i+1 << ") " <<"("<<quaters[i].get_expression()<<")"<<"----->"<< quaters[i].result << " = "
                         << quaters[i].left_exp <<" "<<quaters[i].op<<" "<<quaters[i].right_exp<< endl;
            }
        }
        outfile<<endl<<endl;
    }

    void LL1Semantic(Node *head){
        queue<string> expression;
        queue<string> input;

        while(head != NULL){
            string word=head->content;
            int value=head->value;
            // cout<<word<<" "<<value<<endl;
            //如果不符合算术表达式，则不分析
            if(lexical_value.count(value) == 0){
                while(!expression.empty()) expression.pop();
                while(!input.empty()) input.pop();
            }
            else{
                string cur=token_convert(value);
                input.push(cur);

                if(cur == "#"){
                    if(input.size()>3) analysis(expression,input);
                    while(!expression.empty()) expression.pop();
                    while(!input.empty()) input.pop();
                }
                else{
                    expression.push(word);
                }
            }
            head=head->next;
        }
    }
};

#endif

// int main(){
//     LL1_Semantic LL1_Semantic;
//     Node *head=lexical_result();
//     LL1_Semantic.LL1Semantic(head);
//     cout<<"LL1 semantic analysis is complete,the result of LL1 is saved in LL1_output.txt."<<endl;
//     getchar();
//     getchar();
//     return 0;
// }