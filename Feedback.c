#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX 99
#define Q 1

typedef struct process //���μ��� ������
{
	int id; //PID
	int reach; //�����ð� 
	int service; //���� �ð�
	int remain; //���� �ð�
	int wait; // ���ð�
	int turn_around; //��ȯ �ð�
	int end; // ����ð�
	double nomalizedtime; //����ȭ�� ��ȯ�ð�
}process;

typedef struct ListNode { //��� ����ü
	struct process pro; //��忡 ����� ���μ��� ������
	struct ListNode *link; //���� ��带 ����Ű�� �ּ� ��
}ListNode;

typedef struct Queue {
	ListNode *front, *last; //�� �հ� �� �ڸ� ����Ŵ
	int count;	
}Queue;

void initQueue(Queue *q) {
	q->front = NULL;
	q->last = NULL;
	q->count = 0; 
}

int isEmpty(Queue *q) {
	return q->count == 0;
}

void enqueue(Queue *q, process *p) { // ť�� ���μ��� q�� ����
	ListNode *new_node = (ListNode*)malloc(sizeof(ListNode)); // ���ο� ��� ����
	new_node->pro.id = p->id; // ��� �ʱ�ȭ
	new_node->pro.reach = p->reach;
	new_node->pro.service = p->service;
	new_node->pro.remain = p->remain;
	new_node->pro.turn_around = p->turn_around;
	new_node->pro.wait = p->wait;
	new_node->pro.end = p->end;
	new_node->pro.nomalizedtime = p->nomalizedtime;
	new_node->link = NULL;

	if (isEmpty(q)) { //ť�� ������� ��
		q->front = new_node;
	}
	else { //ť�� ������� ���� ��
		q->last->link = new_node;
	}
	q->last = new_node; //�� �� ��带 last�� ����
	q->count++; //���� 1 ����
}

struct process dequeue(Queue *q) {
	ListNode *now;
	process p;

	if (isEmpty(q)) {
		return ;
	}
	now = q->front;
	p = now->pro;

	q->front = now->link;
	free(now);

	q->count--;

	return p;
}

