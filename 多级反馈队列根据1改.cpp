    #include "stdio.h"
    #include <stdlib.h>
    #include <conio.h>
    #define getpch(type) (type*)malloc(sizeof(type))
    //#define NULL 0
    int  timeSlot=0;
    int  relativeTime=0;
    struct pcb { /* ������̿��ƿ�PCB */
        char name[10];
        char state;
        int arriveTime=0;
        int finishTime;
        int needTime;
        int runTime;
        struct pcb* link= NULL;

        int timeSlot;
        int belong2Index;
    }*ready = NULL, *processorInRunning=NULL;
    typedef struct pcb PCB;
    PCB** array4PCB;
    int amount4PCBArray=0;
    int doneIndex4PCB=0;

    double* array4Time;
    int doneIndex4Time=0;

    PCB** multi;
    int amount4Multi=0;
                moveDown(){
                    PCB* son,*rear;
                    int index=processorInRunning->belong2Index;
                	son=processorInRunning;

                    if(index<amount4Multi-1){
                        
                        //son= multi[index]->link;
                        //multi[index]->link=multi[index]->link->link;

                        for(rear=multi[index+1];rear->link!=NULL;rear=rear->link);
                        rear->link=son;

                        son->belong2Index++;
                        son->timeSlot=multi[index+1]->timeSlot;
                        son->link=NULL;
                    }
                    else{
                        for(rear=multi[index];rear->link!=NULL;rear=rear->link);
                        rear->link=processorInRunning;
                        //printf("catch����!movedown���ͷ");
					}
                }
                int judgeNULL(){
                    int index=0;
                    for(;multi[index]->link==NULL;index++);
                    return index;
                }
                PCB* extract(){
                    int index=judgeNULL();
                    PCB* son=NULL;
                    if(index<amount4Multi){
                        son= multi[index]->link;
                        multi[index]->link=multi[index]->link->link;
                        //��������Ƚ�����ΪNULL;
                        son->link=NULL;
                    }
                    return son;
                }
                //�ж��жϵ�operate
        int operateReady(){
            int nowIndex=judgeNULL();
            //ȫΪ��
            if(nowIndex==amount4Multi){
                //���ڱ����߳�ռ�ݵĻ�����ִ�е��̰߳�,ʱ�仹û��
                if(processorInRunning==NULL&&doneIndex4PCB<amount4PCBArray){
                    multi[0]->link=array4PCB[doneIndex4PCB];
                    array4PCB[doneIndex4PCB]->belong2Index=0;
                    array4PCB[doneIndex4PCB]->timeSlot=multi[0]->timeSlot;
                    relativeTime=array4PCB[doneIndex4PCB]->arriveTime;
                    doneIndex4PCB++;
                    return 0;
                }
/*                nowIndex=judgeNULL();//is 0!!!!!
                PCB  *rear=multi[nowIndex]->link;
                    for(;rear!=NULL&&rear->link!=NULL;rear=rear->link);
                    //һ��ʱ��Ƭ�����м�Ķ�����
                    for(;rear!=NULL&&doneIndex4PCB<amount4PCBArray&&
                       array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+multi[nowIndex]->timeSlot;
                       doneIndex4PCB++){
                        rear->link=array4PCB[doneIndex4PCB];
                        rear=rear->link;
                    }*/
            }
            else{
                PCB  *rear=multi[0]->link;
                for(;rear!=NULL&&rear->link!=NULL;rear=rear->link);
                //�ж�һ�ζ���!!
                if(doneIndex4PCB<amount4PCBArray&&
                array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+multi[nowIndex]->timeSlot){
                            rear->link=array4PCB[doneIndex4PCB];
                            rear=rear->link;
                            array4PCB[doneIndex4PCB]->belong2Index=0;
                            array4PCB[doneIndex4PCB]->timeSlot=multi[0]->timeSlot;
                            doneIndex4PCB++;
                            return 1;
                }
                    //rear->link=processorInRunning;
            }
            return 0;
        }
