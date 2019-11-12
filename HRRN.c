#include <stdio.h>
#pragma warning (disable :4996)
#define MAX 110
#define KINDS 4 

FILE* fp;

// void Descending(int(*pro)[KINDS]);  //일케 해도된다.
void Descending(int pro[][KINDS], int pronum);
double response(int wait, int servise);    // 응답률\

void main()
{
	int pronum = 0;					 // 프로세스 갯수
	int start = 0;                   // 시작 버튼
	int flag = 1;                    // 100개 유/무 판단
	int wait = 0;                    // 기다린 시간
	int end = 0;                     // 종료 시간
	double turn = 0;                 // 반환 시간
	int responnum = 0;               // 현재 실행한 프로세스 갯수
	int I_temp = 0;                  // int형 temp
	double D_temp = 0;               // double형 temp


	fp = fopen("E:\\inputtest.txt", "r");

	int process[MAX][KINDS] = { NULL };
	// process[0][0]  = 프로세스 이름
	// process[0][1]  = 도착시간
	// process[0][2]  = 서비스시간  
	// process[0][3]  = 기다린 시간
	double respon[MAX] = { 0 };
	// 응답률 계산후 저장

	for (int i = 0; i < MAX; i++)
	{
		fscanf(fp, "%d,%d,%d", &process[i][0], &process[i][1], &process[i][2]);
		fscanf(stdin, "%d,%d,%d", &process[i][0], &process[i][1], &process[i][2]);
		
		if (process[i][0] == 0)
		{
			break;
		}
		else if (process[i][0] >= 100)
		{
			// 주석 풀기
			// fprintf(stdout, "프로세스가 100개 넘었습니다.\n종료합니다.\n");
			flag = 0;    // 프로세스 갯수가 99개 넘으면 실행 안하기
			break;
		}
		pronum++; // 한줄 읽을 때 마다 프로세스 갯수 1개 씩 추가
	}
	fclose(fp);

	if (flag == 1)
	{    // 입력받은 프로세스 출력하기

		// 주석 풀기
		// fprintf(stdout, "\t\t    입력된 프로세스\n");
		// fprintf(stdout, "┏ 프로세스 아이디 ━┳━ 도착시간 ━┳━ 서비스 받은시간 ┓\n");
		for (int j = 0; j < pronum; j++)
		{
			fprintf(stdout, "┃\t%d\t   ┃\t %d\t┃\t %d\t   ┃\n", 
				process[j][0], process[j][1],process[j][2]);
		}
		fprintf(stdout, "┗━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┛\n");
		
		// 주석 풀기
		// fprintf(stdout, "HRRN 스케줄링\n");
		// fprintf(stdout, "시작 : 1\n종료 : 0\n");
		fscanf(stdin, "%d", &start);
	}
	switch (start)
	{
	case 1:
	{
		// 주석 풀기
		// fprintf(stdout, "┏  아이디 ━┳━ 도착 시간 ━┳━ 서비스 시간 ━┳━ 종료 시간 ━┳━ 반환 시간 ━┳━ 정규화된 반환 시간 ┓\n");
		Descending(process,pronum);  // 도착 시간 순으로 정렬
		for (int i = 0; i < pronum; i++)
		{
			if (process[i][1] == 0)    // 도착 시간이 0 인경우
			{ 
				turn = end - process[i][1] + process[i][2];     // 도착시간이 0인것이 2개 이상일 경우를 대비함
				end = end + process[i][1] + process[i][2];      // 끝난 시간 = 도착 시간 + 서비스 시간
				
				respon[i] = response(process[i][3], process[i][2]);     // 함수로 들어가 응답률을 계산함
				fprintf(stdout, "┃    %2d    ┃     %2d\t ┃      %2d\t ┃     %3d     ┃   %6.2lf    ┃       %6.2lf        ┃\n",
					process[i][0], process[i][1], process[i][2], end, turn, respon[i]);	
				process[i][3] = end - process[i][1];       // wait 기다린 시간을 계산하여 저장한다.
				responnum++;  // 도착시간 0 인거 몇개 실행했는지 갯수저장 현재 2개
				
			}
			else if(process[i][1] != 0)    // 도착 시간이 0이 아닌 경우
			{
				for (int j = 0; j < pronum -responnum; j++)     // 입력받은 프로세스 -출력한 프로세스 갯수 만큼 루틴
				{  
					if (end < process[responnum + j][1])     // 지금까지 실행하면서 그 중간에 들어온 것이 없으면 나가기 
					{
						break;
					}
					else if (end > process[responnum + j][1])  
					{		// 지금까지 실행 하면서 중간에 들어 온것이 있으면 기다린 시간과 응답률을 계산후 저장
						process[responnum+j][3] = end - process[responnum + j][1];   // 기다린 시간 저장
						respon[responnum + j] = response(process[responnum + j][3], process[responnum + j][2]);  // 응답률 계산 후 저장
					}
				}
				for (int i = responnum; i < pronum; i++)
				{    // 실행한것 다음 부터  입력 받은 프로세스 갯수 까지 
					for (int j = i + 1; j < pronum; j++)
					{   // i 다음번 프로세스부터 
						if (respon[i] < respon[j])
						{   // 응답률를 비교하여 먼저 수행해야할 프로세스를 우선순위로 지정 해준다.
							// 스왑 단계
							I_temp = process[i][0];
							process[i][0] = process[j][0];
							process[j][0] = I_temp;

							I_temp = process[i][1];
							process[i][1] = process[j][1];
							process[j][1] = I_temp;

							I_temp = process[i][2];
							process[i][2] = process[j][2];
							process[j][2] = I_temp;

							D_temp = respon[i];  // 응답률 까지 스왑
							respon[i] = respon[j];
							respon[j] = D_temp;
						}
					}
				}
				turn = end - process[i][1] + process[i][2];  //  반환 시간
				end = end + process[i][2];    // 끝난 시간
				responnum++;    // 사용한 프로세스 갯수 추가

				fprintf(stdout, "┃    %2d    ┃     %2d\t ┃      %2d\t ┃     %3d     ┃   %6.2lf    ┃       %6.2lf        ┃\n",
					process[i][0], process[i][1], process[i][2], end, turn, respon[i]);	 // 출력
				
			}
			process[i][0] = NULL;  // 사용 후 초기화
			process[i][1] = NULL;
			process[i][2] = NULL;
			process[i][3] = NULL;
			// respon[i] = 0.0;
		}
		fprintf(stdout, "┗━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━┛\n");
	}
	case 0:
	{   //  프로세스 갯수가 99개가 넘으면 실행x
		break;
	}
	}
	system("pause");
}

void Descending(int pro[][KINDS], int pronum)  // 도착시간 내림차순으로 정리하기
{
	int temp;
	for (int i = 0; i < pronum; i++)
	{
		for (int j = i + 1; j < pronum; j++)
		{
			if (pro[i][1] > pro[j][1])
			{
				temp = pro[i][0];
				pro[i][0] = pro[j][0];
				pro[j][0] = temp;

				temp = pro[i][1];
				pro[i][1] = pro[j][1];
				pro[j][1] = temp;

				temp = pro[i][2];
				pro[i][2] = pro[j][2];
				pro[j][2] = temp;
			}
		}
	}
}

double response(int wait, int servise)
{   // 응답률 계산하는 함수
	double respon = 0;
	respon = (double)(wait+servise) / (double)servise;
	return respon;
}