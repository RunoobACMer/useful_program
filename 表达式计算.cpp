#include <iostream>
#include <stdio.h>
#include <stack>
#include <stdlib.h>
#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iomanip>
using namespace std;
#define MAXSIZE 100 //栈空间的初始分配量

typedef struct{
    char *base;	//栈底指针
    char *top;	//栈顶指针
    int stacksize;	//栈可用的最大容量
}Stack_char;

typedef struct{
    double *base;	//栈底指针
    double *top;	//栈顶指针
    int stacksize;	//栈可用的最大容量
}Stack_double;

template <typename Stack ,typename any>
//构建栈
int Create_Stack(Stack &s,any type)
{
    any c = type;
    s.base = new any[MAXSIZE];  //给栈分配空间
    if(!s.base) return 1;   //分配失败
    s.top = s.base; //top和base指向同一位置
    s.stacksize = MAXSIZE;  //stack标记为栈的最大长度
    return 0;
}

template <typename Stack ,typename any>
//向栈里压入元素
double Push_Stack(Stack &s,any c)
{
    if(s.top-s.base==s.stacksize){  //栈满
        return 1;
    }
    *s.top++ = c;   //压入栈顶，栈顶指针加1
    return 0;
}
template <typename Stack>
//元素出栈（删除）
int Pop_Stack(Stack &s)
{
    if(s.top==s.base){  //栈空
        return 1;
    }
    --s.top;   //栈顶元素赋给c
}
template <typename Stack>
//获取栈顶元素（不删除）
double Get_Stack(Stack &s)
{
    if(s.top!=s.base)   //栈非空
    {
        return *(s.top-1);
    }
}

bool isInteger(double a){
    double absolute = abs(a);
    return absolute == floor(absolute);
}

// 括号匹配检测
bool checkBracket(char str[]){
    int cntSmall = 0, cntMiddle = 0, cntBig = 0;
    for(int i=0;i<strlen(str);i++){
        if(str[i] == '('){
            if(cntSmall == 1)
                return false;
            else
                cntSmall = 1;
        }else if(str[i] == '['){
            if(cntMiddle == 1)
                return false;
            else
                cntMiddle = 1;
        }else if(str[i] == '{'){
            if(cntBig == 1)
                return false;
            else
                cntBig = 1;
        }else if(str[i] == ')'){
            if(cntSmall == 0)
                return false;
            else
                cntSmall = 0;
        }else if(str[i] == ']'){
            if(cntMiddle == 0)
                return false;
            else
                cntMiddle = 0;
        }else if(str[i] == '}'){
            if(cntBig == 0)
                return false;
            else
                cntBig = 0;
        }
    }
    if(cntSmall || cntMiddle || cntBig) return false;
    return true;
}

// 中缀表达式转后缀表达式

//运算符比较函数
//   Compare(栈顶，C)    
char Compare(char stack_top,char c){
    char priority[8][8]={
        {'<','<','<','<','<','>','<','>'},
        {'<','<','<','<','<','>','<','>'},
        {'>','>','<','<','<','>','<','>'},
        {'>','>','<','<','<','>','<','>'},
        {'>','>','<','<','<','>','<','>'},
        {'>','>','>','>','>','>','0','>'},
        {'<','<','<','<','<','=','<','0'}, // 我感觉原来的注释错了，这里应该是 = 代表左右空号相遇，0代表语法非法
        {'<','<','<','<','<','0','<','='}  // = 表示两个#相遇，也就是整个表达式求值完成。（两个#相遇和左右括号相遇的处理是一样的）
    };                                    
    /*
       /// 这里是原来的矩阵，左侧是栈顶元素，上侧是当前元素C              ！！！
       /// 我把这个矩阵处理了一下，现在左侧是当前元素，上侧是栈顶元素了   ！！！
    char priority[8][8]={
              --------- 当前元素 C ----------
              +   -   *   /   %   (   )   #
      |  +  {'>','>','<','<','<','<','>','>'},
      |  -  {'>','>','<','<','<','<','>','>'},
      |  *  {'>','>','>','>','>','<','>','>'}, 
 栈顶    /  {'>','>','>','>','>','<','>','>'}, 
 元素    %  {'>','>','>','>','>','<','>','>'},
      |  (  {'<','<','<','<','<','<','=','0'},  
      |  )  {'>','>','>','>','>','0','>','>'}, 
      |  #  {'<','<','<','<','<','<','0','='}
    };
    
    
    char priority[8][8]={
               --------- 栈顶元素  ----------
               +   -   *   /   %   (   )   #
       |  +  {'<','<','<','<','<','>','<','>'},
       |  -  {'<','<','<','<','<','>','<','>'},
       |  *  {'>','>','<','<','<','>','<','>'},
 当前     /  {'>','>','<','<','<','>','<','>'},
 元素C    %  {'>','>','<','<','<','>','<','>'},
       |  (  {'>','>','>','>','>','>','0','>'},
       |  )  {'<','<','<','<','<','=','<','0'},
       |  #  {'<','<','<','<','<','0','<','='}
    };
       C
      (#
    
    */
    int i,j;    //行，列
    switch(c){  //跳转到行对应的符号
        case'+':i=0;break;
        case'-':i=1;break;
        case'*':i=2;break;
        case'/':i=3;break;
        case'%':i=4;break;
        case'(':i=5;break;
        case')':i=6;break;
        case'#':i=7;break;   // # 是表达式的结束符
    }
    switch(stack_top){  //跳转到列对应的符号
        case'+':j=0;break;
        case'-':j=1;break;
        case'*':j=2;break;
        case'/':j=3;break;
        case'%':j=4;break;
        case'(':j=5;break;
        case')':j=6;break;
        case'#':j=7;break;   // # 是表达式的结束符
    }
    return priority[i][j];
}

