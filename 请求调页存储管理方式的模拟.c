    #include <stdio.h>
    #include <stdlib.h>
    #define MAXSIZE 85
    int pageFaultCount = 0; //缺页次数
    int instructionArray[MAXSIZE]; //指令地址流数组
    int pageArray[MAXSIZE]; //页地址流数组

    typedef struct Block{
        int blockID; //主存物理块号
        int pageID; //块中装进的虚存页号,初始值为-1
        int firstInIndex;//首次调入内存的指令index,初始值为MAXSIZE+1
        int leastRecentlyUsedIndex;//最后一次使用的指令index初始值为-99
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
    //没发生置换的通用代码块,实现功能聚合的代码复用
    int commonPack(int pageID, int instructionID, int NO,Block *pointer){
        for(int i = 0; i < blockNumber; i++) {
            //块为空闲
            if(pointer->pageID == -1) {
                pointer->pageID = pageID;
                pageFaultCount++; //缺页次数+1
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
        //根据题目要求初始化指令序列
    	blockList=(Block**)malloc(blockNumber*sizeof(Block*));
        for(int i=0;i<blockNumber;i++){
            //根据输入的块数生成内存块链表
            blockList[i]=(Block*)malloc(sizeof(Block));
            blockList[i]->pageID = -1;
            blockList[i]->blockID = i;
            blockList[i]->firstInIndex = MAXSIZE+1;
            blockList[i]->leastRecentlyUsedIndex = MAXSIZE+1;
        }
        for(int i=0;i<blockNumber;i++){
            //链接成单向循环链表
            blockList[i]->next=blockList[(i+1)%blockNumber];
        }
        for(int i = 0; i < MAXSIZE; ) {
        //初始化地址流//指令可能能重复,0和MAXSIZE都出现了,27也出现了两次
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

    //NO为所调用页在地址流中的位置
    int Optimal(int pageID, int instructionID, int NO) {
        Block *pointer = blockList[0];
        if(1==commonPack(pageID,instructionID,NO,pointer))return 1;

        //OPT页面置换
        int allBlockPageNum[blockNumber]; //记录已装入内存的页地址
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
                    //找到第一个位置即停止
                    nextAddr[i] = j;
                    break;
                }
            }
        }
        int tempPageID = 0; //页地址
        int blockNO; //内存块的地址
        //选出距离最远的页地址在内存块中对应的位置
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
        //计算缺页率
        char trigger[]={'\0','\0'};char* to0;
        do{
            fgets(trigger,2,stdin);
            to0=NULL;to0 = strchr(trigger, '\n');if(to0)*to0 = '\0';  fflush(stdin);
        }while(trigger[0]<'0'||trigger[0]>'3');

        printf("T is true,F is false,R is replace\n");
        printf("NO.\t指令号\tat逻辑\tat物理\t缺页否\t");
        for(int i=0;i<blockNumber;i++){
            printf("内存块%d\t",i);
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
        printf("缺页次数：%d\n", pageFaultCount);
        printf("缺页率为：%.4f \n", pageFaultCount*1.0 / MAXSIZE);
    }

    int main(){
        printf("先输入内存块总数!\n");
        scanf("%d",&blockNumber);
        printf("----------最佳置换算法(OPT)请按1----------\n");
        printf("----------先进先出算法(FIFO)请按2----------\n");
        printf("----------最近最久未使用算法(LRU)请按3----------\n");

        initializeInstruction();
        calculatePageFaultCount();
        return 0;
    }