int main() {
	process pro[MAX]; // ó�� ���ĵ� ���μ������� �޾ƿ� ���μ��� �迭
	process next;  // ���� ���μ��� ������ ���μ��� ����ü
	Queue queue[5]; // 5���� ť ����
	Queue result; // ���� �Ϸ�� ���μ������� ����Ǵ� ť
	process pop_pro; // dequeue ���� �� �޴� ���μ��� ����ü

	int i, j, k;
	int end = 0;
	int tmp; //���� �ӽú���
	int pronum = 0; //���μ��� ����
	int time = 0; // ���� �ð�
	FILE *f;

	f = fopen("input.txt", "r");  //�ش� ������ �б���� open
	if (f == NULL) {              //���� open����
		printf("file open fail\n");
		return 1;
	}
	for (i = 0; i < MAX; i++) {        //���μ����� 15������ ����ü �迭�� ����
		fscanf(f, "%d,%d,%d", &pro[i].id, &pro[i].reach, &pro[i].service);
	}
	fclose(f);
	for (i = 0; i < MAX; i++) {			//���μ��� ID�� 0�� ���ö����� ���μ��� ������ ����
		if (pro[i].id == 0) {
			break;
		}
		else {
			pronum++;
		}
	}

	for (j = 0; j < pronum; j++) {
		for (k = j + 1; k < pronum; k++) {
			if (pro[j].reach > pro[k].reach) {       //���μ��� �����ð����� �������� ����
				tmp = pro[j].reach;
				pro[j].reach = pro[k].reach;
				pro[k].reach = tmp;

				tmp = pro[j].id;
				pro[j].id = pro[k].id;
				pro[k].id = tmp;

				tmp = pro[j].service;
				pro[j].service = pro[k].service;
				pro[k].service = tmp;


			}
		}
	}

	// ���� �ּ� Ǯ��
	// fprintf(stdout, "[ Feedback �����층 ]\n\n");
	// fprintf(stdout, "�Է¹��� ���μ��� \n");
	// fprintf(stdout, "�� ���μ��� ID ������ ���� �ð� ������ ���� �ð� ��\n");

	for (j = 0; j < pronum; j++) {
		fprintf(stdout, "�� \t%2d     ��      %2d     ��       %2d\t    ��\n", pro[j].id, pro[j].reach, pro[j].service);
		pro[j].remain = pro[j].service;
		pro[j].wait = 0;
		pro[j].turn_around = 0;
		pro[j].end = 0;
		pro[j].nomalizedtime = 0.0;
	}
	fprintf(stdout, "������������������������������������������������������������������������������������������\n");
	// ť �ʱ�ȭ 
	for (i = 0; i < 5; i++) {
		initQueue(&queue[i]);
	}
	initQueue(&result);

	j = 0;

	while (result.count != pronum) {
		for (i = 0; i < pronum; i++) { //�����ð��� ���� enqueue, (Q���� �˻�)
			if (time >= pro[i].reach) { // �����ð��� ���� �ð����� ���ų� ū ���μ����� ������ 
				// printf("���ο� ���μ��� ���� \n");
				enqueue(&queue[0], &pro[i]); // ù��° ť�� enqueue 
				pro[i].reach = 1000; // ť�� enqueue ������ �� �̻� �Ȱɸ��� 1000���� �ʱ�ȭ
				next = pro[i + 1]; // ���� ���μ����� next ���μ��� ������ ����
				if (i >= pronum-1) {
					next = pro[i];
				}
			}
		}

		// printf("���μ��� ���� \n");
		// ���μ��� ����
		while (j < pronum) {
			if (!isEmpty(&queue[j])) { // ť�� ������� ������ == ť�� ��������
				pop_pro = dequeue(&queue[j]); //ť 0������ 4������ �˻��ϰ� �ش� ť dequeue

				if (isEmpty(&queue[j])) {
					initQueue(&queue[j]);
				}
				// printf("in Queue %d \n", j);
				pop_pro.remain -= Q; // �ش� ���μ��� ���� �ð� q��ŭ ����

				if (pop_pro.remain > 0) { // ���� �ð��� �ִٸ�
					k = j;
					while (k < 5) {
						if (!isEmpty(&queue[k])) { // ����� ���� ť�� ���μ����� ������
							if (j == (5 - 1) && k == (5 - 1)) {
								enqueue(&queue[j], &pop_pro);
								// printf("���� ť�� ����1 \n");
							}
							else {
								enqueue(&queue[j + 1], &pop_pro); // ���� ť�� �ֱ�
								// printf("���� ť�� ���� \n");
							}
							break;
						}
						if (k == (5 - 1) && queue[j].count == 0) {
							if (time + Q >= next.reach) { // ���� ������ ������ �ٷ� ������ ���μ����� ������ 
								enqueue(&queue[j + 1], &pop_pro); // ���� ť�� �ֱ�
								// printf("���� ť�� ���� \n");
							}
							else {
								enqueue(&queue[j], &pop_pro);
								// printf("���� ť�� ����2 \n");
							}
						}
						k++;
					}
				}
				else { // ���� �ð��� ���ٸ� 
					pop_pro.end = time + 1;
					pop_pro.wait = pop_pro.end - pop_pro.service - pop_pro.reach;
					pop_pro.turn_around = pop_pro.end - pop_pro.reach;
					pop_pro.nomalizedtime = (double)pop_pro.turn_around / (double)pop_pro.service;
					enqueue(&result, &pop_pro); // result ť�� enqueue
					// printf("result count = %d \n", result.count);
					// printf("���μ��� ����Ϸ� \n");
				}
				j = 0; //�ٽ� ù��° ť���� Ȯ���ϱ� ���� 0���� �ʱ�ȭ
				time += Q; // ����ð� Q��ŭ ����
				break;

			}
			else { // ť�� ���������
				// printf("hello \n");
				if (j >= (5 - 1)) {
					// printf("��� ť�� ����ֽ��ϴ�. \n");
					j = 0;
					time++; // ���� �ð� ����
					break;
				}

				j++;
				continue;
			}
		}
	}

	// ���� �ּ� Ǯ��
	// fprintf(stdout, "���� ���μ��� ID ������ ���� �ð� ������ ���� �ð� ������ ���� �ð� ������ ��ȯ �ð� ������ ����ȭ�� ��ȯ �ð� ����\n");
	while (!isEmpty(&result)) { // ��� ����� ���μ��� dequeue
		pop_pro = dequeue(&result);
		fprintf(stdout, "�� \t%2d      ��      %2d     ��       %2d      ��    %3d      �� \t %3d      �� \t\t%.2lf\t ��\n",
			pop_pro.id, pop_pro.reach, pop_pro.service, pop_pro.end, pop_pro.turn_around, pop_pro.nomalizedtime);
	}
	fprintf(stdout, "����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	// printf("��� ���μ����� ����Ǿ����ϴ� \n");
	system("pause");

	return 0;
	
}