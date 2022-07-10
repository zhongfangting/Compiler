// 构建符号表
#ifndef _SYMBOL_TABLE_HPP
#define _SYMBOL_TABLE_HPP

#include "define.hpp"

// 程序入口地址
#define PROGRAM_ADRESS 0x00001000 

// 总表设计
struct SYNBL{
    string name;  //标识符名字
    int type;     //指针，指向类型表TYPE表 
    char cat;     // 种类,指向不同表， p-程序， c-常量， t-类型， v-程序变量
    int addr;     //指针，指向不同表，p指向PFINFL, c指向CONSL， t指向RINFL，v指向LENL
    int active;  //活跃信息，1为活跃，0为不活跃
    SYNBL(){ };
    SYNBL(string name1, int type1, char cat1, int addr1, int active1){
        name=name1;
        type=type1;
        cat=cat1;
        addr=addr1;
        active=active1;
    };
};

// 类型表
struct TYPEL{
public:
    char tavl;    // 类型代码, i整型, r实型, c字符型, b布尔型, d结构型
    TYPEL(char tval1){ 
        tavl=tval1;
    };
};

// 结构表
struct RINFL{
public:
    string id;   // 域名
    int off;     // 第i个id首地址相对结构头的距离
    int tp;      // 域成分类型指针
    RINFL(string id1, int off1, int tp1){
        id=id1;
        off=off1;
        tp=tp1;
    };
};

// 过程表
struct PFINFL{
    int num;      // 参数个数
    int param;   // 指针，指向形参表
    int address;   // 目标代码运行时，程序首地址
    PFINFL(int num1, int param1, int address1){ 
        num=num1;
        param=param1;
        address=address1;
    };
};

// 符号表系统
class SymbolSystem{
private:
    vector<int> LENL_table;         // 长度表, 存放相应数据类型所占字节长度
    vector<TYPEL> TYPEL_table;      // 类型表, 存放变量的类型
    vector<RINFL> RINFL_table;     // 结构表, 存放结构的内容
    vector<PFINFL> PFINFL_table;   // 过程表, 存放过程(程序)的信息
public:
    vector<SYNBL> SYNBL_table;   // 符号表总表
    vector<string> CONSL_table;  // 常数表, 存放常量的初值

    SymbolSystem(){ }
    ~SymbolSystem(){ 
        // 清空符号表
        CONSL_table.clear();
        LENL_table.clear();
        SYNBL_table.clear();
        TYPEL_table.clear();
        RINFL_table.clear();
        PFINFL_table.clear();
    }
    
    // 创建一个过程表项目, num为该过程的参数个数
    void createPFINFL(string program, int num, int program_adress){
        //过程在符号表总表的下标
        int program_index;     
        for(int i=0; i<SYNBL_table.size(); i++){
            if(program == SYNBL_table[i].name)
                program_index=i;
        }

        // 地址指向过程表的下一个位置
        SYNBL_table[program_index].addr = PFINFL_table.size();

        // 参数指针指向结构表的下一个位置(形参表)
        int param = RINFL_table.size();

        // 构建形参表
        int off = 0;
        for(int i = 1; i <= num; i++){
            // 参数在符号表总表的下标
            int var_idx = program_index + i;
            string id = SYNBL_table[var_idx].name;
            int tp = SYNBL_table[var_idx].type;
            // cout<<"off"<<off<<endl;
            RINFL new_RINFL_item = RINFL(id, off, tp);
            // cout<<new_RINFL_item.off<<" "<<off<<endl;
            RINFL_table.push_back(new_RINFL_item);
            // // 增加偏移
            int len_idx = SYNBL_table[var_idx].addr;
            off += LENL_table[len_idx];
        }

        // 构建过程表项目并压入过程表
        PFINFL new_PFINFL_item = PFINFL(num, param, program_adress);
        PFINFL_table.push_back(new_PFINFL_item);
    }

    // 返回类型变量的长度
    int getTypeSize(char tval){
        if(tval == 'i'){
            return 4;
        } else if(tval == 'r'){
            return 8;
        } else if(tval == 'c'){
            return 1;
        } else if(tval == 'b'){
            return 1;
        }
        return 0;
    }

