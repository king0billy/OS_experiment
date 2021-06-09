    #include <stdio.h>
    #include <stdlib.h>
    #define MAXSIZE 320
    //#define blockNumber 4
    float count = 0; //缺页次数
    int instrAddr[MAXSIZE]; //指令地址流数组
    int pageAddr[MAXSIZE]; //页地址流数组

    typedef struct Data //数据域
    {
        int pageNum; //装进的用户虚存页号
        int blockNum; //块号
        int firstInIndex;//首次调入内存的index//MAXSIZE+1
        int leastRecentlyUsedIndex;//最后一次使用的index//-99
    } Data;

    typedef struct BlockNode //单向循环链表
    {
        Data data;
        struct BlockNode *next;
    } Block, *blockP;

    //定义内存块
    int blockNumber=3;
    blockP* blockList;
/*    blockP block1;
    blockP block2;
    blockP block3;
    blockP block4;*/
    void print(int pageNum, int virAddr, int pos,Block *p,char* shit){
        printf("%d\t", pos);
        printf("%d\t", virAddr);
        printf("%d\t", pageNum);
        printf("%d\t", p->data.blockNum);
        printf("%s\t",shit);
        for(int i=0;i<blockNumber;i++){
            printf("%d\t",blockList[i]->data.pageNum);
        }
/*        printf("%d\t",block1->data.pageNum);
        printf("%d\t",block2->data.pageNum);
        printf("%d\t",block3->data.pageNum);
        printf("%d\t",block4->data.pageNum);*/
        printf("\n");
    }
    int commonPack(int pageNum, int virAddr, int pos,Block *p){
            //遍历所有内存块，若不进行任何操作则遍历结束时候仍指向block1
            //if(type==1){
                for(int i = 0; i < blockNumber; i++) {
                    //块为空闲
                    if(p->data.pageNum == -1) {
                        p->data.pageNum = pageNum;
                        count++; //缺页次数+1R
                        print(pageNum,virAddr,pos,p,"T");
                        p->data.firstInIndex=pos;
                        p->data.leastRecentlyUsedIndex=pos;
                        return 1;
                    }
                    if(p->data.pageNum == pageNum){
                        print(pageNum,virAddr,pos,p,"F");
                        p->data.leastRecentlyUsedIndex=pos;
                        return 1;
                    }
                    p = p->next;
                }
           // }
            return 0;
    }
    void initialize() //初始化
    {	
    	blockList=(blockP*)malloc(blockNumber*sizeof(blockP));
        for(int i=0;i<blockNumber;i++){
            blockList[i]=(blockP)malloc(sizeof(Block));
            blockList[i]->data.pageNum = -1;
            blockList[i]->data.blockNum = i;
            blockList[i]->data.firstInIndex = MAXSIZE+1;
            blockList[i]->data.leastRecentlyUsedIndex = MAXSIZE+1;
        }
        for(int i=0;i<blockNumber;i++){
            blockList[i]->next=blockList[i+1%blockNumber];
        }
/*
        block1 = (blockP)malloc(sizeof(Block));
        block2 = (blockP)malloc(sizeof(Block));
        block3 = (blockP)malloc(sizeof(Block));
        block4 = (blockP)malloc(sizeof(Block));

        block1->data.pageNum = -1;
        block2->data.pageNum = -1;
        block3->data.pageNum = -1;
        block4->data.pageNum = -1;

        block1->data.blockNum = 0;
        block2->data.blockNum = 1;
        block3->data.blockNum = 2;
        block4->data.blockNum = 3;

        block1->data.firstInIndex = MAXSIZE+1;
        block2->data.firstInIndex = MAXSIZE+1;
        block3->data.firstInIndex = MAXSIZE+1;
        block4->data.firstInIndex = MAXSIZE+1;

        block1->data.leastRecentlyUsedIndex = MAXSIZE+1;
        block2->data.leastRecentlyUsedIndex = MAXSIZE+1;
        block3->data.leastRecentlyUsedIndex = MAXSIZE+1;
        block4->data.leastRecentlyUsedIndex = MAXSIZE+1;

        block1->next = block2;
        block2->next = block3;
        block3->next = block4;
        block4->next = block1;
*/

        for(int i = 0; i < MAXSIZE; ) //初始化地址流//没说指令不能重复?0,320都出现了,如27出现了两次
        {
            int m = rand() % MAXSIZE;
            instrAddr[i] = m + 1;
            pageAddr[i] = instrAddr[i] / 10;
            i++;

            int m1 = rand() % (m - 1);
            instrAddr[i] = m1;
            pageAddr[i] = m1 / 10;
            i++;

            instrAddr[i] = m1 + 1;
            pageAddr[i] = instrAddr[i] / 10;
            i++;

            int m2 = rand() % (319 - m1 - 1) + m1 + 2;
            instrAddr[i] = m2;
            pageAddr[i] = m2 / 10;
            i++;

            instrAddr[i] = m2 + 1;
            pageAddr[i] = instrAddr[i] / 10;
            i++;
        }
    }

    //pos为所调用页在地址流中的位置
    int Optimal(int pageNum, int virAddr, int pos) {
        Block *p = blockList[0];
        if(1==commonPack(pageNum,virAddr,pos,p))return 1;

        //OPT页面置换
        int allBlockPageNum[blockNumber]; //记录已装入内存的页地址
        for(int i = 0; i < blockNumber; i++){
            allBlockPageNum[i] = p->data.pageNum;
            p = p->next;
        }
        //int nextAddr[blockNumber]={MAXSIZE+1};
        int nextAddr[blockNumber];
        for(int i=0;i<blockNumber;i++){
            nextAddr[i]=MAXSIZE+1;
        }
        //int nextAddr[blockNumber]={0,0,0,0}; //记录已装入内存的页地址下次在指令流中出现的位置
        //int nextAddr[blockNumber]; //记录已装入内存的页地址下次在指令流中出现的位置
        for(int i = 0; i < blockNumber; i++){
            for(int j = pos; j < MAXSIZE; j++){
                if(allBlockPageNum[i] == pageAddr[j]){ //找到第一个位置即停止
                    nextAddr[i] = j;
                    break;
                }
            }
        }
        int temp = 0; //页地址
        int blockPos; //内存块的地址
        //选出距离最远的页地址在内存块中对应的位置
        for(int i = 0; i < blockNumber; i++) {
            if(nextAddr[i] > temp){
                temp = nextAddr[i];
                blockPos = i;
            }
        }
        for(int i = 0; i < blockNumber; i++){
            if(p->data.blockNum == blockPos){
                p->data.pageNum = pageNum;
                count++;
                print(pageNum,virAddr,pos,p,"T&R");
            }
            p = p->next;
        }
        return 1;
    }
    int FIFO(int pageNum, int virAddr, int pos){
        Block *p = blockList[0];
        if(1==commonPack(pageNum,virAddr,pos,p))return 1;
        int minFIndex=MAXSIZE+1;
        for(int i=0;i<blockNumber;p=p->next,i++){
            minFIndex=minFIndex<p->data.firstInIndex?minFIndex:p->data.firstInIndex;
        }
        for(int i=0;i<blockNumber;p=p->next,i++){
            if(p->data.firstInIndex==minFIndex){
                p->data.pageNum = pageNum;
                p->data.firstInIndex=pos;
                p->data.leastRecentlyUsedIndex=pos;
                count++;
                print(pageNum,virAddr,pos,p,"T&R");
            }
        }
        return 1;
    }
    int LRU(int pageNum, int virAddr, int pos){
        Block *p = blockList[0];
        if(1==commonPack(pageNum,virAddr,pos,p))return 1;
        int minLIndex=MAXSIZE+1;
        for(int i=0;i<blockNumber;p=p->next,i++){
            minLIndex=minLIndex<p->data.leastRecentlyUsedIndex?minLIndex:p->data.leastRecentlyUsedIndex;
        }
        for(int i=0;i<blockNumber;p=p->next,i++){
            if(p->data.leastRecentlyUsedIndex==minLIndex){
                p->data.pageNum = pageNum;
                p->data.firstInIndex=pos;
                p->data.leastRecentlyUsedIndex=pos;
                count++;
                print(pageNum,virAddr,pos,p,"T&R");
            }
        }
        return 1;
    }

    void calculate() //计算缺页率
    {
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
                Optimal(pageAddr[i], instrAddr[i], i);
            }
        }
        else if(trigger[0]-'0'==2){
            for(int i = 0; i < MAXSIZE; i++){
                FIFO(pageAddr[i], instrAddr[i], i);
            }
        }
        else{
            for(int i = 0; i < MAXSIZE; i++){
                LRU(pageAddr[i], instrAddr[i], i);
            }
        }
        printf("\n");
        printf("缺页次数：%.0f\n", count);
        printf("计算得到的缺页率为：%.4f \n", count / MAXSIZE);
    }

    int main()
    {
        printf("先输入内存块总数!\n");
        scanf("%d",&blockNumber);
        printf("----------最佳置换算法(OPT)请按1----------\n");
        printf("----------先进先出算法(FIFO)请按2----------\n");
        printf("----------最近最久未使用算法(LRU)请按3----------\n");

        initialize();
        calculate();

        return 0;
    }
