#include <stdio.h>
#pragma warning (disable :4996)

#define MAX 99
#define Q 1

typedef struct process{
	int id; // 프로세스 ID
	int reach; // 도착시간
	int service; // 서비스 시간
	int remain; // 남은 시간
}process;

typedef struct Queue{
	int front;
	int rear;
	int count;
	process pro[MAX];   // 큐안에 배열
}Queue;

void QueueInit(Queue* q) {   // 큐 내부 초기화
	q->front = 0;
	q->rear = 0;
	q->count = 0;
	for (int i = 0; i < MAX; i++){
		q->pro[i].id = 0;   //프로세스 id 0으로 초기화 쓰레기값 보기 싫어서.
	}
}

int QIsEmpty(Queue* q){  // 큐 비었는지 확인
	if (q->count == 0) return 1;
	else return 0;
}

int NextPosIdx(int pos) {  // 함수 사용 시 인덱스 += 1 
	if (pos == MAX - 1)  // 배열의 마지막 요소의 인덱스 값이라면
		return 0;
	else return pos + 1;
}

void Enqueue(Queue* q, process *p)
{   // 큐 내부 배열 데이터 생성
	if (NextPosIdx(q->rear) == q->front){
		fprintf(stdout, "Q full.\n");
		exit(-1);
	}
	
	if (q->count > 0 && q->pro[q->rear].id != p->id) {  // 큐에 한개라도있고,  수행후 끝나지 않은 프로세스가 다시 들어올때 
		q->rear = NextPosIdx(q->rear);  // rear 한 칸 이동
	}
	q->pro[q->rear].id = p->id;
	q->pro[q->rear].reach = p->reach;
	q->pro[q->rear].service = p->service;
	q->pro[q->rear].remain = p->remain;
	
	q->count++;
}

struct process Dequeue(Queue* q)
{   // 구조체 리턴 해주기
	process temppro; // 잠시 구조체 데이터 저장 할 구조체 선언
	if (QIsEmpty(q))
	{
		fprintf(stdout, "Q empty");
		exit(-1);
	}
	// 큐 내부 배열 첫번째 데이터 삭제하기전 temppro에 저장
	temppro = q->pro[0];

	for (int i = 0; i < q->rear; i++)
	{   // 배열 한 칸 씩 앞으로 당기기
		q->pro[i] = q->pro[i + 1];
	}

	if (temppro.remain > Q)   // 큐 인덱스0 남은시간이 Q 보다 많으면
		q->pro[q->rear] = temppro;   // 큐 마지막에 다시 넣어준다.
	if (temppro.remain <= Q){  // 남은시간이 Q보다 작거나 같으면
		if (q->rear == 0)   // 큐가 비어있으면
			q->rear = 0;     // 그대로
		else if (q->rear > 0)  //큐에 한개라도 있으면
			q->rear--;     //  큐에 마지막 인덱스 -1;
	}
	q->count--;  // 큐에 프로세스 갯수 1감소

	return temppro;  // 배열 첫번째 데이터 구조체 리턴
}

void main()
{
	process pro[MAX];
	process temppro;   // 리턴할때 받아 놓은 구조체 (임시 구조체)
	Queue queue;
	int i, j, k;   // 반복문
	int b = 0;
	int a = 0; // 큐 내부 배열에 들어간 구조체 갯수
	int pronum = 0;  // 갯수
	int turn = 0;    // 반환
	int time = 0;     // 시간
	int end = 0;     // 끝난 시간
	int temp = 0;    // 데이터 이동 할 때 사용
	double normalization = 0;   // 정규화된 반환시간

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

	for (i = 0; i < pronum; i++)   // 도착 시간 순 정렬
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
	// 주석 풀기
	// fprintf(stdout, "입력받은 프로세스\n");
	// fprintf(stdout, "┏ 프로세스 ID ━┳━ 도착 시간 ━┳━ 서비스 시간 ┓\n");
	for (j = 0; j < pronum; j++) {
		fprintf(stdout, "┃ \t%2d     ┃      %2d     ┃       %2d\t    ┃\n", pro[j].id, pro[j].reach, pro[j].service);
		pro[j].remain = pro[j].service;
	}
	fprintf(stdout, "┗━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┛\n");

	QueueInit(&queue);   // 큐 초기화

	// 주석 풀기
	// fprintf(stdout, "┏━ 프로세스 ID ━┳━ 도착 시간 ━┳━ 서비스 시간 ━┳━ 종료 시간 ━┳━ 반환 시간 ━┳━ 정규화된 반환 시간 ━┓\n");

	while (a != pronum) {  // 수행완료된 프로세스 갯수가 pronum 과 같을 때 벗어난다.
		for (int i = b; i < pronum; i++) {  
			if (pro[i].reach <= time+Q && pro[i].remain > 0) { // 도착시간이 현재시간 +Q보다 작거나 같으면, 남은시간이 있으면
				Enqueue(&queue, &pro[i]);  //큐에 넣기
				//pro[i].reach = 9999;
				b++;  // 큐에 넣은 프로세스 다음부터 체크하기 위함
			}
		}
		if (!QIsEmpty(&queue)) {  //큐가 비어있지 않으면
			temppro = Dequeue(&queue);  //큐에서 꺼냄

			if (temppro.remain <= Q) { // 남은시간이 Q보다 작거나 같으면
				time = time + temppro.remain;  // 연산
				temppro.remain = 0; // 수행완료된 프로세스, 음수도 0으로 초기화

				if (temppro.remain == 0) { // 프로세스가 끝나면 연산 후 출력
					turn = time - temppro.reach;
					normalization = turn / (temppro.service*1.0);
					fprintf(stdout, "┃ \t%2d      ┃      %2d     ┃       %2d      ┃    %3d      ┃ \t %3d      ┃ \t\t%.2lf\t ┃\n", temppro.id, temppro.reach, temppro.service, time, turn, normalization);
				}
				a++; // 수행완료된 프로세스 갯수 증가
			}
			else if (temppro.remain > Q) {  // 남은시간이  Q 보다 많이 남아있으면
				time = time + Q;  // 연산
				temppro.remain -= Q;
				Enqueue(&queue, &temppro);   // 연산 후 큐에 넣기
			}
		}
		else 
			printf("Q full empty \n");
	}


	fprintf(stdout, "┗━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┛\n");
	// printf("모든 프로세스가 수행되었습니다 \n");
	system("pause");
}