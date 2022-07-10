#include "define.hpp"
#include "lexical_analysis.hpp"
#include "semantic_analysis.hpp"
int main(){
    string file;
    // cout<<"please input the test file,such as test/text1.txt:";
    // cin>>file;
    lexical_analysis lexical;  //词法分析
    semantic_analysis semantic;  //语法分析+语义分析+中间代码生成
    lexical.analyse("test/test2");
    semantic.LL1Semantic(lexical.getTokens());
    cout<<"the result save in result"<<endl;
    getchar();
    getchar();
    return 0;
}