    // 创建一个总表项目
    void createSYNBL(string name, char tval, char cat, int active){
        SYNBL new_SYNBL_item;
        if(cat == 'p'){
            // 程序, 指向PFINFL, 由词法分析器进一步构造
            new_SYNBL_item = SYNBL(name, -1, cat, -1, active);
        } else if(cat == 'c'){
            // 常量, 指向CONSL
            int addr = CONSL_table.size()-1;
            new_SYNBL_item = SYNBL(name, -1, cat, addr, active);
        } else if(cat == 't'){
            // 类型, 指向RINFL
            int addr = RINFL_table.size();
            new_SYNBL_item = SYNBL(name, -1, cat, addr, active);
        } else if(cat == 'v'){
            // 变量，指向LENL
            int addr = LENL_table.size();
            LENL_table.emplace_back(getTypeSize(tval));
            // 创建类型表新项目
            int typ = TYPEL_table.size();
            TYPEL new_TYPEL_item = TYPEL(tval);
            TYPEL_table.emplace_back(new_TYPEL_item);
            // 创建总表项目
            new_SYNBL_item = SYNBL(name, typ, cat, addr, active);
        }
        SYNBL_table.emplace_back(new_SYNBL_item);
    }

    // 获得参数变量的内存偏移地址
    int getVarAddr(string name){
        for(int i = 0; i < RINFL_table.size(); i++){
            if(RINFL_table[i].id == name){
                return RINFL_table[i].off;
            }
        }
        return -1;
    }

    // 获得总表中的ADDR指向地址
    string getAddr(int idx){
        char cat = SYNBL_table[idx].cat;
        string addr = to_string(SYNBL_table[idx].addr);
        if(cat == 'p'){
            return "PFINFL[" + addr + "]";
        } else if(cat == 'c'){
            return "CONSL[" + addr + "]";
        } else if(cat == 't'){
            return "RINFL[" + addr + "]";
        } else if(cat == 'v'){
            return "LENL[" + addr + "]";
        }
        return "";
    }
    
    // 打印符号表
    void printSystem(){
        // 文件输出
        ofstream outfile;
        outfile.open("result/symbol_table.txt");
        string str(27, '*');
        // outfile << str << "  Output of Symbol System: " << str << endl << endl;
        outfile << setiosflags(ios::right);
        string str1="";
        for(int i=0;i<18;i++){
            str1 += "~·";
        }
        str=str1;
        outfile << str << "SYNBL(总表)" << str << endl;
        outfile << setw(6) << "idx" << setw(20) << "name" << setw(10) << "type"
                << setw(10) << "cat" << setw(17) << "addr" << setw(14) << "active" << endl;
        for(int i = 0; i < SYNBL_table.size(); i++){
            outfile << setw(6) << "[" + to_string(i) + "]" << setw(20) << SYNBL_table[i].name << setw(10)
                << convert_type(SYNBL_table[i].type) << setw(10) << SYNBL_table[i].cat << setw(17) << getAddr(i) 
                << setw(14) << SYNBL_table[i].active << endl;
        }
        outfile << endl;
        outfile << str << "TYPEL(类型表)" << str << endl;
        outfile << setw(6) << "idx" << setw(20) << "tval" << endl;
        for(int i = 0; i < TYPEL_table.size(); i++){
            outfile << setw(6) << "[" + to_string(i) + "]" << setw(20) << TYPEL_table[i].tavl << endl;
        }
        outfile << endl;
        outfile << str << "CONSL(常数表)" << str << endl;
        outfile << setw(6) << "idx" << setw(20) << "const" << endl;
        for(int i = 0; i < CONSL_table.size(); i++){
            outfile << setw(6) << "[" + to_string(i) + "]" << setw(20) << CONSL_table[i] << endl;
        }
        outfile << endl;
        outfile << str << "LENL(长度表)" << str << endl;
        outfile << setw(6) << "idx" << setw(20) << "len" << endl;
        for(int i = 0; i < LENL_table.size(); i++){
            outfile << setw(6) << "[" + to_string(i) + "]" << setw(20) << LENL_table[i] << endl;
        }
        outfile << endl;
        outfile << str << "RINFL(结构表)" << str << endl;
        outfile << setw(6) << "idx" << setw(20) << "id" << setw(10) << "off" << setw(10) << "tp" << endl;
        for(int i = 0; i < RINFL_table.size(); i++){
            outfile << setw(6) << "[" + to_string(i) + "]" << setw(20) << RINFL_table[i].id << setw(10) << RINFL_table[i].off
            << setw(10) << RINFL_table[i].tp << endl;
        }
        outfile << endl;
        outfile << str << "PFINFL(过程表)" << str << endl;
        outfile << setw(6) << "idx" << setw(20) << "num" << setw(20) << "param" << setw(10) << "address" << endl;
        for(int i = 0; i < PFINFL_table.size(); i++){
            outfile << setw(6) << "[" + to_string(i) + "]" << setw(20) << PFINFL_table[i].num << setw(20) << "RINFL[" + to_string(PFINFL_table[i].param) + "]"
            << setw(10) << PFINFL_table[i].address << endl;
        }
        outfile << endl;   
        // outfile << str << "  End of Symbol System: " << str << endl;
    }
};

#endif