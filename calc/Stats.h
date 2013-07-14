#pragma once

#include <time.h>
#include <vector>
#include <string.h>


#ifdef _WIN32 || _WIN64
// Windows Includes Here
#include <windows.h>
#define CLASS_DECLSPEC    __declspec(dllexport)
#endif

#ifdef __linux__
  // Linux Includes Here
  #define CLASS_DECLSPEC
#endif



using namespace std;
//класс для хранения статистик
class CLASS_DECLSPEC Stats
{
	int sizeOfMatrixDim;
public:
	int *partialSubstitution; // массив для хранения кол-ва частичных подстановок на iом уровне
		
	Stats();
	Stats(int sizeOfMatrixDim);
	~Stats();
	vector<time_t> list;
	//Begin iterator
	vector<time_t>::iterator it;
	void Add(time_t time);
	void Add(int num);
	unsigned long size;


	void AddTimePoint();
	void AddSubstitutionToLevel(int level);
	time_t operator[](const unsigned long index);
	//
};
