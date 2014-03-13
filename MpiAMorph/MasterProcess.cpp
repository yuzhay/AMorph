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

	//��������� 0
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

		//MPI_Send((void *)q.front(),2,MPI_LONG,i+1,1,MPI_COMM_WORLD);

		
		MPI_Send((void *)&i,1,MPI_LONG,i+1,1,MPI_COMM_WORLD);
		Print(0,i+1,1,i+1);
		//q.pop();
	}

	//���������� �������� ��������, ���� ���������� 
	//����� ������, ��� ������ �������
	//if(totalNodes > sizeMtx)
	//{
	//	MPI_Status status;

	//	//������� ����� ����� ����� �� ��������� ���������
	//	for (int i = 0; i < totalNodes - sizeMtx; i++)
	//	{
	//		bool result = false;

	//		//��������� ������ �� ��������� ���������
	//		MPI_Send((void *)&result,1,MPI_C_BOOL,i+1,SlaveSubTreeDevisibility,MPI_COMM_WORLD);

	//		int error,count ;

	//		//�������� ������ ������
	//		error = MPI_Probe(i+1,SlaveSubTreeDevisibility,MPI_COMM_WORLD,&status);

	//		//�������� ����� ������������ ��������� (��� ������� ���������)
	//		error = MPI_Get_count(&status,MPI_LONG,&count);

	//		if(count > 0)
	//		{
	//			unsigned long* buffer;

	//			//�������� ������
	//			buffer = (unsigned long*)malloc (sizeof(long)*count) ;

	//			MPI_Recv((void *)buffer,1,MPI_LONG,i+1,SlaveSubTreeDevisibility,MPI_COMM_WORLD,&status);

	//			//������� �������� �� ������
	//			//if(status.MPI_ERROR)


	//			free(buffer);

	//		}

	//	}
	//}

	//MPState1();


	while(true)
	{
		//MPI_Recv(&i,1, MPI_INT,0,1,MPI_COMM_WORLD,&status);
	}

	//������������ ������
	free(src_vector);
	free(dst_vector);
}


void MPState1()
{

}

void MPState2()
{

}

void MPState3()
{

}

void MPState4()
{

}

void MPState5()
{

}

void MPState6()
{

}

void Print(int src, int dst, int tag, char *str)
{
	char buf[256];
	sprintf(buf, "[%d] -> [%d]: (%d) \"%s\"",src,dst,tag,str);
	std::cout << buf <<endl;
}

void Print(int src, int dst, int tag, int str)
{
	char buf[256];
	sprintf(buf, "[%d] -> [%d]: (%d) \"%d\"",src,dst,tag,str);
	std::cout << buf <<endl;
}

void Print(int src, int dst, int tag, unsigned long *v, long len)
{
	char buf[256];
	sprintf(buf, "[%d] -> [%d]: (%d) ",src,dst,tag);
	std::cout << buf << "\"";
	for (int i = 0; i < len; i++)
	{
		std::cout << v[i];
		if(i != len-1)
			cout << ",";
	}
	std::cout << "\""<< endl;
}

void PrintVector(unsigned long *v, long len)
{
	std::cout << "\"";
	for (int i = 0; i < len; i++)
	{
		std::cout << v[i];
		if(i != len-1)
			cout << ",";
	}
	std::cout << "\""<< endl;
}