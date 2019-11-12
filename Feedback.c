#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX 99
#define Q 1

typedef struct process //프로세스 데이터
{
	int id; //PID
	int reach; //도착시간 
	int service; //서비스 시간
	int remain; //남은 시간
	int wait; // 대기시간
	int turn_around; //반환 시간
	int end; // 종료시간
	double nomalizedtime; //정규화된 반환시간
}process;

typedef struct ListNode { //노드 구조체
	struct process pro; //노드에 저장될 프로세스 데이터
	struct ListNode *link; //다음 노드를 가리키는 주소 값
}ListNode;

typedef struct Queue {
	ListNode *front, *last; //맨 앞과 맨 뒤를 가리킴
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

void enqueue(Queue *q, process *p) { // 큐에 프로세스 q값 넣음
	ListNode *new_node = (ListNode*)malloc(sizeof(ListNode)); // 새로운 노드 생성
	new_node->pro.id = p->id; // 노드 초기화
	new_node->pro.reach = p->reach;
	new_node->pro.service = p->service;
	new_node->pro.remain = p->remain;
	new_node->pro.turn_around = p->turn_around;
	new_node->pro.wait = p->wait;
	new_node->pro.end = p->end;
	new_node->pro.nomalizedtime = p->nomalizedtime;
	new_node->link = NULL;

	if (isEmpty(q)) { //큐가 비어있을 때
		q->front = new_node;
	}
	else { //큐가 비어있지 않을 때
		q->last->link = new_node;
	}
	q->last = new_node; //맨 뒤 노드를 last로 설정
	q->count++; //개수 1 증가
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
	process pro[MAX]; // 처음 정렬된 프로세스들을 받아올 프로세스 배열
	process next;  // 다음 프로세스 저장할 프로세스 구조체
	Queue queue[5]; // 5개의 큐 생성
	Queue result; // 수행 완료된 프로세스들이 저장되는 큐
	process pop_pro; // dequeue 했을 때 받는 프로세스 구조체

	int i, j, k;
	int end = 0;
	int tmp; //스왑 임시변수
	int pronum = 0; //프로세스 갯수
	int time = 0; // 현재 시간
	FILE *f;

	f = fopen("input.txt", "r");  //해당 파일을 읽기모드로 open
	if (f == NULL) {              //파일 open실패
		printf("file open fail\n");
		return 1;
	}
	for (i = 0; i < MAX; i++) {        //프로세스를 15개까지 구조체 배열에 저장
		fscanf(f, "%d,%d,%d", &pro[i].id, &pro[i].reach, &pro[i].service);
	}
	fclose(f);
	for (i = 0; i < MAX; i++) {			//프로세스 ID가 0이 나올때까지 프로세스 갯수를 측정
		if (pro[i].id == 0) {
			break;
		}
		else {
			pronum++;
		}
	}

	for (j = 0; j < pronum; j++) {
		for (k = j + 1; k < pronum; k++) {
			if (pro[j].reach > pro[k].reach) {       //프로세스 도착시간으로 내림차순 정렬
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

	// 여기 주석 풀기
	// fprintf(stdout, "[ Feedback 스케쥴링 ]\n\n");
	// fprintf(stdout, "입력받은 프로세스 \n");
	// fprintf(stdout, "┏ 프로세스 ID ━┳━ 도착 시간 ━┳━ 서비스 시간 ┓\n");

	for (j = 0; j < pronum; j++) {
		fprintf(stdout, "┃ \t%2d     ┃      %2d     ┃       %2d\t    ┃\n", pro[j].id, pro[j].reach, pro[j].service);
		pro[j].remain = pro[j].service;
		pro[j].wait = 0;
		pro[j].turn_around = 0;
		pro[j].end = 0;
		pro[j].nomalizedtime = 0.0;
	}
	fprintf(stdout, "┗━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┛\n");
	// 큐 초기화 
	for (i = 0; i < 5; i++) {
		initQueue(&queue[i]);
	}
	initQueue(&result);

	j = 0;

	while (result.count != pronum) {
		for (i = 0; i < pronum; i++) { //도착시간에 따라 enqueue, (Q마다 검사)
			if (time >= pro[i].reach) { // 도착시간이 현재 시간보다 같거나 큰 프로세스가 있으면 
				// printf("새로운 프로세스 도착 \n");
				enqueue(&queue[0], &pro[i]); // 첫번째 큐에 enqueue 
				pro[i].reach = 1000; // 큐에 enqueue 했으니 더 이상 안걸리게 1000으로 초기화
				next = pro[i + 1]; // 다음 프로세스를 next 프로세스 변수에 저장
				if (i >= pronum-1) {
					next = pro[i];
				}
			}
		}

		// printf("프로세스 수행 \n");
		// 프로세스 수행
		while (j < pronum) {
			if (!isEmpty(&queue[j])) { // 큐가 비어있지 않으면 == 큐가 차있으면
				pop_pro = dequeue(&queue[j]); //큐 0번부터 4번까지 검사하고 해당 큐 dequeue

				if (isEmpty(&queue[j])) {
					initQueue(&queue[j]);
				}
				// printf("in Queue %d \n", j);
				pop_pro.remain -= Q; // 해당 프로세스 남은 시간 q만큼 감소

				if (pop_pro.remain > 0) { // 남은 시간이 있다면
					k = j;
					while (k < 5) {
						if (!isEmpty(&queue[k])) { // 현재와 하위 큐에 프로세스가 있으면
							if (j == (5 - 1) && k == (5 - 1)) {
								enqueue(&queue[j], &pop_pro);
								// printf("현재 큐에 넣음1 \n");
							}
							else {
								enqueue(&queue[j + 1], &pop_pro); // 다음 큐에 넣기
								// printf("다음 큐에 넣음 \n");
							}
							break;
						}
						if (k == (5 - 1) && queue[j].count == 0) {
							if (time + Q >= next.reach) { // 현재 수행이 끝나고 바로 도착할 프로세스가 있으면 
								enqueue(&queue[j + 1], &pop_pro); // 다음 큐에 넣기
								// printf("다음 큐에 넣음 \n");
							}
							else {
								enqueue(&queue[j], &pop_pro);
								// printf("현재 큐에 넣음2 \n");
							}
						}
						k++;
					}
				}
				else { // 남은 시간이 없다면 
					pop_pro.end = time + 1;
					pop_pro.wait = pop_pro.end - pop_pro.service - pop_pro.reach;
					pop_pro.turn_around = pop_pro.end - pop_pro.reach;
					pop_pro.nomalizedtime = (double)pop_pro.turn_around / (double)pop_pro.service;
					enqueue(&result, &pop_pro); // result 큐에 enqueue
					// printf("result count = %d \n", result.count);
					// printf("프로세스 수행완료 \n");
				}
				j = 0; //다시 첫번째 큐부터 확인하기 위해 0으로 초기화
				time += Q; // 현재시간 Q만큼 증가
				break;

			}
			else { // 큐가 비어있으면
				// printf("hello \n");
				if (j >= (5 - 1)) {
					// printf("모든 큐가 비어있습니다. \n");
					j = 0;
					time++; // 현재 시간 증가
					break;
				}

				j++;
				continue;
			}
		}
	}

	// 여기 주석 풀기
	// fprintf(stdout, "┏━ 프로세스 ID ━┳━ 도착 시간 ━┳━ 서비스 시간 ━┳━ 종료 시간 ━┳━ 반환 시간 ━┳━ 정규화된 반환 시간 ━┓\n");
	while (!isEmpty(&result)) { // 모두 수행된 프로세스 dequeue
		pop_pro = dequeue(&result);
		fprintf(stdout, "┃ \t%2d      ┃      %2d     ┃       %2d      ┃    %3d      ┃ \t %3d      ┃ \t\t%.2lf\t ┃\n",
			pop_pro.id, pop_pro.reach, pop_pro.service, pop_pro.end, pop_pro.turn_around, pop_pro.nomalizedtime);
	}
	fprintf(stdout, "┗━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┛\n");
	// printf("모든 프로세스가 수행되었습니다 \n");
	system("pause");

	return 0;
	
}