/*    operateReady(){
        if(ready==NULL){
            if(processorInRunning==NULL&&doneIndex4PCB<amount4PCBArray){
                ready=array4PCB[doneIndex4PCB];
                relativeTime=array4PCB[doneIndex4PCB]->arriveTime;
                doneIndex4PCB++;
            }
            PCB  *rear=ready;
            for(;rear!=NULL&&rear->link!=NULL;rear=rear->link);
            for(;rear!=NULL&&doneIndex4PCB<amount4PCBArray&&
               array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+timeSlot;doneIndex4PCB++){
                rear->link=array4PCB[doneIndex4PCB];
                rear=rear->link;
            }
        }
        else{
            PCB *rear=ready;
            for(;rear->link!=NULL;rear=rear->link);

            for(;doneIndex4PCB<amount4PCBArray&&
            array4PCB[doneIndex4PCB]->arriveTime<=relativeTime+timeSlot;doneIndex4PCB++){
                        rear->link=array4PCB[doneIndex4PCB];
                        rear=rear->link;
            }
                rear->link=processorInRunning;
        }
    }*/
    /* �������̿��ƿ麯��*/
    input() {
        printf("���������total number? ");
        scanf("%d", &amount4PCBArray);
        printf("\n");
        array4Time=(double*)malloc(amount4PCBArray*sizeof(int));
        array4PCB=(pcb**)malloc(amount4PCBArray*sizeof(processorInRunning));
        for (int i = 0; i<amount4PCBArray; i++){
            printf("���̺�No.%d:", i);
            processorInRunning = getpch(PCB);
            processorInRunning->name[0]=(char)(i+'0');
            processorInRunning->name[1]='\0';
            printf("����arriveTime:");
            scanf("%d", &processorInRunning->arriveTime);
            printf(" �����������ʱ��in need : ");
            scanf("%d", &processorInRunning->needTime);
            printf("\n");
            processorInRunning->runTime = 0; processorInRunning->state = 'w';
            processorInRunning->link = NULL;
            array4PCB[i]=processorInRunning;
            array4Time[i]=0;
        }
        // ѡ������
        pcb* minP=array4PCB[0];pcb* tempP=array4PCB[0];
        for(int i=0,minI=i;i<amount4PCBArray;i++){
            minI=i;
            minP=array4PCB[i];
                for(int j=i;j<amount4PCBArray;j++){
                    if(array4PCB[j]->arriveTime < minP->arriveTime){
                        minI=j;
                        minP=array4PCB[j];
                        }
                    tempP=array4PCB[i];array4PCB[i]=minP;array4PCB[minI]=tempP;
                }
            }
        for(int i=0;i<amount4PCBArray;i++){
            printf("i=%d,arriveTime=%d\n",i,array4PCB[i]->arriveTime);
        }
        processorInRunning=NULL;
        operateReady(); /* ����sort����*/
    }
    /*����������ʾ����,������ʾ��ǰ����*/
    show1PCB(PCB * pr) {
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
    /* �������̲鿴���� */
    showMultiArray() {
        //PCB* pr;
        printf("\n **** ��ǰ�������еĽ�����:%s", processorInRunning->name); /*��ʾ��ǰ���н���*/
        show1PCB(processorInRunning);
        printf("\n relativeTime is the time at starting this slice!");
        //pr = ready;
        for(int i=0;i<amount4Multi;i++){
            printf("\n ****��ǰ��������%d״̬Ϊ:\n",i); /*��ʾ��������״̬*/
            for (PCB* pr=multi[i]->link;pr != NULL;pr = pr->link){
                show1PCB(pr);
            }
        }
        printf("\n relativeTime is the time at starting this slice!\n");
    }

    /*�������̳�������(�������н���,��������)*/
    destroy() {
        printf("\n ���� [%s] �����.", processorInRunning->name);
        //show1PCB(processorInRunning);
        array4Time[doneIndex4Time++]=(processorInRunning->finishTime - processorInRunning->arriveTime)*1.0/processorInRunning->needTime;
        printf("\n ���� [%s] �Ĵ�Ȩ��תʱ��Ϊ=%lf", processorInRunning->name,array4Time[doneIndex4Time-1]);
        free(processorInRunning);
        processorInRunning=NULL;
    }
    /* �������̾�������(��������ʱ�䵽,�þ���״̬*/
/*    running() {
    *//*    if(processorInRunning==NULL)operateReady();*//*
        if (processorInRunning->runTime+timeSlot >= processorInRunning->needTime){
            relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
            processorInRunning->finishTime=relativeTime;//!!!!!!!!!!!!!
            processorInRunning->runTime=processorInRunning->needTime;
            destroy(); *//* ����destroy����*//*
        }
        else{
            relativeTime+=timeSlot;
            processorInRunning->runTime = processorInRunning->runTime+timeSlot;
            processorInRunning->state = 'w';
            operateReady(); *//*����sort����*//*
        }
    }*/
        void running() {
        /*    if(processorInRunning==NULL)operateReady();*/
            //int nowIndex=0;
            if(0==operateReady()){
               //operateReady();
                //nowIndex=judgeNULL();
                processorInRunning=extract();
                if(processorInRunning==NULL)return;
                //û����һ��ʱ��Ƭ��ִ�����
                if (processorInRunning->runTime+processorInRunning->timeSlot >= processorInRunning->needTime){
                    relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
                    processorInRunning->finishTime=relativeTime;//!!!!!!!!!!!!!
                    processorInRunning->runTime=processorInRunning->needTime;
                    destroy(); //* ����destroy����*//*
                }
                //ִ����һ��ʱ��Ƭ
                else{
                    relativeTime+=processorInRunning->timeSlot;
                    processorInRunning->runTime = processorInRunning->runTime+ processorInRunning->timeSlot;
                    processorInRunning->state = 'w';
                    moveDown(); //*����sort����*//*
                }
            }
            else{
                //һֱ����
                do{
                    //nowIndex=judgeNULL();
                    processorInRunning=extract();
                    if(processorInRunning==NULL)return;
                    //û����һ��ʱ��Ƭ��ִ�����
                    if (processorInRunning->runTime+array4PCB[doneIndex4PCB-1]->arriveTime >= processorInRunning->needTime){
                        relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
                        processorInRunning->finishTime=relativeTime;//!!!!!!!!!!!!!
                        processorInRunning->runTime=processorInRunning->needTime;
                        destroy(); //* ����destroy����*//*
                    }
                    //ִ����һ�������
                    else{
                        if(processorInRunning->belong2Index!=0){
                            relativeTime+=array4PCB[doneIndex4PCB-1]->arriveTime;
                            processorInRunning->runTime = processorInRunning->runTime+ array4PCB[doneIndex4PCB-1]->arriveTime ;
                            processorInRunning->state = 'w';
                        }
                        else{
                            relativeTime+=processorInRunning->timeSlot;
                            processorInRunning->runTime = processorInRunning->runTime+ processorInRunning->timeSlot;
                            processorInRunning->state = 'w';
                            moveDown(); //*����sort����*//*
                        }
                    }
                }while(1==operateReady());
            }
/*            if(judgeNULL()==amount4Multi){}
            }*/
        }
    /*������*/
    main() {
        char ch;
/*        int lengthOfReady, h = 0;
            printf("������time slice?  ");
            scanf("%d", &timeSlot);*/
        relativeTime=0;
                printf("������amount4Multi?  ");
                scanf("%d", &amount4Multi);
                multi=(PCB**)malloc(amount4Multi*sizeof(PCB*));
                for(int i=0;i<amount4Multi;i++){
                    printf("������%d�����е�timeSlot  ",i);
                    scanf("%d", &multi[i]->timeSlot);
                    multi[i]->belong2Index=i;
                    multi[i]->link=0;
                }
                printf("ǰ%d��Ϊ FCFS ,��%dΪ RR\n",amount4Multi-1,amount4Multi);
        input();
        while (doneIndex4PCB<amount4PCBArray || (judgeNULL() != amount4Multi)  ||
            ( (processorInRunning!=NULL) && processorInRunning->runTime<processorInRunning->needTime ) ){
            ch = getchar();
/*           if(ready!=NULL){
                processorInRunning = ready;
                ready = processorInRunning->link;*/
            if(judgeNULL() != amount4Multi){
                processorInRunning = multi[judgeNULL()]->link;
                //ready = processorInRunning->link;
                processorInRunning->link = NULL;
                processorInRunning->state = 'R';
                showMultiArray();
                running();
            }
            else{
                if(processorInRunning!=NULL){
                    processorInRunning->state = 'R';
                    showMultiArray();
                    running();
                }
                showMultiArray();
                operateReady();
            }
            printf("\n ����һ������......");
            ch = getchar();
        }
        printf("\n\n �����Ѿ����.\n");
        double sum=0;
        for(int i=0;i<amount4PCBArray;i++){
            sum+=array4Time[i];
        }
        printf("\n ƽ����Ȩ��תʱ��Ϊ=%lf\n", sum/amount4PCBArray);
        ch = getchar();
        ch = getchar();
    }

