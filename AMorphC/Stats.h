#pragma once

#include <time.h>
#include <vector>


#define CLASS_DECLSPEC    __declspec(dllimport)


using namespace std;
//����� ��� �������� ���������
class CLASS_DECLSPEC Stats
{
int sizeOfMatrixDim;
public:
	int *partialSubstitution; // ������ ��� �������� ���-�� ��������� ����������� �� i�� ������
		
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
	
};
