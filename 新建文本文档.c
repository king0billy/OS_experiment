#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MEMORY_SIZE 640
#define MAX_PRO 20
#define ALLOCED 1
#define FREE 0
#define FILE_NAME "/home/whileskies/Desktop/input"

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
	p->pid = -1;
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
		newSubArea->pid = -1;

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


//循环首次适应
SubAreaList nfFindFreeSubArea(int psize) {
	SubAreaList tmp = nowList;

	while (nowList) {
		if (nowList->stat == FREE && nowList->size >= psize) {
			return nowList;
		}
		nowList = nowList->next == NULL ? head : nowList->next;
		if (nowList == tmp) return NULL;
	}
}

int nfAlloc(int pid, int psize) {
	int ret = alloc(pid, psize, nfFindFreeSubArea(psize));
	nowList = nowList->next == NULL ? head : nowList->next;
	return ret;
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


//最坏适应算法
SubAreaList wfFindFreeSubArea(int psize) {
	SubAreaList p = head, maxP = NULL;
	int maxSize = -1;

	while (p) {
		if (p->stat == FREE && p->size >= psize) {
			if (p->size > maxSize) {
				maxSize = p->size;
				maxP = p;
			}
		}
		p = p->next;
	}

	return maxP;
}


int wfAlloc(int pid, int psize) {
	return alloc(pid, psize, wfFindFreeSubArea(psize));
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
			nextNode->next->pre = p;
			p->stat = FREE;
			p->pid = -1;

			free(nextNode);
		} else {
			p->stat = FREE;
			p->pid = -1;
		}
	}

	return 1;
}


//绘制内存分配图
void disAllocGraph(int prec) {
	SubAreaList p = head;

	for (int i = 0; i < MEMORY_SIZE/prec; i++)
		printf("-");
	printf("\n");

	int addr[MAX_PRO];
	int pid[MAX_PRO];
	int minSize = MEMORY_SIZE;
	int n = 0;
	while (p) {
		addr[n] = p->addr;
		pid[n++] = p->pid;

		minSize = p->size < minSize ? p->size : minSize;
		p = p->next;
	}
	addr[n] = MEMORY_SIZE;
	pid[n++] = -1;
	if (minSize < 10) {
		printf("内存间隔过小，不显示分配图\n");
		return;
	}

	for (int l = 0; l < 3; l++) {
		for (int i = 0, j = 0, k = 0; i < MEMORY_SIZE/prec; i++) {
			if (i == addr[j]/prec) {
				printf("|");
				j++;
			} else
				printf(" ");

			if (l == 1 && i == addr[k]/prec +
						(addr[k+1]/prec-addr[k]/prec)/2) {
				if (pid[k] != -1) {
					printf("%d", pid[k]);
					i = pid[k] < 10 ? i+1 : i+2;
				}
				k++;
			}

			if (i == MEMORY_SIZE/prec - 2) printf("|");
		}
		printf("\n");
	}


	for (int i = 0; i < MEMORY_SIZE/prec; i++)
			printf("-");
	printf("\n");
	int cnt = 0;
	for (int i = 0, k = 0; i < MEMORY_SIZE/prec; i++) {
		int digit = 0;
		int tmp = addr[k];
		while (tmp) { digit++; tmp /= 10; }
		if (i == addr[k]/prec - digit/2) {
			printf("%d", addr[k]);
			k++;
			cnt = digit;
		} else {
			if (cnt > 1) { cnt--; continue; }
			printf(" ");
		}
	}

	printf("\n");
}


void printSepLine() {
	printf("\n*****************************************************************************\n\n");
}

void displayAlloc() {
	SubAreaList p = head;

	//printf("\n*****************************************************************************\n");
	printf("\n|%4s\t|%3s\t|%3s\t|%3s\n", "分区号","起始地址", "分区大小", "分区状态");

	while (p) {
		printf("|%3d\t|%3d\t\t|%3d\t\t|%3d\n",p->pid, p->addr,p->size, p->stat);
		p = p->next;
	}

	printf("\n");
	disAllocGraph(10);

	printSepLine();
}


void inputCtrl(int (*allocAlogrithm)(int,int)) {
    system("cls");
//	system("clear");
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
				printf("给作业id %d 分配了  %d KB空间\n", pid, size);
				displayAlloc();
			}
			else {
				printf("\n内存不足 分配失败\n\n");
				printSepLine();
			}
		} else{
		    if (T[0] == '2') {
                int pid;
                printf("输入要释放空间的作业id: ");scanf("%d", &pid);
                int ret = freeAlloc(pid);
                if (ret) {
                    printf("作业id %d  已回收\n", pid);
                    displayAlloc();
                } else {
                    printf("未找到相关作业，回收失败\n\n");
                    printSepLine();
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
//	system("clear");
	printf("\n***********************************\n\n");
	printf("            请选择你的分配算法           \n\n");
	printf("             1.首次适应算法                \n");
	printf("             2.最佳适应算法             \n");
	printf("\n***********************************\n\n");

	char op[20];
	printf(">  ");
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
