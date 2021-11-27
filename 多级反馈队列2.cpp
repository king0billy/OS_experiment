#include<iostream>
#include <fstream>
using namespace std;


typedef struct tag_pcb {
    char name[8];
    int need = 0;//需要运行的时间
    int turn = 0;//周转时间=等待时间+运行时间
    struct tag_pcb* next = NULL;
}PCB;
PCB* RQ1=new PCB, * RQ2 = new PCB, * Finish = new PCB;
const int TimePiece = 7;//时间片长度

void ReadFile(){

    ifstream In("RQ1.txt");
    PCB* Current = RQ1;

    while (!In.eof()) {
        PCB* Cur = new PCB;

        In >> Cur->name >> Cur->need>> Cur->turn;

        Current->next = Cur;
        Current = Current->next;
    }
    In.close();

    ifstream In1("RQ2.txt");
    PCB* Current1 = RQ2;

    while (!In1.eof()) {
        PCB* Cur1 = new PCB;

        In1 >> Cur1->name >> Cur1->need >> Cur1->turn;
        Current1->next = Cur1;
        Current1 = Current1->next;
    }
    In1.close();
}

void Q1_Insert(PCB a) { //时间片轮转算法队列的插入(插入尾部)

    PCB* Current = RQ1;
    while (Current->next != NULL)
        Current = Current->next;
    Current->next = new PCB;

    *Current->next = a;
    //Current->next = &a;

    Current->next->next = NULL;

}
void Q2_Insert(PCB b) { //短进程优先调度算法队列的插入

    
    PCB* Current = RQ2;
    while (Current->next != NULL)
        Current = Current->next;
    Current->next = new PCB;

    *Current->next = b;
    Current->next->next = NULL;
    
}
void Fin_Insert(PCB c) { //短进程优先调度算法队列的插入
    
    PCB* cc = new PCB;
    *cc = c;

    cc->next = Finish->next;
    Finish->next = cc;   
}
void Q2_sort(PCB *T) {

    PCB* X = new PCB;//用来保存排序后的链表
    PCB* p = new PCB;//用来保存当此最小值的前一位
    PCB* Current = T->next;
    PCB * PreCurrent = T;
    PCB* TailX = X;
    
    while (T->next != NULL) {
        int tem = 999999;

        Current = T->next;
        PreCurrent = T;
        while (Current != NULL) {
            if (Current->need < tem) {
                tem = Current->need;
                
                p = PreCurrent;
                //cout << "处理" << p->name << p->need << "\n";
            }
            Current = Current->next;
            PreCurrent = PreCurrent->next;
        }
     

        TailX->next = p->next;
        TailX = TailX->next;

        if (p->next->next != NULL)
            p->next = p->next->next;
        else
            p->next = NULL;      
    }
    *T = *X;
}

int main()
{
    ReadFile();

    int clock = 0; //时钟
    while (RQ1->next != NULL) {//表示RQ1还有元素
        int t = TimePiece;
        PCB* Current = RQ1->next;
        int fin = 0;

        if (Current->need <= t)
            t = Current->need, fin = 1;
           
        clock += t;//表示pi运行t

        //输出计算过程
        //cout << "\n" << Current->name << "_____" << Current->turn << "__+ ___" << clock << "__= ___" << Current->turn +clock << "\n";
       
        Current->need -= t;
  
        if (fin)
            Current->turn += clock, Fin_Insert(*Current);//运行结束    
        else
            Q1_Insert(*Current);//进入队尾等待运行

        if (Current->next == NULL)
            break;
        RQ1->next = Current->next;
    }


    clock = 0;//时钟要清空一次
    Q2_sort(RQ2);//先排序

    cout << "RQ2:__"; 
    for (PCB* Current2 = RQ2->next; Current2 != NULL; Current2 = Current2->next)
        cout << Current2->name << "--";

    while (RQ2->next != NULL) {//表示RQ2还有元素(到这一步默认RQ1已经为空)
        PCB* Current3 = RQ2->next;
        int t = Current3->need;

        clock += t;//表示pi运行t
        Current3->need -= t;//实质为清空
        Current3->turn += clock;

        Fin_Insert(*Current3);

        if (Current3->next == NULL)
            break;
        RQ2->next = Current3->next;
    }

    int SUM = 0;
    for (PCB* Current2 = Finish->next; Current2 != NULL; Current2 = Current2->next) {
        cout << "\n" << Current2->name <<"\t"<< Current2->turn ;
        SUM += Current2->turn;
    }

    cout << "\n总周转时间为：" << SUM << "\n";
}
