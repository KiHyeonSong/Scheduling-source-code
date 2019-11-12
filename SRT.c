#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#define MAX 99

struct process {
	int num;      // ���μ��� �ѹ�
	int arrive;   // �����ð�
	int service;  // ���� �ð�
	int respone;  // ����ð� = ���� �ð�
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
	int end = 0;                // ���� �ð�
	int i = 0;
	int flag = 0;               // ����, ���� & ���μ��� ���� 99�� �Ǻ�
	int smallest = 0;;			// smallest:����Ʈ�ð� ���������� ���� 
	int remain = 0;             // ����Ϸ�� ���μ��� ����
	int pronum = 0;             // �Է� ���� ���μ��� ����
	int time = 0;               // ���� �ð�
	int wait = 0;               // ��ٸ� �ð�
	int turn= 0;		//remain:�����ϰ� ������   wait:���ð� ���� sum_turnaround:��ȯ�ð� ���� 
	double normalization = 0;   // ����ȭ�� ��ȯ�ð�
 	
	for (i = 0; i < MAX; i++)
	{
		fscanf(fp, "%d,%d,%d", &pro[i].num, &pro[i].arrive, &pro[i].service);
		if (pro[i].num == 0)
		{  // ���μ��� ���� ����
			flag = 1;
			break;
		}
		if (pro[i].num >= 100)
		{   // ���μ��� ���� 100�̻� �̹Ƿ� ����x
			flag = 0;
			break;
		}
		pro[i].respone = pro[i].service;
		
		pronum++;  // ���μ��� ���� ���� ��
	}
	fclose(fp);
	
	// �ּ� Ǯ��
	//fprintf(stdout, "[ SRT �����ٸ� ] \n\n");
	// fprintf(stdout, "--------------�Է¹��� ���μ���--------------\n");
	// fprintf(stdout, "�� ���μ��� ID ������ ���� �ð� ������ ���� �ð� ��\n");
	// �Է� ���� ���μ��� ���
	for (int i = 0; i < pronum; i++)
	{
		fprintf(stdout, "�� \t%2d     ��      %2d     ��       %2d\t    ��\n", pro[i].num, pro[i].arrive, pro[i].service);
	}
	fprintf(stdout, "������������������������������������������������������������������������������������������\n");

	switch(flag)
	{
	case 1:
	{
		//printf("\n\nProcess\t|   ��ȯ�ð�   | ���ð� \n\n");	 
		// �ּ� Ǯ��
		// fprintf(stdout, "���� ���μ��� ID ������ ���� �ð� ������ ���� �ð� ������ ���� �ð� ������ ��ȯ �ð� ������ ����ȭ�� ��ȯ �ð� ����\n");
		pro[pronum].respone = 9999;
		for (time = 0; remain != pronum; time++) //time:�ݺ�  remain:�����ϰ� ������  smallest:����Ʈ�ð� ���������� ���� 
		{
			smallest = pronum;
			for (i = 0; i < pronum; i++)
			{
				if (pro[i].arrive <= time && pro[i].respone < pro[smallest].respone && pro[i].respone>0) 
				{		// ���μ����� �����ð��� ���� �ð����� �۰�, ���ݱ��� ����Ʈ �ð��� ���� ���� ���μ������� ���� �ð��� ����, ���� ����ð��� 0���� ū ���μ��� ����
					smallest = i;
				}
			}
			pro[smallest].respone--; // ���� ����ð��� ���� ���� ���μ����� ����
			if (pro[smallest].respone == 0) // ���μ����� ������ ��� �Ϸ�Ǿ��� ��
			{
				remain++; // ����Ϸ�� ���μ����� ���� 1 ����
				end = time + 1;
				normalization = (double)(end - pro[smallest].arrive) / (double)pro[smallest].service;
				// ����ȭ�� ��ȯ �ð� = ��ȯ�ð� / ���� �ð�
				fprintf(stdout, "�� \t%2d      ��      %2d     ��       %2d      ��    %3d      �� \t %3d      �� \t\t%.2lf\t ��\n", smallest + 1, pro[smallest].arrive, pro[smallest].service, end, end - pro[smallest].arrive , normalization);

			    // printf("\n  %d \t|\t%d\t|\t%d", smallest + 1, end - pro[smallest].arrive, end - pro[smallest].service - pro[smallest].arrive);
				wait += end - pro[smallest].service - pro[smallest].arrive;
				turn += end - pro[smallest].arrive;
			}
		}
		fprintf(stdout, "����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
		// printf("\n\n��� ��� �ð� = %.3f\n", wait*1.0 / pronum);
		// printf("��� ��ȯ �ð� = %.3f\n\n", turn*1.0 / pronum);
	}
	case 0:
	{
		break;
	}
	}
	system("pause");
	return 0;
}