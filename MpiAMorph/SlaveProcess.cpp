#include "mpi.h"
#include <iostream>
#include <queue>
#include <vector>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"

using namespace std;



//����������� �������
void SlaveProcess(char *srcFile,char *dstFile, int myNode)
{
	int i = 0;
	MPI_Status status;

	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	//������������� ���������
	MPI_Barrier(MPI_COMM_WORLD);

	cout << "Current node: " << myNode << endl;

	//�������� ��������� �����������
	load(srcFile, &src_vector, &sizeMtx, &sizeCell);

	//�������� ��������� �����������
	load(dstFile, &dst_vector, &sizeMtx, &sizeCell);

	//for(;;)
	{
		int error,count ;
		unsigned long* buffer;
		MPI_Status status ;

		//��������� ��������� ������� ��������� ��� �� ��������� ������
		error = MPI_Probe(0,1,MPI_COMM_WORLD,&status);

		//�������� ����� ������������ ���������
		error = MPI_Get_count(&status,MPI_LONG,&count);

		//�������� ������
		buffer = (unsigned long*)malloc (sizeof(long)*count) ;

		if ( buffer == NULL ){};

		//MPI_Recv:
		//OUT	 buf	 -	 ����� ������ ������������ ������������ ���������;
		//IN	 count	 -	 ������������ ����� ����������� ���������;
		//IN	 datatype	 -	 ��� ��������� ������������ ���������;
		//IN	 source	 -	 ����� ��������-�����������;
		//IN	 tag	 -	 ������������� ���������;
		//IN	 comm	 -	 ������������ ������� �����;
		//OUT	 status	 -	 �������� ��������� ���������.
		error = MPI_Recv (buffer,count,MPI_LONG,0,1,MPI_COMM_WORLD,&status);

		for (int i = 0; i < count; i++)
		{
			cout<<buffer[i]<< " ";
		}
		cout<< endl;

		//ToDo: �������� �� ����� ������ �� �������

		free(buffer);
	}

	free(src_vector);
	free(dst_vector);
}