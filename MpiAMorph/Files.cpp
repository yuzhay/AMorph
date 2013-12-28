#include "Files.h"
#include "bitmap_image.hpp"
#include <iostream>

using namespace std;

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

int load(char *filename, unsigned char **vector, unsigned long *sizeMtx, unsigned long *sizeCell)
{
	string file_name(filename);
	bitmap_image image(file_name);

	//cout << "����������� ��������� �������:" << filename << endl; 
	//cout << "������ " << image.width() << "x" << image.height() << endl;
	//cout << "����� ��������: " << image.pixel_count() << endl;
	//cout << "���� �� �������: " << image.bytes_per_pixel() << endl;
	//cout << "������ int: " << sizeof(int) << endl;

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

	//���������� ������
	//for (int i = 0; i < index; i++)
	//	cout << (int)(*vector)[i] << " ";
	return 0;
}