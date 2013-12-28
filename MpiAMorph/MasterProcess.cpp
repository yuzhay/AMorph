#include "mpi.h"
#include <iostream>
#include <queue>
#include <vector>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"

using namespace std;

//����������� �������
void MasterProcess(char *srcFile,char *dstFile, int totalNodes)
{
	queue<unsigned long *> q;

	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	cout << "Total nodes: " << totalNodes << endl;

	//������������� ���������
	MPI_Barrier(MPI_COMM_WORLD);

	//�������� ��������� �����������
	load(srcFile, &src_vector, &sizeMtx, &sizeCell);

	//�������� ��������� �����������
	load(dstFile, &dst_vector, &sizeMtx, &sizeCell);

	for (int i = 0; i < sizeMtx; i++)
	{
		unsigned long *arr = (unsigned long *)malloc(sizeof(long)* 2);
		arr[0] = i;
		arr[1] = i+1;
		q.push(arr);
	}

	//����� � ������


	//��������� 1
	//������� ������� ��������� �� �����
	for (int i = 0; i < totalNodes && i < sizeMtx; i++)
	{
		//MPI_Send:
		//buf	 -	 ����� ������ ������������ ������������ ������;
		//count	 -	 ����� ������������ ���������;
		//datatype	 -	 ��� ���������� ���������;
		//dest	 -	 ����� ��������-���������� � ������, ��������� � �������������� comm;
		//tag	 -	 ������������� ��������� (������ ���� ��������� ������� nread � nwrite PSE nCUBE2);
		//comm	 -	 ������������ ������� �����.

		MPI_Send((void *)q.front(),2,MPI_LONG,i+1,1,MPI_COMM_WORLD);
		q.pop();
	}

	MPI_Status status;
	while(true)
	{
		//MPI_Recv(&i,1, MPI_INT,0,1,MPI_COMM_WORLD,&status);
	}

	//������������ ������
	free(src_vector);
	free(dst_vector);
}
