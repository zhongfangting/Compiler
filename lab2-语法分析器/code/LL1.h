#ifndef _LL1_H_
#define _LL1_H_


#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include<fstream>
#include "lexical_analysis.hpp"
using namespace std;

#define is_print 0


template<class T>
    void drawCeil(int width, T container)
    {
        for(auto item: container)
        {
            cout<<item;
        }
        for(int i = container.size(); i < width; i++)
        {
            printf(" ");
        }
        printf("|");
    }

//LL1分析法
class LL1{
protected:
    set<string> VN;    //非终结符
    set<string> VT;    //终结符
    set<int> lexical_value;  //表达式字符的内码值
    map<string,vector<string>> Grammars;  //文法
    ofstream outfile;  //输出文件
    map<string,map<string,string>> LL1Table;  //LL1分析表
public:
    //构造LL1分析表
    LL1(){
        outfile.open("LL1_output.txt");

        VN = {"S", "E", "A", "T", "B", "F"};
        VT = {"i","p","x","y","(",")","#"};
        lexical_value = {33,34,35,36,45,48,49,50,51,52,70};
        vector<string> temp;
        temp = {"p=E"};
        Grammars["S"]=temp;
        temp = {"TA"};
        Grammars["E"]=temp;
        temp = {"xTA","e"};
        Grammars["A"]=temp;
        temp = {"FB"};
        Grammars["T"]=temp;
        temp = {"yFB","e"};
        Grammars["B"]=temp;
        temp = {"i","p","(E)"};
        Grammars["F"]=temp;

        LL1Table["S"]["p"]="p=E";
        LL1Table["E"]["i"]="TA";
        LL1Table["E"]["p"]="TA";
        LL1Table["E"]["("]="TA";
        LL1Table["A"]["x"]="xTA";
        LL1Table["A"][")"]="e";
        LL1Table["A"]["#"]="e";
        LL1Table["T"]["i"]="FB";
        LL1Table["T"]["p"]="FB";
        LL1Table["T"]["("]="FB";
        LL1Table["B"]["x"]="e";
        LL1Table["B"][")"]="e";
        LL1Table["B"]["#"]="e";
        LL1Table["B"]["y"]="yFB";
        LL1Table["F"]["i"]="i";
        LL1Table["F"]["p"]="p";
        LL1Table["F"]["("]="(E)";
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
        }

        return str;
    }

    //LL1语法分析
    void analysis(string expression,queue<string> input){
        if(is_print){
            cout<<"*************************** Output of LL1 synatactic analyzer:***************************"<<endl;
            cout<<"expression:"<<expression<<"——————>"<<queue_to_string(input)<<endl;
        }

        outfile<<"*************************** Output of LL1 synatactic analyzer:***************************"<<endl;
        outfile<<"expression:"<<expression<<"——————>"<<queue_to_string(input)<<endl;

        stack<string> LL1Stack;  //LL1分析栈
        string cur_symbol="";   //当前字符
        string operation="Start";    //操作

        LL1Stack.push("#");
        LL1Stack.push("S");   //S是开始符号
        if(is_print){
            cout << setiosflags(ios::left);
            cout << setw(25) << "Analysis stack" << setw(25) << "Current character"         
                << setw(25) << "Remaining sequence" << setw(30) << "Operation" << endl;
            cout << setw(25) << stack_to_string(LL1Stack) << setw(25) << cur_symbol 
                << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
        }
        outfile << setiosflags(ios::left);
        outfile << setw(25) << "Analysis stack" << setw(25) << "Current symbol"         
             << setw(25) << "Remaining seq" << setw(30) << "Operation" << endl;
        outfile << setw(25) << stack_to_string(LL1Stack) << setw(25) << cur_symbol 
             << setw(25) << queue_to_string(input) << setw(30) << operation << endl;

        while(!LL1Stack.empty()){
            string stack_symbol=LL1Stack.top();  //取出栈的字符
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

            string stack_cur=stack_to_string(LL1Stack);   //当前栈的内容
            string cur=cur_symbol; 
            LL1Stack.pop();

            //当前栈顶字符匹配上当前读入的字符
            if(stack_symbol == cur_symbol){
                operation="match:"+stack_symbol+"="+cur_symbol;
                cur_symbol.clear();
            }
            else if(VN.count(stack_symbol) == 1){
                if(LL1Table.count(stack_symbol) == 0 || LL1Table[stack_symbol].count(cur_symbol) == 0)
                {
                    if(is_print){
                        cout<<"error: search table failed!"<<endl<<endl;
                    }
                    outfile<<"error: search table failed!"<<endl<<endl;
                    return;
                }

                string grammer=LL1Table[stack_symbol][cur_symbol];  //得出匹配的产生式
                operation="select:"+stack_symbol+"->"+grammer;
                if(grammer != "e"){
                    for(int i=grammer.size()-1; i>=0; i--){
                        string str(1,grammer[i]);
                        LL1Stack.push(str);
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
            if(is_print){
                cout << setw(25) << stack_cur << setw(25) << cur 
                     << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
            }
            outfile << setw(25) << stack_cur << setw(25) << cur 
                 << setw(25) << queue_to_string(input) << setw(30) << operation << endl;
        }
        if(is_print){
            cout << "Right!" << endl << endl; 
        }
        outfile << "Right!" << endl << endl; 
    }

    void LL1Parse(Node *head){
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

    void p(){
        printf("--------------------------------------------PARSE TABLE---------------------------------------------\n");
        printf("|");
        drawCeil(8, string(""));
        for(auto key: VT)
        {
            drawCeil(8, "   " + key);
        }
        printf("\n|");
        for(int i = 0; i < VT.size() + 1; i++)
        {
            drawCeil(8, string("--------"));
        }
        printf("\n");
        for(auto line: LL1Table)
        {
            string str;
            printf("|");
            string key1 = line.first;
            drawCeil(8, "   " + line.first);
            for(auto key2: VT)
            {
                str = "";
                if(LL1Table[key1].count(key2) != 0)
                    str = key1 + "->" + LL1Table[key1][key2];
                drawCeil(8, str);
            }
            printf("\n");
            
        }
    }
};

void LL1_Parse(){
    // print();
    LL1 LL1;
    // LL1.p();
    Node *head=lexical_result();
    // printResult(head);
    LL1.LL1Parse(head);
    cout<<"LL1 analysis is complete,the result of LL1 is saved in LL1_output.txt."<<endl;
    // getchar();
    // getchar();
    // return 0;
}

#endif


