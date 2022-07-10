// 中间代码
#ifndef _MINCODE_HPP
#define _MINCODE_HPP

#include "define.hpp"

//中间代码，四元式的形式
struct mincode{
    string op;  
    string expression1;
    string expression2;
    string result;
    //活跃信息
    int active1;
    int active2;
    int active3;

    mincode(string op1="",string exp1="",string exp2="",string res=""){
        op=op1;
        expression1=exp1;
        expression2=exp2;
        result=res;
    }

    // 判断中间代码是否为空
    bool is_emmpty(){
        if(op=="") return true;
        else return false;
    }

    // 返回中间代码
    string get_mincode(){
        string mincode=op+" "+expression1+" "+expression2+" "+result;
        return mincode;
    }

    // 返回活跃信息
    string is_active(string exp, bool active){
        return exp == "_" ? ("") : (active ? "(y)" : "(n)");
    }

    //清除中间代码
    void clear(){
        op="";
        expression1="";
        expression2="";
        result="";
    }

};



#endif