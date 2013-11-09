// MpiAMorph.cpp : Defines the entry point for the console application.
//

#include <mpi.h>
#include <iostream>
#include "bitmap_image.hpp"


using namespace std;


int load(char *fielname, unsigned char **vector, unsigned long *sizeMtx, unsigned long *sizeCell);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	int myNode, totalNodes;

	if(argc != 2)
	{
		cout << "Параметры введены неправильно." << endl;
		cout << "Пример: amorph src_bmp dst_bmp" << endl;
		return;
	}


	int rc;
	MPI::Init();
	if(rc = MPI_Init(&argc,&argv))
	{
		cout << "MPI_Init ошибка " << endl;
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


	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char * src_vector;
	unsigned char * dst_vector;

	load(argv[1], &src_vector, &sizeMtx, &sizeCell);
	load(argv[2], &dst_vector, &sizeMtx, &sizeCell);

	free(src_vector);
	free(dst_vector);
	MPI_Finalize();
	return 0;
}

int load(char *filename, unsigned char **vector, unsigned long *sizeMtx, unsigned long *sizeCell)
{
	string file_name(filename);
	bitmap_image image(file_name);

	cout << "Изображение загружено успешно:" << filename << endl; 
	cout << "Размер " << image.width() << "x" << image.height() << endl;
	cout << "Всего пикселей: " << image.pixel_count() << endl;
	cout << "Байт на пиксель: " << image.bytes_per_pixel() << endl;
	cout << "Размер int: " << sizeof(int) << endl;
 
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
			cout << (int)red << "-" << (int)green << "-" << (int)blue << "\t";
			if(x == image.width() - 1) cout << endl;
		}
	}

	//Отладочная печать
	//for (int i = 0; i < index; i++)
	//	cout << (int)(*vector)[i] << " ";
	return 0;
}

