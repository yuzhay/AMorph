// MpiAMorph.cpp : Defines the entry point for the console application.
//

#include "mpi.h"
#include <iostream>
#include <vector>
#include "msgtags.h"
#include "msp.h"
#include "Files.h"
#include "Library.h"

#include "VertexContainer.h"

using namespace std;

//��������� �� �����
char *srcFile,*dstFile;


void Start();





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
	* ��� �������������� �������.
	* �������� ������ ������ (�� ���� ����� ���������� �����, �������������� � �� ������� �����) 
	* � ���������� ����� ���������� ������.
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
	//v[2] = 6;

	//unsigned long *v2 = (unsigned long *)malloc(sizeof(long)*4);

	//v2[0] = 3;
	//v2[1] = 1;
	//v2[2] = 5;
	//v2[3] = 9;

	//VertexContainer vc;
	////vc.Add(v,3);
	//vc.Add(v2,4);


	//cout << vc.Find(v,3,2) <<endl;

	//free(v);
	//free(v2);

	//vc.Print();

	
}