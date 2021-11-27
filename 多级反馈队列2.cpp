#include<iostream>
#include <fstream>
using namespace std;


typedef struct tag_pcb {
    char name[8];
    int need = 0;//��Ҫ���е�ʱ��
    int turn = 0;//��תʱ��=�ȴ�ʱ��+����ʱ��
    struct tag_pcb* next = NULL;
}PCB;
PCB* RQ1=new PCB, * RQ2 = new PCB, * Finish = new PCB;
const int TimePiece = 7;//ʱ��Ƭ����

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

void Q1_Insert(PCB a) { //ʱ��Ƭ��ת�㷨���еĲ���(����β��)

    PCB* Current = RQ1;
    while (Current->next != NULL)
        Current = Current->next;
    Current->next = new PCB;

    *Current->next = a;
    //Current->next = &a;

    Current->next->next = NULL;

}
void Q2_Insert(PCB b) { //�̽������ȵ����㷨���еĲ���

    
    PCB* Current = RQ2;
    while (Current->next != NULL)
        Current = Current->next;
    Current->next = new PCB;

    *Current->next = b;
    Current->next->next = NULL;
    
}
void Fin_Insert(PCB c) { //�̽������ȵ����㷨���еĲ���
    
    PCB* cc = new PCB;
    *cc = c;

    cc->next = Finish->next;
    Finish->next = cc;   
}
void Q2_sort(PCB *T) {

    PCB* X = new PCB;//������������������
    PCB* p = new PCB;//�������浱����Сֵ��ǰһλ
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
                //cout << "����" << p->name << p->need << "\n";
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

    int clock = 0; //ʱ��
    while (RQ1->next != NULL) {//��ʾRQ1����Ԫ��
        int t = TimePiece;
        PCB* Current = RQ1->next;
        int fin = 0;

        if (Current->need <= t)
            t = Current->need, fin = 1;
           
        clock += t;//��ʾpi����t

        //����������
        //cout << "\n" << Current->name << "_____" << Current->turn << "__+ ___" << clock << "__= ___" << Current->turn +clock << "\n";
       
        Current->need -= t;
  
        if (fin)
            Current->turn += clock, Fin_Insert(*Current);//���н���    
        else
            Q1_Insert(*Current);//�����β�ȴ�����

        if (Current->next == NULL)
            break;
        RQ1->next = Current->next;
    }


    clock = 0;//ʱ��Ҫ���һ��
    Q2_sort(RQ2);//������

    cout << "RQ2:__"; 
    for (PCB* Current2 = RQ2->next; Current2 != NULL; Current2 = Current2->next)
        cout << Current2->name << "--";

    while (RQ2->next != NULL) {//��ʾRQ2����Ԫ��(����һ��Ĭ��RQ1�Ѿ�Ϊ��)
        PCB* Current3 = RQ2->next;
        int t = Current3->need;

        clock += t;//��ʾpi����t
        Current3->need -= t;//ʵ��Ϊ���
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

    cout << "\n����תʱ��Ϊ��" << SUM << "\n";
}
