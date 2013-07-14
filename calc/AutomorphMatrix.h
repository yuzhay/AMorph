#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
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
* @brief ����� ��� ������ ������������� �� ��������
*/
class CLASS_DECLSPEC AutomorphMatrix
{
private:
	/**
	* ������ �������
	*/
	unsigned long sizeMtx;

	/**
	* ������ ������ �������
	*/
	unsigned long sizeCell;

	/**
	* ��������� �� ���������� �������
	*/
	unsigned char ***mtx;

	/**
	* ��������� �� ������� ����������� � ������
	*/
	unsigned char *mtxVector;

	/**
	* ������ �������� ������������
	*/
	PermVectorTrunk *substitutions;

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
	bool CompareAutomorph(unsigned long depth, unsigned long vector[]);

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
	AutomorphMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVector);

	/**
	* @brief ����������
	*/
	~AutomorphMatrix(void);

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

	/**
	* @brief �������� ���������� �������� �������� ������������
	* @return ���������� ���������� �������� �������� ������������
	*/
	unsigned long GetSubstitutionsCount();

	/**
	* @brief �������� ��������� ���������
	* @return ���������� ��������� ���������
	*/
	double GetProgress();
};

