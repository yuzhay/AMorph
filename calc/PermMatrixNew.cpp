#include "PermMatrixNew.h"

/**
* Конструктор 
*
* @matrixSize Размер матрицы (количество строк и столбцов)
* @cellSize Размер ячейки матрицы в байтах
* @matrixVector Указатель на матрицу развернутую в вектор
*/
PermMatrixNew::PermMatrixNew(unsigned long matrixSize, unsigned long cellSize
	, unsigned char *matrixVector)
{
	this->matrixSize = matrixSize;
	this->cellSize = cellSize;
	this->matrixVector = matrixVector;

	this->currRows = new unsigned long[matrixSize];
	this->prevRows = new unsigned long[matrixSize];
	this->currColumns = new unsigned long[matrixSize];
	this->prevColumns = new unsigned long[matrixSize];

	this->matrix = NULL;
	this->matrixVector = NULL;


	this->matrix = (unsigned char***)malloc(sizeof(char**)*matrixSize);
	for(int i=0;i<matrixSize;i++){
		this->matrix[i] = (unsigned char**)malloc(sizeof(char**)*matrixSize);
		for(int j=0;j<matrixSize;j++){
			this->matrix[i][j] = (unsigned char*)malloc(cellSize);
		}
	}

	VectorToMatrix();
}

PermMatrixNew::PermMatrixNew(unsigned long matrixSize, unsigned long cellSize, unsigned char *matrix,
				  unsigned long originalMtxSize, unsigned long origCellSize, unsigned char *originalMtx,
				  unsigned long encryptMtxSize, unsigned long encrCellSize, unsigned char *ecnryptMtx)
{
	InitAutoMorphMatrix(matrixSize, cellSize, matrixVector);
	InitOriginalMatrix(originalMtxSize, origCellSize, originalMtxVector);
	InitEncryptMatrix(encryptMtxSize,encrCellSize,encryptMtxVector);

	this->currRows = new unsigned long[matrixSize];
	this->prevRows = new unsigned long[matrixSize];
	this->currColumns = new unsigned long[matrixSize];
	this->prevColumns = new unsigned long[matrixSize];

	for(int i = 0; i < matrixSize; ++i)
	{
		this->currRows[i] = i;
		this->currColumns[i] = i;
		this->prevRows[i] = i;
		this->prevColumns[i] = i;
	}

	this->automorphTree = new PermVectorTrunk(matrixSize);
	this->isomorphTree = new PermVectorTrunk(matrixSize);
}
	
/**
* Деструктор 
*
*/
PermMatrixNew::~PermMatrixNew(void)
{
	if(this->matrix != NULL)
	{
		for(int i=0;i<matrixSize;i++){
			for(int j=0;j<matrixSize;j++){
				free(this->matrix[i][j]);
			}
			free(this->matrix[i]);
		}
		free(this->matrix);
	}

	if(this->matrixVector != NULL)
	{
		free(matrixVector);
	}

	delete[] this->currRows;
	delete[] this->prevRows;
	delete[] this->currColumns;
	delete[] this->prevColumns;
}

////Init Matrix
void PermMatrixNew::InitAutoMorphMatrix(unsigned long matrixSize, unsigned long cellSize
	, unsigned char *matrixVector)
{
	this->matrixSize = matrixSize;
	this->cellSize = cellSize;
	this->matrixVector = matrixVector;

	//this->currRows = new unsigned long[matrixSize];
	//this->prevRows = new unsigned long[matrixSize];
	//this->currColumns = new unsigned long[matrixSize];
	//this->prevColumns = new unsigned long[matrixSize];

	this->matrix = NULL;
	this->matrixVector = NULL;


	this->matrix = (unsigned char***)malloc(sizeof(char**)*matrixSize);
	for(int i=0;i<matrixSize;i++){
		this->matrix[i] = (unsigned char**)malloc(sizeof(char**)*matrixSize);
		for(int j=0;j<matrixSize;j++){
			this->matrix[i][j] = (unsigned char*)malloc(cellSize);
		}
	}

	VectorToMatrix();
}
void PermMatrixNew::InitOriginalMatrix(unsigned long matrixSize, unsigned long cellSize
	, unsigned char *matrixVector)
{
	this->originalMtxSize = matrixSize;
	this->origCellSize = cellSize;
	this->originalMtxVector = matrixVector;
	///TODO вынести этот участок
	//this->currRows = new unsigned long[matrixSize];
	//this->prevRows = new unsigned long[matrixSize];
	//this->currColumns = new unsigned long[matrixSize];
	//this->prevColumns = new unsigned long[matrixSize];

	this->originalMtx = NULL;
	this->originalMtxVector = NULL;


	this->originalMtx = (unsigned char***)malloc(sizeof(char**)*matrixSize);
	for(int i=0;i<matrixSize;i++){
		this->originalMtx[i] = (unsigned char**)malloc(sizeof(char**)*matrixSize);
		for(int j=0;j<matrixSize;j++){
			this->originalMtx[i][j] = (unsigned char*)malloc(cellSize);
		}
	}

	VectorToOriginalMtx();
}

