#include <stdio.h>
#include <string.h>
#define is_print 0

typedef struct STUDENT{
    int num;
    char s;
}Student;

int main(){
    Student *student1;
    student1->num=3;
    student1->s='q';

    int a = 10;
    double b = -20.9;
    int c=3;
    a ^= 2;
    if(a<=b)        
    a+=b;
    else a = 33.334;
    a <<= c;
    a = ((a&2)*b+c-a)/(-12.3);
    int i=0;
    while(i<9){
        a++;
        i++;
    }
    for(int j=10;j>1;j--);

    //switch用法
    int num;
    printf("input integer number: ");  /*打印*/
    i=7;
    switch (i){
        case 1:printf("Monday\n");
        case 2:printf("Tuesday\n");
        case 3:printf("Wednesday\n");
        case 4:printf("Thursday\n");
        case 5:printf("Friday\n");
        case 6:printf("Saturday\n");
        case 7:printf("Sunday\n");
        default:printf("Error\n");
    }
    return 0;
}
