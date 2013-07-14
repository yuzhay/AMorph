#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include "PermVectorTrunk.h"
#include "Stats.h"
#include "showg.h"
#include <string>


#define CLASS_DECLSPEC    __declspec(dllexport)

/*
Elem - type of the custom element
Size - matrix line size
*/
//#define DEBUG_MODE
//


template <typename Elem>
class CLASS_DECLSPEC PermMatrix
{
private:
	Elem **data;
	Elem **matrix;
	
	unsigned long size;

	unsigned long *currRows;
	unsigned long *prevRows;
	unsigned long *currColumns;
	unsigned long *prevColumns;

	PermVectorTrunk *automorphTree;
//	Stats			*timeStats;
	Stats			*substitutionStats;
public:
	PermMatrix(unsigned long);
	PermMatrix(unsigned long, Elem &);
	PermMatrix(unsigned long, Elem **);
	PermMatrix(unsigned long, char *);
	~PermMatrix(void);

	void Fill(Elem &elem);
	void Null(void);
	void LeftTransposition(unsigned long vector[]);
	void RightTransposition(unsigned long vector[]);
	void Trans(void);
	bool ExportToFile(char *path);
	bool ImportFromFile(char *path);
	void GetMatrix(int matrix[]);
	void SetMatrix(unsigned long, Elem **);
	//ф-ция для получения статистики из класса
	void GetSubstitutionVector(int substitutionVector[]);
	void GetTimeStatsVector(int timeVector[]);
	int GetTimeStatsSize();

	void Transposition(unsigned long vector[]);
	void SearchAutomorph(unsigned long length, unsigned long vector[]);
	bool CompareAutomorph(unsigned long length, unsigned long vector[]);
	void ResetTransposition(); //временная
	/*Elem *ReverseArray(Elem*orig,unsigned long b);*/

	void TimeAndNumAuthGrapthPointsMaker();

	void Print();
	friend std::ostream& operator<<(std::ostream& os, const PermMatrix &obj);

	//private: bool firstTransposition;

	//Import/Export 
	//ONLY Eleme = INT, LONG
	bool LoadFromFile(char *path, char separator = ',');
	bool SaveToFile(char *path, char separator = ',');
};


//Пустой конструктор
template <typename Elem>
PermMatrix<Elem>::PermMatrix(unsigned long Size):size(Size)
{
	this->data = new Elem*[size];
	for(int i = 0; i < size; ++i)
	{
		this->data[i] = new Elem[size];
	}

	this->currRows = new unsigned long[size];
	this->prevRows = new unsigned long[size];
	this->currColumns = new unsigned long[size];
	this->prevColumns = new unsigned long[size];

	automorphTree = new PermVectorTrunk(size);
	//timeStats = new Stats();
	substitutionStats = new Stats(this->size);
}

//Конструктор загрузки матрицы из файла
template <typename Elem>
PermMatrix<Elem>::PermMatrix(unsigned long Size, char *path):size(Size)
{
	this->data = new Elem*[size];
	for(int i = 0; i < size; ++i)
	{
		this->data[i] = new Elem[size];
	}

	this->currRows = new unsigned long[size];
	this->prevRows = new unsigned long[size];
	this->currColumns = new unsigned long[size];
	this->prevColumns = new unsigned long[size];

	
	ImportFromFile(path);
	automorphTree = new PermVectorTrunk(size);
	substitutionStats = new Stats(this->size);
}