void PermMatrixNew::InitEncryptMatrix(unsigned long matrixSize, unsigned long cellSize
	, unsigned char *matrixVector)
{
	this->encryptMtxSize = matrixSize;
	this->encrCellSize = cellSize;
	this->encryptMtxVector = matrixVector;

	//this->currRows = new unsigned long[matrixSize];
	//this->prevRows = new unsigned long[matrixSize];
	//this->currColumns = new unsigned long[matrixSize];
	//this->prevColumns = new unsigned long[matrixSize];

	this->encryptMtx = NULL;
	this->encryptMtxVector = NULL;


	this->encryptMtx = (unsigned char***)malloc(sizeof(char**)*matrixSize);
	for(int i=0;i<matrixSize;i++){
		this->encryptMtx[i] = (unsigned char**)malloc(sizeof(char**)*matrixSize);
		for(int j=0;j<matrixSize;j++){
			this->encryptMtx[i][j] = (unsigned char*)malloc(cellSize);
		}
	}

	VectorToEncryptMtx();
}
/////InitMatrix End

/**
* Преобразовывает вектор в матрицу
*
*/
int PermMatrixNew::VectorToMatrix(void)
{
	if(this->matrixVector != NULL)
	{
		unsigned long index = 0;

		for(int i=0;i<matrixSize;i++){
			for(int j=0;j<matrixSize;j++){
				for(int n=0;n<cellSize;n++){
					this->matrix[i][j][n] = this->matrixVector[index++];
				}
			}
		}
		return 0;
	}else{
		return -1;
	}
}


int PermMatrixNew::VectorToOriginalMtx(void)
{
	if(this->originalMtxVector != NULL)
	{
		unsigned long index = 0;

		for(int i=0;i<originalMtxSize;i++){
			for(int j=0;j<originalMtxSize;j++){
				for(int n=0;n<origCellSize;n++){
					this->originalMtx[i][j][n] = this->originalMtxVector[index++];
				}
			}
		}
		return 0;
	}else{
		return -1;
	}
}

int PermMatrixNew::VectorToEncryptMtx(void)
{
	if(this->encryptMtxVector != NULL)
	{
		unsigned long index = 0;

		for(int i=0;i<encryptMtxSize;i++){
			for(int j=0;j<encryptMtxSize;j++){
				for(int n=0;n<encrCellSize;n++){
					this->encryptMtx[i][j][n] = this->encryptMtxVector[index++];
				}
			}
		}
		return 0;
	}else{
		return -1;
	}
}
////VectorToMatrix End

////Setters Start
void PermMatrixNew::SetMatrix(unsigned char ***Mtx)
{
	if(this->matrix != NULL)
	{
		for(int i=0;i<matrixSize;i++){
			for(int j=0;j<matrixSize;j++){
				free(this->matrix[i][j]);
			}
			free(this->matrix[i]);
		}
		free(this->matrix);
	}
	this->matrix = Mtx;
}
void PermMatrixNew::SetOriginalMtx(unsigned char ***originalMtx)
{
	if(this->originalMtx != NULL)
	{
		for(int i=0;i<originalMtxSize;i++){
			for(int j=0;j<originalMtxSize;j++){
				free(this->originalMtx[i][j]);
			}
			free(this->originalMtx[i]);
		}
		free(this->originalMtx);
	}
	this->originalMtx = originalMtx;
}
void PermMatrixNew::SetEncryptMtx(unsigned char ***encryptMtx)
{
	if(this->encryptMtx != NULL)
	{
		for(int i=0;i<encryptMtxSize;i++){
			for(int j=0;j<encryptMtxSize;j++){
				free(this->encryptMtx[i][j]);
			}
			free(this->encryptMtx[i]);
		}
		free(this->encryptMtx);
	}
	this->encryptMtx = encryptMtx;
}
////Setters END

