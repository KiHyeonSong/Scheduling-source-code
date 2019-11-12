#include <stdio.h>
#pragma warning (disable :4996)

#define MAX 99
#define Q 1

typedef struct process{
	int id; // ���μ��� ID
	int reach; // �����ð�
	int service; // ���� �ð�
	int remain; // ���� �ð�
}process;

typedef struct Queue{
	int front;
	int rear;
	int count;
	process pro[MAX];   // ť�ȿ� �迭
}Queue;

void QueueInit(Queue* q) {   // ť ���� �ʱ�ȭ
	q->front = 0;
	q->rear = 0;
	q->count = 0;
	for (int i = 0; i < MAX; i++){
		q->pro[i].id = 0;   //���μ��� id 0���� �ʱ�ȭ �����Ⱚ ���� �Ⱦ.
	}
}

int QIsEmpty(Queue* q){  // ť ������� Ȯ��
	if (q->count == 0) return 1;
	else return 0;
}

int NextPosIdx(int pos) {  // �Լ� ��� �� �ε��� += 1 
	if (pos == MAX - 1)  // �迭�� ������ ����� �ε��� ���̶��
		return 0;
	else return pos + 1;
}

void Enqueue(Queue* q, process *p)
{   // ť ���� �迭 ������ ����
	if (NextPosIdx(q->rear) == q->front){
		fprintf(stdout, "Q full.\n");
		exit(-1);
	}
	
	if (q->count > 0 && q->pro[q->rear].id != p->id) {  // ť�� �Ѱ����ְ�,  ������ ������ ���� ���μ����� �ٽ� ���ö� 
		q->rear = NextPosIdx(q->rear);  // rear �� ĭ �̵�
	}
	q->pro[q->rear].id = p->id;
	q->pro[q->rear].reach = p->reach;
	q->pro[q->rear].service = p->service;
	q->pro[q->rear].remain = p->remain;
	
	q->count++;
}

struct process Dequeue(Queue* q)
{   // ����ü ���� ���ֱ�
	process temppro; // ��� ����ü ������ ���� �� ����ü ����
	if (QIsEmpty(q))
	{
		fprintf(stdout, "Q empty");
		exit(-1);
	}
	// ť ���� �迭 ù��° ������ �����ϱ��� temppro�� ����
	temppro = q->pro[0];

	for (int i = 0; i < q->rear; i++)
	{   // �迭 �� ĭ �� ������ ����
		q->pro[i] = q->pro[i + 1];
	}

	if (temppro.remain > Q)   // ť �ε���0 �����ð��� Q ���� ������
		q->pro[q->rear] = temppro;   // ť �������� �ٽ� �־��ش�.
	if (temppro.remain <= Q){  // �����ð��� Q���� �۰ų� ������
		if (q->rear == 0)   // ť�� ���������
			q->rear = 0;     // �״��
		else if (q->rear > 0)  //ť�� �Ѱ��� ������
			q->rear--;     //  ť�� ������ �ε��� -1;
	}
	q->count--;  // ť�� ���μ��� ���� 1����

	return temppro;  // �迭 ù��° ������ ����ü ����
}

void main()
{
	process pro[MAX];
	process temppro;   // �����Ҷ� �޾� ���� ����ü (�ӽ� ����ü)
	Queue queue;
	int i, j, k;   // �ݺ���
	int b = 0;
	int a = 0; // ť ���� �迭�� �� ����ü ����
	int pronum = 0;  // ����
	int turn = 0;    // ��ȯ
	int time = 0;     // �ð�
	int end = 0;     // ���� �ð�
	int temp = 0;    // ������ �̵� �� �� ���
	double normalization = 0;   // ����ȭ�� ��ȯ�ð�

	FILE* fp;
	fp = fopen("input.txt", "r");
	if (fp == NULL)
	{
		fprintf(stdout, "file not open");
		system("pause");
		exit(-1);
	}

	for (i = 0; i < MAX; i++){
		fscanf(fp, "%d,%d,%d", &pro[i].id, &pro[i].reach, &pro[i].service);
		if (pro[i].id == 0){
			break;
		}
		pronum++;
	}
	fclose(fp);

	for (i = 0; i < pronum; i++)   // ���� �ð� �� ����
	{
		for (j = i + 1; j < pronum; j++)
		{
			if (pro[i].reach > pro[j].reach)
			{
				temp = pro[i].id;
				pro[i].id = pro[j].id;
				pro[j].id = temp;

				temp = pro[i].reach;
				pro[i].reach = pro[j].reach;
				pro[j].reach = temp;

				temp = pro[i].service;
				pro[i].service = pro[j].service;
				pro[j].service = temp;
			}
		}
	}
	// �ּ� Ǯ��
	// fprintf(stdout, "�Է¹��� ���μ���\n");
	// fprintf(stdout, "�� ���μ��� ID ������ ���� �ð� ������ ���� �ð� ��\n");
	for (j = 0; j < pronum; j++) {
		fprintf(stdout, "�� \t%2d     ��      %2d     ��       %2d\t    ��\n", pro[j].id, pro[j].reach, pro[j].service);
		pro[j].remain = pro[j].service;
	}
	fprintf(stdout, "������������������������������������������������������������������������������������������\n");

	QueueInit(&queue);   // ť �ʱ�ȭ

	// �ּ� Ǯ��
	// fprintf(stdout, "���� ���μ��� ID ������ ���� �ð� ������ ���� �ð� ������ ���� �ð� ������ ��ȯ �ð� ������ ����ȭ�� ��ȯ �ð� ����\n");

	while (a != pronum) {  // ����Ϸ�� ���μ��� ������ pronum �� ���� �� �����.
		for (int i = b; i < pronum; i++) {  
			if (pro[i].reach <= time+Q && pro[i].remain > 0) { // �����ð��� ����ð� +Q���� �۰ų� ������, �����ð��� ������
				Enqueue(&queue, &pro[i]);  //ť�� �ֱ�
				//pro[i].reach = 9999;
				b++;  // ť�� ���� ���μ��� �������� üũ�ϱ� ����
			}
		}
		if (!QIsEmpty(&queue)) {  //ť�� ������� ������
			temppro = Dequeue(&queue);  //ť���� ����

			if (temppro.remain <= Q) { // �����ð��� Q���� �۰ų� ������
				time = time + temppro.remain;  // ����
				temppro.remain = 0; // ����Ϸ�� ���μ���, ������ 0���� �ʱ�ȭ

				if (temppro.remain == 0) { // ���μ����� ������ ���� �� ���
					turn = time - temppro.reach;
					normalization = turn / (temppro.service*1.0);
					fprintf(stdout, "�� \t%2d      ��      %2d     ��       %2d      ��    %3d      �� \t %3d      �� \t\t%.2lf\t ��\n", temppro.id, temppro.reach, temppro.service, time, turn, normalization);
				}
				a++; // ����Ϸ�� ���μ��� ���� ����
			}
			else if (temppro.remain > Q) {  // �����ð���  Q ���� ���� ����������
				time = time + Q;  // ����
				temppro.remain -= Q;
				Enqueue(&queue, &temppro);   // ���� �� ť�� �ֱ�
			}
		}
		else 
			printf("Q full empty \n");
	}


	fprintf(stdout, "����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	// printf("��� ���μ����� ����Ǿ����ϴ� \n");
	system("pause");
}