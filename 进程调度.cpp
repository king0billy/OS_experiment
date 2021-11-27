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
        struct pcb* link;
    }*ready = NULL, *processorInRunning=NULL;
    typedef struct pcb PCB;
    PCB** Array4PCB;
    double* Array4Time;
    int doneIndex4Time=0;
    int doneIndex4PCB=0;
    int number4Array=0;
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
    /* �������̿��ƿ麯��*/
    input() {
        printf("���������total number? ");
        scanf("%d", &number4Array);
        printf("\n");
        Array4Time=(double*)malloc(number4Array*sizeof(int));
        Array4PCB=(pcb**)malloc(number4Array*sizeof(processorInRunning));
        for (int i = 0; i<number4Array; i++){
            printf("���̺�No.%d:", i);
            processorInRunning = getpch(PCB);
    /*		printf("\n ���������:");
            scanf("%s", processorInRunning->name);*/
            processorInRunning->name[0]=(char)(i+'0');
            processorInRunning->name[1]='\0';
    //		printf("\n �������������:");
    //		scanf("%d", &processorInRunning->super);
            printf("����arriveTime:");
            scanf("%d", &processorInRunning->arriveTime);
            printf(" �����������ʱ��in need : ");
            scanf("%d", &processorInRunning->needTime);
            printf("\n");
            processorInRunning->runTime = 0; processorInRunning->state = 'w';
            processorInRunning->link = NULL;
            Array4PCB[i]=processorInRunning;
            Array4Time[i]=0;
        }
        // ѡ������
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
        processorInRunning=NULL;
        operateReady();
    }
    int space(){
        int length = 0; PCB* pr = ready;
        while (pr != NULL){
            length++;
            pr = pr->link;
        }
        return(length);
    }
    /*����������ʾ����,������ʾ��ǰ����*/
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
    /* �������̲鿴���� */
    check() {
        PCB* pr;
        printf("\n **** ��ǰ�������еĽ�����:%s", processorInRunning->name); /*��ʾ��ǰ���н���*/
        disp(processorInRunning);
        printf("\n relativeTime is the time at starting this slice!");
        pr = ready;
        printf("\n ****��ǰ��������״̬Ϊ:\n"); /*��ʾ��������״̬*/
        while (pr != NULL){
            disp(pr);
            pr = pr->link;
        }
        printf("\n relativeTime is the time at starting this slice!\n");
    }
    /*�������̳�������(�������н���,��������)*/
    destroy() {
        printf("\n ���� [%s] �����.", processorInRunning->name);
        //disp(processorInRunning);
        Array4Time[doneIndex4Time++]=(processorInRunning->finishTime - processorInRunning->arriveTime)*1.0/processorInRunning->needTime;
        printf("\n ���� [%s] �Ĵ�Ȩ��תʱ��Ϊ=%lf", processorInRunning->name,Array4Time[doneIndex4Time-1]);
        free(processorInRunning);
        processorInRunning=NULL;
    }
    /* �������̾�������(��������ʱ�䵽,�þ���״̬*/
    running() {
        if (processorInRunning->runTime+timeSlot >= processorInRunning->needTime){
            relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
            processorInRunning->finishTime=relativeTime;//!!!!!!!!!!!!!
            processorInRunning->runTime=processorInRunning->needTime;
            destroy();
        }
        else{
            relativeTime+=timeSlot;
            processorInRunning->runTime = processorInRunning->runTime+timeSlot;
            processorInRunning->state = 'w';
            operateReady();
        }
    }
    /*������*/
    main() {
        int lengthOfReady, h = 0;
        char ch;
            relativeTime=0;
            printf("������time slice? ");
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
            printf("\n ����һ������......");
            ch = getchar();
        }
        printf("\n\n �����Ѿ����.\n");
        double sum=0;
        for(int i=0;i<number4Array;i++){
            sum+=Array4Time[i];
        }
        printf("\n ƽ����Ȩ��תʱ��Ϊ=%lf\n", sum/number4Array);
        ch = getchar();
        ch = getchar();
    }
