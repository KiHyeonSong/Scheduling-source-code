#include <stdio.h>
#pragma warning (disable :4996)
#define MAX 110
#define KINDS 4 

FILE* fp;

// void Descending(int(*pro)[KINDS]);  //���� �ص��ȴ�.
void Descending(int pro[][KINDS], int pronum);
double response(int wait, int servise);    // �����\

void main()
{
	int pronum = 0;					 // ���μ��� ����
	int start = 0;                   // ���� ��ư
	int flag = 1;                    // 100�� ��/�� �Ǵ�
	int wait = 0;                    // ��ٸ� �ð�
	int end = 0;                     // ���� �ð�
	double turn = 0;                 // ��ȯ �ð�
	int responnum = 0;               // ���� ������ ���μ��� ����
	int I_temp = 0;                  // int�� temp
	double D_temp = 0;               // double�� temp


	fp = fopen("E:\\inputtest.txt", "r");

	int process[MAX][KINDS] = { NULL };
	// process[0][0]  = ���μ��� �̸�
	// process[0][1]  = �����ð�
	// process[0][2]  = ���񽺽ð�  
	// process[0][3]  = ��ٸ� �ð�
	double respon[MAX] = { 0 };
	// ����� ����� ����

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
			// �ּ� Ǯ��
			// fprintf(stdout, "���μ����� 100�� �Ѿ����ϴ�.\n�����մϴ�.\n");
			flag = 0;    // ���μ��� ������ 99�� ������ ���� ���ϱ�
			break;
		}
		pronum++; // ���� ���� �� ���� ���μ��� ���� 1�� �� �߰�
	}
	fclose(fp);

	if (flag == 1)
	{    // �Է¹��� ���μ��� ����ϱ�

		// �ּ� Ǯ��
		// fprintf(stdout, "\t\t    �Էµ� ���μ���\n");
		// fprintf(stdout, "�� ���μ��� ���̵� ������ �����ð� ������ ���� �����ð� ��\n");
		for (int j = 0; j < pronum; j++)
		{
			fprintf(stdout, "��\t%d\t   ��\t %d\t��\t %d\t   ��\n", 
				process[j][0], process[j][1],process[j][2]);
		}
		fprintf(stdout, "��������������������������������������������������������������������������������������������������������\n");
		
		// �ּ� Ǯ��
		// fprintf(stdout, "HRRN �����ٸ�\n");
		// fprintf(stdout, "���� : 1\n���� : 0\n");
		fscanf(stdin, "%d", &start);
	}
	switch (start)
	{
	case 1:
	{
		// �ּ� Ǯ��
		// fprintf(stdout, "��  ���̵� ������ ���� �ð� ������ ���� �ð� ������ ���� �ð� ������ ��ȯ �ð� ������ ����ȭ�� ��ȯ �ð� ��\n");
		Descending(process,pronum);  // ���� �ð� ������ ����
		for (int i = 0; i < pronum; i++)
		{
			if (process[i][1] == 0)    // ���� �ð��� 0 �ΰ��
			{ 
				turn = end - process[i][1] + process[i][2];     // �����ð��� 0�ΰ��� 2�� �̻��� ��츦 �����
				end = end + process[i][1] + process[i][2];      // ���� �ð� = ���� �ð� + ���� �ð�
				
				respon[i] = response(process[i][3], process[i][2]);     // �Լ��� �� ������� �����
				fprintf(stdout, "��    %2d    ��     %2d\t ��      %2d\t ��     %3d     ��   %6.2lf    ��       %6.2lf        ��\n",
					process[i][0], process[i][1], process[i][2], end, turn, respon[i]);	
				process[i][3] = end - process[i][1];       // wait ��ٸ� �ð��� ����Ͽ� �����Ѵ�.
				responnum++;  // �����ð� 0 �ΰ� � �����ߴ��� �������� ���� 2��
				
			}
			else if(process[i][1] != 0)    // ���� �ð��� 0�� �ƴ� ���
			{
				for (int j = 0; j < pronum -responnum; j++)     // �Է¹��� ���μ��� -����� ���μ��� ���� ��ŭ ��ƾ
				{  
					if (end < process[responnum + j][1])     // ���ݱ��� �����ϸ鼭 �� �߰��� ���� ���� ������ ������ 
					{
						break;
					}
					else if (end > process[responnum + j][1])  
					{		// ���ݱ��� ���� �ϸ鼭 �߰��� ��� �°��� ������ ��ٸ� �ð��� ������� ����� ����
						process[responnum+j][3] = end - process[responnum + j][1];   // ��ٸ� �ð� ����
						respon[responnum + j] = response(process[responnum + j][3], process[responnum + j][2]);  // ����� ��� �� ����
					}
				}
				for (int i = responnum; i < pronum; i++)
				{    // �����Ѱ� ���� ����  �Է� ���� ���μ��� ���� ���� 
					for (int j = i + 1; j < pronum; j++)
					{   // i ������ ���μ������� 
						if (respon[i] < respon[j])
						{   // ������� ���Ͽ� ���� �����ؾ��� ���μ����� �켱������ ���� ���ش�.
							// ���� �ܰ�
							I_temp = process[i][0];
							process[i][0] = process[j][0];
							process[j][0] = I_temp;

							I_temp = process[i][1];
							process[i][1] = process[j][1];
							process[j][1] = I_temp;

							I_temp = process[i][2];
							process[i][2] = process[j][2];
							process[j][2] = I_temp;

							D_temp = respon[i];  // ����� ���� ����
							respon[i] = respon[j];
							respon[j] = D_temp;
						}
					}
				}
				turn = end - process[i][1] + process[i][2];  //  ��ȯ �ð�
				end = end + process[i][2];    // ���� �ð�
				responnum++;    // ����� ���μ��� ���� �߰�

				fprintf(stdout, "��    %2d    ��     %2d\t ��      %2d\t ��     %3d     ��   %6.2lf    ��       %6.2lf        ��\n",
					process[i][0], process[i][1], process[i][2], end, turn, respon[i]);	 // ���
				
			}
			process[i][0] = NULL;  // ��� �� �ʱ�ȭ
			process[i][1] = NULL;
			process[i][2] = NULL;
			process[i][3] = NULL;
			// respon[i] = 0.0;
		}
		fprintf(stdout, "����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	}
	case 0:
	{   //  ���μ��� ������ 99���� ������ ����x
		break;
	}
	}
	system("pause");
}

void Descending(int pro[][KINDS], int pronum)  // �����ð� ������������ �����ϱ�
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
{   // ����� ����ϴ� �Լ�
	double respon = 0;
	respon = (double)(wait+servise) / (double)servise;
	return respon;
}