#pragma once
#include <map>
#include <string>

#define statmap map<unsigned long,unsigned long>


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

/**
* @brief ����� ��� �������� ���������� �� �������
*/
class CLASS_DECLSPEC StatisticsMatrix
{
private:
	statmap *columns;
	statmap *rows;
	unsigned long *tmpArray;
	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char *mtx;

public:
	/* 
	* @brief �����������
	* @param sizeMtx ������ ������
	* @param sizeCell ������ ����� ������
	* @param mtx  �������
	*/
	StatisticsMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtx);

	/* 
	* @brief ��������� ���� ����������
	* @param options  ������� �����: 3 - ���� ���������� �� �������� � �������, 1 - ���� ���������� �� ��������, 2 - ���� ���������� �� �������
	*/
	void Run(char options = 3);

	/* 
	* @brief �������� ������ ���������� ��� ��������
	* @return ���������� ������ ���������� ��� �������
	*/
	statmap GetColumn(unsigned long);

	/* 
	* @brief �������� ������ ���������� ��� ������
	* @return ���������� ������ ���������� ��� �����
	*/
	statmap GetRow(unsigned long);

	/* 
	* @brief �������� ������ ���������� ��� �����
	* @return ���������� ������ ���������� ��� �����
	*/
	statmap *GetRows();

		/* 
	* @brief �������� ������ ���������� ��� �������
	* @return ���������� ������ ���������� ��� �������
	*/
	statmap *GetColumns();

	/* 
	* @brief �������� ������ ���������� ��� �������
	* @return ���������� ������ ���������� ��� �������
	*/
	unsigned long *GetColumnArray(unsigned long, unsigned long*);

	/* 
	* @brief �������� ������ ���������� ��� �����
	* @return ���������� ������ ���������� ��� �����
	*/
	unsigned long *GetRowArray(unsigned long, unsigned long*);

	/* 
	* @brief ����������
	*/
	~StatisticsMatrix(void);
};

