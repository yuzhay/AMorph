#include "DecryptMatrix.h"


DecryptMatrix::DecryptMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVectorOriginal, unsigned char *mtxVectorModified)
{
	this->sizeMtx = sizeMtx;
	this->sizeCell = sizeCell;
	this->mtxVectorOriginal = mtxVectorOriginal;
	this->mtxVectorModified = mtxVectorModified;

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
	*Создания объекта PermVectorTrunk для хранения векторов перестановок
	*/
	this->substitutions = new PermVectorTrunk(this->sizeMtx);

	/**
	*Выделение памяти под трехмерную матрицу
	*/
	this->mtxOriginal = (unsigned char***)malloc(sizeof(char**)*this->sizeMtx);
	this->mtxModified = (unsigned char***)malloc(sizeof(char**)*this->sizeMtx);
	for(register unsigned long i=0;i<this->sizeMtx;i++){
		this->mtxOriginal[i] = (unsigned char**)malloc(sizeof(char**)*this->sizeMtx);
		this->mtxModified[i] = (unsigned char**)malloc(sizeof(char**)*this->sizeMtx);
		for(register unsigned long j=0;j<this->sizeMtx;j++){
			this->mtxOriginal[i][j] = (unsigned char*)malloc(this->sizeCell);
			this->mtxModified[i][j] = (unsigned char*)malloc(this->sizeCell);
		}
	}

	/**
	*Преобразование вектора матрицы в трехмерную матрицу
	*/
	unsigned long index = 0;

	for(register unsigned long i=0;i<this->sizeMtx;i++){
		for(register unsigned long j=0;j<this->sizeMtx;j++){
			for(register unsigned long n=0;n<this->sizeCell;n++){
				this->mtxOriginal[i][j][n] = mtxVectorOriginal[index];
				this->mtxModified[i][j][n] = mtxVectorModified[index++];
			}
		}
	}

}

DecryptMatrix::~DecryptMatrix(void)
{
	if(this->mtxOriginal != NULL)
	{
		for(register unsigned long i=0;i<this->sizeMtx;i++){
			for(register unsigned long j=0;j<this->sizeMtx;j++){
				free(this->mtxOriginal[i][j]);
			}
			free(this->mtxOriginal[i]);
		}
		free(this->mtxOriginal);
	}
	
	if(this->mtxModified != NULL)
	{
		for(register unsigned long i=0;i<this->sizeMtx;i++){
			for(register unsigned long j=0;j<this->sizeMtx;j++){
				free(this->mtxModified[i][j]);
			}
			free(this->mtxModified[i]);
		}
		free(this->mtxModified);
	}


	//if(this->mtxVectorModified != NULL)
	//{
	//	free(this->mtxVectorModified);
	//}

	//if(this->mtxVectorOriginal != NULL)
	//{
	//	free(this->mtxVectorOriginal);
	//}

	delete[] this->rowsCurr;
	delete[] this->columnsCurr;
	delete[] this->rowsPrev;
	delete[] this->columnsPrev;

	delete this->substitutions;
}

bool DecryptMatrix::CompareIsomorph(unsigned long depth, unsigned long vector[])
{
	/*for(register unsigned long j = 0; j < depth; ++j)
	{
		if(memcmp(this->mtxOriginal[depth - 1][j], this->mtxModified[this->rowsCurr[vector[depth-1]]][this->columnsCurr[vector[j]]],this->sizeCell) == 0 
			&& memcmp(this->mtxOriginal[j][depth - 1], this->mtxModified[this->rowsCurr[vector[j]]][this->columnsCurr[vector[depth - 1]]],this->sizeCell) == 0 ) 
		{
			continue;
		}
		return false;
	}*/

	unsigned int cellSumm = 0;
	unsigned long colSumm = 0;
	unsigned long rowSumm = 0;
	for(register unsigned long j = 0; j < depth; ++j)
	{
		/*cellSum = 0;
		for(register unsigned int = 0; i < this->sizeCell; i++)
		{
			cellSumm += this->mtxModified[this->rowsCurr[vector[depth-1]]][this->columnsCurr[vector[j]]][i];
		}*/

		
		if(memcmp(this->mtxOriginal[depth - 1][j], this->mtxModified[this->rowsCurr[vector[depth-1]]][this->columnsCurr[vector[j]]],this->sizeCell) == 0 
			&& memcmp(this->mtxOriginal[j][depth - 1], this->mtxModified[this->rowsCurr[vector[j]]][this->columnsCurr[vector[depth - 1]]],this->sizeCell) == 0 ) 
		{
			continue;
		}
		return false;
	}
	//this->sizeCell
	
	return true;
}

void DecryptMatrix:: SearchIsomorph(unsigned long depth, unsigned long vector[])
{
	if(depth == 0) throw "SearchIsomorph: WrongArgumentException: Depth <= 0";
	//bool status;
	unsigned long tmp = 0;
	static int counter = 0;
	//length 1,2,3,..,size
	if(depth <= this->sizeMtx) 
	{
		this->progress = ((double)100/this->sizeMtx)*depth;
		//std:: cout<<"\nLength: "<<length<<"\n";
		for(register unsigned long i = depth - 1; i < this->sizeMtx; ++i)
		{
			//есть тривиальный случай при (i) равном (length - 1)
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;
		
			//сравнение матриц
			if(CompareIsomorph(depth, vector))
			{
				//char buf[10];
				//itoa(depth, buf, 10);
				//MessageBoxA(0,buf,"",0);
				//
				if(depth != this->sizeMtx)
				{
					this->SearchIsomorph(depth+1,vector);

				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					std:: cout<<"\nCheck Point Add(vector) Start\n";
					std:: cout<<"\nNew vector find\n";
					for(unsigned long k = 0; k < this->sizeMtx; ++k)
						std:: cout<<vector[k]<<" | ";
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

/*void DecryptMatrix:: VectorsToMatrices()
{

}*/

void DecryptMatrix::TranspositionLeft(unsigned long vector[])
{
	memcpy(this->rowsPrev,this->rowsCurr, sizeof(unsigned long) * this->sizeMtx);
	for(register unsigned long i = 0; i < this->sizeMtx; ++i)
		this->rowsCurr[vector[i]] = this->rowsPrev[i];
}

void DecryptMatrix::TranspositionRight(unsigned long vector[])
{
	memcpy(this->columnsPrev, this->columnsCurr, sizeof(unsigned long) * this->sizeMtx);
	for(register unsigned long i = 0; i < this->sizeMtx; ++i)
		this->columnsCurr[vector[i]] = this->columnsPrev[i];
}

void DecryptMatrix::Transposition(unsigned long vector[])
{
	TranspositionLeft(vector);
	TranspositionRight(vector);
}

unsigned long *DecryptMatrix::GetSubstitutions(unsigned long index)
{
	return (*substitutions)[index];
}

unsigned long DecryptMatrix::GetSubstitutionsCount()
{
	return this->substitutions->GetSize();
}

double  DecryptMatrix::GetProgress()
{
	return this->progress;
}