#include "stdio.h"
#include <stdlib.h>
#include <conio.h>
#define getpch(type) (type*)malloc(sizeof(type))
#define NULL 0
int time[3];
struct program {
char name[10];
char state;
int queue;//进程队列
int priority;   // 数字越小优先级越高
int needtime;//需运行时间
int runtime; //已经运行时间
struct program *link;
}*ready=NULL;
typedef struct program PROGRAM;
PROGRAM *run=NULL,*head1=NULL,*head2=NULL,*head3=NULL,*end1=NULL,*end2=NULL,*end3=NULL;


void sort(PROGRAM *p)
{switch(p->queue)
{case 1:
{if(head1==NULL) {head1=p;end1=p;}
else
{end1->link=p;end1=p;p->link=NULL;}
p->state='w';
break;
}
   case 2:
{if(head2==NULL)
{head2=p;end2=p;p->state='w';}
else
{end2->link=p;end2=p;p->link=NULL;}
p->state='w';
break;
}
   case 3:
{if(head3==NULL)
{head3=p;end3=p;}
else
{end3->link=p;end3=p;p->link=NULL;}
p->state='w';
break;
}
}
}
void input()
{ PROGRAM *p;
int i,num;

system("cls");
printf("\n 多级反馈队列调度算法 \n");
printf("\n 请输入进程个数:\n");
scanf("%d",&num);

//printf("\n qname \t state \t queue \t needtime \t runtime \n");
printf("\n 输入第一个进程名:");
ready=getpch(PROGRAM);
scanf("%s",ready->name);
printf("\n 输入第一个进程优先级:");
scanf("%d",&ready->priority);
printf("\n 输入第一个进程运行时间:");

scanf("%d",&ready->needtime);
printf("\n");
ready->runtime=0;ready->state='r';
ready->queue=1;
ready->link=NULL;

for(i=0;i<num-1;i++)
{ printf("\n 进程号No.%d:\n",i+2);
p=getpch(PROGRAM);
printf("\n 输入进程名:");
scanf("%s",p->name);
printf("\n 输入进程优先级:");
scanf("%d",&ready->priority);
p->queue=1;
//printf("\n 输入进程队伍数1~3:");
//scanf("%d",&p->queue);
printf("\n 输入进程运行时间:");
scanf("%d",&p->needtime);
printf("\n");
p->runtime=0;p->state='w';
p->link=NULL;
sort(p);
}
}
void disp(PROGRAM * pr)
{
printf("\n qname\t priority\t state\t queue\t needtime\t runtime \n");
printf("|%s\t",pr->name);
printf("|%c\t",pr->priority);
printf("|%c\t",pr->state);
printf("|%d\t",pr->queue);
printf("|%d\t",pr->needtime);
printf("|%d\t",pr->runtime);
printf("\n");
}
void check()//进程查看函数
{PROGRAM *pr;
printf("\n****当前正在运行的进程%s",run->name);
disp(run);
printf("\n****当前正准备好的进程%s",ready->name);
if(ready!=NULL)
{disp(ready);
printf("\n****当前就绪的队列状态为:\n");}
pr=head1;
while(pr!=NULL)
{
disp(pr);
pr=pr->link;
}
pr=head2;
while(pr!=NULL)
{
disp(pr);
pr=pr->link;
}
pr=head3;
while(pr!=NULL)
{
disp(pr);
pr=pr->link;
}
}
//三个队伍的进程的执行函数
void runpro()
{
run=ready;
if(head1!=NULL)
{ready=head1;head1=head1->link;ready->state='r';}
else
{
if(head2!=NULL)
{ready=head2;head2=head2->link;ready->state='r';}
else
{if(head3!=NULL)
{ready=head3;head3=head3->link;ready->state='r';}
else
ready=NULL;}
}
if(run!=NULL)
{check();//显示运行及等待队列的状况
run->runtime=run->runtime+time[run->queue-1];
if(run->runtime>run->needtime)
run->runtime=run->needtime;
if(run->queue<3)
run->queue+=1;
//disp(run);
if(run->runtime<run->needtime)
sort(run);
}
}
void main()
{
int h=0;
char ch;
input();
time[0]=1;
time[1]=2;
time[2]=3;

while(ready!=NULL)
{
ch=getchar();
h++;//标志执行的步骤
printf("\n The execute number:%d \n",h);

runpro();
if(run->runtime==run->needtime)
{printf("\n进程%s已经完成\n",run->name);}
printf("\n 按任一键继续......");
ch=getchar();
}
printf("整个进程运行完毕");
}
