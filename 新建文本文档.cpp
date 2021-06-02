    #include "stdio.h"
    #include <stdlib.h>
    #include <conio.h>
    #define getpch(type) (type*)malloc(sizeof(type))
    //#define NULL 0
    int  timeSlot=0;
    int  relativeTime=0;
    struct pcb { /* 定义进程控制块PCB */
        char name[10];
        char state;
        int arriveTime=0;
        int finishTime;
        int needTime;
        int runTime;
        struct pcb* link;
    }*ready = NULL, *processorInRunning=NULL;
    typedef struct pcb PCB;
    PCB** Array4PCB;
    double* Array4Time;
    int doneIndex4Time=0;
    int doneIndex4PCB=0;
    int number4Array=0;
    //operateReady() /* 建立对进程进行优先级排列函数*/
    //{
    //
    //	PCB *first, *rear;
    //
    //	if (ready == NULL)
    //	{
    //		processorInRunning->link = ready;
    //		ready = processorInRunning;
    //	}
    //	else /* 进程比较优先级,插入适当的位置中*/
    //	{
    //		first=ready;
    //		for(rear=ready;rear->link!=NULL;rear=rear->link);
    //		processorInRunning->link=NULL;
    //		rear->link=processorInRunning;
    //		first=first->link;
    //	}
    //
    //}
    operateReady(){
        if(ready==NULL){
            if(processorInRunning==NULL&&doneIndex4PCB<number4Array){
                ready=Array4PCB[doneIndex4PCB];
                relativeTime=Array4PCB[doneIndex4PCB]->arriveTime;
                doneIndex4PCB++;
            }
            PCB  *rear=ready;
            for(;rear!=NULL&&rear->link!=NULL;rear=rear->link);
            for(;rear!=NULL&&doneIndex4PCB<number4Array&&
               Array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+timeSlot;doneIndex4PCB++){
                rear->link=Array4PCB[doneIndex4PCB];
                rear=rear->link;
            }
        }
        else{
            PCB *rear=ready;
            for(;rear->link!=NULL;rear=rear->link);

            for(;doneIndex4PCB<number4Array&&
            Array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+timeSlot;doneIndex4PCB++){
                        rear->link=Array4PCB[doneIndex4PCB];
                        rear=rear->link;
            }
                rear->link=processorInRunning;
        }
    }
    /* 建立进程控制块函数*/
    input() {
        //system("cls");
        //clrscr(); /*清屏*/
        printf("请输入进程total number? ");
        scanf("%d", &number4Array);
        printf("\n");
        Array4Time=(double*)malloc(number4Array*sizeof(int));
        Array4PCB=(pcb**)malloc(number4Array*sizeof(processorInRunning));
        for (int i = 0; i<number4Array; i++){
            printf("进程号No.%d:", i);
            processorInRunning = getpch(PCB);
    /*		printf("\n 输入进程名:");
            scanf("%s", processorInRunning->name);*/
            processorInRunning->name[0]=(char)(i+'0');
            processorInRunning->name[1]='\0';
    //		printf("\n 输入进程优先数:");
    //		scanf("%d", &processorInRunning->super);
            printf("输入arriveTime:");
            scanf("%d", &processorInRunning->arriveTime);
            printf(" 输入进程运行时间in need : ");
            scanf("%d", &processorInRunning->needTime);
            printf("\n");
            processorInRunning->runTime = 0; processorInRunning->state = 'w';
            processorInRunning->link = NULL;
            Array4PCB[i]=processorInRunning;
            Array4Time[i]=0;
        }
        // 选择排序
        pcb* minP=Array4PCB[0];pcb* tempP=Array4PCB[0];
        for(int i=0,minI=i;i<number4Array;i++){
            minI=i;
            minP=Array4PCB[i];
                for(int j=i;j<number4Array;j++){
                    if(Array4PCB[j]->arriveTime < minP->arriveTime){
                        minI=j;
                        minP=Array4PCB[j];
                        }
                    tempP=Array4PCB[i];Array4PCB[i]=minP;Array4PCB[minI]=tempP;
                }
            }
        for(int i=0;i<number4Array;i++){
            printf("i=%d,arriveTime=%d\n",i,Array4PCB[i]->arriveTime);
        }
        processorInRunning=NULL;
        operateReady(); /* 调用sort函数*/
    }
    int space(){
        int length = 0; PCB* pr = ready;
        while (pr != NULL){
            length++;
            pr = pr->link;
        }
        return(length);
    }
    /*建立进程显示函数,用于显示当前进程*/
    disp(PCB * pr) {
        printf("\n qname\t state\t ndtime\t runTime\t arriveTime\t finishTime\t relativeTime\t timeSlot\n");
        printf("|%s\t", pr->name);
        printf("|%c\t", pr->state);
        printf("|%d\t", pr->needTime);
        printf("|%d\t", pr->runTime);
        printf("\t|%d\t", pr->arriveTime);
        printf("\t|%d\t", pr->finishTime);
        printf("\t|%d\t", relativeTime);
        printf("\t|%d\t", timeSlot);
    }
    /* 建立进程查看函数 */
    check() {
        PCB* pr;
        printf("\n **** 当前正在运行的进程是:%s", processorInRunning->name); /*显示当前运行进程*/
        disp(processorInRunning);
        printf("\n relativeTime is the time at starting this slice!");
        pr = ready;
        printf("\n ****当前就绪队列状态为:\n"); /*显示就绪队列状态*/
        while (pr != NULL){
            disp(pr);
            pr = pr->link;
        }
        printf("\n relativeTime is the time at starting this slice!\n");
    }
    /*建立进程撤消函数(进程运行结束,撤消进程)*/
    destroy() {
        printf("\n 进程 [%s] 已完成.", processorInRunning->name);
        //disp(processorInRunning);
        Array4Time[doneIndex4Time++]=(processorInRunning->finishTime - processorInRunning->arriveTime)*1.0/processorInRunning->needTime;
        printf("\n 进程 [%s] 的带权周转时间为=%lf", processorInRunning->name,Array4Time[doneIndex4Time-1]);
        free(processorInRunning);
        processorInRunning=NULL;
    }
    /* 建立进程就绪函数(进程运行时间到,置就绪状态*/
    running() {
    /*    if(processorInRunning==NULL)operateReady();*/
        if (processorInRunning->runTime+timeSlot >= processorInRunning->needTime){
            relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
            processorInRunning->finishTime=relativeTime;//!!!!!!!!!!!!!
            processorInRunning->runTime=processorInRunning->needTime;
            destroy(); /* 调用destroy函数*/
        }
        else{
            relativeTime+=timeSlot;
            processorInRunning->runTime = processorInRunning->runTime+timeSlot;
            processorInRunning->state = 'w';
            operateReady(); /*调用sort函数*/
        }

    }
    /*主函数*/
    main() {
        int lengthOfReady, h = 0;
        char ch;
            relativeTime=0;
            printf("请输入time slice? ");
            scanf("%d", &timeSlot);
        input();
        lengthOfReady = space();
        while (doneIndex4PCB<number4Array || (lengthOfReady != 0 && ready != NULL)  ||
            ( (processorInRunning!=NULL) && processorInRunning->runTime<processorInRunning->needTime ) ){
            ch = getchar();
            h++;
            printf("\n The execute number:%d ", h);
            if(ready!=NULL){
                processorInRunning = ready;
                ready = processorInRunning->link;
                processorInRunning->link = NULL;
                processorInRunning->state = 'R';
                check();
                running();
            }
            else{
                if(processorInRunning!=NULL){
                    processorInRunning->state = 'R';
                    check();
                    running();
                }
                check();
                operateReady();
            }
            printf("\n 按任一键继续......");
            ch = getchar();
        }
        printf("\n\n 进程已经完成.\n");
        double sum=0;
        for(int i=0;i<number4Array;i++){
            sum+=Array4Time[i];
        }
        printf("\n 平均带权周转时间为=%lf\n", sum/number4Array);
        ch = getchar();
        ch = getchar();
    }
