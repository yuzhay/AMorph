#include "StatisticsMatrix.h"
#include <string>

using namespace std;


StatisticsMatrix::StatisticsMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtx)
{
	if(sizeCell > 4)
		throw new exception("Cell size cannot > 4");

	this->columns  = new statmap[sizeMtx];
	this->rows  = new statmap[sizeMtx];
	this->sizeCell = sizeCell;
	this->sizeMtx = sizeMtx;
	this->mtx = mtx;
	this->tmpArray = NULL;
}


StatisticsMatrix::~StatisticsMatrix(void)
{
	if(this->columns)
		delete[] this->columns;
	
	if(this->rows)
		delete[] this->rows;

	if(this->tmpArray)
		delete[] this->tmpArray;
}


void StatisticsMatrix::Run(char options)
{
	/*
	options - Битовая маска: 
		0000 0001 - сбор статистики по колонкам, 
		0000 0010 - сбор статистики по строкам
	*/
	unsigned long checksum = 0;
	//register unsigned long jsum =0;
	//register unsigned long isum = 0;
	for(register unsigned long i = 0; i<sizeMtx;i++)
	{
		for(register unsigned long j = 0;j<sizeMtx;j++)
		{
			checksum = 0;

			memcpy(&checksum,&this->mtx[i*sizeMtx*sizeCell+j*sizeCell],sizeCell);

			//Сохраняем статистику для строк, если статистика будет использоваться для несемметричной подстановки
			if((options & 0x2) == 0x2)
				this->rows[i][checksum]++;

			//Сохраняем статистику для колонок
			if((options & 0x1) == 0x1)
				this->columns[j][checksum]++;
		}
	}
}

statmap StatisticsMatrix::GetColumn(unsigned long index)
{
	
	return this->columns[index];
}

statmap StatisticsMatrix::GetRow(unsigned long index)
{
	return this->rows[index];
}

statmap *StatisticsMatrix::GetColumns()
{
	
	return this->columns;
}

statmap *StatisticsMatrix::GetRows()
{
	return this->rows;
}

unsigned long *StatisticsMatrix::GetColumnArray(unsigned long index, unsigned long* arraySize)
{
	statmap::iterator p;
	if(this->tmpArray)
		delete[] this->tmpArray;
	this->tmpArray  = new unsigned long[this->columns[index].size()*2];
	
	*arraySize = this->columns[index].size()*2;

	unsigned long i = 0;
	for(p = this->columns[index].begin(); p!=this->columns[index].end(); ++p)
	{
		this->tmpArray[i++] = p->first;
		this->tmpArray[i++] = p->second;
	}

	return this->tmpArray;
}

unsigned long *StatisticsMatrix::GetRowArray(unsigned long index, unsigned long* arraySize)
{
	statmap::iterator p;
	if(this->tmpArray)
		delete this->tmpArray;
	this->tmpArray  = new unsigned long[this->columns[index].size()*2];

	*arraySize = this->rows[index].size()*2;
	
	unsigned long i = 0;
	for(p = this->rows[index].begin(); p!=this->rows[index].end(); ++p)
	{
		this->tmpArray[i++] = p->first;
		this->tmpArray[i++] = p->second;
	}

	return this->tmpArray;
}