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


//������λ��p��Ϊ���ΪprocessID����СΪprocessSize����ҵ����ռ�
int allocation(int processID, int processSize, partitionList pointer) {
	if (pointer == NULL)   //�޺��ʿռ�ɷ���
		return 0;

	if (pointer->size == processSize) {   //�����������
		pointer->status = ALLOCATED;
		pointer->processID = processID;
	} else {                  //�ָ����
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


//�״���Ӧ�㷨
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



//�����Ӧ�㷨
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
	//Ѱ����ҵ���ڷ���
	partitionList pointer = head;

	while (pointer) {
		if (pointer->processID == processID) {
			break;
		}
		pointer = pointer->next;
	}

	if (pointer == NULL)
		return 0;

	//�����׿��������ǰһ���з�������
	if (pointer != head && pointer->previous->status == FREE &&
			pointer->previous->startingAddress + pointer->previous->size == pointer->startingAddress) {
		partitionList preNode = pointer->previous;
		partitionList nextNode = pointer->next;

		preNode->size += pointer->size;
		preNode->next = pointer->next;
		nextNode->previous = preNode;

		//���һ���з�������
		if (pointer->next->status == FREE &&
				 pointer->startingAddress + pointer->size == pointer->next->startingAddress) {
			preNode->size += nextNode->size;
			preNode->next = nextNode->next;
			nextNode->next->previous = preNode;

			free(nextNode);
		}

		free(pointer);

	} else {
		//�������һ�����������һ���з�������
		if (pointer->next != NULL && pointer->next->status == FREE &&
				pointer->startingAddress + pointer->size == pointer->next->startingAddress) {
			partitionList nextNode = pointer->next;
			pointer->size += nextNode->size;
			pointer->next = nextNode->next;
			if(nextNode->next!=NULL)nextNode->next->previous = pointer;
			//todo ���ɾ��Ψһһ��������bug
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

	printf("\n|%4s\t|%3s\t|%3s\t|%3s\n", "������","��ʼ��ַ", "������С", "����״̬");

	while (pointer) {
		printf("|%3d\t|%3d\t\t|%3d\t\t|",pointer->processID, pointer->startingAddress,pointer->size);
		if(pointer->status==1)printf("�ѷ���");
		else printf("����");
		printf("\n");
		pointer = pointer->next;
	}
	printf("\n");
}


void inputControl(int (*allocAlgorithm)(int,int)) {
	printf("���밴 1 �ͷŰ� 2 �˳��� 9\n");
	char T[5];
	scanf("%s", T);
	while (T[0] != '9') {
		if (T[0] == '1') {
			int processID, size;
			printf("����Ҫ����ռ����ҵid: ");
			scanf("%d", &processID );
            printf("�����С: ");
            scanf("%d", &size);
			int ret = allocAlgorithm(processID, size);
			if (ret) {
				printf("����ɹ�");
				displayAllocation();
			}
			else {
				printf("\n�ڴ治�� ����ʧ��\n\n");
			}
		} else{
		    if (T[0] == '2') {
                int processID;
                printf("����Ҫ�ͷſռ����ҵid: ");scanf("%d", &processID);
                int ret = freeAllocation(processID);
                if (ret) {
                    printf("�ͷųɹ�");
                    displayAllocation();
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


void firstFitAllocationControl() {
		inputControl(firstFitAllocation);
}

void bestFitAllocationControl() {
		inputControl(bestFitAllocation);
}



void selectAlgorithm() {
	system("cls");
	printf("��1_�״���Ӧ�㷨                ");
	printf("��2_�����Ӧ�㷨             \n");

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
