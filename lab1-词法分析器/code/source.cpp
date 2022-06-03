#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;

#define MAX 50

#define KEY "关键字"
#define IDENTIFIER "标识符"
#define IDENTIFIER_NUM 33
#define CONSTANT "常数"
#define CONSTANT_NUM 34
#define OPERATOR "运算符"
#define DELIMITER "界符"
#define HEADER "头文件"
#define HEADER_NUM 86
#define MACRO "宏定义"
#define MACRO_NUM 85
#define CONSTANT_STRING "字符串"
#define CONSTANT_STRING_NUM 84
#define CONSTANT_CHARACTER "字符"
#define CONSTANT_CHARACTER_NUM 83
#define NOTE "注释"
#define NOTE_NUM 140

#define is_print 0

//c语言的关键字 1~32
const char* key[]={"auto","break","case","char","const","continue","default","do","double",
                      "else","enum","extern","float","for","goto","if","int","long","register",
                      "return","short","signed","sizeof","static","struct","switch","typedef",
                      "union","unsigned","void","volatile","while"};
//标识符 33
//常数 34
//界符 35~45
const char *delimiters[]={"(",")","[","]","{","}","\'","\"","#",",",";","//","/**/"};
const char *operators[]={"+","-","*","/","%","++","--",">","<","==",">=","<=","!=","&",
                         "|","~","^","<<",">>","&&","||","!","=","+=","-=","*=","/=","%=",
                         "&=","|=","^=","<<=",">>=","?=","->"};
//注释
//运算符 

//节点
struct Node{
    char content[MAX];
    char describe[MAX];
    int value; //种别码
    int addr; //内码值
    int line; //行号
    Node *next; //下一个结点
    Node(char *t_content="", char *t_describe="", int t_value=-1, int t_addr=-1, int t_line=-1){
        strcpy(content,t_content);
        strcpy(describe,t_describe);
        value=t_value;
        addr=t_addr;
        line=t_line;
        next=NULL;
    }
};

Node *head;
Node *Idenhead;  //记录标识符 
int iden_number=0; //模拟标识符的内码值

// //初始化结点
// void initNode(){
//     head=new Node();
//     strcpy(head->content,"");
//     strcpy(head->describe,"");
//     head->value=-1;
//     head->addr=-1;
//     head->line=-1;
//     head->next=NULL;
// }

//将新结点加入链表中
void addNewNode(char *content, char *describe, int value, int addr, int line){
    Node *node=head;
    Node *temp=new Node(content,describe,value,addr,line); //新结点

    // strcpy(temp->content,content);
    // strcpy(temp->describe,describe);
    // temp->value=value;
    // temp->addr=addr;
    // temp->line=line;
    // temp->next=NULL;

    while(node->next != NULL) node=node->next;
    
    node->next=temp;
}

//增加标识符结点
//返回值是新的标识符的内码值
int addNewIdenNode(char *content, char *describe, int value, int addr, int line){
    Node *node=Idenhead;
    int flag=0;
    int addr_temp;
    while(node->next != NULL){
        if(strcmp(content,node->next->content) == 0){
            flag = 1;
            addr_temp = node->next->addr;
        }
        node=node->next;
    }
    //用自增来模拟入口地址
    if(!flag) addr_temp = (++iden_number);
    Node *temp=new Node(content,describe,value,addr_temp,line); //新结点
    node->next=temp;
    return addr_temp;
}

//预处理，处理头文件和宏定义
void preProcess(char *str,int line){
    const char *t_include="#include";
    const char *t_define="#define";
    char *s_include,*s_define;
    s_include=strstr(str,t_include); //返回除"include”之外剩余的部分
    // cout<<s_include<<endl;
    //判断头文件
    if(s_include != NULL){
        int i=7;
        while(i){
            if(*(s_include+i) == ' ' || *(s_include+i) == '\t') i++;
            else break;
        }
        addNewNode(s_include,HEADER,HEADER_NUM,-1,line);
    }
    else{   //判断宏定义
        s_define=strstr(str,t_define);
        int i=7;
        while(i){
            if(*(s_define+i) == ' ' || *(s_define+i) == '\t') i++;
            else break;
        }
        addNewNode(s_define,MACRO,MACRO_NUM,-1,line);
    }

}

