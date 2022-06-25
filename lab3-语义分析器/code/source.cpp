#include "semantic.h"

int main(){
    LL1_Semantic LL1_Semantic;
    Node *head=lexical_result();
    LL1_Semantic.LL1Semantic(head);
    cout<<"LL1 semantic analysis is complete,the result of LL1 is saved in LL1_output.txt."<<endl;
    getchar();
    getchar();
    return 0;
}