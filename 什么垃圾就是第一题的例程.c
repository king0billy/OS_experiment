#include "stdio.h"
#include <stdlib.h>
#include <conio.h>
#define getpch(type) (type*)malloc(sizeof(type))
#define NULL 0
int time[3];
struct program {
char name[10];
char state;
int queue;//���̶���
int priority;   // ����ԽС���ȼ�Խ��
int needtime;//������ʱ��
int runtime; //�Ѿ�����ʱ��
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
printf("\n �༶�������е����㷨 \n");
printf("\n ��������̸���:\n");
scanf("%d",&num);

//printf("\n qname \t state \t queue \t needtime \t runtime \n");
printf("\n �����һ��������:");
ready=getpch(PROGRAM);
scanf("%s",ready->name);
printf("\n �����һ���������ȼ�:");
scanf("%d",&ready->priority);
printf("\n �����һ����������ʱ��:");

scanf("%d",&ready->needtime);
printf("\n");
ready->runtime=0;ready->state='r';
ready->queue=1;
ready->link=NULL;

for(i=0;i<num-1;i++)
{ printf("\n ���̺�No.%d:\n",i+2);
p=getpch(PROGRAM);
printf("\n ���������:");
scanf("%s",p->name);
printf("\n ����������ȼ�:");
scanf("%d",&ready->priority);
p->queue=1;
//printf("\n ������̶�����1~3:");
//scanf("%d",&p->queue);
printf("\n �����������ʱ��:");
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
void check()//���̲鿴����
{PROGRAM *pr;
printf("\n****��ǰ�������еĽ���%s",run->name);
disp(run);
printf("\n****��ǰ��׼���õĽ���%s",ready->name);
if(ready!=NULL)
{disp(ready);
printf("\n****��ǰ�����Ķ���״̬Ϊ:\n");}
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
//��������Ľ��̵�ִ�к���
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
{check();//��ʾ���м��ȴ����е�״��
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
h++;//��־ִ�еĲ���
printf("\n The execute number:%d \n",h);

runpro();
if(run->runtime==run->needtime)
{printf("\n����%s�Ѿ����\n",run->name);}
printf("\n ����һ������......");
ch=getchar();
}
printf("���������������");
}
