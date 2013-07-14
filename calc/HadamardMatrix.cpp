#include "HadamardMatrix.h"
#include "windows.h"
#include <string>
using namespace std;
HadamardMatrix::HadamardMatrix(unsigned long sizeMtx, char *mtxVector)
{
	this->counter = 0;
	this->sizeMtx = sizeMtx;
	this->mtxVector = mtxVector;

	this->mtx = NULL;
	this->mtxVector = NULL;

	/**
	*Выделение памяти под двумерную матрицу
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
	*Создания объекта PermVectorTrunk для хранения векторов перестановок
	*/
	this->substitutions = new SubstitutionCollection(this->sizeMtx);

	/**
	*Преобразование вектора матрицы в трехмерную матрицу
	*/
	unsigned long index = 0;

	for(register unsigned long i=0;i<this->sizeMtx;i++){
		for(register unsigned long j=0;j<this->sizeMtx;j++){
			this->mtx[i][j] = mtxVector[index++];
		}
	}
}

HadamardMatrix::~HadamardMatrix(void)
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

unsigned long HadamardMatrix:: GetIterationDepthValue(unsigned long depth)
{
	if(depth >= 0)
	{
		return this->substitutions->GetIterationDepthValue(depth);
	}
	return -1;
}

bool HadamardMatrix::CompareAutomorph(unsigned long depth, unsigned long vector[], char* signs)
{
	counter++;
	this->substitutions->IncrementDepthValue(depth - 1);
	for(register unsigned long j = 0; j < depth; ++j)
	{
		if((this->mtx[depth - 1][j] == this->mtx[vector[depth-1]][vector[j]] * signs[depth-1] * signs[j]) 
			&& (this->mtx[j][depth - 1] == this->mtx[vector[j]][vector[depth - 1]] * signs[depth-1] * signs[j])  )
		{
			continue;
		}
		return false;
	}
	
	return true;
}

void HadamardMatrix:: SearchAutomorph(unsigned long depth, unsigned long vector[])
{
	if(this->substitutions) delete this->substitutions;
	this->substitutions = new SubstitutionCollection(this->sizeMtx);
	this->substitutions->SetInversionIsEnabled(true);
	char *signs = new char[this->sizeMtx];
	for(int i = 0; i < this->sizeMtx; i++)
	{
		signs[i] = 1;
	}
	this->counter = 0;
	SearchAutomorphRecursive(depth, vector, signs);
	delete [] signs;
}

void HadamardMatrix:: SearchAutomorphRecursive(unsigned long depth, unsigned long vector[], char* signs)
{
	if(depth == 0) throw "SearchHadamard: WrongArgumentException: Depth == 0";
	bool status;
	unsigned long tmp = 0;
	
	//length 1,2,3,..,size
	if(depth <= this->sizeMtx) 
	{
		this->progress = ((double)100/this->sizeMtx)*depth;
		for(register unsigned long i = depth - 1; i < this->sizeMtx; ++i)
		{
			//есть тривиальный случай при (i) равном (length - 1)
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;
		
			//сравнение матриц
			if(CompareAutomorph(depth, vector, signs))
			{
				if(depth != this->sizeMtx)
				{
					this->SearchAutomorphRecursive(depth+1,vector, signs);

				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					this->substitutions->Add(vector, signs, counter);
					counter = 0;
					
				}
			}
			signs[depth - 1] = -1;
			if(CompareAutomorph(depth, vector, signs))
			{
				if(depth != this->sizeMtx)
				{
					this->SearchAutomorphRecursive(depth+1,vector, signs);

				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					this->substitutions->Add(vector, signs, counter);
					counter = 0;
				/*	tmp = vector[depth-1]; 
					vector[depth-1] = vector[i];
					vector[i] = tmp;

					signs[depth - 1] = 1;
					counter = 0;

					return;*/
				}
			}


			//не зависимо от того успешно или нет отработал алгоритм поиска, следует вернуть эл-ты на свои места
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;
			signs[depth - 1] = 1;


		}
	}
	else//(если length > size)
	{
		return;
	}
	return;
}

char *HadamardMatrix::GetSigns(unsigned long index)
{
	return (*substitutions)[index]->signs;
}

unsigned long *HadamardMatrix::GetSubstitutions(unsigned long index)
{
	return (*substitutions)[index]->vector;
}

unsigned long HadamardMatrix::GetSubstitutionsParts(unsigned long index)
{
	return (*substitutions)[index]->partitions;
}


unsigned long HadamardMatrix::GetSubstitutionsCount()
{
	return this->substitutions->GetSize();
}

double  HadamardMatrix::GetProgress()
{
	return this->progress;
}