////Getters Start
unsigned char *** PermMatrixNew::GetMatrix(void)
{
	return this->matrix;
}
unsigned char *** PermMatrixNew::GetOriginalMtx(void)
{
	return this->originalMtx;
}
unsigned char *** PermMatrixNew::GetEncryptMtx(void)
{
	return this->encryptMtx;
}
unsigned char *** PermMatrixNew::GetTranspositionMtx(void)
{
	unsigned char ***mtx;
	mtx = (unsigned char***)malloc(sizeof(char**)*this->matrixSize);
	for(int i=0;i<this->matrixSize;i++){
		mtx[i] = (unsigned char**)malloc(sizeof(char**)*this->matrixSize);
		for(int j=0;j<this->matrixSize;j++){
			mtx[i][j] = (unsigned char*)malloc(this->cellSize);
		}
	}

	for(unsigned long i = 0; i < this->matrixSize; ++i)
	{
		for(unsigned long j = 0; j < this->matrixSize; ++j)
		{
			for(unsigned long n = 0; n < this->cellSize; ++n)
			{
				mtx[i][j][n] = this->matrix[currRows[i]][currColumns[j]][n];
			}
			//std::cout << " ";/*Does it make copy of instance?*/
		}
		//std::cout << std::endl;
	}
	return mtx;
}
////Getters END


void PermMatrixNew::LeftTransposition(unsigned long vector[])
{
	memcpy(prevRows,currRows, sizeof(unsigned long) * this->matrixSize);
	for(unsigned long i = 0; i < this->matrixSize; ++i)
		currRows[vector[i]] = prevRows[i];
	
}

/**
* Правая транспозиция
*
* @vector Вектор преобразования
*/
void PermMatrixNew::RightTransposition(unsigned long vector[])
{
	memcpy(prevColumns, currColumns, sizeof(unsigned long) * this->matrixSize);
	for(unsigned long i = 0; i < this->matrixSize; ++i)
		currColumns[vector[i]] = prevColumns[i];
}

/**
* Полная (левая и правая) транспозиция
*
* @vector Вектор преобразования
*/
void PermMatrixNew::Transposition(unsigned long vector[])
{
	LeftTransposition(vector);
	RightTransposition(vector);
}

