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
struct Substitution
{
	unsigned long size;

	unsigned long partitions;
	char *signs;
	unsigned long* vector;
};
class CLASS_DECLSPEC SubstitutionCollection
{
private:
	unsigned long size;
	bool InversionIsEnabled;
	unsigned long *iterations;
	vector<Substitution*> trunk;

	//Begin iterator
	vector<Substitution*>::iterator it;
	
public:
	/**
	* @brief �����������
	* @param Size ����� ������� ��� ��������
	*/
	SubstitutionCollection(unsigned long Size);

	/**
	* @brief ����������
	*/
	~SubstitutionCollection(void);

	void IncrementDepthValue(unsigned long);
	unsigned long  GetIterationDepthValue(unsigned long);
	/**
	* @brief �������� ������� � ����������� �������� ������������
	* @param index ������
	*/
	Substitution *operator[](const unsigned long index);

	/**
	* @brief ���������� ������ ������� ������������ ��� ��������
	* @param vector ������ ������������
	*/
	void Add(Substitution *sub);
	void Add(unsigned long*, char*, unsigned long);
	void Add(unsigned long*, unsigned long);

	/**
	* @brief ����� ������� ������������ ����� ���������
	* @param vector ������ ������������
	* @return ���������� TRUE, ���� ����� ������ ������. ����� FALSE.
	*/
	bool GetInversionIsEnabled();
	void SetInversionIsEnabled(bool arg);

	long GetSize();
};

