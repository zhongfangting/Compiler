// 词法分析器,将源程序转为Token序列
#ifndef _LEXICAL_ANALYSIS_HPP
#define _LEXICAL_ANALYSIS_HPP
#include "define.hpp"
#include "symbol_table.hpp"

// 检查vector中是否存在某个元素
bool vector_find(vector<string> v, string obj){
    for(auto item: v){
        if(item == obj){
            return true;
        }
    }
    return false;
}

class lexical_analysis{
protected:
    int line;                       //行标记
    int char_flag;                  //char类型指标
    int param_num;                //参数个数
    int program_flag;              // 参数入口指标
    string program_name;
    int var_flag;                   // 声明指标
    vector<string> vars;           //存储声明的变量
    SymbolSystem symbol_system;   //符号表系统
    vector<string> keywords;     // 关键字字典
    vector<string> separators;   // 分隔符字典
    vector<string> operators;     // 运算符字典
    vector< pair<string, int> > tokens;  // 输出结果: Token序列
public:
    //初始化
    lexical_analysis(){
        // 初始化关键字字典，分隔符字典，运算符字典
        keywords = {"program","var","integer","real","char","bool"
                  "true","false","if","elif","else", "while",
                  "begin","end","and","or","not"};
        separators = {":",";",",","'","(",")","[","]","{","}"};
        operators = {"+","-","*","/",":=",">","<","="};

        line = 1;
        char_flag = 0;
        param_num = 0;
        program_flag = 0;
        program_name = "";   
        var_flag = 0;
    };

    ~lexical_analysis(){ };
    
    //查找关键字
    int getKeyword(string keyword){
        for(int i=0; i<keywords.size(); i++){
            if(keywords[i] == keyword)
                return i+4;
        }
        return 0;
    }

    //查找分隔符
    int getSeparator(string separator){
        for(int i=0; i<separators.size(); i++){
            if(separators[i] == separator)
                return i+21;
        }
        return 0;
    }

    //查找运算符
    int getOperator(string op){
        for(int i=0; i<operators.size(); i++){
            if(operators[i] == op)
                return i+31;
        }
        return 0;
    }

