    #include "stdio.h"
    #include <stdlib.h>
    #include <conio.h>
    int  relativeTime=0;//ʵʱʱ��
    struct pcb { /* ������̿��ƿ�PCB */
        char name[10];
        char state;
        int arriveTime=0;
        int finishTime;
        int needTime;
        int runTime;
        struct pcb* link= NULL;
        int belong2Index;//���ڵڼ�������
        int timeSlot;//�ü����е�ʱ��Ƭ����
    }*processorInRunning=NULL;
    typedef struct pcb PCB;
    PCB** array4PCB;
    int amount4PCBArray=0;//�ܹ���������
    int doneIndex4PCB=0;//��ǰʵʱʱ�䵽���˶��ٸ�����

    double* array4Time;//���ڼ�¼��תʱ�������
    int doneIndex4Time=0;//��¼��n����תʱ�������
    int FirstInsertIndex=-1;//��doneIndex��������ж����޷�������

    PCB** multi;//�༶�������ж�ͷָ��
    int amount4Multi=0;//�����ټ�����
        moveDown(){
        //�������һ��ʱ��Ƭ����ƶ����� 
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
        //�ҳ��׸�Ӧ��ִ�еĽ������ڶ��м���
            int index=0;
            for(;index<amount4Multi&&multi[index]->link==NULL;index++);
            return index;
        }
        PCB* extract(){
        //�Ӷ༶�����г��Ӧ��ִ�еĽ���
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
        if(nowIndex==amount4Multi){//processorInRunning==NULL&&���ڱ����߳�ռ�ݵĻ�����ִ�е��̰߳�,ʱ�仹û��
            if(doneIndex4PCB<amount4PCBArray){
            //�����м�Ŀհ�ʱ��
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
            //�ж�һ�ζ���!!
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
            //�ڵ�һ��һ��ʱ��Ƭ�м䵽��Ķ�Ҫ����
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
    /*����������ʾ����,������ʾ��ǰ����*/
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
    /* �������̲鿴���� */
    showMultiArray() {
        if(processorInRunning!=NULL){
            printf("\n **** ��ǰ�������еĽ�����:%s", processorInRunning->name); //��ʾ��ǰ���н���
            show1PCB(processorInRunning);
        }
        for(int i=0;i<amount4Multi;i++){
            printf("\n ****��������������һ��ʱ��Ƭ�򱻴�Ϻ�ľ�������%d״̬Ϊ:",i); /*��ʾ��������״̬*/
            for (PCB* pr=multi[i]->link;pr != NULL;pr = pr->link){
                show1PCB(pr);
            }
        }
        printf("\n relativeTime is the time at ending this piece!\n");
    }

    /* �������̿��ƿ麯��*/
    input() {
        printf("������amount4Multi?  ");
        scanf("%d", &amount4Multi);
        multi=(PCB**)malloc(amount4Multi*sizeof(PCB*));
        for(int i=0;i<amount4Multi;i++){
            multi[i]=(PCB*)malloc(sizeof(PCB));
            printf("������%d�����е�timeSlot  ",i);
            scanf("%d", &multi[i]->timeSlot);
            multi[i]->belong2Index=i;
            multi[i]->link=0;
        }
        printf("ǰ%d��Ϊ FCFS ,��%dΪ RR\n",amount4Multi-1,amount4Multi);
        printf("���������total number of process? ");
        scanf("%d", &amount4PCBArray);
        printf("\n");
        array4Time=(double*)malloc(amount4PCBArray*sizeof(double));
        array4PCB=(pcb**)malloc(amount4PCBArray*sizeof(pcb*));
        for (int i = 0; i<amount4PCBArray; i++){
            printf("���̺�No.%d:", i);
            processorInRunning = (PCB*)malloc(sizeof(PCB));
            processorInRunning->name[0]=(char)(i+'0');
            processorInRunning->name[1]='\0';
            printf("����arriveTime:");
            scanf("%d", &processorInRunning->arriveTime);
            printf(" �����������ʱ��in need : ");
            scanf("%d", &processorInRunning->needTime);
            printf("\n");
            processorInRunning->runTime = 0; processorInRunning->state = 'w';
            processorInRunning->link = NULL;
            processorInRunning->belong2Index=0;
            array4PCB[i]=processorInRunning;
            array4Time[i]=0;
        }
        //ð������
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
    /*�������̳�������(�������н���,��������)*/
    destroy() {
        printf("\n ���� [%s] �����.", processorInRunning->name);
        show1PCB(processorInRunning);
        array4Time[doneIndex4Time++]=(processorInRunning->finishTime - processorInRunning->arriveTime)*1.0/processorInRunning->needTime;
        printf("\n ���� [%s] �Ĵ�Ȩ��תʱ��Ϊ=%lf", processorInRunning->name,array4Time[doneIndex4Time-1]);
        free(processorInRunning);
        processorInRunning=NULL;
    }
    /* �������̾�������(��������ʱ�䵽,�þ���״̬*/
    void running() {
        int nowIndex=0;
        //��ʼ��
        if(doneIndex4PCB==0)operateReady();
        nowIndex=judgeNULL();
        if(nowIndex!=amount4Multi)processorInRunning=extract();
        if(processorInRunning==NULL){printf("processorInRunning==NULL\n");return;}
        if(0==operateReady()||processorInRunning->belong2Index==0){
            //û��������������뵫����0������
            if (processorInRunning->runTime + processorInRunning->timeSlot >= processorInRunning->needTime){
            //û����һ��ʱ��Ƭ��ִ�����
                relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
                processorInRunning->finishTime=relativeTime;//!!!!!!!!!!!!!
                processorInRunning->runTime=processorInRunning->needTime;
                destroy();
            }
            //ִ����һ��ʱ��Ƭ
            else{
                relativeTime += processorInRunning->timeSlot;
                processorInRunning->runTime = processorInRunning->runTime+ processorInRunning->timeSlot;
                processorInRunning->state = 'w';
                moveDown();
            }
            showMultiArray();
        }
        else{
            //ʱ��Ƭ�з�������
            if (processorInRunning->runTime + array4PCB[FirstInsertIndex]->arriveTime - relativeTime >= processorInRunning->needTime){
            //����֮ǰ�����
                    FirstInsertIndex++;
                relativeTime  +=  processorInRunning->needTime  -  processorInRunning->runTime;
                processorInRunning->finishTime=relativeTime;//!
                processorInRunning->runTime=processorInRunning->needTime;
                destroy();
            }
            //ִ��һ��ʱ��Ƭ��;�����,�����������ڵ�i������Ϊĳ���̷���ʱ�����½��̽�����һ���ȼ��ϸߵĶ��У���ʱ���������������еĽ��̷Żص���i���е�ĩβ
            else{
                //if(processorInRunning->belong2Index!=0)
                processorInRunning->runTime = processorInRunning->runTime+ array4PCB[FirstInsertIndex]->arriveTime - relativeTime;
                relativeTime += array4PCB[FirstInsertIndex]->arriveTime - relativeTime;
                                            FirstInsertIndex++;
                processorInRunning->state = 'w';
                // �Ż�׼��������
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
    /*������*/
    main() {
        char ch;
        relativeTime=0;
        input();
        while (doneIndex4PCB<amount4PCBArray || (judgeNULL() != amount4Multi)  ||
            ( (processorInRunning!=NULL) && processorInRunning->runTime<processorInRunning->needTime ) ){
            running();
            printf("\n ����һ������......");
            ch = getchar();
        }
        printf("\n\n �����Ѿ����.\n");
        double sum=0;
        for(int i=0;i<amount4PCBArray;i++){
            sum+=array4Time[i];
        }
        printf("\n ƽ����Ȩ��תʱ��Ϊ=%lf\n", sum/amount4PCBArray);
        ch = getchar();ch = getchar();
    }

