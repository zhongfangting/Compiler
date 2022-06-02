#ifndef _LR0_H_
#define _LR0_H_

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

#define is_print 0

vector<string> symbol={"i","p","x","y","(",")","=","#","Z", "S", "E", "T", "F"};
set<int> lexical_value= {33,34,35,36,45,48,49,50,51,52,70};;  //表达式字符的内码值
vector<string> Grammars_left={"Z", "S", "E","E", "T", "T", "F", "F", "F"};  //产生式的左部
vector<string> Grammars_right={"S","p=E","ExT","T","TyF","F","i","p","(E)"}; //产生式的右部

void print(){
    for(int i=0; i<Grammars_left.size();i++){
        cout<<Grammars_left[i]<<"->"<<Grammars_right[i]<<endl;
    }
}

//将词法分析后的序列转为语法分析的输入字符
string token_convert(int value){
    if(value == 33) return "p";  //返回标识符
    else if(value == 34) return "i"; //返回常数
    else if(value == 35) return "(";
    else if(value == 36) return ")";
    else if(value == 45) return "#";  //当识别为;时，返回"#"作为算术表达式的结束符号
    else if(value == 48 || value == 49) return "x";  //当识别为+,-时，返回x
    else if(value == 50 || value == 51 || value == 52) return "y";
    else if(value == 70) return "="; 
}

//得到列号
int get_column(string str){
    for(int i=0;i<symbol.size();i++){
        if(symbol[i] == str)
            return i;
    }
    return -1;
}

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
string stack_to_string(stack<string> stack){
    vector<string> temp;
    string str="";
    while(!stack.empty()){
        temp.emplace_back(stack.top());
        stack.pop();
    }

    for(int i=temp.size()-1; i>=0; i--){
        str += temp[i];
        if(i != 0) str +=" ";
    }

    return str;
}


//LR分析法
class LR{
protected:
    ofstream outfile;  //输出文件
    vector<vector<string>> LRTable;  //LR分析表
public:
    //构造LL1分析表
    LR(){
        outfile.open("LR_output.txt");
        LRTable={
            {"","p2","","","","","","","","S1","","",""},
            {"","","","","","","","OK","","","","",""},
            {"","","","","","","=3","","","","","",""},
            {"i8","p9","","","(7","","","","","","E4","T5","F6"},
            {"","","x10","","","","","r2","","","","",""},
            {"","","r4","y11","","","","r4","","","","",""},
            {"r6","r6","r6","r6","r6","r6","r6","r6","","","","",""},
            {"i8","p9","","","(7","","","","","","E12","T5","F6"},
            {"r7","r7","r7","r7","r7","r7","r7","r7","","","","",""},
            {"r8","r8","r8","r8","r8","r8","r8","r8","","","","",""},
            {"i8","p9","","","(7","","","","","","","T13","F6"},
            {"i8","p9","","","(7","","","","","","","","F14"},
            {"","","x10","","",")15","","","","","","",""},
            {"","","r3","y11","","","","r3","","","","",""},
            {"r5","r5","r5","r5","r5","r5","r5","r5","","","","",""},
            {"r9","r9","r9","r9","r9","r9","r9","r9","","","","",""}
        };
    }

    int string_to_int(string str){
        string s(str,1);
        int number=stoi(s);
        return number;
    }

