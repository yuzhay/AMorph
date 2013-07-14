#include "AutomorphMatrix.h"
#include "windows.h"
#include <string>
using namespace std;
AutomorphMatrix::AutomorphMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVector)
{
	this->sizeMtx = sizeMtx;
	this->sizeCell = sizeCell;
	this->mtxVector = mtxVector;

	this->mtx = NULL;
	this->mtxVector = NULL;

	/**
	*Выделение памяти под трехмерную матрицу
	*/
	this->mtx = (unsigned char***)malloc(sizeof(char**)*this->sizeMtx);
	for(unsigned long i=0;i<this->sizeMtx;i++){
		this->mtx[i] = (unsigned char**)malloc(sizeof(char**)*this->sizeMtx);
		for(unsigned long j=0;j<this->sizeMtx;j++){
			this->mtx[i][j] = (unsigned char*)malloc(this->sizeCell);
		}
	}

	/**
	*Создания объекта PermVectorTrunk для хранения векторов перестановок
	*/
	this->substitutions = new PermVectorTrunk(this->sizeMtx);

	/**
	*Преобразование вектора матрицы в трехмерную матрицу
	*/
	unsigned long index = 0;

	for(register unsigned long i=0;i<this->sizeMtx;i++){
		for(register unsigned long j=0;j<this->sizeMtx;j++){
			for(register unsigned long n=0;n<this->sizeCell;n++){
				this->mtx[i][j][n] = mtxVector[index++];
			}
		}
	}
}

AutomorphMatrix::~AutomorphMatrix(void)
{
	if(this->mtx != NULL)
	{
		for(register unsigned long i=0;i<this->sizeMtx;i++){
			for(register unsigned long j=0;j<this->sizeMtx;j++){
				free(this->mtx[i][j]);
			}
			free(this->mtx[i]);
		}
		free(this->mtx);
	}

	if(this->mtxVector != NULL)
	{
		free(mtxVector);
	}

	delete this->substitutions;
}

bool AutomorphMatrix::CompareAutomorph(unsigned long depth, unsigned long vector[])
{
	for(register unsigned long j = 0; j < depth; ++j)
	{
		if(memcmp(this->mtx[depth - 1][j], this->mtx[vector[depth-1]][vector[j]],this->sizeCell) == 0 
			&& memcmp(this->mtx[j][depth - 1], this->mtx[vector[j]][vector[depth - 1]],this->sizeCell) == 0 )
		{
			continue;
		}
		return false;
	}
	
	return true;
}

void AutomorphMatrix:: SearchAutomorph(unsigned long depth, unsigned long vector[])
{
	if(depth == 0) throw "SearchAutomorph: WrongArgumentException: Depth == 0";
	bool status;
	unsigned long tmp = 0;
	static int counter = 0;
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
			if(CompareAutomorph(depth, vector))
			{
				if(depth != this->sizeMtx)
				{
					this->SearchAutomorph(depth+1,vector);
				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					this->substitutions->Add(vector,0);
					tmp = vector[depth-1]; 
					vector[depth-1] = vector[i];
					vector[i] = tmp;
					return;
				}
			}
			//не зависимо от того успешно или нет отработал алгоритм поиска, следует вернуть эл-ты на свои места
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;
		}
	}
	else//(если length > size)
	{
		return;
	}
	return;
}

unsigned long *AutomorphMatrix::GetSubstitutions(unsigned long index)
{
	return (*substitutions)[index];
}


unsigned long AutomorphMatrix::GetSubstitutionsCount()
{
	return this->substitutions->GetSize();
}

double  AutomorphMatrix::GetProgress()
{
	return this->progress;
}