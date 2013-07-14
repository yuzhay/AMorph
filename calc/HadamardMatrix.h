#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include "SubstitutionCollection.h"
#include "Stats.h"

#ifdef _WIN32 || _WIN64
	// Windows Includes Here
	#include <windows.h>
	#define CLASS_DECLSPEC    __declspec(dllexport)
#endif

#ifdef __linux__
  // Linux Includes Here
  #define CLASS_DECLSPEC
#endif


/**
* @brief ����� ��� ������ ������������� �� ��������
*/
class CLASS_DECLSPEC HadamardMatrix
{
private:
	/**
	* ������ �������
	*/
	unsigned long sizeMtx;
	unsigned long counter;
	/**
	* ��������� �� ���������� �������
	*/
	char **mtx;

	/**
	* ��������� �� ������� ����������� � ������
	*/
	char *mtxVector;

	/**
	* ������ �������� ������������
	*/
	SubstitutionCollection *substitutions;

	/**
	* ��������� ���������
	*/
	double progress;

	/**
	* @brief ��������� �������������
	* @param depth ������� ������ �������������
	* @param vector ������ ������������
	* @return ���� ����������� ����, ����� TRUE. ����� FALSE.
	*/
	bool CompareAutomorph(unsigned long depth, unsigned long vector[], char* signs);

	/**
	* @brief ��������� ���������� ������� �� ����������� ������������� �������
	*/
	/*void VectorToMatrix();*/
public:
	/**
	* @brief �����������
	* @param sizeMtx ������ �������
	* @param sizeCell ������ ������ �������
	* @param mtxVector ������ �������
	*/
	HadamardMatrix(unsigned long sizeMtx, char *mtxVector);

	/**
	* @brief ����������
	*/
	~HadamardMatrix(void);

	/**
	* @brief ����� �������������
	* @param depth ������� ������ �������������
	* @param vector ������ ������������
	*/
	void SearchAutomorphRecursive(unsigned long depth, unsigned long vector[], char signs[]);

		/**
	* @brief ����� �������������
	* @param depth ������� ������ �������������
	* @param vector ������ ������������
	*/
	void SearchAutomorph(unsigned long depth, unsigned long vector[]);

	/**
	* @brief �������� ������ �������� ������������ 
	* @return ���������� ������ �������� ������������
	*/
	unsigned long *GetSubstitutions(unsigned long index);

	unsigned long GetSubstitutionsParts(unsigned long index);

	char *GetSigns(unsigned long index);
	/**
	* @brief �������� ���������� �������� �������� ������������
	* @return ���������� ���������� �������� �������� ������������
	*/
	unsigned long GetSubstitutionsCount();
	unsigned long GetIterationDepthValue(unsigned long depth);

	/**
	* @brief �������� ��������� ���������
	* @return ���������� ��������� ���������
	*/
	double GetProgress();
};

