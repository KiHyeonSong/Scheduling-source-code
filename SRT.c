#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#define MAX 99

struct process {
	int num;      // 프로세스 넘버
	int arrive;   // 도착시간
	int service;  // 서비스 시간
	int respone;  // 응답시간 = 남은 시간
};		



void main()
{
	FILE* fp;
	fp = fopen("input.txt","r");

	if (fp == NULL) {
		printf("no input file \n");
		return 0;
	}

	struct process pro[MAX] = { 0 };
	int end = 0;                // 끝난 시간
	int i = 0;
	int flag = 0;               // 시작, 종료 & 프로세스 갯수 99개 판별
	int smallest = 0;;			// smallest:버스트시간 낮은순으로 정렬 
	int remain = 0;             // 수행완료된 프로세스 갯수
	int pronum = 0;             // 입력 받은 프로세스 갯수
	int time = 0;               // 현재 시간
	int wait = 0;               // 기다린 시간
	int turn= 0;		//remain:도착하고 남은것   wait:대기시간 총합 sum_turnaround:반환시간 총합 
	double normalization = 0;   // 정규화된 반환시간
 	
	for (i = 0; i < MAX; i++)
	{
		fscanf(fp, "%d,%d,%d", &pro[i].num, &pro[i].arrive, &pro[i].service);
		if (pro[i].num == 0)
		{  // 프로세스 정상 실행
			flag = 1;
			break;
		}
		if (pro[i].num >= 100)
		{   // 프로세스 갯수 100이상 이므로 실행x
			flag = 0;
			break;
		}
		pro[i].respone = pro[i].service;
		
		pronum++;  // 프로세스 갯수 설정 중
	}
	fclose(fp);
	
	// 주석 풀기
	//fprintf(stdout, "[ SRT 스케줄링 ] \n\n");
	// fprintf(stdout, "--------------입력받은 프로세스--------------\n");
	// fprintf(stdout, "┏ 프로세스 ID ━┳━ 도착 시간 ━┳━ 서비스 시간 ┓\n");
	// 입력 받은 프로세스 출력
	for (int i = 0; i < pronum; i++)
	{
		fprintf(stdout, "┃ \t%2d     ┃      %2d     ┃       %2d\t    ┃\n", pro[i].num, pro[i].arrive, pro[i].service);
	}
	fprintf(stdout, "┗━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┛\n");

	switch(flag)
	{
	case 1:
	{
		//printf("\n\nProcess\t|   반환시간   | 대기시간 \n\n");	 
		// 주석 풀기
		// fprintf(stdout, "┏━ 프로세스 ID ━┳━ 도착 시간 ━┳━ 서비스 시간 ━┳━ 종료 시간 ━┳━ 반환 시간 ━┳━ 정규화된 반환 시간 ━┓\n");
		pro[pronum].respone = 9999;
		for (time = 0; remain != pronum; time++) //time:반복  remain:도착하고 남은것  smallest:버스트시간 낮은순으로 정렬 
		{
			smallest = pronum;
			for (i = 0; i < pronum; i++)
			{
				if (pro[i].arrive <= time && pro[i].respone < pro[smallest].respone && pro[i].respone>0) 
				{		// 프로세스의 도착시간이 현재 시간보다 작고, 지금까지 버스트 시간이 제일 작은 프로세스보다 남은 시간이 적고, 남은 실행시간이 0보다 큰 프로세스 선택
					smallest = i;
				}
			}
			pro[smallest].respone--; // 남은 실행시간이 제일 작은 프로세스의 수행
			if (pro[smallest].respone == 0) // 프로세스의 수행이 모두 완료되었을 때
			{
				remain++; // 수행완료된 프로세스의 갯수 1 증가
				end = time + 1;
				normalization = (double)(end - pro[smallest].arrive) / (double)pro[smallest].service;
				// 정규화된 반환 시간 = 반환시간 / 서비스 시간
				fprintf(stdout, "┃ \t%2d      ┃      %2d     ┃       %2d      ┃    %3d      ┃ \t %3d      ┃ \t\t%.2lf\t ┃\n", smallest + 1, pro[smallest].arrive, pro[smallest].service, end, end - pro[smallest].arrive , normalization);

			    // printf("\n  %d \t|\t%d\t|\t%d", smallest + 1, end - pro[smallest].arrive, end - pro[smallest].service - pro[smallest].arrive);
				wait += end - pro[smallest].service - pro[smallest].arrive;
				turn += end - pro[smallest].arrive;
			}
		}
		fprintf(stdout, "┗━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┛\n");
		// printf("\n\n평균 대기 시간 = %.3f\n", wait*1.0 / pronum);
		// printf("평균 반환 시간 = %.3f\n\n", turn*1.0 / pronum);
	}
	case 0:
	{
		break;
	}
	}
	system("pause");
	return 0;
}