#include "stdio.h" 
#include <stdlib.h> 
#include <conio.h> 
#define getpch(type) (type*)malloc(sizeof(type)) 
//#define NULL 0 
int  slot=0;
int  relativeTime=0;
struct pcb { /* 定义进程控制块PCB */
	char name[10];
	char state;
	int arriveTime=0;
	int finishTime;
	int needTime;
	int runTime;
	struct pcb* link;
}*ready = NULL, *p=NULL;
typedef struct pcb PCB;
pcb** tempArr;
double* tempTime;
int doneIndex4tempTime=0;
int doneIndex4tempArr=0;
int num4tempArr=0;
//sort() /* 建立对进程进行优先级排列函数*/
//{
//
//	PCB *first, *second;
//
//	if (ready == NULL)
//	{
//		p->link = ready;
//		ready = p;
//	}
//	else /* 进程比较优先级,插入适当的位置中*/
//	{
//		first=ready;
//		for(second=ready;second->link!=NULL;second=second->link);
//		p->link=NULL;
//		second->link=p;
//		first=first->link;
//	}
//
//}
sort(){
        if(ready==NULL){
            if(p==NULL&&doneIndex4tempArr<num4tempArr){
                ready=tempArr[doneIndex4tempArr];
				relativeTime=tempArr[doneIndex4tempArr]->arriveTime;
                doneIndex4tempArr++;
            }
            PCB *first, *second=ready;
            for(;second!=NULL&&second->link!=NULL;second=second->link);
            for(;second!=NULL&&doneIndex4tempArr<num4tempArr&&
            tempArr[doneIndex4tempArr]->arriveTime<=relativeTime+slot;doneIndex4tempArr++){
                        second->link=tempArr[doneIndex4tempArr];
                        second=second->link;
            }
        }
		else{
			PCB *first, *second=ready;
        	for(;second->link!=NULL;second=second->link);

            for(;doneIndex4tempArr<num4tempArr&&
            tempArr[doneIndex4tempArr]->arriveTime<=relativeTime+slot;doneIndex4tempArr++){
                        second->link=tempArr[doneIndex4tempArr];
                        second=second->link;
            }
                second->link=p;
        }

		
}
	
input() /* 建立进程控制块函数*/
{
	system("cls");
	//clrscr(); /*清屏*/
	printf("\n 请输入进程total number?");
	scanf("%d", &num4tempArr);
	tempTime=(double*)malloc(num4tempArr*sizeof(int));
	tempArr=(pcb**)malloc(num4tempArr*sizeof(p));
	for (int i = 0; i<num4tempArr; i++)
	{
		printf("\n 进程号No.%d:\n", i);
		p = getpch(PCB);

/*		printf("\n 输入进程名:");
		scanf("%s", p->name);*/

		p->name[0]=(char)(i+'0');
		p->name[1]='\0';

//		printf("\n 输入进程优先数:");
//		scanf("%d", &p->super);

		printf("\n 输入arriveTime:");
		scanf("%d", &p->arriveTime);

		printf("\n 输入进程运行时间in need : ");
		scanf("%d", &p->needTime);
		printf("\n");
		p->runTime = 0; p->state = 'w';
		p->link = NULL;
		tempArr[i]=p;
		tempTime[i]=0;
	}
	// 选择排序
	pcb* minP=tempArr[0];pcb* tempP=tempArr[0];
	for(int i=0,minI=i;i<num4tempArr;i++){
        minI=i;
        minP=tempArr[i];
            for(int j=i;j<num4tempArr;j++){
                if(tempArr[j]->arriveTime < minP->arriveTime){
                    minI=j;
                    minP=tempArr[j];
                    }
                    tempP=tempArr[i];tempArr[i]=minP;tempArr[minI]=tempP;
            }
		}
    for(int i=0;i<num4tempArr;i++){
        printf("i=%d,arriveTime=%d\n",i,tempArr[i]->arriveTime);
	}
	p=NULL;
	sort(); /* 调用sort函数*/
}
int space()
{
	int length = 0; PCB* pr = ready;
	while (pr != NULL)
	{
		length++;
		pr = pr->link;
	}
	return(length);
}
disp(PCB * pr) /*建立进程显示函数,用于显示当前进程*/
{
	printf("\n qname\t state\t ndtime\t runTime\t relativeTime\t timeSlot\n");
	printf("|%s\t", pr->name);
	printf("|%c\t", pr->state);
	printf("|%d\t", pr->needTime);
	printf("|%d\t", pr->runTime);
	printf("\t|%d\t", relativeTime);
	printf("\t|%d\t", slot);
	printf("\n");
	printf("relativeTime is the time at starting this slice!\n");
}
check() /* 建立进程查看函数 */
{
	PCB* pr;
	printf("\n **** 当前正在运行的进程是:%s", p->name); /*显示当前运行进程*/
	disp(p);
	pr = ready;
	printf("\n ****当前就绪队列状态为:\n"); /*显示就绪队列状态*/
	while (pr != NULL)
	{
		disp(pr);
		pr = pr->link;
	}
}
destroy() /*建立进程撤消函数(进程运行结束,撤消进程)*/
{
	printf("\n 进程 [%s] 已完成.\n", p->name);
	tempTime[doneIndex4tempTime++]=(p->finishTime-p->arriveTime)*1.0/p->needTime;
	printf("\n 进程 [%s] 的带权周转时间为=%lf\n", p->name,tempTime[doneIndex4tempTime-1]);
	free(p);
	p=NULL;
}
running() /* 建立进程就绪函数(进程运行时间到,置就绪状态*/
{
/*    if(p==NULL)sort();*/
    if(NULL){}
    else{
        if (p->runTime+slot >= p->needTime){
            relativeTime+=p->needTime-p->runTime;
            p->finishTime=relativeTime;//!!!!!!!!!!!!!
            p->runTime=p->needTime;
            destroy(); /* 调用destroy函数*/
        }
        else
        {
            relativeTime+=slot;
            (p->runTime)=p->runTime+slot;
            p->state = 'w';
            sort(); /*调用sort函数*/
        }
	}
}
main() /*主函数*/
{
	int len, h = 0;
	char ch;
		relativeTime=0;
		
		printf("\n 请输入time slice? ");
		scanf("%d", &slot);
		
	input();
	len = space();
	while (doneIndex4tempArr<num4tempArr||(len != 0 && ready != NULL)||((p!=NULL)&&p->runTime<p->needTime) )
	//while(doneIndex4tempArr<num4tempArr)
	{
		ch = getchar();
		h++;
		printf("\n The execute number:%d \n", h);

/*            p = ready;
            if(p!=NULL){
                ready = p->link;
                p->link = NULL;
                p->state = 'R';
                check();
            }
            running();*/

            if(ready!=NULL){
                p = ready;
                ready = p->link;
                p->link = NULL;
                p->state = 'R';
                check();
                running();
            }
            else{
                if(p!=NULL){
                    p->state = 'R';
                    check();
                    running();
                }
                //check();
                sort();
            }

        printf("\n 按任一键继续......");
        ch = getchar();
	}
	printf("\n\n 进程已经完成.\n");
	double sum=0;
	for(int i=0;i<num4tempArr;i++){
	    sum+=tempTime[i];
	}
	printf("\n 平均带权周转时间为=%lf\n", sum/num4tempArr);
	ch = getchar();
}