//Конструктор класса с параметром матрица
template <typename Elem> 
PermMatrix<Elem>::PermMatrix(unsigned long Size, Elem **matrix):size(Size)
{
	this->data = new Elem*[size];
	for(unsigned long i = 0; i < size; ++i)
	{
		this->data[i] = new Elem[size];
	}

	this->currRows = new unsigned long[size];
	this->prevRows = new unsigned long[size];
	this->currColumns = new unsigned long[size];
	this->prevColumns = new unsigned long[size];

	Elem *tmp = (Elem*)matrix;
	//инициализация эл-тов матрицы data
	for(unsigned long i = 0; i < Size; ++i)
	{
		for(unsigned long j = 0; j < Size; ++j)
		{
			this->data[i][j] = tmp[i * Size+ j];
		}
	}

	//ВОЗМОЖНО ОБЪЕДИНИТЬ ЭТИ 2 ЦИКЛЫ ДЛЯ ОПТИМИЗАЦИИ!
	//Цикл инициализации вспомогательных массивов
	for(unsigned long i = 0; i < Size; ++i)
	{
		//привязка указателей к строкам массива data в порядке возрастания 
		prevRows[i] = currRows[i] = i;
		//заполнение массива индексов в порядке возрастания
		currColumns[i] = prevColumns[i] = i;
	}
	automorphTree = new PermVectorTrunk(size);
	substitutionStats = new Stats(this->size);
}

//Конструктор класса с параметром элемент матрицы (заполняет матрицу этим эл-том)
template <typename Elem>
PermMatrix<Elem>::PermMatrix(unsigned long Size, Elem &elem):size(Size)
{
	this->data = new Elem*[size];
	for(int i = 0; i < size; ++i)
	{
		this->data[i] = new Elem[size];
	}

	this->currRows = new unsigned long[size];
	this->prevRows = new unsigned long[size];
	this->currColumns = new unsigned long[size];
	this->prevColumns = new unsigned long[size];

	this->Fill(elem);
	//firstTransposition = false;
	automorphTree = new PermVectorTrunk(size);
	substitutionStats = new Stats(this->size);
}

//Деструктор
template <typename Elem>
PermMatrix<Elem>::~PermMatrix(void)
{
	for(int i = 0; i < size; ++i)
	{
		delete[] this->data[i];
	}
	delete[] this->data;

	delete[] this->currRows;
	delete[] this->prevRows;
	delete[] this->currColumns;
	delete[] this->prevColumns;

	delete this->automorphTree;
	delete this->substitutionStats;
	//if(this->matrix!=NULL)
	//{
	//	for(int i = 0; i < size; ++i)
	//	{
	//		delete[] this->matrix[i];
	//	}
	//	delete[] this->matrix;
	//}
}




////////////////МЕТОДЫ///////////////////

//Конструктор класса с параметром матрица
template <typename Elem> 
void PermMatrix<Elem>::SetMatrix(unsigned long Size, Elem **matrix)
{
	if(Size == this->size)
	{
		Elem *tmp = (Elem*)matrix;
		for(unsigned long i = 0; i < Size; ++i)
		{
			for(unsigned long j = 0; j < Size; ++j)
			{
				this->data[i][j] = tmp[i * Size+ j];
			}
		}

		for(unsigned long i = 0; i < Size; ++i)
		{
			//привязка указателей к строкам массива data в порядке возрастания 
			prevRows[i] = currRows[i] = i;
			//заполнение массива индексов в порядке возрастания
			currColumns[i] = prevColumns[i] = i;
		}

		if(automorphTree!=NULL) delete automorphTree;
		if(this->substitutionStats!=NULL) delete substitutionStats;

		automorphTree = new PermVectorTrunk(size);
		substitutionStats = new Stats(this->size);

	}else
	{
		//Destructor
		for(int i = 0; i < size; ++i)
		{
			delete[] this->data[i];
		}
		delete[] this->data;

		delete[] this->currRows;
		delete[] this->prevRows;
		delete[] this->currColumns;
		delete[] this->prevColumns;

		delete this->automorphTree;
		delete this->substitutionStats;


		//New source matrix
		this->size = Size;
		this->data = new Elem*[size];
		for(unsigned long i = 0; i < size; ++i)
		{
			this->data[i] = new Elem[size];
		}

		this->currRows = new unsigned long[size];
		this->prevRows = new unsigned long[size];
		this->currColumns = new unsigned long[size];
		this->prevColumns = new unsigned long[size];

		Elem *tmp = (Elem*)matrix;
		//инициализация эл-тов матрицы data
		for(unsigned long i = 0; i < Size; ++i)
		{
			for(unsigned long j = 0; j < Size; ++j)
			{
				this->data[i][j] = tmp[i * Size+ j];
			}
		}

		//ВОЗМОЖНО ОБЪЕДИНИТЬ ЭТИ 2 ЦИКЛЫ ДЛЯ ОПТИМИЗАЦИИ!
		//Цикл инициализации вспомогательных массивов
		for(unsigned long i = 0; i < Size; ++i)
		{
			//привязка указателей к строкам массива data в порядке возрастания 
			prevRows[i] = currRows[i] = i;
			//заполнение массива индексов в порядке возрастания
			currColumns[i] = prevColumns[i] = i;
		}
		automorphTree = new PermVectorTrunk(size);
		this->substitutionStats = new Stats(this->size);
	}

	
}

