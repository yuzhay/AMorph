#pragma once

#include <iostream>
#include <ostream>
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
/**
* @brief ����� ��� ������������ �������
*/
class CLASS_DECLSPEC BasicMatrix
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
	unsigned char ***mtx;

	/**
	* ��������� �� ������������ ������� ����������� � ������
	*/
	unsigned char *mtxVector;

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
public:
	BasicMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtx);

	/**
	* @brief ����������
	*/
	~BasicMatrix(void);
	
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

	unsigned char *GetMatrix();


	/**
	* @brief ��������� �����
	*/
	void ImposeGamma(unsigned char* vector, unsigned long length);

};

