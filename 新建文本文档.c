#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MEMORY_SIZE 640
#define ALLOCATED 1
#define FREE 0

typedef struct partitionNode *partitionList;
struct partitionNode{
	int processID;
	int startingAddress;
	int size;
	int status;
	partitionList previous;
	partitionList next;
};

partitionList head, nowList;

partitionList getAPartition() {
	return (partitionList)malloc(sizeof(struct partitionNode));
}

void selectAlgorithm();

partitionList initPartition() {
	partitionList pointer = getAPartition();
	pointer->startingAddress = 0;
	pointer->size = MEMORY_SIZE;
	pointer->status = FREE;
	pointer->processID = -99;
	pointer->previous = NULL;
	pointer->next = NULL;
	return pointer;
}


//在链表位置p处为编号为processID，大小为processSize的作业分配空间
int allocation(int processID, int processSize, partitionList pointer) {
	if (pointer == NULL)   //无合适空间可分配
		return 0;

	if (pointer->size == processSize) {   //分区整块分配
		pointer->status = ALLOCATED;
		pointer->processID = processID;
	} else {                  //分割分区
		partitionList newPartition = getAPartition();
		newPartition->startingAddress = pointer->startingAddress + processSize;
		newPartition->size = pointer->size - processSize;
		newPartition->status = FREE;
		newPartition->processID = -99;

		pointer->size = processSize;
		pointer->status = ALLOCATED;
		pointer->processID = processID;

		newPartition->next = pointer->next;
		pointer->next = newPartition;
		newPartition->previous = pointer;
	}
	return 1;
}


//首次适应算法
partitionList firstFitFindFreePartition(int processSize) {
	partitionList pointer = head;

	while (pointer) {
		if (pointer->status == FREE && pointer->size >= processSize)
			return pointer;
		pointer = pointer->next;
	}

	return NULL;
}


int firstFitAllocation(int processID, int processSize) {
	return allocation(processID, processSize, firstFitFindFreePartition(processSize));
}



//最佳适应算法
partitionList bestFitFindFreePartition(int processSize) {
	partitionList pointer = head, minP = NULL;
	int minSize = MEMORY_SIZE + 1;

	while (pointer) {
		if (pointer->status == FREE && pointer->size >= processSize) {
			if (pointer->size < minSize) {
				minSize = pointer->size;
				minP = pointer;
			}
		}
		pointer = pointer->next;
	}

	return minP;
}

int bestFitAllocation(int processID, int processSize) {
	return allocation(processID, processSize, bestFitFindFreePartition(processSize));
}


int freeAllocation(int processID) {
	//寻找作业所在分区
	partitionList pointer = head;

	while (pointer) {
		if (pointer->processID == processID) {
			break;
		}
		pointer = pointer->next;
	}

	if (pointer == NULL)
		return 0;

	//不是首块分区且与前一空闲分区相连
	if (pointer != head && pointer->previous->status == FREE &&
			pointer->previous->startingAddress + pointer->previous->size == pointer->startingAddress) {
		partitionList preNode = pointer->previous;
		partitionList nextNode = pointer->next;

		preNode->size += pointer->size;
		preNode->next = pointer->next;
		nextNode->previous = preNode;

		//与后一空闲分区相连
		if (pointer->next->status == FREE &&
				 pointer->startingAddress + pointer->size == pointer->next->startingAddress) {
			preNode->size += nextNode->size;
			preNode->next = nextNode->next;
			nextNode->next->previous = preNode;

			free(nextNode);
		}

		free(pointer);

	} else {
		//不是最后一个分区且与后一空闲分区相连
		if (pointer->next != NULL && pointer->next->status == FREE &&
				pointer->startingAddress + pointer->size == pointer->next->startingAddress) {
			partitionList nextNode = pointer->next;
			pointer->size += nextNode->size;
			pointer->next = nextNode->next;
			if(nextNode->next!=NULL)nextNode->next->previous = pointer;
			//todo 解决删除唯一一个分区的bug
			pointer->status = FREE;
			pointer->processID = -99;
			free(nextNode);
		} else {
			pointer->status = FREE;
			pointer->processID = -99;
		}
	}
	return 1;
}

void displayAllocation() {
	partitionList pointer = head;

	printf("\n|%4s\t|%3s\t|%3s\t|%3s\n", "分区号","起始地址", "分区大小", "分区状态");

	while (pointer) {
		printf("|%3d\t|%3d\t\t|%3d\t\t|",pointer->processID, pointer->startingAddress,pointer->size);
		if(pointer->status==1)printf("已分配");
		else printf("空闲");
		printf("\n");
		pointer = pointer->next;
	}
	printf("\n");
}


void inputControl(int (*allocAlgorithm)(int,int)) {
	printf("申请按 1 释放按 2 退出按 9\n");
	char T[5];
	scanf("%s", T);
	while (T[0] != '9') {
		if (T[0] == '1') {
			int processID, size;
			printf("输入要申请空间的作业id: ");
			scanf("%d", &processID );
            printf("输入大小: ");
            scanf("%d", &size);
			int ret = allocAlgorithm(processID, size);
			if (ret) {
				printf("申请成功");
				displayAllocation();
			}
			else {
				printf("\n内存不足 申请失败\n\n");
			}
		} else{
		    if (T[0] == '2') {
                int processID;
                printf("输入要释放空间的作业id: ");scanf("%d", &processID);
                int ret = freeAllocation(processID);
                if (ret) {
                    printf("释放成功");
                    displayAllocation();
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


void firstFitAllocationControl() {
		inputControl(firstFitAllocation);
}

void bestFitAllocationControl() {
		inputControl(bestFitAllocation);
}



void selectAlgorithm() {
	system("cls");
	printf("按1_首次适应算法                ");
	printf("按2_最佳适应算法             \n");

	char op[20];
	//printf(">  ");
	scanf("%s", op);

	if (!strcmp(op, "1"))
		firstFitAllocationControl();
	else if (!strcmp(op, "2"))
		bestFitAllocationControl();
	else {
		exit(0);
	}
}

int main()
{
	head = initPartition();
	nowList = head;
	selectAlgorithm();
	return 0;
}
