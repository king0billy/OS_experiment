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


//������λ��p��Ϊ���ΪprocessID����СΪprocessSize����ҵ����ռ�
int allocate(int processID, int processSize, struct partitionNode* pointer) {
	if (pointer == NULL){
	    //û�п����ڴ���,����ʧ��
		return 0;
	}

	if (pointer->size == processSize) {
	    //��������ķ���
		pointer->status = 1;
		pointer->processID = processID;
	} else {
	    //�ָ���е��ڴ��ٷ���
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


//�״���Ӧ�㷨
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



//�����Ӧ�㷨
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
	//���������processIDѰ�ҽ������ڵķ���
	struct partitionNode* pointer = headNode;
	while (pointer) {
		if (pointer->processID == processID) {
			break;
		}
		pointer = pointer->next;
	}

	if (pointer == NULL){
	    //�Ҳ���
	    return 0;
	}


	if (pointer != headNode && pointer->previous->status == 0 &&
			pointer->previous->startingAddress + pointer->previous->size == pointer->startingAddress) {
			//ǰ�������һ��size��Ϊ0�Ŀ��з���
		struct partitionNode* preNode = pointer->previous;
		struct partitionNode* nextNode = pointer->next;

		preNode->size += pointer->size;
		preNode->next = pointer->next;
		nextNode->previous = preNode;


		if (pointer->next->status == 0 &&
				 pointer->startingAddress + pointer->size == pointer->next->startingAddress) {
				 //���������һ��size��Ϊ0�Ŀ��з���
			preNode->size += nextNode->size;
			preNode->next = nextNode->next;
			nextNode->next->previous = preNode;
			free(nextNode);
		}
		free(pointer);

	} else {
		if (pointer->next != NULL && pointer->next->status == 0 &&
				pointer->startingAddress + pointer->size == pointer->next->startingAddress) {
			//���������һ��size��Ϊ0�Ŀ��з���
			struct partitionNode* nextNode = pointer->next;
			pointer->size += nextNode->size;
			pointer->next = nextNode->next;
			if(nextNode->next!=NULL)nextNode->next->previous = pointer;
			//todo ���ɾ��Ψһһ��������bug
			pointer->status = 0;
			pointer->processID = -99;
			free(nextNode);
		} else {
		    //���Ϳ��з�������
			pointer->status = 0;
			pointer->processID = -99;
		}
	}
	return 1;
}

void displayAllocation() {
	struct partitionNode* pointer = headNode;

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


void inputControl(int (*allocateAlgorithm)(int,int)) {
    //����Ĳ�����һ������
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
			int allocateResult = allocateAlgorithm(processID, size);
			if (allocateResult) {
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
                int allocateResult = freeAllocation(processID);
                if (allocateResult) {
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
	headNode = initPartition();
	selectAlgorithm();
	return 0;
}
