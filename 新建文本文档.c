    #include <stdio.h>
    #include <stdlib.h>

    float count = 0; //ȱҳ����
    int instrAddr[320]; //ָ���ַ������
    int pageAddr[320]; //ҳ��ַ������

    typedef struct Data //������
    {
        int pageNum; //װ�����û����ҳ��
        int blockNum; //���
    } Data;

    typedef struct BlockNode //����ѭ������
    {
        Data data;
        struct BlockNode *next;
    } Block, *BlockList;

    //�����ڴ��
    BlockList block1;
    BlockList block2;
    BlockList block3;
    BlockList block4;
    void print(int pageNum, int virAddr, int pos,Block *p,char* shit){
        printf("%d\t", pos);
        printf("%d\t", virAddr);
        printf("%d\t", pageNum);
        printf("%d\t", p->data.blockNum);
        printf("%s\t",shit);
        printf("%d\t",block1->data.pageNum);
        printf("%d\t",block2->data.pageNum);
        printf("%d\t",block3->data.pageNum);
        printf("%d\t",block4->data.pageNum);
        printf("\n");
    }
    void OPT(int pageNum, int virAddr, int pos,Block *p,char* shit){
        //OPTҳ���û�
        int allBlockPageNum[4]; //��¼��װ���ڴ��ҳ��ַ
        for(int i = 0; i < 4; i++){
            allBlockPageNum[i] = p->data.pageNum;
            p = p->next;
        }
        int nextAddr[4]; //��¼��װ���ڴ��ҳ��ַ�´���ָ�����г��ֵ�λ��
        for(int i = 0; i < 4; i++){
            for(int j = pos; j < 320; j++){
                if(allBlockPageNum[i] == pageAddr[j]){ //�ҵ���һ��λ�ü�ֹͣ
                    nextAddr[i] = j;
                    break;
                }
            }
        }
        int temp = 0; //ҳ��ַ
        int blockPos; //�ڴ��ĵ�ַ
        //ѡ��������Զ��ҳ��ַ���ڴ���ж�Ӧ��λ��
        for(int i = 0; i < 4; i++) {
            if(nextAddr[i] > temp){
                temp = nextAddr[i];
                blockPos = i;
            }
        }
        for(int i = 0; i < 4; i++){
            if(p->data.blockNum == blockPos){
                p->data.pageNum = pageNum;
                count++;
                print(pageNum,virAddr,pos,p,"T&R");
            }
            p = p->next;
        }
    }
    void initialize() //��ʼ��
    {
        block1 = (BlockList)malloc(sizeof(Block));
        block2 = (BlockList)malloc(sizeof(Block));
        block3 = (BlockList)malloc(sizeof(Block));
        block4 = (BlockList)malloc(sizeof(Block));

        block1->data.pageNum = -1;
        block2->data.pageNum = -1;
        block3->data.pageNum = -1;
        block4->data.pageNum = -1;

        block1->data.blockNum = 0;
        block2->data.blockNum = 1;
        block3->data.blockNum = 2;
        block4->data.blockNum = 3;

        block1->next = block2;
        block2->next = block3;
        block3->next = block4;
        block4->next = block1;

        for(int i = 0; i < 320; ) //��ʼ����ַ��//û˵ָ����ظ�?0,320��������,��27����������
        {
            int m = rand() % 320;
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
    int Optimal(int pageNum, int virAddr, int pos) //����û��㷨
    {
        Block *p = block1;

        for(int i = 0; i < 4; i++) //���������ڴ�飬���������κβ������������ʱ����ָ��block1
        {
            if(p->data.pageNum == -1) //��Ϊ����
            {
                p->data.pageNum = pageNum;
                count++; //ȱҳ����+1R
                print(pageNum,virAddr,pos,p,"T");
                return 1;
            }

            if(p->data.pageNum == pageNum)
            {
                print(pageNum,virAddr,pos,p,"F");
                return 1;
            }

            p = p->next;
        }

        OPT(pageNum,virAddr,pos,p,"T&R");
        /*
        //OPTҳ���û�
        int allBlockPageNum[4]; //��¼��װ���ڴ��ҳ��ַ
        for(int i = 0; i < 4; i++){
            allBlockPageNum[i] = p->data.pageNum;
            p = p->next;
        }
        int nextAddr[4]; //��¼��װ���ڴ��ҳ��ַ�´���ָ�����г��ֵ�λ��
        for(int i = 0; i < 4; i++){
            for(int j = pos; j < 320; j++){
                if(allBlockPageNum[i] == pageAddr[j]){ //�ҵ���һ��λ�ü�ֹͣ
                    nextAddr[i] = j;
                    break;
                }
            }
        }
        int temp = 0; //ҳ��ַ
        int blockPos; //�ڴ��ĵ�ַ
        //ѡ��������Զ��ҳ��ַ���ڴ���ж�Ӧ��λ��
        for(int i = 0; i < 4; i++) {
            if(nextAddr[i] > temp){
                temp = nextAddr[i];
                blockPos = i;
            }
        }
        for(int i = 0; i < 4; i++){
            if(p->data.blockNum == blockPos){
                p->data.pageNum = pageNum;
                count++;
                print(pageNum,virAddr,pos,p,"T&R");
            }
            p = p->next;
        }*/
        return 1;
    }

    void calculate() //����ȱҳ��
    {

        printf("T is true,F is false,R is replace\n");
        printf("NO.\tָ���\tat�߼�\tat����\tȱҳ��\t�ڴ��0\t�ڴ��1\t�ڴ��2\t�ڴ��3\n");
        for(int i = 0; i < 320; i++)
        {
            Optimal(pageAddr[i], instrAddr[i], i);
        }

        printf("\n");
        printf("ȱҳ������%.0f\n", count);
        printf("����õ���ȱҳ��Ϊ��%.4f \n", count / 320);
    }

    int main()
    {
        printf("----------����û��㷨(OPT)�밴1----------\n\n");
        printf("----------�Ƚ��ȳ��㷨(FIFO)�밴2----------\n\n");
        printf("----------������δʹ���㷨(LRU)�밴3----------\n\n");
        initialize();
        calculate();

        return 0;
    }
