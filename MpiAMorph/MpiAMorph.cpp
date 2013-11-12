// MpiAMorph.cpp : Defines the entry point for the console application.
//

#include "mpi.h"
#include <iostream>
#include <queue>
#include <vector>
#include "bitmap_image.hpp"
#include "msgtags.h"

using namespace std;

int load(char *fielname, unsigned char **vector, unsigned long *sizeMtx, unsigned long *sizeCell);
void SlaveProcess();
void MasterProcess();
bool FileExists(char *filename);

int myNode, totalNodes;
char *srcFile,*dstFile;



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


		MasterProcess();
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

		SlaveProcess();
	}



	MPI_Finalize();
	return 0;
}


bool FileExists(char *filename)
{
	bool exists = false;
	FILE *f = fopen(filename,"r");
	if (f!=NULL){
		exists = true;
		fclose(f);
	}
	return exists;
}

//Управляющий процесс
void MasterProcess()
{
	queue<unsigned long *> q;

	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	cout << "Total nodes: " << totalNodes << endl;
	MPI_Barrier(MPI_COMM_WORLD);

	load(srcFile, &src_vector, &sizeMtx, &sizeCell);
	load(dstFile, &dst_vector, &sizeMtx, &sizeCell);

	for (int i = 0; i < sizeMtx; i++)
	{
		unsigned long *arr = (unsigned long *)malloc(sizeof(long)* 2);
		arr[0] = i;
		arr[1] = i+1;
		q.push(arr);
	}
	
	//Обход в ширину


	//Раздать вершины подграфов по узлам
	for (int i = 0; i < totalNodes && i < sizeMtx; i++)
	{
		MPI_Send((void *)q.front(),2,MPI_LONG,i+1,1,MPI_COMM_WORLD);
		q.pop();
	}

	MPI_Status status;
	while(true)
	{
		//MPI_Recv(&i,1, MPI_INT,0,1,MPI_COMM_WORLD,&status);
	}

	free(src_vector);
	free(dst_vector);
}

void SlaveProcess()
{
	int i = 0;
	MPI_Status status;

	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	MPI_Barrier(MPI_COMM_WORLD);
	cout << "Current node: " << myNode << endl;

	load(srcFile, &src_vector, &sizeMtx, &sizeCell);
	load(dstFile, &dst_vector, &sizeMtx, &sizeCell);

	//for(;;)
	{
		int error,count ;
		unsigned long* buffer;
		MPI_Status status ;
		error = MPI_Probe(0,1,MPI_COMM_WORLD,&status);
		error = MPI_Get_count(&status,MPI_LONG,&count);
		buffer = (unsigned long*)malloc (sizeof(long)*count) ;
		if ( buffer == NULL ){};
		error = MPI_Recv (buffer,count,MPI_LONG,0,1,MPI_COMM_WORLD,&status);

		for (int i = 0; i < count; i++)
		{
			cout<<buffer[i]<< " ";
		}
		cout<< endl;

		free(buffer);
	}

	free(src_vector);
	free(dst_vector);
}

int load(char *filename, unsigned char **vector, unsigned long *sizeMtx, unsigned long *sizeCell)
{
	string file_name(filename);
	bitmap_image image(file_name);

	//cout << "Изображение загружено успешно:" << filename << endl; 
	//cout << "Размер " << image.width() << "x" << image.height() << endl;
	//cout << "Всего пикселей: " << image.pixel_count() << endl;
	//cout << "Байт на пиксель: " << image.bytes_per_pixel() << endl;
	//cout << "Размер int: " << sizeof(int) << endl;

	unsigned char red, green, blue;

	*sizeMtx = image.width();
	*sizeCell = image.bytes_per_pixel();

	unsigned long index = 0;
	*vector = (unsigned char *)calloc((*sizeMtx)*(*sizeMtx),*sizeCell);

	for(register int y = 0; y < image.height();y++)
	{
		for(register int x = 0; x < image.width();x++)
		{
			image.get_pixel(x,y,red,green,blue);
			(*vector)[index++] = red;
			(*vector)[index++] = green;
			(*vector)[index++] = blue;
			//cout << (int)red << "-" << (int)green << "-" << (int)blue << "\t";
			//(x == image.width() - 1) cout << endl;
		}
	}

	//Отладочная печать
	//for (int i = 0; i < index; i++)
	//	cout << (int)(*vector)[i] << " ";
	return 0;
}
