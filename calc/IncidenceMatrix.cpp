#include "IncidenceMatrix.h"
#include "windows.h"
#include <string>
using namespace std;
IncidenceMatrix::IncidenceMatrix(unsigned long sizeMtx, char *mtxVector)
{
	this->counter = 0;
	this->sizeMtx = sizeMtx;
	this->mtxVector = mtxVector;

	this->mtx = NULL;
	this->mtxVector = NULL;

	/**
	*��������� ������ ��� ��������� �������
	*/
	this->mtx = new char*[sizeMtx];
	for(int i = 0; i < sizeMtx; ++i)
	{
		this->mtx[i] = new char[sizeMtx];
	}

	//this->mtx = (char**)malloc(sizeof(char**));
	//	for(unsigned long i=0;i<this->sizeMtx;i++){
	//		this->mtx[i] = (char*)malloc(sizeof(char)*this->sizeMtx);
	//}
	

	/**
	*�������� ������� PermVectorTrunk ��� �������� �������� ������������
	*/
	this->substitutions = new SubstitutionCollection(this->sizeMtx);

	/**
	*�������������� ������� ������� � ���������� �������
	*/
	unsigned long index = 0;

	for(register unsigned long i=0;i<this->sizeMtx;i++){
		for(register unsigned long j=0;j<this->sizeMtx;j++){
			this->mtx[i][j] = mtxVector[index++];
		}
	}
}

IncidenceMatrix::~IncidenceMatrix(void)
{
	if(this->mtx != NULL)
	{
		for(int i = 0; i < sizeMtx; ++i)
		{
			delete[] this->mtx[i];
		}
		delete[] this->mtx;
	}

	if(this->mtxVector != NULL)
	{
		free(mtxVector);
	}

	delete this->substitutions;
}

bool IncidenceMatrix::CompareAutomorph(unsigned long depth, unsigned long vector[])
{
	counter++;
	this->substitutions->IncrementDepthValue(depth - 1);
	for(register unsigned long j = 0; j < depth; ++j)
	{
		if((this->mtx[depth - 1][j] == this->mtx[vector[depth-1]][vector[j]]) 
			&& (this->mtx[j][depth - 1] == this->mtx[vector[j]][vector[depth - 1]]) )
		{
			continue;
		}
		return false;
	}
	
	return true;
}

void IncidenceMatrix:: SearchAutomorph(unsigned long depth, unsigned long vector[])
{
	if(this->substitutions)
	{
		delete this->substitutions;
	}
	this->substitutions = new SubstitutionCollection(this->sizeMtx);
	this->substitutions->SetInversionIsEnabled(false);
	this->counter = 0;
	SearchAutomorphRecursive(depth, vector);
}

void IncidenceMatrix:: SearchAutomorphRecursive(unsigned long depth, unsigned long vector[])
{
	if(depth == 0) throw "SearchIncidence: WrongArgumentException: Depth == 0";
	bool status;
	static int ctr = 0;
	unsigned long tmp = 0;
	
	//length 1,2,3,..,size
	if(depth <= this->sizeMtx) 
	{
		this->progress = ((double)100/this->sizeMtx)*depth;
		for(register unsigned long i = depth - 1; i < this->sizeMtx; ++i)
		{
			//���� ����������� ������ ��� (i) ������ (length - 1)
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;
		
			//��������� ������
			if(CompareAutomorph(depth, vector))
			{
				if(depth != this->sizeMtx)
				{
					this->SearchAutomorphRecursive(depth+1,vector);

				}			
				else	//���� ������� ��-� ��������� � ������� �����, �� ������ ����� ����������� � ���� ��� ��������
				{
					this->substitutions->Add(vector, counter);
					counter = 0;
					ctr++;
					
				}
			}
			//�� �������� �� ���� ������� ��� ��� ��������� �������� ������, ������� ������� ��-�� �� ���� �����
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;
		}
	}
	else//(���� length > size)
	{
		return;
	}
	return;
}
unsigned long IncidenceMatrix:: GetIterationDepthValue(unsigned long depth)
{
	if(depth >= 0)
	{
		return this->substitutions->GetIterationDepthValue(depth);
	}
	return -1;
}

unsigned long *IncidenceMatrix::GetSubstitutions(unsigned long index)
{
	return (*substitutions)[index]->vector;
}

unsigned long IncidenceMatrix::GetSubstitutionsParts(unsigned long index)
{
	return (*substitutions)[index]->partitions;
}


unsigned long IncidenceMatrix::GetSubstitutionsCount()
{
	return this->substitutions->GetSize();
}

double  IncidenceMatrix::GetProgress()
{
	return this->progress;
}