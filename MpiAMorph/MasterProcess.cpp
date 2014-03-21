#include "mpi.h"
#include <iostream>
#include <queue>
#include <vector>
#include <ctime>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"

using namespace std;

//������ ��������� ���������
ProcessState *NodeStates;

//����� ������
clock_t start;

queue<unsigned long> *tasks;

void MPState1();
void MPState2();
void MPState3(int);

//����������� �������
void MasterProcess(char *srcFile,char *dstFile, int totalNodes)
{
	start = std::clock();
	tasks = new queue<unsigned long>();

	queue<unsigned long *> q;

	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	cout << "Total nodes: " << totalNodes << endl;

	NodeStates = (ProcessState*)calloc(totalNodes,sizeof(ProcessState));

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
	for (int i = 0; i < totalNodes -1  && i < sizeMtx; i++)
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
		NodeStates[i+1] = STATE1_WORKING;
		Print(0,i+1,1,i);
		//q.pop();
	}

	//���������� �������� ��������, ���� ���������� 
	//����� ������, ��� ������ �������
	if(totalNodes > sizeMtx && sizeMtx > DIVISIBILITY_CRITERIA)
	{
		//MPI_Status status;
		//unsigned long currNode = 0;

		////������� ����� ����� ����� �� ��������� ���������
		//for (int i = 0; i < totalNodes - sizeMtx; i++)
		//{
		//	unsigned long len = 0;
		//	unsigned long *vector = IsDivisibilityCriteria(i + 1, &len);

		//	//���� �� ������ ����� �������� ��������� ���������
		//	if(vector != NULL)
		//	{
		//		//��������� ������ ���������� ����
		//		MPI_Send((void *)vector,len,MPI_LONG,sizeMtx + currNode,1,MPI_COMM_WORLD);
		//		NodeStates[sizeMtx + currNode] = STATE1_WORKING;
		//		Print(0,sizeMtx + currNode,1,len);
		//	}else
		//	{

		//	}
		//}
	}

	//���� ������ ������� ������, ��� ���������� �����
	if(sizeMtx > totalNodes - 1)
	{
		for (int i = 0; i < sizeMtx - (totalNodes - 1); i++)
		{
			tasks->push((totalNodes-1) + i);
		}
	}


	MPState1();


	while(true)
	{
		//MPI_Recv(&i,1, MPI_INT,0,1,MPI_COMM_WORLD,&status);
	}

	//������������ ������
	free(src_vector);
	free(dst_vector);
	free(NodeStates);
	delete tasks;
}


void MPState1()
{
	MPState2();
}

void MPState2()
{
	MPI_Status status;
	int error;
	bool buf;
	int count;

	while(true)
	{
		//��������� ��������� ������� ��������� ��� �� ��������� ������
		error = MPI_Probe(MPI_ANY_SOURCE,SlaveWorkEnded,MPI_COMM_WORLD,&status);

		//�������� ����� ������������ ���������
		error = MPI_Get_count(&status,MPI_LONG,&count);

		MPI_Recv(&buf,1,MPI_C_BOOL,MPI_ANY_SOURCE,SlaveWorkEnded,MPI_COMM_WORLD,&status);
		NodeStates[status.MPI_SOURCE] = STATE_DONE;
		Print(status.MPI_SOURCE, 0, SlaveWorkEnded,"1111111111");

		MPState3(status.MPI_SOURCE);

	}
}

void MPState3(int node)
{
	//�������� ����� ���������
	if(tasks->size()>0)
	{
		unsigned long val = tasks->front();
		tasks->pop();

		MPI_Send((void *)&val,1,MPI_LONG,node,1,MPI_COMM_WORLD);
		Print(0,node,1,&val,1);
		NodeStates[node] = STATE1_WORKING;
	}

	MPState2();
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
	double cpu_time_used = ((double)(std::clock() - start)) / ((double)CLOCKS_PER_SEC);

	sprintf(buf, "%.15f [%d] -> [%d]: (%s) \"%s\"",cpu_time_used, src,dst,MsgTagsStr[tag],str);
	std::cout << buf <<endl;

}

void Print(int src, int dst, int tag, int str)
{
	char buf[256];
	double cpu_time_used = ((double)(std::clock() - start)) / ((double)CLOCKS_PER_SEC);
	sprintf(buf, "%.15f [%d] -> [%d]: (%s) \"%d\"",cpu_time_used,src,dst,MsgTagsStr[tag],str);
	std::cout << buf <<endl;
}

void Print(int src, int dst, int tag, unsigned long *v, long len)
{
	char buf[256];
	double cpu_time_used = ((double)(std::clock() - start)) / ((double)CLOCKS_PER_SEC);
	sprintf(buf, "%.15f [%d] -> [%d]: (%s) ",cpu_time_used,src,dst,MsgTagsStr[tag]);
	std::cout << buf << "\"";
	for (int i = 0; i < len; i++)
	{
		std::cout << v[i];
		if(i != len-1)
			cout << ",";
	}
	std::cout << "\""<< endl;
}

void PrintVector(int node, unsigned long *v, long len)
{
	std::cout<< node << ": ";
	std::cout << "\"";
	for (int i = 0; i < len; i++)
	{
		std::cout << v[i];
		if(i != len-1)
			cout << ",";
	}
	std::cout << "\""<< endl;
}

unsigned long *IsDivisibilityCriteria(int node, unsigned long *len)
{
	MPI_Status status;
	bool result = false;

	//��������� ������ �� ��������� ���������
	MPI_Send((void *)&result,1,MPI_C_BOOL,node,SlaveSubTreeDevisibility,MPI_COMM_WORLD);

	int error,count ;

	//�������� ������ ������
	error = MPI_Probe(node,SlaveSubTreeDevisibility,MPI_COMM_WORLD,&status);

	//�������� ����� ������������ ��������� (��� ������� ���������)
	error = MPI_Get_count(&status,MPI_LONG,&count);

	if(count > 0)
	{
		unsigned long* buffer;

		//�������� ������
		buffer = (unsigned long*)malloc (sizeof(long)*count) ;

		MPI_Recv((void *)buffer,count,MPI_LONG,node,SlaveSubTreeDevisibility,MPI_COMM_WORLD,&status);

		//������� �������� �� ������
		//if(status.MPI_ERROR)

		(*len) = count;
		return buffer;

		free(buffer);

	}

	return NULL;
}

