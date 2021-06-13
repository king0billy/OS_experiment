#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 640

struct partitionNode{
	int processID;
	int status;
	int startingAddress;
	int size;
	struct partitionNode* previous;
	struct partitionNode* next;
};

struct partitionNode* headNode;
struct partitionNode* getAPartition() {
	return (struct partitionNode*)malloc(sizeof(struct partitionNode));
}

struct partitionNode* initPartition() {
	struct partitionNode* pointer = getAPartition();
	pointer->startingAddress = 0;
	pointer->size = MAXSIZE;
	pointer->status = 0;
	pointer->processID = -99;
	pointer->previous = NULL;
	pointer->next = NULL;
	return pointer;
}


//在链表位置p处为编号为processID，大小为processSize的作业分配空间
int allocate(int processID, int processSize, struct partitionNode* pointer) {
	if (pointer == NULL){
	    //没有空闲内存了,分配失败
		return 0;
	}

	if (pointer->size == processSize) {
	    //分配整块的分区
		pointer->status = 1;
		pointer->processID = processID;
	} else {
	    //分割空闲的内存再分配
		struct partitionNode* newPartition = getAPartition();
		newPartition->startingAddress = pointer->startingAddress + processSize;
		newPartition->size = pointer->size - processSize;
		newPartition->status = 0;
		newPartition->processID = -99;

		pointer->size = processSize;
		pointer->status = 1;
		pointer->processID = processID;

		newPartition->next = pointer->next;
		pointer->next = newPartition;
		newPartition->previous = pointer;
	}
	return 1;
}


//首次适应算法
struct partitionNode* firstFitFindFreePartition(int processSize) {
	struct partitionNode* pointer = headNode;

	while (pointer) {
		if (pointer->status == 0 && pointer->size >= processSize)
			return pointer;
		pointer = pointer->next;
	}

	return NULL;
}


int firstFitAllocation(int processID, int processSize) {
	return allocate(processID, processSize, firstFitFindFreePartition(processSize));
}



//最佳适应算法
struct partitionNode* bestFitFindFreePartition(int processSize) {
	struct partitionNode* pointer = headNode; struct partitionNode* minPointer = NULL;
	int minSize = MAXSIZE + 1;

	while (pointer) {
		if (pointer->status == 0 && pointer->size >= processSize) {
			if (pointer->size < minSize) {
				minSize = pointer->size;
				minPointer = pointer;
			}
		}
		pointer = pointer->next;
	}

	return minPointer;
}

int bestFitAllocation(int processID, int processSize) {
	return allocate(processID, processSize, bestFitFindFreePartition(processSize));
}


int freeAllocation(int processID) {
	//根据输入的processID寻找进程所在的分区
	struct partitionNode* pointer = headNode;
	while (pointer) {
		if (pointer->processID == processID) {
			break;
		}
		pointer = pointer->next;
	}

	if (pointer == NULL){
	    //找不到
	    return 0;
	}


	if (pointer != headNode && pointer->previous->status == 0 &&
			pointer->previous->startingAddress + pointer->previous->size == pointer->startingAddress) {
			//前面紧接着一个size不为0的空闲分区
		struct partitionNode* preNode = pointer->previous;
		struct partitionNode* nextNode = pointer->next;

		preNode->size += pointer->size;
		preNode->next = pointer->next;
		nextNode->previous = preNode;


		if (pointer->next->status == 0 &&
				 pointer->startingAddress + pointer->size == pointer->next->startingAddress) {
				 //后面紧接着一个size不为0的空闲分区
			preNode->size += nextNode->size;
			preNode->next = nextNode->next;
			nextNode->next->previous = preNode;
			free(nextNode);
		}
		free(pointer);

	} else {
		if (pointer->next != NULL && pointer->next->status == 0 &&
				pointer->startingAddress + pointer->size == pointer->next->startingAddress) {
			//后面紧接着一个size不为0的空闲分区
			struct partitionNode* nextNode = pointer->next;
			pointer->size += nextNode->size;
			pointer->next = nextNode->next;
			if(nextNode->next!=NULL)nextNode->next->previous = pointer;
			//todo 解决删除唯一一个分区的bug
			pointer->status = 0;
			pointer->processID = -99;
			free(nextNode);
		} else {
		    //不和空闲分区相连
			pointer->status = 0;
			pointer->processID = -99;
		}
	}
	return 1;
}

void displayAllocation() {
	struct partitionNode* pointer = headNode;

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


void inputControl(int (*allocateAlgorithm)(int,int)) {
    //传入的参数是一个函数
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
			int allocateResult = allocateAlgorithm(processID, size);
			if (allocateResult) {
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
                int allocateResult = freeAllocation(processID);
                if (allocateResult) {
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
	headNode = initPartition();
	selectAlgorithm();
	return 0;
}
