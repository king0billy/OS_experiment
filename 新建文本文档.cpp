#include "stdio.h" 
#include <stdlib.h> 
#include <conio.h> 
#define getpch(type) (type*)malloc(sizeof(type)) 
//#define NULL 0 
int  slot=0;
int  relativeTime=0;
struct pcb { /* ������̿��ƿ�PCB */
	char name[10];
	char state;
	int arriveTime=0;
	int finishTime;
	int needTime;
	int runTime;
	struct pcb* link;
}*ready = NULL, *p;
typedef struct pcb PCB;
pcb** tempArr;
int doneIndex4tempArr=0;
int num4tempArr=0;
//sort() /* �����Խ��̽������ȼ����к���*/
//{
//
//	PCB *first, *second;
//
//	if (ready == NULL)
//	{
//		p->link = ready;
//		ready = p;
//	}
//	else /* ���̱Ƚ����ȼ�,�����ʵ���λ����*/
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
            if(doneIndex4tempArr<num4tempArr){
                ready=tempArr[doneIndex4tempArr];
                //if(doneIndex4tempArr==0)
					relativeTime=tempArr[doneIndex4tempArr]->arriveTime;
                doneIndex4tempArr++;
                }
        }
		else{
			PCB *first, *second=ready;
        	for(;second->link!=NULL;second=second->link);

            if(doneIndex4tempArr<num4tempArr){
                int i=doneIndex4tempArr;
                for(;tempArr[i]->arriveTime<=relativeTime;i++){
                            /*first=ready;*/
                            /*first=first->link;*/
                            second->link=tempArr[i];
                            second=second->link;
                }
                doneIndex4tempArr=i;

                second->link=p;
                //ready=ready->link;
            }

            else{
                second->link=p;
               // ready=ready->link;
            }
		}
}
	
input() /* �������̿��ƿ麯��*/
{
//	int i, num;
	int i;
	system("cls");
	//clrscr(); /*����*/
	printf("\n ���������total number?");
	scanf("%d", &num4tempArr);
	tempArr=(pcb**)malloc(num4tempArr*sizeof(p));
	for (i = 0; i<num4tempArr; i++)
	{
		printf("\n ���̺�No.%d:\n", i);
		p = getpch(PCB);
		printf("\n ���������:");
		scanf("%s", p->name);

//		printf("\n �������������:");
//		scanf("%d", &p->super);

/*		printf("\n ����arriveTime:");
		scanf("%d", &p->arriveTime);*/

		printf("\n �����������ʱ��in need :");
		scanf("%d", &p->needTime);
		printf("\n");
		p->runTime = 0; p->state = 'w';
		p->link = NULL;
		tempArr[i]=p;
	}
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
	sort(); /* ����sort����*/
}
int space()
{
	int l = 0; PCB* pr = ready;
	while (pr != NULL)
	{
		l++;
		pr = pr->link;
	}
	return(l);
}
disp(PCB * pr) /*����������ʾ����,������ʾ��ǰ����*/
{
	printf("\n qname\t state\t ndtime\t runTime\t relativeTime\t timeSlot\n");
	printf("|%s\t", pr->name);
	printf("|%c\t", pr->state);
	printf("|%d\t", pr->needTime);
	printf("|%d\t", pr->runTime);
	printf("|%d\t", relativeTime);
	printf("|%d\t", slot);
	printf("\n");
	printf("relativeTime is the time at starting this slice");
}
check() /* �������̲鿴���� */
{
	PCB* pr;
	printf("\n **** ��ǰ�������еĽ�����:%s", p->name); /*��ʾ��ǰ���н���*/
	disp(p);
	pr = ready;
	printf("\n ****��ǰ��������״̬Ϊ:\n"); /*��ʾ��������״̬*/
	while (pr != NULL)
	{
		disp(pr);
		pr = pr->link;
	}
}
destroy() /*�������̳�������(�������н���,��������)*/
{
	printf("\n ���� [%s] �����.\n", p->name);
	free(p);
	p=NULL;
}
running() /* �������̾�������(��������ʱ�䵽,�þ���״̬*/
{
/*    if(p==NULL)sort();*/
    if(NULL){}
    else{
        if (p->runTime+slot >= p->needTime){
            relativeTime+=p->needTime-p->runTime;
            p->finishTime=relativeTime;//!!!!!!!!!!!!!
            p->runTime=p->needTime;
            destroy(); /* ����destroy����*/
        }
        else
        {
            relativeTime+=slot;
            (p->runTime)=p->runTime+slot;
            p->state = 'w';
            sort(); /*����sort����*/
        }
	}
}
main() /*������*/
{
	int len, h = 0;
	char ch;
		relativeTime=0;
		
		printf("\n ������time slice?");
		scanf("%d", &slot);
		
	input();
	len = space();
	while (doneIndex4tempArr<num4tempArr||(len != 0 && ready != NULL)|| ((p!=NULL)&&p->runTime<p->needTime) )
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

        printf("\n ����һ������......");
        ch = getchar();
	}
	printf("\n\n �����Ѿ����.\n");
	ch = getchar();
}
