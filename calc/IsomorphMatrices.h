#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include "PermVectorTrunk.h"
#include "StatisticsMatrix.h"
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
* @brief ����� ��� ������ ������������ �� ��������
*/
class CLASS_DECLSPEC IsomorphMatrices
{
private:
	/**
	* ������ ������
	*/
	unsigned long sizeMtx;

	/**
	* ������� ���������
	*/
	unsigned long counter;

	/**
	* ������ ������ ������
	*/
	unsigned long sizeCell;

	/**
	* ��������� �� ���������� ������� �������� 
	*/
	unsigned char ***mtxOriginal;

	/**
	* ��������� �� ���������� ������� ����������������
	*/
	unsigned char ***mtxModified;

	/**
	* ��������� �� ������������ ������� ����������� � ������
	*/
	unsigned char *mtxVectorOriginal;

	/**
	* ��������� �� ���������������� ������� ����������� � ������
	*/
	unsigned char *mtxVectorModified;

	
	/**
	* ������ ������� ������������ �����
	*/
	unsigned long *rowsCurr;

	/**
	* ������ ������� ������������ �����
	*/
	unsigned long *rowsPrev;

	/**
	* ������ ���������� ������������ ��������
	*/
	unsigned long *columnsCurr;

	/**
	* ������ ���������� ������������ ��������
	*/
	unsigned long *columnsPrev;

	/**
	* ������ �������� ������������
	*/
	PermVectorTrunk *substitutions;

	/**
	* ������ ���������� ��� ���������� �������
	*/
	vector<unsigned long> parts;

	/**
	* ���������� �� �������� �������
	*/
	StatisticsMatrix *mtxStatOriginal;

	/**
	* ���������� �� ���������� �������
	*/
	StatisticsMatrix *mtxStatModified;

	/**
	* ��������� ����������� ������ ��� ������ ������������ �� ����������
	*/
	bool *passed;

	/**
	* ��������� ���������
	*/
	double  progress;

	/**
	* @brief ��������� ������������
	* @param depth ������� ������ ������������
	* @param vector ������ ������������
	* @return ���� ���������� ����, ����� TRUE. ����� FALSE.
	*/
	bool CompareIsomorph(unsigned long depth, unsigned long vector[]);

	/**
	* @brief ����� ������������ ��������� ��������� ����������, ����������� �������
	* @param depth ������� ������ ������������
	* @param vector ������ ������������
	*/
	void SearchStatisticsIsomorphRecursive(unsigned int depth, unsigned long *vector,statmap *mtxOriginalColumns, statmap *mtxModifiedColumns);

	/**
	* @brief ����� ������������ ��������� ��������� ����������, ����������� �������
	* @param depth ������� ������ ������������
	* @param vector ������ ������������
	* @param mtxOriginalColumns ���������� �� �������� ���������
	* @param mtxModifiedColumns ���������� �� �������� �����
	* @param mtxOriginalRows ���������� �� ������� ���������
	* @param mtxModifiedRows ���������� �� ������ �����
	*/
	void SearchStatisticsIsomorphRecursiveFull(unsigned int depth, unsigned long *vector,statmap *mtxOriginalColumns, statmap *mtxModifiedColumns, statmap *mtxOriginalRows, statmap *mtxModifiedRows);

	/**
	* @brief ��������� ���������� ������� �� ���������� ������������� ������
	*/
	/*void VectorsToMatrices();*/

public:
	/**
	* @brief �����������
	* @param sizeMtx ������ ������
	* @param sizeCell ������ ����� ������
	* @param mtxVectorOrig  ������ ������������ �������
	* @param mtxVectorModif ������ ���������������� �������
	*/
	IsomorphMatrices(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVectorOrig, unsigned char *mtxVectorModif);

	/**
	* @brief ����������
	*/
	~IsomorphMatrices(void);
	

	/**
	* @brief ����� ������������
	* @param vector ������ ������������
	*/
	void TranspositionLeft(unsigned long vector[]);

	/**
	* @brief ������ ������������
	* @param vector ������ ������������
	*/
	void TranspositionRight(unsigned long vector[]);

	/**
	* @brief ������������ ������������ ����������� (����� � ������)
	* @param vector ������ ������������
	*/
	void Transposition(unsigned long vector[]);

	/**
	* @brief ����� ������������
	* @param depth ������� ������ ������������
	* @param vector ������ ������������
	*/
	void SearchIsomorph(unsigned long depth, unsigned long vector[]);

	/**
	* @brief ����� ������������ ��������� ��������� ����������
	* @param options ������� �����: 0000 0001 - ���� ���������� �� ��������, 0000 0010 - ���� ���������� �� �������
	*/
	void SearchStatisticsIsomorph(char options);



	/**
	* @brief �������� ������ �������� ������������ 
	* @return ���������� ������ �������� ������������
	*/
	unsigned long *GetSubstitutions(unsigned long index);

	/**
	* @brief �������� ����������� ����������� ������� �����������, �� ���� ��� ������� ������ �����������
	* @return ���������� ����������� ����������� ������� �����������, �� ���� ��� ������� ������ �����������
	*/
	unsigned long GetSubstitutionsParts(unsigned long index);

	/**
	* @brief �������� ���������� �������� �������� ������������
	* @return ���������� ���������� �������� �������� ������������
	*/
	unsigned long GetSubstitutionsCount();

	/**
	* @brief �������� ��������� ���������
	* @return ���������� ��������� ���������
	*/
	double  GetProgress();

	/**
	* @brief �������� ��������� ���������
	* @return ���������� ��������� ���������
	*/
	unsigned long IsomorphMatrices::GetParts(unsigned long index);
};