//运算函数
double Operate(double m,double n,char x,int& status){
    double esp = 1e-8;
    status = 0;
    
    if(x=='+')
        return m+n;
    if(x=='-')
        return n-m;
    if(x=='*')
        return m*n;
    if(x=='/'){
        if(fabs(m) < esp){
            cout<<"出错！除零错误！"<<endl;
            return status = 1;
        }
        return int(n/m);
    }
    if(x=='%'){
        if(m <= 0){
            cout<<"出错！模数小于等于零！"<<endl;
            return status = 2;
        }
        if(!isInteger(m)){
            cout<<"出错！模数不是整数！"<<endl;
            return status = 3;
        }
        if(!isInteger(n)){
            cout<<"出错！被模数不是整数！"<<endl;
            return status = 4;
        }
        return (int)n % (int)m;
    }
}

int main(){
    Stack_char op;  //创建运算符栈
    Stack_double num;   //创建数字栈

    double a = 0;
    char b = 'a';
    Create_Stack(num,a);    //初始化栈
    Create_Stack(op,b);

    Push_Stack(op,'#'); //在运算符栈压入#
    char end[2]="#";    //尾部有\0
    char s[MAXSIZE];
    cout << "请输入运算式：";
    cin>>s;
    if(checkBracket(s) == false){
        cout<<"错误！括号不匹配!"<<endl;
        return 0;
    }
    for(int i=0;i<strlen(s);i++){
        if(s[i] == '[' || s[i] == '{')
            s[i] = '(';
        else if(s[i] == ']' || s[i] == '}')
            s[i] = ')';
    }
    strcat(s,end);// 运算式尾部加#
    char c=s[0];
    int k=1;//下标
    int status = 0;
    while(c!='#'||Get_Stack(op)!='#'){  //表达式未读完或者运算未完
        double y=0.0;
        double z=0.0;
        int count=0;
        // 操作数处理
        if(c>='0'&&c<='9'){
            while(c>='0'&&c<='9'){  // 读入连续的数字
                y=y*10+(c-'0');
                c=s[k++];
                if(c=='.'){ //读入小数点
                    c=s[k++];
                    while (c>='0'&&c<='9'){
                        ++count;
                        z=z+pow(0.1,count)*(c-'0');
                        c=s[k++];
                    }
                }
            }
            Push_Stack(num,y+z);  // 把读进的数字入数字栈
        }
        else{
            //操作符处理
            switch(Compare(Get_Stack(op),c))
            {
                case'>':  //栈顶元素优先权低,C优先级高
                    Push_Stack(op,c);
                    c=s[k++];
                    break;
                case'=':
                    Pop_Stack(op);  //括号匹配成功，左右括号出栈
                    c=s[k++];  // 读入下一个字符
                    break;
                case'<':  //退栈并将运算结果入栈
                    char x=Get_Stack(op);Pop_Stack(op);
                    double m=Get_Stack(num);Pop_Stack(num);
                    double n=Get_Stack(num);Pop_Stack(num);
                    status = 0;
                    double op_num = Operate(m,n,x,status);
                    if(status != 0){
                        break;
                    }
                    Push_Stack(num,op_num);
                    break;
            }
            if(status){
                break;
            }
        }
    }
    if(status == 0){
        cout << endl << "运算结果为："<<setiosflags(ios::fixed) << setprecision(4) <<Get_Stack(num);
    }
    return 0;
}

/*
(6%4-1)*3-6 
-3

1+[(2+3)*4]-5
16

5%2+8*((2-7)/(3%5))%4
报错,括号不匹配

5%2+8*[(2-7)/(3%5)]%4
报错，被模数不是整数

3%2*8*[(7-1)/(3%5)]%4
0

5*6/3+4*3
22

 */
