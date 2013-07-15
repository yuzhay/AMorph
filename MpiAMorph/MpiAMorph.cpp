// MpiAMorph.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
	int myNode, totalNodes;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &totalNodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myNode);



	MPI_Finalize();
	return 0;
}