//查找关键字
int findKey(char * str){
    for(int i=0; i<32; i++){
        if(strcmp(str,key[i]) == 0) return i+1;  //返回关键字的种别编码
    }
    return IDENTIFIER_NUM;  //返回标识符的种别编码
}

//查找运算符
int findOperator(char * str){
    for(int i=0; i<35; i++){
        if(strcmp(str,operators[i]) == 0) return i+48;  //返回关键字的种别编码
    }
    return -1;  //返回标识符的种别编码
}

//查找界符
int findDelimiter(char * str){
    for(int i=0; i<11; i++){
        if(strcmp(str,delimiters[i]) == 0) return i+35;  //返回关键字的种别编码
    }
    return -1;  //返回标识符的种别编码
}

void lexical_analysis(){
    char filename[MAX];  
    char character;
    char array[MAX];  //单词长度上限是MAX
    char *str;
    int i;
    int line=1;  //行号

    FILE *input_file;
    printf("Please input the C language program for lexical analysis:");
    scanf("%s",filename);
    input_file=fopen(filename,"r");
    while(!input_file){
        printf("read file error!\n");
        return;
    }

    character=fgetc(input_file);
    while(character != EOF){
        // cout<<character;
        i=0;
        //判断标识符或者关键字
        if((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || character == '_'){
            while((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') || character == '_'){
                array[i++]=character;
                character=fgetc(input_file);
            }
            str=new char[i+1];
            memcpy(str,array,i);
            str[i]='\0';
            int value=findKey(str);
            //如果不是关键字，则是标识符
            if(value != IDENTIFIER_NUM) addNewNode(str,KEY,value,-1,line);
            else{
                int addr_temp=addNewIdenNode(str,IDENTIFIER,value,-1,line);
                addNewNode(str,IDENTIFIER,value,addr_temp,line);
            }
            fseek(input_file,-1L,SEEK_CUR);//向后回退一位
        }
        //以数字开头，判断数字
        else if(character >= '0' && character <= '9'){
            //判断整数
            while(character >= '0' && character <= '9'){
                array[i++] = character;
                character=fgetc(input_file);
            }

            //判断浮点数float
            if(character == '.'){
                array[i++] = character;
                character=fgetc(input_file);
                if(character >= '0' && character <= '9'){
                    while(character >= '0' && character <= '9'){
                        array[i++] = character;
                        character=fgetc(input_file);
                    }
                }

                //判断double
                if(character == 'E' || character == 'e'){
                    array[i++] = character;
                    character=fgetc(input_file);
                    if(character == '+' || character == '-'){
                        array[i++] = character;
                        character=fgetc(input_file);
                    }
                    if(character >= '0' && character <= '9'){
                        array[i++] = character;
                        character=fgetc(input_file);
                    }
                }
            }

            str = new char[i+1];
            memcpy(str,array,i);
            str[i]='\0';
            addNewNode(str,CONSTANT,CONSTANT_NUM,-1,line);
            fseek(input_file,-1L,SEEK_CUR);//向后回退一位
        }
        //以"/"开头
        else if(character == '/'){
            character=fgetc(input_file);
             //处理运算符"/="
            if(character == '='){
                addNewNode("/=",OPERATOR,findOperator("/="),-1,line);
            }
             //处理"/**/"形注释
            else if(character == '*'){
                character=fgetc(input_file);
                string s_note="/*";
                s_note += character;
                while(1){
                    while(character != '*'){
                        if(character == '\n') line++;
                        character=fgetc(input_file);
                        if(character != '*') s_note += character;
                        if(character == EOF) return;
                    }
                    character=fgetc(input_file);
                    if(character == '/') break;
                    if(character == EOF) return;
                }
                s_note += "*/";
                char *ss_note=(char *)s_note.data();
                addNewNode(ss_note,NOTE,46,-1,line);
            }
            //处理"//"型注释
            else if(character == '/'){
                string s_note="//";
                while(character != '\n'){
                    character=fgetc(input_file);
                    if(character != '\n'){
                        s_note += character;
                    }
                    if(character == EOF){
                        char *ss_note=(char *)s_note.data();
                        addNewNode(ss_note,NOTE,47,-1,line);
                        return;
                    }
                }
                line++;
                char *ss_note=(char *)s_note.data();
                addNewNode(ss_note,NOTE,47,-1,line);
                if(character == EOF) return;
            }
            //处理除号/
            else{
                addNewNode("/",OPERATOR,findOperator("/"),-1,line);
                fseek(input_file,-1L,SEEK_CUR);//
            }
        }
        //处理常量字符串
        else if(character == '"'){
            addNewNode("\"",DELIMITER,findDelimiter("\""),-1,line);
            character=fgetc(input_file);
            i=0;
            while(character != '"'){
                array[i++]=character;
                if(character == '\n') line++;
                character=fgetc(input_file);
                if(character == EOF) return;
            }
            str=new char[i+1];
            memcpy(str,array,i);
            str[i]='\0';
            addNewNode(str,CONSTANT_STRING,CONSTANT_STRING_NUM,-1,line);
            addNewNode("\"",DELIMITER,findDelimiter("\""),-1,line);
        }
        //处理常量字符
        else if(character == '\''){
            addNewNode("\'",DELIMITER,findDelimiter("\'"),-1,line);
            character=fgetc(input_file);
            i=0;
            while(character != '\''){
                array[i++]=character;
                if(character == '\n') line++;
                character=fgetc(input_file);
                if(character == EOF) return;
            }
            str=new char[i+1];
            memcpy(str,array,i);
            str[i]='\0';
            addNewNode(str,CONSTANT_CHARACTER,CONSTANT_CHARACTER_NUM,-1,line);
            addNewNode("\'",DELIMITER,findDelimiter("\'"),-1,line);
        }
        //处理换行
        else if(character == ' ' || character == '\t' || character == '\r' || character == '\n')
        {
            if(character == '\n') line++;
        }
        else{
            if(character == EOF) return;
            //处理头文件和宏常量（预处理）
            else if(character == '#'){
                while(character != '\n' && character != EOF){
                    array[i++] = character;
                    character =fgetc(input_file);
                }
                str = new char[i+1];
                memcpy(str,array,i);
                str[i] = '\0';
                preProcess(str,line);  //预处理
                fseek(input_file,-1L,SEEK_CUR);//向后回退一位
            }
            //处理以-开头的运算符
            else if(character == '-'){
                array[i++] = character;
                character =fgetc(input_file);
                //判断常数
                if(character >= '0' && character <= '9'){
                    //判断整数
                    while(character >= '0' && character <= '9'){
                        array[i++] = character;
                        character=fgetc(input_file);
                    }
                    //判断浮点数float
                    if(character == '.'){
                        array[i++] = character;
                        character=fgetc(input_file);
                        if(character >= '0' && character <= '9'){
                            while(character >= '0' && character <= '9'){
                                array[i++] = character;
                                character=fgetc(input_file);
                            }
                        }

                        //判断double
                        if(character == 'E' || character == 'e'){
                            array[i++] = character;
                            character=fgetc(input_file);
                            if(character == '+' || character == '-'){
                                array[i++] = character;
                                character=fgetc(input_file);
                            }
                            if(character >= '0' && character <= '9'){
                                array[i++] = character;
                                character=fgetc(input_file);
                            }
                        }
                    }
                    str = new char[i+1];
                    memcpy(str,array,i);
                    str[i] = '\0';
                    addNewNode(str,CONSTANT,CONSTANT_NUM,-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
                //判断指针->
                else if(character == '>'){
                    addNewNode("->",OPERATOR,findOperator("->"),-1,line);
                }
                //判断自减--
                else if(character == '-'){
                    addNewNode("--",OPERATOR,findOperator("--"),-1,line);
                }
                //判断-=
                else if(character == '='){
                    addNewNode("-=",OPERATOR,findOperator("-="),-1,line);
                }
                //判断-
                else{
                    addNewNode("-",OPERATOR,findOperator("-"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理+开头的运算符
            else if(character == '+'){
                character =fgetc(input_file);
                //判断自增++
                if(character == '+'){
                    addNewNode("++",OPERATOR,findOperator("++"),-1,line);
                }
                //判断+=
                else if(character == '='){
                    addNewNode("+=",OPERATOR,findOperator("+="),-1,line);
                }
                //判断+
                else{
                    addNewNode("+",OPERATOR,findOperator("+"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理*开头的运算符
            else if(character == '*'){
                character =fgetc(input_file);
                //判断*=
                if(character == '='){
                    addNewNode("*=",OPERATOR,findOperator("*="),-1,line);
                }
                //判断*
                else{
                    addNewNode("*",OPERATOR,findOperator("*"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理^开头的运算符
            else if(character == '^'){
                character =fgetc(input_file);
                //判断^=
                if(character == '='){
                    addNewNode("^=",OPERATOR,findOperator("^="),-1,line);
                }
                //判断^
                else{
                    addNewNode("^",OPERATOR,findOperator("^"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理%开头的运算符
            else if(character == '%'){
                character =fgetc(input_file);
                //判断%=
                if(character == '='){
                    addNewNode("%=",OPERATOR,findOperator("%="),-1,line);
                }
                //判断%
                else{
                    addNewNode("%",OPERATOR,findOperator("%"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理&开头的运算符
            else if(character == '&'){
                character =fgetc(input_file);
                //判断&&
                if(character == '&'){
                    addNewNode("&&",OPERATOR,findOperator("&&"),-1,line);
                }
                //判断&=
                else if(character == '='){
                    addNewNode("&=",OPERATOR,findOperator("&="),-1,line);
                }
                //判断&
                else{
                    addNewNode("&",OPERATOR,findOperator("&"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理~开头的运算符
            else if(character == '~'){
                character =fgetc(input_file);
                //判断~=
                if(character == '='){
                    addNewNode("~=",OPERATOR,findOperator("~="),-1,line);
                }
                //判断~
                else{
                    addNewNode("~",OPERATOR,findOperator("~"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理!开头的运算符
            else if(character == '!'){
                character =fgetc(input_file);
                //判断!=
                if(character == '='){
                    addNewNode("!=",OPERATOR,findOperator("!="),-1,line);
                }
                //判断!
                else{
                    addNewNode("!",OPERATOR,findOperator("!"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理<开头的运算符
            else if(character == '<'){
                character =fgetc(input_file);
                //判断<<或者<<=
                if(character == '<'){
                    character =fgetc(input_file);
                    if(character == '=') addNewNode("<<=",OPERATOR,findOperator("<<="),-1,line);
                    else addNewNode("<<",OPERATOR,findOperator("<<"),-1,line);
                }
                //判断<=
                else if(character == '='){
                    addNewNode("<=",OPERATOR,findOperator("<="),-1,line);
                }
                //判断<
                else{
                    addNewNode("<",OPERATOR,findOperator("<"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理>开头的运算符
            else if(character == '>'){
                character =fgetc(input_file);
                //判断>>或者>>=
                if(character == '>'){
                    character =fgetc(input_file);
                    if(character == '=') addNewNode(">>=",OPERATOR,findOperator(">>="),-1,line);
                    else addNewNode(">>",OPERATOR,findOperator(">>"),-1,line);
                }
                //判断>=
                else if(character == '='){
                    addNewNode(">=",OPERATOR,findOperator(">="),-1,line);
                }
                //判断<
                else{
                    addNewNode(">",OPERATOR,findOperator(">"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理|开头的运算符
            else if(character == '|'){
                character =fgetc(input_file);
                //判断||
                if(character == '|'){
                    addNewNode("||",OPERATOR,findOperator("||"),-1,line);
                }
                //判断|=
                else if(character == '='){
                    addNewNode("|=",OPERATOR,findOperator("|="),-1,line);
                }
                //判断|
                else{
                    addNewNode("|",OPERATOR,findOperator("|"),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //处理=开头的运算符
            else if(character == '='){
                character =fgetc(input_file);
                //判断==
                if(character == '='){
                    addNewNode("==",OPERATOR,findOperator("=="),-1,line);
                }
                //判断=
                else{
                    addNewNode("=",OPERATOR,findOperator("="),-1,line);
                    fseek(input_file,-1L,SEEK_CUR);//向后回退一位
                }
            }
            //判断左括号(
            else if(character == '('){
                addNewNode("(",DELIMITER,findDelimiter("("),-1,line);
            }
            //判断右括号）
            else if(character == ')'){
                addNewNode(")",DELIMITER,findDelimiter(")"),-1,line);
            }
            //判断[
            else if(character == '['){
                addNewNode("[",DELIMITER,findDelimiter("["),-1,line);
            }
            //判断]
            else if(character == ']'){
                addNewNode("]",DELIMITER,findDelimiter("]"),-1,line);
            }
            //判断{
            else if(character == '{'){
                addNewNode("{",DELIMITER,findDelimiter("{"),-1,line);
            }
            //判断}
            else if(character == '}'){
                addNewNode("}",DELIMITER,findDelimiter("}"),-1,line);
            }
            //判断.
            else if(character == '.'){
                addNewNode(".",DELIMITER,findDelimiter("."),-1,line);
            }
            //判断,
            else if(character == ','){
                addNewNode(",",DELIMITER,findDelimiter(","),-1,line);
            }
            //判断;
            else if(character == ';'){
                addNewNode(";",DELIMITER,findDelimiter(";"),-1,line);
            }
        }
        character=fgetc(input_file);
    }
}

void printResult(){
    ofstream outfile;  //输出文件
    outfile.open("tokens.txt");
    Node * node=head;
    node=node->next;
    // outfile << setiosflags(ios::left);
    outfile << fixed << right;
    if(is_print){
        cout<<"************************************自制C语言词法分析表******************************"<<endl<<endl;
        cout<<setw(9)<<"内容"<<setw(10)<<"描述"<<"\t"<<"种别码"<<"\t"<<"内码值"<<"\t"<<"行号"<<endl;
    }

    outfile<<"************************************自制C语言词法分析表******************************"<<endl<<endl;
    outfile<<setw(23)<<"内容"<<"\t\t"<<"描述"<<"  \t\t"<<"种别码"<<"\t\t"<<"内码值"<<"\t\t"<<"行号"<<endl;
    while(node != NULL){
        if(node->value == IDENTIFIER_NUM){
            if(is_print){
                cout<<setw(30)<<node->content<<setw(10)<<node->describe<<"\t"<<node->value<<"\t"<<node->addr<<"\t"<<node->line<<endl;
            }
            outfile<<setw(20)<<node->content<<"\t\t"<<node->describe<<"  \t\t"<<node->value<<"\t\t\t"<<node->addr<<"    \t\t"<<node->line<<endl;
        }
        else{
            if(is_print){
                cout<<setw(30)<<node->content<<setw(10)<<node->describe<<"\t"<<node->value<<"\t"<<'_'<<"\t"<<node->line<<endl;
            }
            outfile<<setw(20)<<node->content<<"\t\t"<<node->describe<<"  \t\t"<<node->value<<"\t\t\t"<<'_'<<"    \t\t"<<node->line<<endl;
        }
        node = node->next;
    }
    if(is_print){
        cout<<endl<<endl;
    }
    outfile<<endl<<endl;
}

int main(){
    head=new Node();
    Idenhead=new Node();
    lexical_analysis();
    printResult();
    delete head;
    delete Idenhead;
    cout<<"The result is in tokens.txt."<<endl;
    getchar();
    getchar();
}