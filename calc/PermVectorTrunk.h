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
protected:
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
	virtual long VectorCmp(const unsigned long *v1, const unsigned long* v2);
public:
	/**
	* @brief �����������
	* @param Size ����� ������� ��� ��������
	*/
	PermVectorTrunk(unsigned long Size);

	/**
	* @brief ����������
	*/
	virtual ~PermVectorTrunk(void);

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
	* @brief �������� ������� ������������

	*/
	void Clear();

	/**
	* @brief ����� ������� ������������ ����� ���������
	* @param vector ������ ������������
	* @return ���������� TRUE, ���� ����� ������ ������. ����� FALSE.
	*/
	virtual bool Find(unsigned long *vector);

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

	//virtual long FindMask(unsigned long *vector);

	//long PermVectorTrunk::MaskCmp(const unsigned long *v1, const unsigned long* v2);
};

/**
* @brief ����� ��� �������� ����� ��������
*/
class CLASS_DECLSPEC MaskVectorTrunk: public PermVectorTrunk
{
public:
	/**
	* @brief �����������
	* @param Size ����� ������� ��� ��������
	*/
	MaskVectorTrunk(unsigned long Size);

	/**
	* @brief ����������
	*/
	virtual ~MaskVectorTrunk(void);

	long FindMask(unsigned long *vector);

	long MaskCmp(const unsigned long *v1, const unsigned long* v2);
};