/**
* Поиск автоморфизмов
*
* @param depth Глубина 
* @vector Вектор преобразования
*/
void PermMatrixNew::SearchAutomorph(unsigned long depth, unsigned long vector[])
{
	if(depth <= 0) throw "SearchAutomorph: WrongArgumentException: Length <= 0";
	bool status;
	unsigned long tmp = 0;
	static int counter = 0;
	//length 1,2,3,..,size
	if(depth <= this->matrixSize) 
	{
		//std:: cout<<"\nLength: "<<length<<"\n";
		for(unsigned long i = depth - 1; i < this->matrixSize; ++i)
		{
			//есть тривиальный случай при (i) равном (length - 1)
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;
		
			//сравнение матриц
			if(CompareAutomorph(depth, vector))
			{
				char buf[10];
				itoa(depth, buf, 10);
				//MessageBoxA(0,buf,"",0);
				//
				if(depth != this->matrixSize)
				{
					this->SearchAutomorph(depth+1,vector);

				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					//std:: cout<<"\nCheck Point Add(vector) Start\n";
					//std:: cout<<"\nNew vector found\n";
					std::cout << endl;
					for(int k = 0; k < this->matrixSize; ++k)
						std:: cout<<vector[k]<<" | ";
					
					automorphTree->Add(vector);
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
		//std:: cout<<"\nLength: "<<length<<"\n";
		return;
	}
	//std:: cout<<"\nLength: "<<length<<"\n";
	return;
}

/**
* Сравнение для автоморфизмов
*
* @depth Глубина 
* @vector Вектор преобразования
*/
bool PermMatrixNew::CompareAutomorph(unsigned long depth, unsigned long vector[])
{
	for(register unsigned long j = 0; j < depth; ++j)
	{
		if(memcmp(this->matrix[depth - 1][j], this->matrix[currRows[vector[depth-1]]][currColumns[vector[j]]],this->cellSize) == 0 
			&& memcmp(this->matrix[j][depth - 1], this->matrix[currRows[vector[j]]][currColumns[vector[depth - 1]]],this->cellSize) == 0 )
		{
			continue;
		}
		return false;
	}
	
	return true;
}

/**
* Поиск изоморфизмов
*
* @depth Глубина 
* @vector Вектор преобразования
*/
void PermMatrixNew::SearchIsomorph(unsigned long depth, unsigned long vector[])
{
	if(depth <= 0) throw "SearchIsomorph: WrongArgumentException: Length <= 0";
	bool status;
	unsigned long tmp = 0;
	static int counter = 0;
	//length 1,2,3,..,size
	if(depth <= this->originalMtxSize) 
	{
		//std:: cout<<"\nLength: "<<length<<"\n";
		for(unsigned long i = depth - 1; i < this->originalMtxSize; ++i)
		{
			//есть тривиальный случай при (i) равном (length - 1)
			tmp = vector[depth-1]; 
			vector[depth-1] = vector[i];
			vector[i] = tmp;
		
			//сравнение матриц
			if(CompareIsomorph(depth, vector))
			{
				char buf[10];
				itoa(depth, buf, 10);
				//MessageBoxA(0,buf,"",0);
				//
				if(depth != this->originalMtxSize)
				{
					this->SearchIsomorph(depth+1,vector);

				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					std:: cout<<"\nCheck Point Add(vector) Start\n";
					std:: cout<<"\nNew vector find\n";
					for(int k = 0; k < this->originalMtxSize; ++k)
						std:: cout<<vector[k]<<" | ";
					isomorphTree->Add(vector);
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
		//std:: cout<<"\nLength: "<<length<<"\n";
		return;
	}
	//std:: cout<<"\nLength: "<<length<<"\n";
	return;
}

/**
* Сравнение для изоморфизмов
*
* @depth Глубина 
* @vector Вектор преобразования
*/
bool PermMatrixNew::CompareIsomorph(unsigned long depth, unsigned long vector[])
{
	for(register unsigned long j = 0; j < depth; ++j)
	{
		if(memcmp(this->originalMtx[depth - 1][j], this->encryptMtx[currRows[vector[depth-1]]][currColumns[vector[j]]],this->cellSize) == 0 
			&& memcmp(this->originalMtx[j][depth - 1], this->encryptMtx[currRows[vector[j]]][currColumns[vector[depth - 1]]],this->cellSize) == 0 ) 
		{
			continue;
		}
		return false;
	}
	
	return true;
}

/**
* Сбросить транспозицию
*
*/
void PermMatrixNew::ResetTransposition()
{
	for(int i = 0; i < this->matrixSize; ++i)
	{
		//привязка указателей к строкам массива data в порядке возрастания 
		prevRows[i] = currRows[i] = i;
		//заполнение массива индексов в порядке возрастания
		currColumns[i] = prevColumns[i] = i;
	}
}

/**
* Печать
*
*/
void PermMatrixNew::Print()
{
	std::cout<< "Matrix" << std::endl;
	for(unsigned long i = 0; i < this->matrixSize; ++i)
	{
		for(unsigned long j = 0; j < this->matrixSize; ++j)
		{
			for(unsigned long n = 0; n < this->cellSize; ++n)
			{
				std::cout << this->matrix[currRows[i]][currColumns[j]][n];
				if(n != this->cellSize-1)
					std::cout << ",";
			}
			std::cout << " ";/*Does it make copy of instance?*/
		}
		std::cout << std::endl;
	}
}

/**
* 
*
* @substitutionVector 
*/
void PermMatrixNew::GetSubstitutionVector(int substitutionVector[])
{
	
	for(unsigned long i = 0; i < this->matrixSize; ++i)
	{
		substitutionVector[i] = this->substitutionStats->partialSubstitution[i];
		
	}
}

/**
* 
*
* @timeVector 
*/
void PermMatrixNew::GetTimeStatsVector(int timeVector[])
{
	for(unsigned long i = 0; i < substitutionStats->list.size(); ++i)
		timeVector[i] = substitutionStats->list[i];

}

/**
* 
*
*/
int PermMatrixNew::GetTimeStatsSize()
{
	return substitutionStats->list.size();
}
