#include "IsomorphMatrices.h"
#include "StatisticsMatrix.h"


IsomorphMatrices::IsomorphMatrices(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVectorOriginal, unsigned char *mtxVectorModified)
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

	/**
	*
	*/
	this->mtxStatOriginal = new StatisticsMatrix(this->sizeMtx,this->sizeCell,this->mtxVectorOriginal);
	this->mtxStatModified = new StatisticsMatrix(this->sizeMtx,this->sizeCell,this->mtxVectorModified);

	this->passed = NULL;

	this->counter = 0;

	this->ignors = new MaskVectorTrunk(this->sizeMtx);

	this->startDepth = 0;
}

IsomorphMatrices::~IsomorphMatrices(void)
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

	delete[] this->rowsCurr;
	delete[] this->columnsCurr;
	delete[] this->rowsPrev;
	delete[] this->columnsPrev;

	delete this->substitutions;

	delete this->mtxStatOriginal;
	delete this->mtxStatModified;

	if(this->passed)
		delete this->passed;

	delete this->ignors;

}

bool IsomorphMatrices::CompareIsomorph(unsigned long depth, unsigned long vector[])
{
	this->counter++;
	for(register unsigned long j = 0; j < depth; ++j)
	{
		if(memcmp(this->mtxOriginal[depth - 1][j], this->mtxModified[this->rowsCurr[vector[depth-1]]][this->columnsCurr[vector[j]]],this->sizeCell) == 0 
			&& memcmp(this->mtxOriginal[j][depth - 1], this->mtxModified[this->rowsCurr[vector[j]]][this->columnsCurr[vector[depth - 1]]],this->sizeCell) == 0 ) 
		{
			continue;
		}
		return false;
	}

	return true;
}