//Занулить матрицу
template <typename Elem>
void PermMatrix<Elem>::Null(void)
{
	memset(data, 0x0, sizeof(Elem)*size*size);
}

//Заполняет матрицу входным эл-том
template <typename Elem>
void PermMatrix<Elem>::Fill(Elem &elem)
{
	for(int i = 0; i < size; ++i)
	{
		for(int j = 0; j < size; ++j)
		{
			this->data[i][j] = elem;/*Does it make copy of instance?*/
		}
	}
}

template <typename Elem>
bool PermMatrix<Elem>::ExportToFile(char *path)
{
	/*Open file and write to file.
	If there is some error, return false*/
	return true;
}

template <typename Elem>
bool PermMatrix<Elem>::ImportFromFile(char *path)
{
	/*Open file and read from file.
	If there is some error, return false*/
	return true;
}

//Перестановка строк
template <typename Elem>
void PermMatrix<Elem>:: LeftTransposition(unsigned long vector[])
{
			
		
		
	//if(!firstTransposition)
	//{
	//	for(unsigned long i = 0; i < size; ++i)
	//		currRows[vector[i]] = i;
	//	firstTransposition = true;
	//}else
	//{
	//	memcpy(prevRows,currRows, sizeof(unsigned long) * size);
	//	for(unsigned long i = 0; i < size; ++i)
	//		currRows[i] = prevRows[vector[i]];
	//	//currRows[i] = prevRows[vector[prevRows]];
	//}


	memcpy(prevRows,currRows, sizeof(unsigned long) * size);
	for(unsigned long i = 0; i < size; ++i)
		currRows[vector[i]] = prevRows[i];
	
}
//template <typename Elem>
//Elem *PermMatrix<Elem>::ReverseArray(Elem*orig,unsigned long b)
//{ 
//    unsigned short int a=0;
//    int swap;
//    
//    for(a;a<--b;a++) //increment a and decrement b until they meet eachother
//    {
//        swap=orig[a];       //put what's in a into swap space
//        orig[a]=orig[b];    //put what's in b into a
//        orig[b]=swap;       //put what's in the swap (a) into b
//    }
//    
//    return orig;    //return the new (reversed) string (a pointer to it)
//}

//Перестановка столбцов
template <typename Elem>
void PermMatrix<Elem>::  RightTransposition(unsigned long vector[])
{

	memcpy(prevColumns, currColumns, sizeof(unsigned long) * size);
	for(unsigned long i = 0; i < size; ++i)
		currColumns[vector[i]] = prevColumns[i];
}

//Перестановка строк и столбцов
template <typename Elem>
void PermMatrix<Elem>::  Transposition(unsigned long vector[])
{
	this->LeftTransposition(vector);
	this->RightTransposition(vector);
}

