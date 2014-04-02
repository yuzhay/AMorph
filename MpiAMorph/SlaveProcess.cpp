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

char space[16] = "";

void SPState1(unsigned long *, long );
void SPState(unsigned long *, long );
void SPState2();
void SaveSolutionToFile(IsomorphMatrices* im);


int node = 0;
//Исполняющий процесс
void SlaveProcess(char *srcFile,char *dstFile, int myNode)
{
	int i = 0;
	MPI_Status status;

	node = myNode;

	for (int i = 0; i < myNode; i++)
	{
		strcat(space, "\t");
	}

	//Синхронизация процессов
	MPI_Barrier(MPI_COMM_WORLD);

	//cout << "Current node: " << myNode << endl;

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



		//SPState1(buffer,count);
		SPState(buffer,count);

		//for (int i = 0; i < count; i++)
		//{
		//	cout<<buffer[i]<< " ";
		//}
		//cout<< endl;

		//ToDo: Отправка на обход дерева по вектору

		//free(buffer);
	}

	free(src_vector);
	free(dst_vector);

	delete ism;
}

MPI_Request callbackRequest;

void Callback(IsomorphMatrices* im, unsigned long *vector, unsigned long length, unsigned long depth)
{
	
	//printf("%d Callback Started\n", myNode);
	MPI_Status status;
	int flag = 0;

	//Проверка, есть ли сообщение на деление дерева
	MPI_Test(&callbackRequest, &flag, &status);

	if(vector == NULL || length == 0 || depth == 0)
	{
		//cout << space << "[" << node << "] State Callback 0 MPI_Send block"<<endl;
		MPI_Send(NULL,0,MPI_LONG,0,TAG_DEVIDE_TASK,MPI_COMM_WORLD);
		//cout << space << "[" << node << "] State Callback 0 MPI_Send unblock"<<endl;
		return;
	}

	//Если есть, тогда 
	if (flag != 0)
	{ 
		//Выделяем поддерево
		unsigned long *subVector = im->GetSubMatrix(vector);
		if(subVector != NULL)
		{
			//Добавляем в игнор лист
			im->AddToIgnore(subVector);

			unsigned long shortLen = 0;
			unsigned long *shortSubVector = ConstructShortVector(subVector,length,&shortLen);
			free(subVector);
			//Отправить сообщение о выделенном поддереве

			//cout << space << "[" << node << "] State Callback MPI_Send block"<<endl;
			MPI_Send(shortSubVector,shortLen,MPI_LONG,0,TAG_DEVIDE_TASK,MPI_COMM_WORLD);
			//cout << space << "[" << node << "] State Callback MPI_Send unblock"<<endl;
		}else{
			//cout << space << "[" << node << "] State Callback MPI_Send block"<<endl;
			MPI_Send(NULL,0,MPI_LONG,0,TAG_DEVIDE_TASK,MPI_COMM_WORLD);
			//cout << space << "[" << node << "] State Callback MPI_Send unblock"<<endl;
		}
	}

	//printf("%d Callback Ended\n", myNode);
}

//S1 - переход в это состояние выполняется при получении корневой вершины. В этом со-
//стоянии выполняется обход в глубину выделенного поддерева.
void SPState1(unsigned long *vector, long length)
{	
	bool buf;

	MPI_Irecv(&buf,1,MPI_C_BOOL,0,TAG_DEVIDE_TASK,MPI_COMM_WORLD,&callbackRequest);

	unsigned long *v = ConstructFullVector(vector, length,sizeMtx);
	free(vector);

	PrintVector(node,v,sizeMtx);

	ism->ClearSubstitutions();
	ism->SearchIsomorphCallback(length+1,v,Callback,true);
	printf("%d Done\n");
	free(v);

	//int c = ism->GetSubstitutionsCount();
	//for (int i = 0; i < c; i++)
	//{
	//	PrintVector(node,ism->GetSubstitutions(i),sizeMtx);
	//}

	MPI_Send(&buf,1,MPI_C_BOOL,0,TAG_TASK_COMPLETED,MPI_COMM_WORLD);

	SPState2();
}	 

void SPState2()
{
	MPI_Status status;
	int error;
	unsigned long *buffer;
	int count = 0;

	while(true)
	{

		error = MPI_Probe(0,1,MPI_COMM_WORLD,&status);

		//Получить длину принимаемого сообщения
		error = MPI_Get_count(&status,MPI_LONG,&count);

		//Выделить память
		buffer = (unsigned long*)malloc (sizeof(long)*count) ;

		if ( buffer == NULL ){};

		error = MPI_Recv (buffer,count,MPI_LONG,0,1,MPI_COMM_WORLD,&status);


		//unsigned long *v = ConstructFullVector(buffer, count,sizeMtx);

		SPState1(buffer,count);

	}
}

