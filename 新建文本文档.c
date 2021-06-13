#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MEMORY_SIZE 640
#define MAX_PRO 20
#define ALLOCED 1
#define FREE 0

typedef struct SubAreaNode *SubAreaList;
struct SubAreaNode
{
	int addr;
	int size;
	int stat;
	int pid;

	SubAreaList pre;
	SubAreaList next;
};

SubAreaList head, nowList;

SubAreaList getASubArea() {
	return (SubAreaList)malloc(sizeof(struct SubAreaNode));
}

void selectAlogrithm();

SubAreaList initSubArea() {
	SubAreaList p = getASubArea();
	p->addr = 0;
	p->size = MEMORY_SIZE;
	p->stat = FREE;
	p->pid = -99;
	p->pre = NULL;
	p->next = NULL;

	return p;
}


//������λ��p��Ϊ���Ϊpid����СΪpsize����ҵ����ռ�
int alloc(int pid, int psize, SubAreaList p) {
	if (p == NULL)   //�޺��ʿռ�ɷ���
		return 0;

	if (p->size == psize) {   //�����������
		p->stat = ALLOCED;
		p->pid = pid;
	} else {                  //�ָ����
		SubAreaList newSubArea = getASubArea();
		newSubArea->addr = p->addr + psize;
		newSubArea->size = p->size - psize;
		newSubArea->stat = FREE;
		newSubArea->pid = -99;

		p->size = psize;
		p->stat = ALLOCED;
		p->pid = pid;

		newSubArea->next = p->next;
		p->next = newSubArea;
		newSubArea->pre = p;

	}

	return 1;
}


//�״���Ӧ�㷨
SubAreaList ffFindFreeSubArea(int psize) {
	SubAreaList p = head;

	while (p) {
		if (p->stat == FREE && p->size >= psize)
			return p;
		p = p->next;
	}

	return NULL;
}


int ffAlloc(int pid, int psize) {
	return alloc(pid, psize, ffFindFreeSubArea(psize));
}



//�����Ӧ�㷨
SubAreaList bfFindFreeSubArea(int psize) {
	SubAreaList p = head, minP = NULL;
	int minSize = MEMORY_SIZE + 1;

	while (p) {
		if (p->stat == FREE && p->size >= psize) {
			if (p->size < minSize) {
				minSize = p->size;
				minP = p;
			}
		}
		p = p->next;
	}

	return minP;
}

int bfAlloc(int pid, int psize) {
	return alloc(pid, psize, bfFindFreeSubArea(psize));
}


int freeAlloc(int pid) {
	//Ѱ����ҵ���ڷ���
	SubAreaList p = head;

	while (p) {
		if (p->pid == pid) {
			break;
		}
		p = p->next;
	}

	if (p == NULL)
		return 0;

	//�����׿��������ǰһ���з�������
	if (p != head && p->pre->stat == FREE &&
			p->pre->addr + p->pre->size == p->addr) {
		SubAreaList preNode = p->pre;
		SubAreaList nextNode = p->next;

		preNode->size += p->size;
		preNode->next = p->next;
		nextNode->pre = preNode;

		//���һ���з�������
		if (p->next->stat == FREE &&
				 p->addr + p->size == p->next->addr) {
			preNode->size += nextNode->size;
			preNode->next = nextNode->next;
			nextNode->next->pre = preNode;

			free(nextNode);
		}

		free(p);

	} else {
		//�������һ�����������һ���з�������
		if (p->next != NULL && p->next->stat == FREE &&
				p->addr + p->size == p->next->addr) {
			SubAreaList nextNode = p->next;
			p->size += nextNode->size;
			p->next = nextNode->next;
			if(nextNode->next!=NULL)nextNode->next->pre = p;
			p->stat = FREE;
			p->pid = -99;
			free(nextNode);
		} else {
			p->stat = FREE;
			p->pid = -99;
		}
	}

	return 1;
}

void displayAlloc() {
	SubAreaList p = head;

	printf("\n|%4s\t|%3s\t|%3s\t|%3s\n", "������","��ʼ��ַ", "������С", "����״̬");

	while (p) {
		printf("|%3d\t|%3d\t\t|%3d\t\t|",p->pid, p->addr,p->size);
		if(p->stat==1)printf("�ѷ���");
		else printf("����");
		printf("\n");
		p = p->next;
	}
	printf("\n");
}


void inputCtrl(int (*allocAlogrithm)(int,int)) {
	printf("���밴 1 �ͷŰ� 2 �˳��� 9\n");
	char T[5];
	scanf("%s", T);
	while (T[0] != '9') {
		if (T[0] == '1') {
			int pid, size;
			printf("����Ҫ����ռ����ҵid: ");
			scanf("%d", &pid );
            printf("�����С: ");
            scanf("%d", &size);
			int ret = allocAlogrithm(pid, size);
			if (ret) {
				printf("����ɹ�");
				displayAlloc();
			}
			else {
				printf("\n�ڴ治�� ����ʧ��\n\n");
			}
		} else{
		    if (T[0] == '2') {
                int pid;
                printf("����Ҫ�ͷſռ����ҵid: ");scanf("%d", &pid);
                int ret = freeAlloc(pid);
                if (ret) {
                    printf("�ͷųɹ�");
                    displayAlloc();
                } else {
                    printf("δ�ҵ������ҵ���ͷ�ʧ��\n\n");
                }
		    }
		    else{
			    exit(0);
			}
		}
		scanf("%s", T);
	}
}


void ffAllocCtrl() {
		inputCtrl(ffAlloc);
}

void bfAllocCtrl() {
		inputCtrl(bfAlloc);
}



void selectAlogrithm() {
	system("cls");
	printf("��1_�״���Ӧ�㷨                ");
	printf("��2_�����Ӧ�㷨             \n");

	char op[20];
	//printf(">  ");
	scanf("%s", op);

	if (!strcmp(op, "1"))
		ffAllocCtrl();
	else if (!strcmp(op, "2"))
		bfAllocCtrl();
	else {
		exit(0);
	}
}

int main()
{
	head = initSubArea();
	nowList = head;
	selectAlogrithm();
	return 0;
}