//Нахождение автоморфизмов (рекурсивный вариант)
//параметры:
//	length хранит длину рассматриваемой подстановки, length 1,2,3...,size;
//  vector[] служит для хранения массива подстановки
template <typename Elem>
void PermMatrix<Elem>::  SearchAutomorph(unsigned long length, unsigned long vector[])
{
	if(length <= 0) throw "SearchAutomorph: WrongArgumentException: Length <= 0";
	bool status;
	unsigned long tmp = 0;
	static int counter = 0;
	//length 1,2,3,..,size
	if(length <= size) 
	{
		//std:: cout<<"\nLength: "<<length<<"\n";
		for(unsigned long i = length - 1; i < size; ++i)
		{
			//есть тривиальный случай при (i) равном (length - 1)
			tmp = vector[length-1]; 
			vector[length-1] = vector[i];
			vector[i] = tmp;
			
			//test
				/*	std:: cout<<"\nTest vector\n Length: "<<length<<"\n";
					for(int k = 0; k < size; ++k)
					std:: cout<<vector[k]<<" | ";*/
			//

			//сравнение матриц
			if(CompareAutomorph(length, vector))
			{
				this->substitutionStats->AddSubstitutionToLevel(length-1);
				//debug
				char buf[10];
				itoa(length, buf, 10);
				//MessageBoxA(0,buf,"",0);
				//
				if(length != size)
				{
					this->SearchAutomorph(length+1,vector);

				}			
				else	//если текущий эл-т последний и матрицы равны, то найден новый автоморфизм и надо его добавить
				{
					std:: cout<<"\nCheck Point Add(vector) Start\n";
					std:: cout<<"\nNew vector find\n";
					for(int k = 0; k < size; ++k)
						std:: cout<<vector[k]<<" | ";

					automorphTree->Add(vector);
					//timeStats->AddTimePoint();

					tmp = vector[length-1]; 
					vector[length-1] = vector[i];
					vector[i] = tmp;

					//std:: cout<<"\nCheck Point Add(vector) Finished\n";
					//std:: cout<<"\nCounter"<<counter++<<"\n";
					
					/*std:: cout<<"\nNew vector\n";
					for(int k = 0; k < size; ++k)
						std:: cout<<vector[k]<<" | ";*/

					return;
				}
			}
			//не зависимо от того успешно или нет отработал алгоритм поиска, следует вернуть эл-ты на свои места
			tmp = vector[length-1]; 
			vector[length-1] = vector[i];
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
//Сравнение автоморфизма с исходной матрицей
template <typename Elem>
bool PermMatrix<Elem>::  CompareAutomorph(unsigned long length, unsigned long vector[])
{
	for(register unsigned long i = 0; i < length; ++i)
	{
		for(register unsigned long j = 0; j < length; ++j)
		{
			if(data[i][j] == data[currRows[vector[i]]][currColumns[vector[j]]]) continue;
			return false;
		}
	}

	return true;
}

//сброс траспозиции (временная)
template <typename Elem>
void PermMatrix<Elem>::  ResetTransposition()
{
	
	for(int i = 0; i < size; ++i)
	{
		//привязка указателей к строкам массива data в порядке возрастания 
		prevRows[i] = currRows[i] = i;
		//заполнение массива индексов в порядке возрастания
		currColumns[i] = prevColumns[i] = i;
	}
}

//Ф-ция вывода
template <typename Elem>
void PermMatrix<Elem>::Print()
{
	std::cout<< "Matrix" << std::endl;
	for(unsigned long i = 0; i < size; ++i)
	{
		for(unsigned long j = 0; j < size; ++j)
		{
			std::cout << data[currRows[i]][currColumns[j]] << " ";/*Does it make copy of instance?*/
		}
		std::cout << std::endl;
	}
}

template <typename Elem>
void PermMatrix<Elem>::GetMatrix(int *matrix)
{

	unsigned long a,b,c;
	for(unsigned long i = 0; i < size; ++i)
	{
		for(unsigned long j = 0; j < size; ++j)
		{
			//a = currRows[i];
			//b = currColumns[j];
			//c = i*size+j;
			//matrix[i,j] = data[a][b];
			matrix[i*size+j] = data[currRows[i]][currColumns[j]];
			//matrix[i*size+j] = data[currColumns[j]][currRows[i]];
			//char buf[10];


		}
		//fputs("\n",f);
	}
	//fputs("\n\n",f);
	//fclose(f);
}
template <typename Elem>
void PermMatrix<Elem>::GetSubstitutionVector(int substitutionVector[])
{
	
	for(unsigned long i = 0; i < size; ++i)
	{
		substitutionVector[i] = this->substitutionStats->partialSubstitution[i];
		
	}
}
template <typename Elem>
void PermMatrix<Elem>::GetTimeStatsVector(int timeVector[])
{
	for(unsigned long i = 0; i < substitutionStats->list.size(); ++i)
		timeVector[i] = substitutionStats->list[i];

}

template <typename Elem>
int PermMatrix<Elem>::GetTimeStatsSize()
{
	return substitutionStats->list.size();
}
//Недогруженный оператор вывода
template <typename Elem>
std::ostream& operator<<(std::ostream& os, const PermMatrix<Elem> &obj) 
{
	/*Here will be matrix output*/
	os << "Matrix" << std::endl;

	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			os << obj.data[i][j] << " ";/*Does it make copy of instance?*/
		}
		os << std::endl;
	}
	
	return os;
}


//Need to be debuged
template <typename Elem>
bool PermMatrix<Elem>::LoadFromFile(char *path, char separator = ','){
	string line;
	int found;
	Elem** arr;
	long new_size = 0;
	long row = 0;
	long col = 0;
	ifstream i_file(path,ifstream::in);
	if(i_file.is_open()){

		while (! i_file.eof() ){
			getline(i_file,line);
			
			
			//Get new size
			if(new_size == 0){
				const char * l = line.c_str();
				for(unsigned int i = 0;i<line.length(); ++i){
					if(l[i]==separator)
						new_size++;
				}
				new_size++;

				//Delete old data
				if(this->data != NULL && new_size != size){
					for(int i = 0; i < size; ++i)
					{
						delete[] this->data[i];
					}
					delete[] this->data;
					delete[] this->currRows;
					delete[] this->prevRows;
					delete[] this->currColumns;
					delete[] this->prevColumns;

					//Allocate memory
					this->data = new Elem*[new_size];
					for(unsigned long i = 0; i < new_size; ++i)
					{
						this->data[i] = new Elem[new_size];
					}



					this->currRows = new unsigned long[new_size];
					this->prevRows = new unsigned long[new_size];
					this->currColumns = new unsigned long[new_size];
					this->prevColumns = new unsigned long[new_size];

					//ВОЗМОЖНО ОБЪЕДИНИТЬ ЭТИ 2 ЦИКЛЫ ДЛЯ ОПТИМИЗАЦИИ!
					//Цикл инициализации вспомогательных массивов
					for(unsigned long i = 0; i < new_size; ++i)
					{
						//привязка указателей к строкам массива data в порядке возрастания 
						prevRows[i] = currRows[i] = i;
						//заполнение массива индексов в порядке возрастания
						currColumns[i] = prevColumns[i] = i;
					}
					if(automorphTree != NULL) delete automorphTree;
					automorphTree = new PermVectorTrunk(new_size);

					if(substitutionStats != NULL) delete substitutionStats;
					substitutionStats = new Stats();

					this->size = new_size;
				}
			}

			//String splitting by     
			found = line.find_first_of(separator);
			while(found != string::npos){
				if(found > 0){
					//results->push_back(str.substr(0,found));
					this->data[row][col] = atoi(line.substr(0,found).c_str());
					;
					col++;
				}
				line = line.substr(found+1);
				found = line.find_first_of(separator);
			}
			if(line.length() > 0){
				//results->push_back(str);
				this->data[row][col] = atoi(line.c_str());
			}
			row++;
		}
		i_file.close();
		return true;
	}else{
		i_file.close();
		return false;
	}
	
}

template <typename Elem>
bool PermMatrix<Elem>::SaveToFile(char *path, char separator = ','){
	ofstream o_file (path,ios_base::out);
	if(!o_file.fail()){
		for(unsigned long i = 0; i < size; ++i)
		{
			for(unsigned long j = 0; j < size; ++j)
			{
				o_file<<data[currRows[i]][currColumns[j]];
				if(j != size-1)
					o_file<<separator;
			}
			o_file<<endl;
		}
		o_file.flush();
		o_file.close();
		return true;
	}else{
		o_file.close();
		return false;
	}

}