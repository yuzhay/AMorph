#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include "PermVectorTrunk.h"
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
class CLASS_DECLSPEC DecryptMatrix
{
private:
	/**
	* ������ ������
	*/
	unsigned long sizeMtx;

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
	DecryptMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVectorOrig, unsigned char *mtxVectorModif);

	/**
	* @brief ����������
	*/
	~DecryptMatrix(void);
	
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
	* @brief �������� ������ �������� ������������ 
	* @return ���������� ������ �������� ������������
	*/
	unsigned long *GetSubstitutions(unsigned long index);

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
};