void IsomorphMatrices:: SearchIsomorph(unsigned long depth, unsigned long vector[], bool firstCall)
{
	if(depth == 0) throw "SearchIsomorph: WrongArgumentException: Depth <= 0";
	//bool status;
	unsigned long tmp = 0;
	static int counter = 0;
	//length 1,2,3,..,size

	//Если это первый вызов рекурсивной функции тогда нужно сохранить начальную глубину
	if(firstCall == TRUE)
	{
		startDepth = depth;
	}

	for(unsigned long k = 0; k < sizeMtx; ++k)
		std::cout<<vector[k]<<" | ";

	std::cout<< endl;

	if(depth <= this->sizeMtx) 
	{
		this->progress = ((double)100/this->sizeMtx)*depth;
		for(register unsigned long i = depth - 1; i < this->sizeMtx; ++i)
		{


			//есть тривиальный случай при (i) равном (length - 1)
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;

			if(this->ignors->FindMask(vector) == depth)
			{
				tmp = vector[depth-1]; 
				vector[depth-1] = vector[i];
				vector[i] = tmp;
				continue;
			}

			//сравнение матриц
			if(CompareIsomorph(depth, vector))
			{
				if(depth != this->sizeMtx)
				{
					this->SearchIsomorph(depth+1,vector, false);

				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					//std:: cout<<"\nCheck Point Add(vector) Start\n";
					//std:: cout<<"\nNew vector find\n";
					//for(unsigned long k = 0; k < this->sizeMtx; ++k)
					//	std:: cout<<vector[k]<<" | ";
					this->substitutions->Add(vector, this->counter);
					tmp = vector[depth-1]; 
					vector[depth-1] = vector[i];
					vector[i] = tmp;
					this->counter = 0;
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

void IsomorphMatrices:: SearchIsomorphCallback(unsigned long depth, 
											   unsigned long vector[], void (*callback)(IsomorphMatrices*, unsigned long*, unsigned long, unsigned long), bool firstCall)
{
	if(depth == 0) throw "SearchIsomorph: WrongArgumentException: Depth <= 0";
	//bool status;
	unsigned long tmp = 0;
	static int counter = 0;
	static bool terminate = false;
	static long callbackCounter = 0;

	//length 1,2,3,..,size

	//Если это первый вызов рекурсивной функции тогда нужно сохранить начальную глубину
	if(firstCall == TRUE)
	{
		startDepth = depth;
		callbackCounter = 0;
	}

	if(terminate == TRUE)
		return;

	callbackCounter++;
	// && callbackCounter % 1000 == 0
	if(callback != NULL)
	{	
		callback(this, vector, sizeMtx, startDepth);
	}


	if(depth <= this->sizeMtx) 
	{
		this->progress = ((double)100/this->sizeMtx)*depth;
		for(register unsigned long i = depth - 1; i < this->sizeMtx; ++i)
		{


			//есть тривиальный случай при (i) равном (length - 1)
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;

			if(this->ignors->FindMask(vector) == depth)
			{
				tmp = vector[depth-1]; 
				vector[depth-1] = vector[i];
				vector[i] = tmp;
				continue;
			}

			//сравнение матриц
			if(CompareIsomorph(depth, vector))
			{
				if(depth != this->sizeMtx)
				{
					this->SearchIsomorphCallback(depth+1,vector, callback, false);

				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					//std:: cout<<"\nCheck Point Add(vector) Start\n";
					//std:: cout<<"\nNew vector find\n";
					//for(unsigned long k = 0; k < this->sizeMtx; ++k)
					//	std:: cout<<vector[k]<<" | ";
					//std:cout << endl;
					this->substitutions->Add(vector, this->counter);
					tmp = vector[depth-1]; 
					vector[depth-1] = vector[i];
					vector[i] = tmp;
					this->counter = 0;
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

void IsomorphMatrices:: SearchStatisticsIsomorphRecursive(unsigned int depth, unsigned long *vector, statmap *mtxOriginalColumns, statmap *mtxModifiedColumns)
{
	unsigned long tmp = 0;
	for(register unsigned long i = 0; i<this->sizeMtx;++i)
	{
		tmp = vector[depth];
		vector[depth] = i;

		if(!passed[i] && mtxOriginalColumns[depth] == mtxModifiedColumns[i])
		{
			passed[i] = true;
			if(depth == this->sizeMtx-1)
			{
				this->substitutions->Add(vector, this->counter);
				vector[depth] = tmp;
				passed[i] = false;
				this->counter = 0;
				return;
			}
			SearchStatisticsIsomorphRecursive(depth+1,vector,mtxOriginalColumns, mtxModifiedColumns);
			passed[i] = false;
		}
		tmp = vector[depth];
		vector[depth] = i;

	}
}

void IsomorphMatrices:: SearchStatisticsIsomorphRecursiveFull(unsigned int depth, unsigned long *vector, statmap *mtxOriginalColumns, statmap *mtxModifiedColumns, statmap *mtxOriginalRows, statmap *mtxModifiedRows)
{
	unsigned long tmp = 0;
	for(register unsigned long i = 0; i<this->sizeMtx;++i)
	{
		tmp = vector[depth];
		vector[depth] = i;

		if(!passed[i] && mtxOriginalColumns[depth] == mtxModifiedColumns[i] && mtxOriginalRows[depth] == mtxModifiedRows[i])
		{
			passed[i] = true;
			if(depth == this->sizeMtx-1)
			{
				this->substitutions->Add(vector, this->counter);
				vector[depth] = tmp;
				passed[i] = false;
				this->counter = 0;
				return;
			}
			SearchStatisticsIsomorphRecursiveFull(depth+1,vector,mtxOriginalColumns, mtxModifiedColumns, mtxOriginalRows, mtxModifiedRows);
			passed[i] = false;
		}
		tmp = vector[depth];
		vector[depth] = i;

	}
}

void IsomorphMatrices:: SearchStatisticsIsomorph(char options = 3)
{
	/*
	options - Битовая маска: 
	0000 0001 - сбор статистики по колонкам, 
	0000 0010 - сбор статистики по строкам
	*/

	this->mtxStatOriginal->Run(options);	
	this->mtxStatModified->Run(options);	

	//statmap *mtxOriginalColumns;
	//statmap *mtxModifiedColumns;

	//statmap *mtxOriginalRows;
	//statmap *mtxModifiedRows;

	unsigned long *vector = new unsigned long [this->sizeMtx];

	for(register unsigned long i =0; i<this->sizeMtx;++i)
		vector[i] = i;

	if(passed == NULL)
		passed = new bool [this->sizeMtx];

	for(register unsigned long i =0; i<this->sizeMtx;++i)
		passed[i] = false;

	if((options & 0x3) == 0x3)//Колонки + Строки
	{
		statmap *mtxOriginalColumns = this->mtxStatOriginal->GetColumns();
		statmap *mtxModifiedColumns = this->mtxStatModified->GetColumns();
		statmap *mtxOriginalRows = this->mtxStatOriginal->GetRows();
		statmap *mtxModifiedRows = this->mtxStatModified->GetRows();
		SearchStatisticsIsomorphRecursiveFull(0,vector, mtxOriginalColumns, mtxModifiedColumns, mtxOriginalRows, mtxModifiedRows);
	}
	else if((options & 0x1) == 0x1)//Колонки
	{
		statmap *mtxOriginalColumns = this->mtxStatOriginal->GetColumns();
		statmap *mtxModifiedColumns = this->mtxStatModified->GetColumns();
		SearchStatisticsIsomorphRecursive(0,vector, mtxOriginalColumns, mtxModifiedColumns);
	}
	else if((options & 0x2) == 0x2)//Строки
	{
		statmap *mtxOriginalRows = this->mtxStatOriginal->GetRows();
		statmap *mtxModifiedRows = this->mtxStatModified->GetRows();
		SearchStatisticsIsomorphRecursive(0,vector, mtxOriginalRows, mtxModifiedRows);
	}


}

void IsomorphMatrices::TranspositionLeft(unsigned long vector[])
{
	memcpy(this->rowsPrev,this->rowsCurr, sizeof(unsigned long) * this->sizeMtx);
	for(register unsigned long i = 0; i < this->sizeMtx; ++i)
		this->rowsCurr[vector[i]] = this->rowsPrev[i];
}

void IsomorphMatrices::TranspositionRight(unsigned long vector[])
{
	memcpy(this->columnsPrev, this->columnsCurr, sizeof(unsigned long) * this->sizeMtx);
	for(register unsigned long i = 0; i < this->sizeMtx; ++i)
		this->columnsCurr[vector[i]] = this->columnsPrev[i];
}

void IsomorphMatrices::Transposition(unsigned long vector[])
{
	TranspositionLeft(vector);
	TranspositionRight(vector);
}

unsigned long *IsomorphMatrices::GetSubstitutions(unsigned long index)
{
	return (*substitutions)[index];
}

unsigned long IsomorphMatrices::GetSubstitutionsParts(unsigned long index)
{
	return 0;
}

unsigned long IsomorphMatrices::GetSubstitutionsCount()
{
	return this->substitutions->GetSize();
}

double  IsomorphMatrices::GetProgress()
{
	return this->progress;
}

unsigned long IsomorphMatrices::GetParts(unsigned long index)
{
	return this->substitutions->GetParts(index);
}

void IsomorphMatrices::AddToIgnore(unsigned long *vector)
{
	this->ignors->Add(vector,0);
}

unsigned long *IsomorphMatrices::GetSubMatrix(unsigned long *vector)
{
	if(startDepth == 0)
		return NULL;

	//Создаем временную вектор-маску
	unsigned long *mask = (unsigned long*)calloc(this->sizeMtx, sizeof(unsigned long));

	//Заполняем маску. Например: (*0, *1, *2, [3], 4, 5), где глубина = 4
	for (unsigned long i = 0; i < startDepth-1; i++)
	{
		mask[i] = vector[i];
	}

	for (unsigned long i = 0; i < sizeMtx - vector[startDepth-1]; i++)
	{
		if(vector[startDepth-1] + i + 1 < sizeMtx)
		{
			mask[startDepth-1] = vector[startDepth-1] + i + 1;

			//Проверяем маску нет ли ее в игнор списке
			if(this->ignors->FindMask(mask) == 0)
			{
				//Нужно достроить вектор
				return mask;
			}
		}else
		{
			break;
		}
	}

	free(mask);
	return NULL;
}

void IsomorphMatrices::ClearSubstitutions()
{
	this->substitutions->Clear();
}