void SPState(unsigned long *vector, long length)
{
	long state = 0;
	int error = 0;


	while(true)
	{
		switch(state)
		{
		case 0:
			{
				//printf("[%d] state 0\n", node);
				//cout << space << "[" << node << "] State 0"<<endl;

				bool buf;

				//Асинхронное считывание поступающих сообщений на делимость задачи
				error = MPI_Irecv(&buf,1,MPI_C_BOOL,0,TAG_DEVIDE_TASK,MPI_COMM_WORLD,&callbackRequest);
				if(error != MPI_SUCCESS)
				{
					printf("[%d] Terminated\n");
					return;
				}

				//printf("MPI_Irecv = %d\n", error);

				unsigned long *v = ConstructFullVector(vector, length,sizeMtx);
				free(vector);


				ism->SearchIsomorphCallback(length+1,v,Callback,true);
				Callback(ism,NULL,0,0);

				//cout << space << "[" << node << "] Done"<<endl;
				free(v);


				//cout << space << "[" << node << "] State 0 MPI_Send block"<<endl;
				MPI_Send(&buf,1,MPI_C_BOOL,0,TAG_TASK_COMPLETED,MPI_COMM_WORLD);
				//cout << space << "[" << node << "] State 0 MPI_Send unblock"<<endl;
				state = 1;
				break;
			}
		case 1:
			{
				//printf("[%d] state 1\n", node);
				//cout << space << "[" << node << "] State 1"<<endl;
				MPI_Status status;
				int error;
				unsigned long *newVector;
				int newLength = 0;

				//cout << space << "[" << node << "] State 1 MPI_Probe block"<<endl;
				error = MPI_Probe(0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
				//cout << space << "[" << node << "] State 1 MPI_Probe unblock"<<endl;

				//cout << space << "[" << node << "] TAG = " << status.MPI_TAG << endl;
				if(status.MPI_TAG == TAG_NEW_TASK){
					//Получить длину принимаемого сообщения
					error = MPI_Get_count(&status,MPI_LONG,&newLength);

					//Выделить память
					newVector = (unsigned long*)malloc (sizeof(long)*newLength) ;

					if ( newVector == NULL ){};

					//cout << space << "[" << node << "] State 1 MPI_Recv block"<<endl;
					error = MPI_Recv (newVector,newLength,MPI_LONG,0,TAG_NEW_TASK,MPI_COMM_WORLD,&status);
					//cout << space << "[" << node << "] State 1 MPI_Recv unblock"<<endl;


					//unsigned long *v = ConstructFullVector(buffer, count,sizeMtx);

					vector = newVector;
					length = newLength;

					state = 0;
				}else if(status.MPI_TAG == TAG_NODE_TERMINATE)
				{
					//cout << space << "[" << node << "] SaveSolutionToFile"<<endl;
					bool b;

					//cout << space << "[" << node << "] State 1 1 MPI_Recv block"<<endl;
					error = MPI_Recv (&b,1,MPI_C_BOOL,0,TAG_NODE_TERMINATE,MPI_COMM_WORLD,&status);
					//cout << space << "[" << node << "] State 1 1 MPI_Recv unblock"<<endl;


					SaveSolutionToFile(ism);
					//cout << space << node <<" Barrier" << endl;
					MPI_Barrier(MPI_COMM_WORLD);
					return;
				}else if(status.MPI_TAG == TAG_DEVIDE_TASK)
				{
					state = 0;
				}
				break;
			}
		default:break;
		}
	}
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

unsigned long *ConstructShortVector(unsigned long *src, unsigned long srcLength, unsigned long *dstLength)
{

	//Печать исходного вектора
	//PrintVector(myNode,src,srcLength);
	unsigned long val = 0;

	for (int i = 1; i < srcLength; i++)
	{

		if(src[i] == src[0])
		{
			(*dstLength) = i;
			break;
		}
	}

	unsigned long *buffer = (unsigned long*)malloc (sizeof(long)*(*dstLength));
	memcpy(buffer,src,(*dstLength)*sizeof(long));

	//Печать конечного вектора
	//PrintVector(myNode,buffer,*dstLength);

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

void SaveSolutionToFile(IsomorphMatrices* im)
{
	FILE *fp;
	size_t count;
	char filename[64];
	sprintf(filename,"solutions/proc_%d.txt",myNode);

	fp = fopen(filename, "w");

	if(fp == NULL) {
		perror("ошибка открытия пример.txt");
		return;
	}

	unsigned long c = ism->GetSubstitutionsCount();
	unsigned long *v;
	char buf[16];
	for (unsigned long i = 0; i < c; i++)
	{
		//PrintVector(node,ism->GetSubstitutions(i),sizeMtx);
		v = ism->GetSubstitutions(i);
		for (unsigned long j = 0; j < sizeMtx; j++)
		{
			sprintf(buf,"%lu", v[j]);

			if(j != sizeMtx-1)
				sprintf(buf,"%lu,", v[j]);

			//fwrite(buf, strlen(buf), 1, fp);
			fputs(buf, fp);
		}
		fputs("\n", fp);
	}

	fclose(fp);
}