    // 检查是否为字母
    bool is_letter(char ch){
        return ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    // 检查是否为数字
    bool is_digital(char ch){
        return (ch >= '0' && ch <= '9');
    }

    // 检查是否是常数
    bool is_constant(string str){
        for(auto ch:str){
            if(is_digital(ch) || ch == '.' || ch == '_'){
                continue;
            } else {
                return str == "true" || str == "false";
            }
        }
        return true;
    }

    // 检查是否是实数
    bool is_real(string str){
        for(auto ch:str){
            if(ch == '.'){
                return true;
            }
        }
        return false; 
    }

    // 基于状态转换函数
    void change_state(int& state,char ch){
        switch(state){
            // 状态0
            case 0:
                if(ch == '\''){
                    if(char_flag==0) char_flag=1;
                    else char_flag=0;
                    state=5;
                }
                else if(char_flag || is_letter(ch)) state = 1;
                else if(ch == ' ') state = 0;
                else if(is_digital(ch)) state = 2;
                else if(getSeparator(string(1, ch))) state = 5;
                else if (getOperator(string(1, ch))) state = 6;
                else cout<<"state chang error! current state - current char:"<<to_string(state)<<"-"<<string(1, ch)<<endl;  // 非法状态,报错
                break;
            // 状态1
            // 标识符或关键字
            case 1:
                if(is_letter((ch)) || is_digital(ch)) state = 1;
                else state = 0;
                break;
            // 状态2
            // 整数或实数
            case 2:
                if(is_digital(ch)) state = 2;
                else if (ch == '.') state = 3;
                else if(is_letter(ch))
                    cout<<"unable to find numeric literal operator"<<endl;      // 非法状态,报错
                else state = 0;
                break;
            // 状态3
            // 小数点
            case 3:
                if(is_digital(ch)) state = 4;
                else if(ch == '.' || is_letter(ch))
                    cout<<"unable to find numeric literal operator"<<endl;      // 非法状态,报错
                else state = 0;
                break;
            // 状态4
            // 实数
            case 4:
                if(is_digital(ch)) state = 4;
                else if(ch == '.' || is_letter(ch))
                    cout<<"unable to find numeric literal operator"<<endl;      // 非法状态,报错
                else state = 0;
            // 分割符
            case 5:
                if(ch == '=') state = 6;
                else state = 0;
                break;
            // 运算符
            case 6:
                if(getOperator(string(1, ch))) state = 4;
                else state = 0;
                break;
            // 不属于上述任何一个状态,说明非法状态,报错
            default:
                cout<<"there is a wrong state"<<endl;
        }
    }

    // 根据上一个状态,将当前的word转换为Token,并记录表中
    void state_to_token(int pre_state, string word){
        if(pre_state == 0) return;
        else if(pre_state == 1){
            if(char_flag){
                tokens.emplace_back(make_pair(word, 1));
                // 常量不能重复
                if(!vector_find(symbol_system.CONSL_table, word)){
                    // 字符类型的常量, 转换为ASCII码, 存入常量表
                    symbol_system.CONSL_table.emplace_back(to_string(word[0]));
                    symbol_system.createSYNBL(word, -1, 'c', 0);
                }
            }
            //标识符或关键字
            else {
                int token = getKeyword(word);
                // cout<<"aa"<<endl;
                // 关键字
                if(token){
                    tokens.emplace_back(make_pair(word, token));
                    if(word == "program"){
                        program_flag = 1;
                    } 
                    else if(word == "var"){
                        // 准备声明变量
                        var_flag = 1;
                        vars.clear();
                    } 
                    // 当word == begin，创建过程表
                    else if(word == "begin" && program_name.size() != 0){
                        symbol_system.createPFINFL(program_name, param_num, PROGRAM_ADRESS);
                        program_name.clear();
                    } 
                    // 当word == interger or real or char or bool，创建总表
                    else if((word == "integer" || word == "real" || word == "char" || word == "bool") && var_flag == 1){
                        var_flag = 0;
                        for(auto name:vars){
                            symbol_system.createSYNBL(name, word[0], 'v', 1);
                        }
                    }
                } 
                // 标识符
                else {
                    tokens.emplace_back(make_pair(word, 0));
                    if(program_flag == 1){
                        program_name = word;  // 记录过程名字
                        program_flag = 0;    // 记录过程
                        symbol_system.createSYNBL(program_name, -1, 'p', 1);
                    } 
                    // 记录参数个数
                    else if(program_name.size() != 0) param_num++;
                    // 记录声明的变量
                    if(var_flag == 1) vars.emplace_back(word);
                }
            }
    
        }
        else if(pre_state == 2 || pre_state == 3 || pre_state == 4){
            // 数字类型的常量, 存入常量表
            tokens.emplace_back(make_pair(word, 1));
            // 常量不能重复
            if(!vector_find(symbol_system.CONSL_table, word)){
                symbol_system.CONSL_table.emplace_back(word);
                symbol_system.createSYNBL(word, -1, 'c', 0);
            }
        } 
        else if(pre_state == 5){
            // 分隔符
            int token = getSeparator(word);
            if(token) tokens.emplace_back(make_pair(word, token)); 
            else cout<<"stray in program"<<endl;      // error
        } 
        else if(pre_state == 6){
            // 运算符
            int token = getOperator(word);
            if(token) tokens.emplace_back(make_pair(word, token));
            else cout<<"stray in program"<<endl;     // error
        } 
        else cout<<"error in state_to_token"<<endl;        // error
    }
    //词法分析
    void analyse(const char* file_name){
        ifstream infile;
        infile.open(file_name);

        if(infile.fail()){
            cout<<"can't open file"<<endl;
        }

        string code;
        // 初始状态(当前状态)
        int cur_state = 0;
        // 上一个状态
        int pre_state = 0;
        string word;
        while(getline(infile, code)){
            for(char ch:code){
                pre_state=cur_state;
                change_state(cur_state, ch);    // 转换到下一个状态
                // 非0状态, 记录word
                if(cur_state != 0) word += ch; 
                // 0状态,将word转化为Token
                else {                    
                    state_to_token(pre_state, word);
                    word.clear();
                    if(ch != ' '){
                        change_state(cur_state, ch);
                        word += ch;
                    }
                }
            }
            state_to_token(cur_state, word);     // 该行的最后一个Token
            word.clear();
            cur_state = 0;
            // 单引号不匹配,报错
            if(char_flag) cout << "missing terminating ' character" << endl;
            line++;
        }

        // 打印词法分析器的输出
        ofstream outfile;
        outfile.open("result/lexical_tokens.txt");
        // outfile << "**********************************" << "Output of Lexical Analyzer:" << "**********************************" << endl << endl;

        outfile << "**********************************" << "       Tokens Results:     " << "**********************************" << endl;
        for(auto item:tokens){
            outfile << "< " << item.first << " , " << item.second << " >" << endl; 
        }
        outfile << endl;
        // outfile << "**********************************" << " End of Lexical analyzer: " << "**********************************" << endl;

        // // 控制台输出
        // cout << "**********************************" << "Output of Lexical Analyzer:" << "**********************************" << endl;
        // for(auto item:tokens){
        //     cout << "< " << item.first << " , " << item.second << " >" << endl; 
        // }
        // cout << endl;
        
        // 打印符号表
        symbol_system.printSystem();
        // cout<<"词法分析完成!"<<endl;
        // 关闭文件
        infile.close();
        outfile.close();
    }

    //返回token序列
    vector< pair<string, int> > getTokens(){
        return tokens;
    }
    // 返回符号表系统
    SymbolSystem getSymbolSystem(){
        return symbol_system;
    }
};


#endif
