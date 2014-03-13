#include "mpi.h"
#include <iostream>
#include <queue>
#include <vector>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"
#include "../calc/IsomorphMatrices.h"

using namespace std;

unsigned long sizeMtx;
unsigned long sizeCell;
unsigned char * src_vector;
unsigned char * dst_vector;
IsomorphMatrices *ism = NULL;


void SPState1(unsigned long *, long );

//Исполняющий процесс
void SlaveProcess(char *srcFile,char *dstFile, int myNode)
{
	int i = 0;
	MPI_Status status;


	//Синхронизация процессов
	MPI_Barrier(MPI_COMM_WORLD);

	cout << "Current node: " << myNode << endl;

	//Загрузка исходного изображения
	load(srcFile, &src_vector, &sizeMtx, &sizeCell);

	//Загрузка конечного изображения
	load(dstFile, &dst_vector, &sizeMtx, &sizeCell);

	ism = new IsomorphMatrices(sizeMtx,sizeCell,src_vector,dst_vector);

	//for(;;)
	{
		int error,count ;
		unsigned long* buffer;


		//Позволяет проверить входные сообщения без их реального приема
		error = MPI_Probe(0,1,MPI_COMM_WORLD,&status);

		//Получить длину принимаемого сообщения
		error = MPI_Get_count(&status,MPI_LONG,&count);

		//Выделить память
		buffer = (unsigned long*)malloc (sizeof(long)*count) ;

		if ( buffer == NULL ){};

		//MPI_Recv:
		//OUT	 buf	 -	 адрес начала расположения принимаемого сообщения;
		//IN	 count	 -	 максимальное число принимаемых элементов;
		//IN	 datatype	 -	 тип элементов принимаемого сообщения;
		//IN	 source	 -	 номер процесса-отправителя;
		//IN	 tag	 -	 идентификатор сообщения;
		//IN	 comm	 -	 коммуникатор области связи;
		//OUT	 status	 -	 атрибуты принятого сообщения.
		error = MPI_Recv (buffer,count,MPI_LONG,0,1,MPI_COMM_WORLD,&status);

		SPState1(buffer,count);
		//for (int i = 0; i < count; i++)
		//{
		//	cout<<buffer[i]<< " ";
		//}
		//cout<< endl;

		//ToDo: Отправка на обход дерева по вектору

		free(buffer);
	}

	free(src_vector);
	free(dst_vector);

	delete ism;
}

MPI_Request callbackRequest;

void Callback(IsomorphMatrices* im, unsigned long *vector, unsigned long length, unsigned long depth)
{
	//MPI_Status status;
	//int flag;

	////Проверка, есть ли сообщение на деление дерева
	//MPI_Test(&callbackRequest, &flag, &status);

	////Если есть, тогда 
	//if (flag != 0)
	//{ 
	//	//Выделяем поддерево
	//	unsigned long *subVector = im->GetSubMatrix(vector);
	//	if(subVector != NULL)
	//	{
	//		im->AddToIgnore(subVector);
	//		MPI_Send(subVector,length,MPI_LONG,0,SlaveSubTreeDevisibility,MPI_COMM_WORLD);
	//	}
	//}
}

//S1 - переход в это состояние выполняется при получении корневой вершины. В этом со-
//стоянии выполняется обход в глубину выделенного поддерева.
void SPState1(unsigned long *vector, long length)
{	
	//bool buf;

	//MPI_Irecv(&buf,1,MPI_C_BOOL,0,SlaveSubTreeDevisibility,MPI_COMM_WORLD,&callbackRequest);

	unsigned long *v = ConstructFullVector(vector, length,sizeMtx);
	//PrintVector(v,sizeMtx);
	ism->SearchIsomorphCallback(length+1,v,Callback,true);
	
	int c = ism->GetSubstitutionsCount();
	for (int i = 0; i < c; i++)
	{
		PrintVector(ism->GetSubstitutions(i),sizeMtx);
	}
	//MPI_Send(&buf,1,MPI_C_BOOL,0,SlaveWorkEnded,MPI_COMM_WORLD);

}	 

void SPState2()
{

}

void SPState3()
{

}

void SPState4()
{

}

void SPState5()
{

}

void SPState6()
{

}

unsigned long *ConstructFullVector(unsigned long *src, unsigned long srcLength, unsigned long dstLength)
{
	unsigned long *buffer = (unsigned long*)malloc (sizeof(long)*dstLength) ;

	unsigned long val = 0;
	for (int i = 0; i < dstLength; i++)
	{

		if(i < srcLength)
		{
			buffer[i] = src[i];
		}
		else
		{
			while(InVector(val, src, srcLength)){ val++; }
			buffer[i] = val++;
		}
	}

	return buffer;
}

bool InVector(unsigned long elem, unsigned long *src, unsigned long length)
{
	for (int i = 0; i < length; i++)
	{
		if(src[i] == elem)
			return true;
	}
	return false;
}