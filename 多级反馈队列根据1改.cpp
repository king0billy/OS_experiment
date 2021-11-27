    #include "stdio.h"
    #include <stdlib.h>
    #include <conio.h>
    int  relativeTime=0;//实时时间
    struct pcb { /* 定义进程控制块PCB */
        char name[10];
        char state;
        int arriveTime=0;
        int finishTime;
        int needTime;
        int runTime;
        struct pcb* link= NULL;
        int belong2Index;//属于第几级队列
        int timeSlot;//该级队列的时间片长度
    }*processorInRunning=NULL;
    typedef struct pcb PCB;
    PCB** array4PCB;
    int amount4PCBArray=0;//总共几个进程
    int doneIndex4PCB=0;//当前实时时间到达了多少个进程

    double* array4Time;//用于记录周转时间的数组
    int doneIndex4Time=0;//记录第n个周转时间的索引
    int FirstInsertIndex=-1;//和doneIndex配合用于判断有无发生插入

    PCB** multi;//多级反馈队列队头指针
    int amount4Multi=0;//共多少级队列
        moveDown(){
        //进程完成一个时间片后的移动函数 
            PCB* son,*rear;
            int index=processorInRunning->belong2Index;
            son=processorInRunning;
            if(index<amount4Multi-1){
                for(rear=multi[index+1];rear->link!=NULL;rear=rear->link);
                rear->link=son;
                son->belong2Index++;
                son->timeSlot=multi[index+1]->timeSlot;
                son->link=NULL;
            }
            else{
                for(rear=multi[index];rear->link!=NULL;rear=rear->link);
                rear->link=processorInRunning;
            }
        }
        int judgeNULL(){
        //找出首个应该执行的进程所在队列级别
            int index=0;
            for(;index<amount4Multi&&multi[index]->link==NULL;index++);
            return index;
        }
        PCB* extract(){
        //从多级队列中抽出应该执行的进程
            int index=judgeNULL();
            PCB* son=NULL;
            if(index<amount4Multi){
                son= multi[index]->link;
                multi[index]->link=multi[index]->link->link;
                son->link=NULL;
                son->state='R';
            }
            return son;
        }
    int operateReady(){
        int nowIndex=amount4Multi;
        if(processorInRunning!=NULL)nowIndex=processorInRunning->belong2Index;
        PCB  *rear=multi[0];
        if(nowIndex==amount4Multi){//processorInRunning==NULL&&现在被单线程占据的话就先执行单线程吧,时间还没到
            if(doneIndex4PCB<amount4PCBArray){
            //跳过中间的空白时间
                multi[0]->link=array4PCB[doneIndex4PCB];
                array4PCB[doneIndex4PCB]->belong2Index=0;
                array4PCB[doneIndex4PCB]->timeSlot=multi[0]->timeSlot;
                relativeTime=array4PCB[doneIndex4PCB]->arriveTime;
                doneIndex4PCB++;
                FirstInsertIndex=doneIndex4PCB;
            }
            nowIndex=0;
            for(;rear!=NULL&&rear->link!=NULL;rear=rear->link);
        }
        else{
            //中断一次而已!!
            for(;rear!=NULL&&rear->link!=NULL;rear=rear->link);
            if(nowIndex!=0){
                if(doneIndex4PCB<amount4PCBArray&&
                array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+multi[nowIndex]->timeSlot){
                        FirstInsertIndex=doneIndex4PCB;
                    rear->link=array4PCB[doneIndex4PCB];
                    rear=rear->link;
                    array4PCB[doneIndex4PCB]->belong2Index=0;
                    array4PCB[doneIndex4PCB]->timeSlot=multi[0]->timeSlot;
                    doneIndex4PCB++;
                    if(doneIndex4PCB>FirstInsertIndex)return 1;
                }
            }
        }
        if(nowIndex==0){
            //在第一队一个时间片中间到达的都要插入
            if(doneIndex4PCB<amount4PCBArray&&
            array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+multi[nowIndex]->timeSlot){
                FirstInsertIndex=doneIndex4PCB;
            }
            for(;doneIndex4PCB<amount4PCBArray&&
            array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+multi[nowIndex]->timeSlot;doneIndex4PCB++){
                rear->link=array4PCB[doneIndex4PCB];
                rear=rear->link;
                array4PCB[doneIndex4PCB]->belong2Index=0;
                array4PCB[doneIndex4PCB]->timeSlot=multi[0]->timeSlot;
            }
            rear->link=NULL;//todo why this is important
            if(doneIndex4PCB>FirstInsertIndex)return 1;
        }
        return 0;
    }
    /*建立进程显示函数,用于显示当前进程*/
    void show1PCB(PCB * pr) {
        if(pr==NULL)return;
        printf("\n qname\t ndtime\t runTime\t arriveTime\t finishTime\t relativeTime\t timeSlot\n");
        printf("|%s\t", pr->name);
        //printf("|%c\t", pr->state);
        printf("|%d\t", pr->needTime);
        printf("|%d\t", pr->runTime);
        printf("\t|%d\t", pr->arriveTime);
        printf("\t|%d\t", pr->finishTime);
        printf("\t|%d\t", relativeTime);
        printf("\t|%d\t", pr->timeSlot);
    }
    /* 建立进程查看函数 */
    showMultiArray() {
        if(processorInRunning!=NULL){
            printf("\n **** 当前正在运行的进程是:%s", processorInRunning->name); //显示当前运行进程
            show1PCB(processorInRunning);
        }
        for(int i=0;i<amount4Multi;i++){
            printf("\n ****上述进程运行完一个时间片或被打断后的就绪队列%d状态为:",i); /*显示就绪队列状态*/
            for (PCB* pr=multi[i]->link;pr != NULL;pr = pr->link){
                show1PCB(pr);
            }
        }
        printf("\n relativeTime is the time at ending this piece!\n");
    }

    /* 建立进程控制块函数*/
    input() {
        printf("请输入amount4Multi?  ");
        scanf("%d", &amount4Multi);
        multi=(PCB**)malloc(amount4Multi*sizeof(PCB*));
        for(int i=0;i<amount4Multi;i++){
            multi[i]=(PCB*)malloc(sizeof(PCB));
            printf("请输入%d级队列的timeSlot  ",i);
            scanf("%d", &multi[i]->timeSlot);
            multi[i]->belong2Index=i;
            multi[i]->link=0;
        }
        printf("前%d均为 FCFS ,第%d为 RR\n",amount4Multi-1,amount4Multi);
        printf("请输入进程total number of process? ");
        scanf("%d", &amount4PCBArray);
        printf("\n");
        array4Time=(double*)malloc(amount4PCBArray*sizeof(double));
        array4PCB=(pcb**)malloc(amount4PCBArray*sizeof(pcb*));
        for (int i = 0; i<amount4PCBArray; i++){
            printf("进程号No.%d:", i);
            processorInRunning = (PCB*)malloc(sizeof(PCB));
            processorInRunning->name[0]=(char)(i+'0');
            processorInRunning->name[1]='\0';
            printf("输入arriveTime:");
            scanf("%d", &processorInRunning->arriveTime);
            printf(" 输入进程运行时间in need : ");
            scanf("%d", &processorInRunning->needTime);
            printf("\n");
            processorInRunning->runTime = 0; processorInRunning->state = 'w';
            processorInRunning->link = NULL;
            processorInRunning->belong2Index=0;
            array4PCB[i]=processorInRunning;
            array4Time[i]=0;
        }
        //冒泡排序
        pcb* tempP=array4PCB[0];
        int j;
        for(int i=0;i<amount4PCBArray-1;i++){
            for( j=0;j<amount4PCBArray-i-1;j++){
                if(array4PCB[j]->arriveTime > array4PCB[j+1]->arriveTime){
                    tempP=array4PCB[j];array4PCB[j]=array4PCB[j+1];array4PCB[j+1]=tempP;
                }
            }
        }
        processorInRunning=NULL;
    }
    /*建立进程撤消函数(进程运行结束,撤消进程)*/
    destroy() {
        printf("\n 进程 [%s] 已完成.", processorInRunning->name);
        show1PCB(processorInRunning);
        array4Time[doneIndex4Time++]=(processorInRunning->finishTime - processorInRunning->arriveTime)*1.0/processorInRunning->needTime;
        printf("\n 进程 [%s] 的带权周转时间为=%lf", processorInRunning->name,array4Time[doneIndex4Time-1]);
        free(processorInRunning);
        processorInRunning=NULL;
    }
    /* 建立进程就绪函数(进程运行时间到,置就绪状态*/
    void running() {
        int nowIndex=0;
        //初始化
        if(doneIndex4PCB==0)operateReady();
        nowIndex=judgeNULL();
        if(nowIndex!=amount4Multi)processorInRunning=extract();
        if(processorInRunning==NULL){printf("processorInRunning==NULL\n");return;}
        if(0==operateReady()||processorInRunning->belong2Index==0){
            //没发生插入或发生插入但是是0级队列
            if (processorInRunning->runTime + processorInRunning->timeSlot >= processorInRunning->needTime){
            //没用完一个时间片就执行完毕
                relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
                processorInRunning->finishTime=relativeTime;//!!!!!!!!!!!!!
                processorInRunning->runTime=processorInRunning->needTime;
                destroy();
            }
            //执行完一个时间片
            else{
                relativeTime += processorInRunning->timeSlot;
                processorInRunning->runTime = processorInRunning->runTime+ processorInRunning->timeSlot;
                processorInRunning->state = 'w';
                moveDown();
            }
            showMultiArray();
        }
        else{
            //时间片中发生插入
            if (processorInRunning->runTime + array4PCB[FirstInsertIndex]->arriveTime - relativeTime >= processorInRunning->needTime){
            //插入之前已完成
                    FirstInsertIndex++;
                relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
                processorInRunning->finishTime=relativeTime;//!
                processorInRunning->runTime=processorInRunning->needTime;
                destroy();
            }
            //执行一个时间片中途被打断,如果处理机正在第i队列中为某进程服务时又有新进程进入任一优先级较高的队列，此时须立即把正在运行的进程放回到第i队列的末尾
            else{
                //if(processorInRunning->belong2Index!=0)
                processorInRunning->runTime = processorInRunning->runTime+ array4PCB[FirstInsertIndex]->arriveTime - relativeTime;
                relativeTime += array4PCB[FirstInsertIndex]->arriveTime - relativeTime;
                                            FirstInsertIndex++;
                processorInRunning->state = 'w';
                // 放回准备队列里
                PCB*temp;
                for(temp=multi[processorInRunning->belong2Index];temp->link!=NULL;temp=temp->link);
                processorInRunning->link=NULL;
                temp->link=processorInRunning;
                temp->link->link=NULL;//todo why
            }
            showMultiArray();
        }
        return;
    }
    /*主函数*/
    main() {
        char ch;
        relativeTime=0;
        input();
        while (doneIndex4PCB<amount4PCBArray || (judgeNULL() != amount4Multi)  ||
            ( (processorInRunning!=NULL) && processorInRunning->runTime<processorInRunning->needTime ) ){
            running();
            printf("\n 按任一键继续......");
            ch = getchar();
        }
        printf("\n\n 进程已经完成.\n");
        double sum=0;
        for(int i=0;i<amount4PCBArray;i++){
            sum+=array4Time[i];
        }
        printf("\n 平均带权周转时间为=%lf\n", sum/amount4PCBArray);
        ch = getchar();ch = getchar();
    }

