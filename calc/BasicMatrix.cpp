#include "BasicMatrix.h"

BasicMatrix::BasicMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVector)
{
	this->sizeMtx = sizeMtx;
	this->sizeCell = sizeCell;
	this->mtxVector = mtxVector;

	/**
	* Выделение памяти под ветора перестановок
	*/
	this->rowsCurr = new unsigned long[this->sizeMtx];
	this->rowsPrev = new unsigned long[this->sizeMtx];
	this->columnsCurr = new unsigned long[this->sizeMtx];
	this->columnsPrev = new unsigned long[this->sizeMtx];

	/**
	*Инициализация векторов перестановок
	*/
	for(register unsigned long i = 0; i < this->sizeMtx; ++i)
	{
		this->rowsCurr[i] = i;
		this->columnsCurr[i] = i;
		this->rowsPrev[i] = i;
		this->columnsPrev[i] = i;
	}


	/**
	*Выделение памяти под трехмерную матрицу
	*/
	this->mtx = (unsigned char***)malloc(sizeof(char**)*this->sizeMtx);
	for(register unsigned long i=0;i<this->sizeMtx;i++){
		this->mtx[i] = (unsigned char**)malloc(sizeof(char**)*this->sizeMtx);
		for(register unsigned long j=0;j<this->sizeMtx;j++){
			this->mtx[i][j] = (unsigned char*)malloc(this->sizeCell);
		}
	}

	/**
	*Преобразование вектора матрицы в трехмерную матрицу
	*/
	unsigned long index = 0;

	for(register unsigned long i=0;i<this->sizeMtx;i++){
		for(register unsigned long j=0;j<this->sizeMtx;j++){
			for(register unsigned long n=0;n<this->sizeCell;n++){
				this->mtx[i][j][n] = this->mtxVector[index++];
			}
		}
	}

}

/**
* Деструктор 
*
*/
BasicMatrix::~BasicMatrix(void)
{
	if(this->mtx != NULL)
	{
		for(register unsigned long i=0;i<sizeMtx;i++){
			for(register unsigned long j=0;j<sizeMtx;j++){
				free(this->mtx[i][j]);
			}
			free(this->mtx[i]);
		}
		free(this->mtx);
	}


	delete[] this->rowsCurr;
	delete[] this->rowsPrev;
	delete[] this->columnsCurr;
	delete[] this->columnsPrev;
}

void BasicMatrix::TranspositionLeft(unsigned long vector[])
{
	memcpy(this->rowsPrev,this->rowsCurr, sizeof(unsigned long) * this->sizeMtx);
	for(register unsigned long i = 0; i < this->sizeMtx; ++i)
		this->rowsCurr[vector[i]] = this->rowsPrev[i];
}

void BasicMatrix::TranspositionRight(unsigned long vector[])
{
	memcpy(this->columnsPrev, this->columnsCurr, sizeof(unsigned long) * this->sizeMtx);
	for(register unsigned long i = 0; i < this->sizeMtx; ++i)
		this->columnsCurr[vector[i]] = this->columnsPrev[i];
}

void BasicMatrix::Transposition(unsigned long vector[])
{
	TranspositionLeft(vector);
	TranspositionRight(vector);
}

unsigned char *BasicMatrix::GetMatrix()
{
	unsigned char *mtxVectorTmp = (unsigned char*)malloc(sizeof(char) * this->sizeCell * this->sizeMtx * this->sizeMtx);

	unsigned long index = 0;
	for(register  unsigned long i = 0; i < this->sizeMtx; ++i)
	{
		for(register unsigned long j = 0; j < this->sizeMtx; ++j)
		{
			for(register unsigned long n = 0; n < this->sizeCell; ++n)
			{
				mtxVectorTmp[index++] = this->mtx[rowsCurr[i]][columnsCurr[j]][n];
			}
		}
	}
	return mtxVectorTmp;
}

void BasicMatrix::ImposeGamma(unsigned char* gammaVector, unsigned long gammaSize)
{
	unsigned long index = 0;
	for(register unsigned long i = 0; i < this->sizeMtx; ++i)
	{
		for(register unsigned long j = 0; j < this->sizeMtx; ++j)
		{
			for(register unsigned long n = 0; n < this->sizeCell; ++n)
			{
				this->mtx[rowsCurr[i]][columnsCurr[j]][n] += gammaVector[index++ % gammaSize];
			}
		}
	}
}