#pragma once

#include <vector>
#include <stdlib.h>
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

/**
* @brief ����� ��� �������� �������� ������������
*/
class CLASS_DECLSPEC PermVectorTrunk
{
private:
	unsigned long size;
	vector<unsigned long*> trunk;

	vector<unsigned long> parts;
	//Begin iterator
	vector<unsigned long*>::iterator it;
	
	/**
	* @brief ��������� ��������
	* @param v1 ������ ������ ��� ���������
	* @param v2 ������ ������ ��� ���������
	* @return ���������� 0 ���� ������� ���������.
	*/
	int VectorCmp(const unsigned long *v1, const unsigned long* v2);
public:
	/**
	* @brief �����������
	* @param Size ����� ������� ��� ��������
	*/
	PermVectorTrunk(unsigned long Size);

	/**
	* @brief ����������
	*/
	~PermVectorTrunk(void);

	/**
	* @brief �������� ������� � ����������� �������� ������������
	* @param index ������
	*/
	unsigned long* operator[](const unsigned long index);

	/**
	* @brief ���������� ������ ������� ������������ ��� ��������
	* @param vector ������ ������������
	* @param counter ��������������
	*/
	void Add(unsigned long *vector, unsigned long counter);

	/**
	* @brief ����� ������� ������������ ����� ���������
	* @param vector ������ ������������
	* @return ���������� TRUE, ���� ����� ������ ������. ����� FALSE.
	*/
	bool Find(unsigned long *vector);

	/**
	* @brief �������� ���������� ��������� ��������.
	* @return ���������� ���������� ��������� ��������.
	*/
	long GetSize();

	/**
	* @brief �������� ���������� �������.
	* @return ���������� ���������� ��� �������.
	*/
	unsigned long GetParts(unsigned long index);
};

