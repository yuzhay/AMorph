#include "mpi.h"
#include <iostream>
#include <queue>
#include <vector>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"

using namespace std;

//Управляющий процесс
void MasterProcess(char *srcFile,char *dstFile, int totalNodes)
{
	queue<unsigned long *> q;

	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	cout << "Total nodes: " << totalNodes << endl;

	//Синхронизация процессов
	MPI_Barrier(MPI_COMM_WORLD);

	//Загрузка исходного изображения
	load(srcFile, &src_vector, &sizeMtx, &sizeCell);

	//Загрузка конечного изображения
	load(dstFile, &dst_vector, &sizeMtx, &sizeCell);

	for (int i = 0; i < sizeMtx; i++)
	{
		unsigned long *arr = (unsigned long *)malloc(sizeof(long)* 2);
		arr[0] = i;
		arr[1] = i+1;
		q.push(arr);
	}

	//Обход в ширину

	//Состояние 0
	//Раздать вершины подграфов по узлам
	for (int i = 0; i < totalNodes && i < sizeMtx; i++)
	{
		//MPI_Send:
		//buf	 -	 адрес начала расположения пересылаемых данных;
		//count	 -	 число пересылаемых элементов;
		//datatype	 -	 тип посылаемых элементов;
		//dest	 -	 номер процесса-получателя в группе, связанной с коммуникатором comm;
		//tag	 -	 идентификатор сообщения (аналог типа сообщения функций nread и nwrite PSE nCUBE2);
		//comm	 -	 коммуникатор области связи.

		//MPI_Send((void *)q.front(),2,MPI_LONG,i+1,1,MPI_COMM_WORLD);

		
		MPI_Send((void *)&i,1,MPI_LONG,i+1,1,MPI_COMM_WORLD);
		Print(0,i+1,1,i+1);
		//q.pop();
	}

	//Необходимо добавить проверку, если количество 
	//узлов больше, чем размер матрицы
	//if(totalNodes > sizeMtx)
	//{
	//	MPI_Status status;

	//	//Сделать опрос среди узлом на делимость поддерева
	//	for (int i = 0; i < totalNodes - sizeMtx; i++)
	//	{
	//		bool result = false;

	//		//Отправить запрос на выделение поддерева
	//		MPI_Send((void *)&result,1,MPI_C_BOOL,i+1,SlaveSubTreeDevisibility,MPI_COMM_WORLD);

	//		int error,count ;

	//		//Получить статус ответа
	//		error = MPI_Probe(i+1,SlaveSubTreeDevisibility,MPI_COMM_WORLD,&status);

	//		//Получить длину принимаемого сообщения (для вектора поддерева)
	//		error = MPI_Get_count(&status,MPI_LONG,&count);

	//		if(count > 0)
	//		{
	//			unsigned long* buffer;

	//			//Выделить память
	//			buffer = (unsigned long*)malloc (sizeof(long)*count) ;

	//			MPI_Recv((void *)buffer,1,MPI_LONG,i+1,SlaveSubTreeDevisibility,MPI_COMM_WORLD,&status);

	//			//Сделать проверку на ошибку
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

	//Освобождение памяти
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