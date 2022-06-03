#include "LR0.h"
#include "LL1.h"

int main(){
    cout<<"Welcome to LL1 and LR parse!"<<endl;
    cout<<"Input 1:LL1"<<endl;
    cout<<"Input 2:LR"<<endl;
    cout<<"Please input the choice:"<<endl;
    int choice;
    cin>>choice;
    if(choice==1) LL1_Parse();
    else if(choice==2) LR_Parse();
    else cout<<"Input error!"<<endl;
    getchar();
    getchar();
}