    void analysis(string expression,queue<string> input){
        if(is_print){
            cout<<"*************************** Output of LR synatactic analyzer:***************************"<<endl;
            cout<<"expression:"<<expression<<"——————>"<<queue_to_string(input)<<endl;
        }

        outfile<<"*************************** Output of LR synatactic analyzer:***************************"<<endl;
        outfile<<"expression:"<<expression<<"——————>"<<queue_to_string(input)<<endl;

        stack<string> LRStack;  //LR分析栈
        string cur_symbol="";   //当前字符
        string operation="Start";    //操作

        LRStack.push("#0");
        // LRStack.push("S");   //S是开始符号
        if(is_print){
            cout << setiosflags(ios::left);
            cout << setw(40) << "Analysis stack" << setw(25) << "Current character"         
                << setw(25) << "Remaining sequence" << setw(30) << "Operation" << endl;
            cout << setw(40) << stack_to_string(LRStack) << setw(25) << cur_symbol 
                << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
        }

        outfile << setiosflags(ios::left);
        outfile << setw(40) << "Analysis stack" << setw(25) << "Current symbol"         
             << setw(25) << "Remaining seq" << setw(30) << "Operation" << endl;
        outfile << setw(40) << stack_to_string(LRStack) << setw(25) << cur_symbol 
             << setw(25) << queue_to_string(input) << setw(30) << operation << endl;

        while(!LRStack.empty()){
            string stack_symbol=LRStack.top();  //读入栈顶字符
            if(cur_symbol.empty()){     //读取当前字符
                if(!input.empty()){
                    cur_symbol=input.front();
                    input.pop();
                }
            }

            string stack_cur=stack_to_string(LRStack);   //当前栈的内容
            string cur=cur_symbol; 
            int column=get_column(cur_symbol);  //LR分析表的列号
            int row=string_to_int(stack_symbol);  //LR分析表的行号
            string LR_content=LRTable[row][column];
            if(LR_content == ""){
                if(is_print){
                    cout<<"error: search table failed."<<endl<<endl;
                }
                outfile<<"error: search table failed."<<endl<<endl;
                return;
            }
            else if(LR_content[0] == 'r'){    //归约
                int grammar_num=string_to_int(LR_content);  //得到产生式的序号
                string grammar_left=Grammars_left[grammar_num-1];   //取出产生式的左部
                string grammar_right=Grammars_right[grammar_num-1];  //取出产生式的右部
                //弹出
                operation="reduce:"+LR_content+"("+grammar_left+"->"+grammar_right+")";
                if(is_print){
                    cout << setw(40) << stack_to_string(LRStack) << setw(25) << cur 
                    << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
                }

                outfile << setw(40) << stack_to_string(LRStack) << setw(25) << cur 
                << setw(25) << queue_to_string(input) << setw(30) << operation << endl;

                for(int i=grammar_right.size()-1; i>=0; i--){
                    stack_symbol=LRStack.top();   //栈顶字符
                    if(grammar_right[i] == stack_symbol[0]){
                        LRStack.pop();     
                    }
                    else{
                        cout<<"error"<<endl<<endl;
                        return;
                    }
                }
                stack_symbol=LRStack.top();
                row=string_to_int(stack_symbol);
                column=get_column(grammar_left);
                LRStack.push(LRTable[row][column]);
            }
            else if(LR_content == "OK"){
                operation=LR_content;

                if(is_print){
                    cout << setw(40) << stack_to_string(LRStack) << setw(25) << cur 
                    << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
                    cout<<"Right!"<<endl<<endl;
                }

                outfile << setw(40) << stack_to_string(LRStack) << setw(25) << cur 
                << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
                outfile <<"Right!"<<endl<<endl;

                return;
            }
            else{
                operation="push:"+LR_content;
                if(is_print){
                    cout << setw(40) << stack_to_string(LRStack) << setw(25) << cur 
                    << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
                }

                outfile << setw(40) << stack_to_string(LRStack) << setw(25) << cur 
                << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
                LRStack.push(LR_content);
                cur_symbol.clear();
            }
        }
    }

    void LRParse(Node *head){
        string expression="";
        queue<string> input;
        while(head != NULL){
            string word=head->content;
            int value=head->value;
            // cout<<word<<" "<<value<<endl;
            //如果不符合算术表达式，则不分析
            if(lexical_value.count(value) == 0){
                expression="";
                while(!input.empty()) input.pop();
            }
            else{
                string cur=token_convert(value);
                input.push(cur);

                if(cur == "#"){
                    if(input.size()>3) analysis(expression,input);
                    expression="";
                    while(!input.empty()) input.pop();
                }
                else{
                    expression += word;
                }
            }
            head=head->next;
        }
    }

    void print(){
        cout<<"   ";
        for(int i=0;i<symbol.size();i++){
            cout<<symbol[i]<<"   ";
        }
        cout<<endl;
        for(int i=0;i<LRTable.size();i++){
            cout<<i<<"  ";
            for(int j=0;j<LRTable[0].size();j++){
                if(LRTable[i][j] != "")
                    cout<<LRTable[i][j]<<"  ";
                else cout<<"    ";
            }
            cout<<endl;
        }
    }

};

void LR_Parse(){
    LR LR;
    // LR.print();
    Node *head=lexical_result();
    LR.LRParse(head);
    cout<<"LR analysis is complete,the result of LR is saved in LR_output.txt."<<endl;
    // getchar();
    // getchar();
}

#endif