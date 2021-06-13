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


//在链表位置p处为编号为pid，大小为psize的作业分配空间
int alloc(int pid, int psize, SubAreaList p) {
	if (p == NULL)   //无合适空间可分配
		return 0;

	if (p->size == psize) {   //分区整块分配
		p->stat = ALLOCED;
		p->pid = pid;
	} else {                  //分割分区
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


//首次适应算法
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



//最佳适应算法
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
	//寻找作业所在分区
	SubAreaList p = head;

	while (p) {
		if (p->pid == pid) {
			break;
		}
		p = p->next;
	}

	if (p == NULL)
		return 0;

	//不是首块分区且与前一空闲分区相连
	if (p != head && p->pre->stat == FREE &&
			p->pre->addr + p->pre->size == p->addr) {
		SubAreaList preNode = p->pre;
		SubAreaList nextNode = p->next;

		preNode->size += p->size;
		preNode->next = p->next;
		nextNode->pre = preNode;

		//与后一空闲分区相连
		if (p->next->stat == FREE &&
				 p->addr + p->size == p->next->addr) {
			preNode->size += nextNode->size;
			preNode->next = nextNode->next;
			nextNode->next->pre = preNode;

			free(nextNode);
		}

		free(p);

	} else {
		//不是最后一个分区且与后一空闲分区相连
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

	printf("\n|%4s\t|%3s\t|%3s\t|%3s\n", "分区号","起始地址", "分区大小", "分区状态");

	while (p) {
		printf("|%3d\t|%3d\t\t|%3d\t\t|",p->pid, p->addr,p->size);
		if(p->stat==1)printf("已分配");
		else printf("空闲");
		printf("\n");
		p = p->next;
	}
	printf("\n");
}


void inputCtrl(int (*allocAlogrithm)(int,int)) {
	printf("申请按 1 释放按 2 退出按 9\n");
	char T[5];
	scanf("%s", T);
	while (T[0] != '9') {
		if (T[0] == '1') {
			int pid, size;
			printf("输入要申请空间的作业id: ");
			scanf("%d", &pid );
            printf("输入大小: ");
            scanf("%d", &size);
			int ret = allocAlogrithm(pid, size);
			if (ret) {
				printf("申请成功");
				displayAlloc();
			}
			else {
				printf("\n内存不足 申请失败\n\n");
			}
		} else{
		    if (T[0] == '2') {
                int pid;
                printf("输入要释放空间的作业id: ");scanf("%d", &pid);
                int ret = freeAlloc(pid);
                if (ret) {
                    printf("释放成功");
                    displayAlloc();
                } else {
                    printf("未找到相关作业，释放失败\n\n");
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
	printf("按1_首次适应算法                ");
	printf("按2_最佳适应算法             \n");

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
