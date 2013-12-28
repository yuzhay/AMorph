// MpiAMorph.cpp : Defines the entry point for the console application.
//

#include "mpi.h"
#include <iostream>
#include <vector>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"
#include "Library.h"

using namespace std;

//Указатели на файлы
char *srcFile,*dstFile;

void Start();

//Номер текущего процесса
int myNode;

//Количество процессов
int totalNodes; 

int main(int argc, char* argv[])
{

	setlocale(LC_ALL, "Russian");

	int rc;

	if(rc = MPI_Init(&argc,&argv))
	{
		cout << "MPI_Init error " << endl;
		MPI_Abort(MPI_COMM_WORLD,rc);
		return 1;
	}

	/*
	* Две информационных функции.
	* сообщают размер группы (то есть общее количество задач, подсоединенных к ее области связи) 
	* и порядковый номер вызывающей задачи.
	*/
	MPI_Comm_size(MPI_COMM_WORLD, &totalNodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myNode);

	if(myNode == 0)
	{
		Start();

		if(argc != 3)
		{
			cout << "Invalida arguments" << endl;
			cout << "Usage: amorph src_bmp dst_bmp" << endl;
			MPI_Finalize();
			return 1;
		}

		srcFile = argv[1];
		dstFile = argv[2];
		if(!FileExists(srcFile) || !FileExists(dstFile))
		{
			cout << "Input files not found" << endl;
			MPI_Finalize();
			return 1;
		}

		MasterProcess(srcFile,dstFile, totalNodes);
	}else{
		if(argc != 3)
		{
			MPI_Finalize();
			return 1;
		}

		srcFile = argv[1];
		dstFile = argv[2];

		if(!FileExists(srcFile) || !FileExists(dstFile))
		{
			MPI_Finalize();
			return 1;
		}

		SlaveProcess(srcFile,dstFile, myNode);
	}

	MPI_Finalize();
	return 0;
}

void Start()
{
	//unsigned long *v = (unsigned long *)malloc(sizeof(long)*3);

	//v[0] = 3;
	//v[1] = 1;
	//v[2] = 5;

	//unsigned long *v2 = BuildVector(v,3,10);

	//for (int i = 0; i < 10; i++)
	//{
	//	cout << v2[i] << " ";
	//}

	//free(v);
}