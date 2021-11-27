    #include <stdio.h>
    #include <stdlib.h>
    #define MAXSIZE 85
    int pageFaultCount = 0; //ȱҳ����
    int instructionArray[MAXSIZE]; //ָ���ַ������
    int pageArray[MAXSIZE]; //ҳ��ַ������

    typedef struct Block{
        int blockID; //����������
        int pageID; //����װ�������ҳ��,��ʼֵΪ-1
        int firstInIndex;//�״ε����ڴ��ָ��index,��ʼֵΪMAXSIZE+1
        int leastRecentlyUsedIndex;//���һ��ʹ�õ�ָ��index��ʼֵΪ-99
        struct Block *next;
    } Block;

    int blockNumber=3;
    Block** blockList;
    void print(int pageID, int instructionID, int NO,Block *pointer,char* tag){
        printf("%d\t", NO);
        printf("%d\t", instructionID);
        printf("%d\t", pageID);
        printf("%d\t", pointer->blockID);
        printf("%s\t",tag);
        for(int i=0;i<blockNumber;i++){
            printf("%d\t",blockList[i]->pageID);
        }
        printf("\n");
    }
    //û�����û���ͨ�ô����,ʵ�ֹ��ܾۺϵĴ��븴��
    int commonPack(int pageID, int instructionID, int NO,Block *pointer){
        for(int i = 0; i < blockNumber; i++) {
            //��Ϊ����
            if(pointer->pageID == -1) {
                pointer->pageID = pageID;
                pageFaultCount++; //ȱҳ����+1
                print(pageID,instructionID,NO,pointer,"T");
                pointer->firstInIndex=NO;
                pointer->leastRecentlyUsedIndex=NO;
                return 1;
            }
            if(pointer->pageID == pageID){
                print(pageID,instructionID,NO,pointer,"F");
                pointer->leastRecentlyUsedIndex=NO;
                return 1;
            }
            pointer = pointer->next;
        }
        return 0;
    }
    void initializeInstruction() {
        //������ĿҪ���ʼ��ָ������
    	blockList=(Block**)malloc(blockNumber*sizeof(Block*));
        for(int i=0;i<blockNumber;i++){
            //��������Ŀ��������ڴ������
            blockList[i]=(Block*)malloc(sizeof(Block));
            blockList[i]->pageID = -1;
            blockList[i]->blockID = i;
            blockList[i]->firstInIndex = MAXSIZE+1;
            blockList[i]->leastRecentlyUsedIndex = MAXSIZE+1;
        }
        for(int i=0;i<blockNumber;i++){
            //���ӳɵ���ѭ������
            blockList[i]->next=blockList[(i+1)%blockNumber];
        }
        for(int i = 0; i < MAXSIZE; ) {
        //��ʼ����ַ��//ָ��������ظ�,0��MAXSIZE��������,27Ҳ����������
            int m = rand() % MAXSIZE;
            instructionArray[i] = m + 1;
            pageArray[i] = instructionArray[i] / 10;
            i++;

            int m1 = rand() % (m - 1);
            instructionArray[i] = m1;
            pageArray[i] = m1 / 10;
            i++;

            instructionArray[i] = m1 + 1;
            pageArray[i] = instructionArray[i] / 10;
            i++;

            int m2 = rand() % (319 - m1 - 1) + m1 + 2;
            instructionArray[i] = m2;
            pageArray[i] = m2 / 10;
            i++;

            instructionArray[i] = m2 + 1;
            pageArray[i] = instructionArray[i] / 10;
            i++;
        }
    }

    //NOΪ������ҳ�ڵ�ַ���е�λ��
    int Optimal(int pageID, int instructionID, int NO) {
        Block *pointer = blockList[0];
        if(1==commonPack(pageID,instructionID,NO,pointer))return 1;

        //OPTҳ���û�
        int allBlockPageNum[blockNumber]; //��¼��װ���ڴ��ҳ��ַ
        for(int i = 0; i < blockNumber; i++){
            allBlockPageNum[i] = pointer->pageID;
            pointer = pointer->next;
        }
        int nextAddr[blockNumber];
        for(int i=0;i<blockNumber;i++){
            nextAddr[i]=MAXSIZE+1;
        }
        for(int i = 0; i < blockNumber; i++){
            for(int j = NO; j < MAXSIZE; j++){
                if(allBlockPageNum[i] == pageArray[j]){
                    //�ҵ���һ��λ�ü�ֹͣ
                    nextAddr[i] = j;
                    break;
                }
            }
        }
        int tempPageID = 0; //ҳ��ַ
        int blockNO; //�ڴ��ĵ�ַ
        //ѡ��������Զ��ҳ��ַ���ڴ���ж�Ӧ��λ��
        for(int i = 0; i < blockNumber; i++) {
            if(nextAddr[i] > tempPageID){
                tempPageID = nextAddr[i];
                blockNO = i;
            }
        }
        for(int i = 0; i < blockNumber; i++){
            if(pointer->blockID == blockNO){
                pointer->pageID = pageID;
                pageFaultCount++;
                print(pageID,instructionID,NO,pointer,"T&R");
            }
            pointer = pointer->next;
        }
        return 1;
    }
    int FIFO(int pageID, int instructionID, int NO){
        Block *pointer = blockList[0];
        if(1==commonPack(pageID,instructionID,NO,pointer))return 1;
        int minFIndex=MAXSIZE+1;
        for(int i=0;i<blockNumber;pointer=pointer->next,i++){
            minFIndex=minFIndex<pointer->firstInIndex?minFIndex:pointer->firstInIndex;
        }
        for(int i=0;i<blockNumber;pointer=pointer->next,i++){
            if(pointer->firstInIndex==minFIndex){
                pointer->pageID = pageID;
                pointer->firstInIndex=NO;
                pointer->leastRecentlyUsedIndex=NO;
                pageFaultCount++;
                print(pageID,instructionID,NO,pointer,"T&R");
            }
        }
        return 1;
    }
    int LRU(int pageID, int instructionID, int NO){
        Block *pointer = blockList[0];
        if(1==commonPack(pageID,instructionID,NO,pointer))return 1;
        int minLIndex=MAXSIZE+1;
        for(int i=0;i<blockNumber;pointer=pointer->next,i++){
            minLIndex=minLIndex<pointer->leastRecentlyUsedIndex?minLIndex:pointer->leastRecentlyUsedIndex;
        }
        for(int i=0;i<blockNumber;pointer=pointer->next,i++){
            if(pointer->leastRecentlyUsedIndex==minLIndex){
                pointer->pageID = pageID;
                pointer->firstInIndex=NO;
                pointer->leastRecentlyUsedIndex=NO;
                pageFaultCount++;
                print(pageID,instructionID,NO,pointer,"T&R");
            }
        }
        return 1;
    }

    void calculatePageFaultCount() {
        //����ȱҳ��
        char trigger[]={'\0','\0'};char* to0;
        do{
            fgets(trigger,2,stdin);
            to0=NULL;to0 = strchr(trigger, '\n');if(to0)*to0 = '\0';  fflush(stdin);
        }while(trigger[0]<'0'||trigger[0]>'3');

        printf("T is true,F is false,R is replace\n");
        printf("NO.\tָ���\tat�߼�\tat����\tȱҳ��\t");
        for(int i=0;i<blockNumber;i++){
            printf("�ڴ��%d\t",i);
        }
        printf("\n");

        if(trigger[0]-'0'==1){
            for(int i = 0; i < MAXSIZE; i++){
                Optimal(pageArray[i], instructionArray[i], i);
            }
        }
        else if(trigger[0]-'0'==2){
            for(int i = 0; i < MAXSIZE; i++){
                FIFO(pageArray[i], instructionArray[i], i);
            }
        }
        else{
            for(int i = 0; i < MAXSIZE; i++){
                LRU(pageArray[i], instructionArray[i], i);
            }
        }
        printf("\n");
        printf("ȱҳ������%d\n", pageFaultCount);
        printf("ȱҳ��Ϊ��%.4f \n", pageFaultCount*1.0 / MAXSIZE);
    }

    int main(){
        printf("�������ڴ������!\n");
        scanf("%d",&blockNumber);
        printf("----------����û��㷨(OPT)�밴1----------\n");
        printf("----------�Ƚ��ȳ��㷨(FIFO)�밴2----------\n");
        printf("----------������δʹ���㷨(LRU)�밴3----------\n");

        initializeInstruction();
        calculatePageFaultCount();
        return 0;
    }
