#ifndef _DEFINE_HPP
#define _DEFINE_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;


// 将大等于0的数字转换为字符串, 负数返回/
string convert_type(int t){
    if(t >= 0){
        return to_string(t);
    }
    return "/";
}

#endif