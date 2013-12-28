#include "mpi.h"
#include <iostream>
#include <queue>
#include <vector>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"

using namespace std;



//Исполняющий процесс
void SlaveProcess(char *srcFile,char *dstFile, int myNode)
{
	int i = 0;
	MPI_Status status;

	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	//Синхронизация процессов
	MPI_Barrier(MPI_COMM_WORLD);

	cout << "Current node: " << myNode << endl;

	//Загрузка исходного изображения
	load(srcFile, &src_vector, &sizeMtx, &sizeCell);

	//Загрузка конечного изображения
	load(dstFile, &dst_vector, &sizeMtx, &sizeCell);

	//for(;;)
	{
		int error,count ;
		unsigned long* buffer;
		MPI_Status status ;

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

		for (int i = 0; i < count; i++)
		{
			cout<<buffer[i]<< " ";
		}
		cout<< endl;

		//ToDo: Отправка на обход дерева по вектору

		free(buffer);
	}

	free(src_vector);
	free(dst_vector);
}