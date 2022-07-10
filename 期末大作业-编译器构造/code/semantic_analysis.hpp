// 语法分析+语义分析
#ifndef _SEMANTIC_ANALYSIS_HPP
#define _SEMANTIC_ANALYSIS_HPP

#include "lexical_analysis.hpp"
#include "mincode.hpp"

class semantic_analysis{
protected:
    // ofstream outfile;
    vector<string> VN;  //非终结符
    vector<string> VT;    //终结符
    vector<vector<string>> productions;  //产生式
    vector<mincode> mincode_results;  //中间代码
    vector<vector<int>> LL1Table;  //LL1语法分析表

public:
    semantic_analysis(){
        //初始化
        VN={"P", "SP", "VD", "IT", "IT1", "TYPE", "CS", "ST", "AS", "E", "A",
                    "T", "B", "F", "CE", "CE1", "L", "L1", "IFS", "ELIFS", "ELSES", "WHS"};
        VT={"program", "var", ":", ";", "id", ",", "integer", "real", "char", "bool", "begin", "end", ":=", 
                "+", "-", "*", "/", "constant", "(", ")", "and", "or", ">", "<", "=", "true", "false", "if", "elif",
                "else", "while", "#"}; 
        productions={
            {"program","id","SP"},
            {"VD","CS"},{"e"},
            {"var","IT",":","TYPE",";","VD"},{"e"},
            {"id","IT1"},
            {",", "id", "IT1"},{"e"},
            {"integer"}, {"real"}, {"char"}, {"bool"},
            {"begin","ST","end"},
            {"AS",";","ST"},{"IFS","ST"},{"WHS","ST"},{"e"},
            {"id", "PUSH{id}", ":=", "E", "GEQ{:=}"},
            {"T","A"},
            {"+","T","GEQ{+}","A"},{"-","T","GEQ{-}","A"},{"e"},
            {"F","B"},
            {"*","F","GEQ{*}","B"},{"/","F","GEQ{/}","B"},{"e"},
            {"id", "PUSH{id}"},{"constant", "PUSH{constant}"},{"(","E",")"},
            {"L","CE1"},
            {"or","L","GEQ{or}","CE1"},{"and","L","GEQ{and}","CE1"},{"e"},
            {"E","L1"},{"true", "PUSH{true}"}, {"false", "PUSH{false}"},
            {">", "L", "GEQ{>}"},{"<", "L", "GEQ{<}"}, {"=", "L", "GEQ{=}"},{"e"},
            {"if", "(", "CE", ")", "IF{if}", "CS", "IFE{ife}","ELIFS", "ELSES"},{"e"},
            {"elif", "(", "CE", ")", "EIF{elif}", "CS", "ELIFE{elife}", "ELIFS"},{"e"},
            {"else", "ELSE{el}", "CS", "IE{ie}"},{"e"},
            {"while", "WH{wh}", "(", "CE", ")", "DO{do}", "CS", "WE{we}"},{"e"}

        };
        LL1Table={{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
                  {0,4,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
                  {0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,8,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
                  {0,0,0,0,0,0,9,10,11,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,14,0,0,0,0,0,0,17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,0,0,16,17},
                  {0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,19,0,0,0,0,0,0,0,0,0,0,0,0,19,19,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,22,0,0,0,0,0,0,0,0,0,20,21,0,0,0,0,22,22,22,22,22,22,0,0,0,0,0,0,22},
                  {0,0,0,0,23,0,0,0,0,0,0,0,0,0,0,0,0,23,23,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,26,0,0,0,0,0,0,0,0,0,26,26,24,25,0,0,26,26,26,26,26,26,0,0,0,0,0,0,26},
                  {0,0,0,0,27,0,0,0,0,0,0,0,0,0,0,0,0,28,29,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,30,0,0,0,0,0,0,0,0,0,0,0,0,30,30,0,0,0,0,0,0,30,30,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,32,31,0,0,0,0,0,0,0,0,0,33},
                  {0,0,0,0,34,0,0,0,0,0,0,0,0,0,0,0,0,34,34,0,0,0,0,0,0,35,36,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40,41,42,37,38,39,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,41,0,0,0,0},
                  {0,0,0,0,44,0,0,0,0,0,0,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,43,44,44,44},
                  {0,0,0,0,46,0,0,0,0,0,0,46,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,46,0,45,46,46},
                  {0,0,0,0,0,0,0,0,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,47,0}
        };

    }

    ~semantic_analysis(){};

    // 从语义动作中获得符号，如PUSH{id}获得id,GEQ{+}中获得{+}
    string get_symbol(string str){
        string symbol="";
        for(int i=str.find("{")+1;i<str.size();i++){
            if(str[i] == '}') break;
            symbol += str[i];
            // cout<<symbol<<endl;
        }
        return symbol;
    }

    //得到LL1语法分析表列号
    int get_column(string str){
        for(int i=0;i<VT.size();i++){
            if(VT[i] == str)
                return i;
        }
        return -1;
    }

    //得到LL1语法分分析表行号
    int get_row(string str){
        for(int i=0;i<VN.size();i++){
            if(VN[i] == str)
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
    string stack_to_string(stack<string> stack,int type){
        vector<string> temp;
        string str="";
        // cout<<"stack"<<endl;
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
    
    //语法分析+语义分析
    void analysis(queue<string> code,queue<string> input){
        ofstream outfile;  //输出文件
        outfile.open("result/semantic_output.txt");
        code.push("#");
        input.push("#");
        int t_i=1; //t的下标

        stack<string> SYN; //LL1语法分析栈
        stack<string> SEM; //语义分析栈
        // vector<mincode> quaters; //中间代码 

        string cur_symbol="";   //当前字符
        mincode cur_quater;     //当前中间代码
        string operation="Start";    //操作

        SYN.push("#");
        SYN.push("P");  //P是开始符号

        outfile<<"*********************************************************** Output of LL1 Semantic Analyzer:***********************************************************"<<endl;
        outfile << setiosflags(ios::right);
        outfile << setw(70) << "SYN analysis stack" << setw(20) << "Current character"         
         << setw(200) << "Remaining sequence" << setw(55) << "Operation" << setw(31)
         << "SEM analysis stack" << setw(20) << "Quaternion" << endl;
        outfile << setw(70) << stack_to_string(SYN, 0) << setw(20) << cur_symbol 
         << setw(200) << queue_to_string(input) << setw(55) << operation << setw(31)
         << stack_to_string(SEM, 0) << setw(20) << cur_quater.get_mincode() << endl;

        while(!SYN.empty()){
            cur_quater.clear();  //清空中间代码
            // cout<<cur_quater.expression2<<endl;
            string syn_str=SYN.top();  //取出语法分析栈栈顶字符串
            // 取出当前字符
            if(cur_symbol.empty()){
                if(input.empty()){
                    outfile << "error: input is invalid!" << endl <<endl;
                    return;
                }
                cur_symbol=input.front();
                input.pop();
            }
           
            string syn_content=stack_to_string(SYN,0);  //语法分析栈当前内容
            string cur_str=cur_symbol;  //保存当前字符
            SYN.pop();
            
            // 当前栈顶字符匹配上当前读入的字符
            if(syn_str == cur_symbol){
                operation="match:"+syn_str+"="+cur_symbol;
                cur_symbol.clear();
                code.pop();
            }
            // 如果栈顶字符含PUSH，那么语义栈push
            else if(syn_str.find("PUSH") != syn_str.npos){
                string push_symbol=get_symbol(syn_str);
                SEM.push(push_symbol);
                operation="push to SEM:"+push_symbol;
            }
            //如果栈顶字符含GEQ，那么生成中间代码四元式
            else if(syn_str.find("GEQ") != syn_str.npos){
                string op=get_symbol(syn_str);  //运算符

                //当op为赋值符号时
                if(op == ":="){
                    string exp1=SEM.top();
                    SEM.pop();
                    string exp2="_";
                    string result=SEM.top();
                    SEM.pop();
                    cur_quater=mincode(op,exp1,exp2,result);
                }
                else{
                    string exp2=SEM.top();
                    SEM.pop();
                    string exp1=SEM.top();
                    SEM.pop();
                    string result="t"+to_string(t_i);  //生成结果ti
                    // cout<<exp1<<" "<<exp2<<endl;
                    SEM.push(result);
                    cur_quater=mincode(op,exp1,exp2,result);
                    // cout<<cur_quater.expression1<<" "<<cur_quater.expression2<<endl;
                    // cout<<stack_to_string(SYN,0)<<endl;
                    t_i++;
                }

                mincode_results.push_back(cur_quater);
                operation="generate quaternion";
            }
            else if(syn_str.size() > 4 && syn_str.substr(0, 3) != "IFE" && (syn_str.substr(0, 2) == "IF" 
            || syn_str.substr(0, 2) == "DO" || syn_str.substr(0, 3) == "EIF")){
                string op=get_symbol(syn_str);  //运算符
                string exp1=SEM.top();
                SEM.pop();
                cur_quater = mincode(op, exp1, "_", "_");
                mincode_results.push_back(cur_quater);
                operation="if/elif/do generate quaternion";
            }
            else if(syn_str.size() > 5 && syn_str.substr(0, 5) != "ELSES" && (syn_str.substr(0, 2) == "IE" 
            || syn_str.substr(0, 2) == "WH" || syn_str.substr(0, 2) == "WE" || syn_str.substr(0, 5) == "ELIFE" 
            || syn_str.substr(0, 4) == "ELSE" || syn_str.substr(0, 3) == "IFE")){
                string op=get_symbol(syn_str);  //运算符
                cur_quater = mincode(op,"_", "_", "_");
                mincode_results.push_back(cur_quater);
                operation = "ife/elife/el/ie/wh/we generate quaternion";
    
            }
            else{
                int row=get_row(syn_str);  //LL1分析表的行号
                int column=get_column(cur_symbol); //LL1分析表的列号
                // cout<<row<<" "<<syn_str<<" "<<column<<" "<<cur_symbol<<endl;
                int table_content=LL1Table[row][column];
                if(table_content == 0){
                    outfile<<"error: search table failed!"<<endl<<endl;
                    return;
                }
                
                vector<string> new_symbols=productions[table_content-1];
                string production="";
                for(string str:new_symbols)
                    production += str;
                operation="select "+syn_str+" -> "+production;
                
                // 将选择的产生式反向入栈
                for(int i = new_symbols.size()-1; i >= 0; i--){     
                    // cout<<new_symbols[i]<<endl;
                    if(new_symbols[i].find("PUSH") != new_symbols[i].npos){
                        // 得到输入串中的标识符或常数
                        SYN.push("PUSH{" + code.front() + "}");

                    } 
                    else if(new_symbols[i] != "e"){
                        // 非空串
                        SYN.push(new_symbols[i]);
                        
                    }   // 空串, 不进行处理
                }
                // cout<<stack_to_string(SYN,0)<<endl;
            }
            
            // 打印分析栈
            outfile << setw(70) << syn_content << setw(20) << cur_str
                << setw(200) << queue_to_string(input) << setw(55) << operation << setw(31)
                << stack_to_string(SEM, 1) << setw(20) << cur_quater.get_mincode() << endl;
        }
        // cout<<"aa"<<endl;
        // 输出中间代码
        ofstream out;
        out.open("result/mincod.txt");
        for(int i = 0; i < mincode_results.size(); i++){
            out << "(" << i+1 << ") " << mincode_results[i].op << " " 
                << mincode_results[i].expression1 << " " << mincode_results[i].expression2
                << " " << mincode_results[i].result << endl;
        }
        // cout<<"语法分析+语义分析完成,中间代码生成!"<<endl;

    }

    //接收词法扫描器的token序列
    void LL1Semantic(vector<pair<string, int>> tokens){
        queue<string> code;
        queue<string> input;

        for(int i = 0; i < tokens.size(); i++){
            if(tokens[i].first == "'"){
                continue;
            }
            string word = tokens[i].first;
            int order = tokens[i].second;
            
            if(word[0] == '\''){
                continue;
            } else if(order == 0){
                // 将标识符转换为id表示
                input.push("id");
            } else if(order == 1){
                // 将常数转换为cons表示
                input.push("constant");
            } else input.push(word);

            code.push(word);
        }
        analysis(code,input);

    }

    // 返回中间代码
    vector<mincode> get_mincode(){
        return mincode_results;
    }

};

#endif
