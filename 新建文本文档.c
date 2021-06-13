    #include <stdio.h>
    #include <stdlib.h>
    #define MAXSIZE 320
    //#define blockNumber 4
    float count = 0; //ȱҳ����
    int instrAddr[MAXSIZE]; //ָ���ַ������
    int pageAddr[MAXSIZE]; //ҳ��ַ������

    typedef struct Data //������
    {
        int pageNum; //װ�����û����ҳ��
        int blockNum; //���
        int firstInIndex;//�״ε����ڴ��index//MAXSIZE+1
        int leastRecentlyUsedIndex;//���һ��ʹ�õ�index//-99
    } Data;

    typedef struct BlockNode //����ѭ������
    {
        Data data;
        struct BlockNode *next;
    } Block, *blockP;

    //�����ڴ��
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
            //���������ڴ�飬���������κβ������������ʱ����ָ��block1
            //if(type==1){
                for(int i = 0; i < blockNumber; i++) {
                    //��Ϊ����
                    if(p->data.pageNum == -1) {
                        p->data.pageNum = pageNum;
                        count++; //ȱҳ����+1R
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
    void initialize() //��ʼ��
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

        for(int i = 0; i < MAXSIZE; ) //��ʼ����ַ��//û˵ָ����ظ�?0,320��������,��27����������
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

    //posΪ������ҳ�ڵ�ַ���е�λ��
    int Optimal(int pageNum, int virAddr, int pos) {
        Block *p = blockList[0];
        if(1==commonPack(pageNum,virAddr,pos,p))return 1;

        //OPTҳ���û�
        int allBlockPageNum[blockNumber]; //��¼��װ���ڴ��ҳ��ַ
        for(int i = 0; i < blockNumber; i++){
            allBlockPageNum[i] = p->data.pageNum;
            p = p->next;
        }
        //int nextAddr[blockNumber]={MAXSIZE+1};
        int nextAddr[blockNumber];
        for(int i=0;i<blockNumber;i++){
            nextAddr[i]=MAXSIZE+1;
        }
        //int nextAddr[blockNumber]={0,0,0,0}; //��¼��װ���ڴ��ҳ��ַ�´���ָ�����г��ֵ�λ��
        //int nextAddr[blockNumber]; //��¼��װ���ڴ��ҳ��ַ�´���ָ�����г��ֵ�λ��
        for(int i = 0; i < blockNumber; i++){
            for(int j = pos; j < MAXSIZE; j++){
                if(allBlockPageNum[i] == pageAddr[j]){ //�ҵ���һ��λ�ü�ֹͣ
                    nextAddr[i] = j;
                    break;
                }
            }
        }
        int temp = 0; //ҳ��ַ
        int blockPos; //�ڴ��ĵ�ַ
        //ѡ��������Զ��ҳ��ַ���ڴ���ж�Ӧ��λ��
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

    void calculate() //����ȱҳ��
    {
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
        printf("ȱҳ������%.0f\n", count);
        printf("����õ���ȱҳ��Ϊ��%.4f \n", count / MAXSIZE);
    }

    int main()
    {
        printf("�������ڴ������!\n");
        scanf("%d",&blockNumber);
        printf("----------����û��㷨(OPT)�밴1----------\n");
        printf("----------�Ƚ��ȳ��㷨(FIFO)�밴2----------\n");
        printf("----------������δʹ���㷨(LRU)�밴3----------\n");

        initialize();
        calculate();

        return 0;
    }
