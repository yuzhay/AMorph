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
* @brief Класс для транспозиции матрицы
*/
class CLASS_DECLSPEC BasicMatrix
{
private:
		/**
	* Размер матриц
	*/
	unsigned long sizeMtx;

	/**
	* Размер ячейки матриц
	*/
	unsigned long sizeCell;

	/**
	* Указатель на трехмерную матрицу оригинал 
	*/
	unsigned char ***mtx;

	/**
	* Указатель на оригинальную матрицу развернутую в вектор
	*/
	unsigned char *mtxVector;

	/**
	* Вектор текущей перестановки строк
	*/
	unsigned long *rowsCurr;

	/**
	* Вектор текущей перестановки строк
	*/
	unsigned long *rowsPrev;

	/**
	* Вектор предыдущей перестановки столбцов
	*/
	unsigned long *columnsCurr;

	/**
	* Вектор предыдущей перестановки столбцов
	*/
	unsigned long *columnsPrev;
public:
	BasicMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtx);

	/**
	* @brief Деструктор
	*/
	~BasicMatrix(void);
	
	/**
	* @brief Левая транспозиция
	* @param vector вектор перестановок
	*/
	void TranspositionLeft(unsigned long vector[]);

	/**
	* @brief Правая транспозиция
	* @param vector вектор перестановок
	*/
	void TranspositionRight(unsigned long vector[]);

	/**
	* @brief Симметричная транспозиция ранспозиция (левая и правая)
	* @param vector вектор перестановок
	*/
	void Transposition(unsigned long vector[]);

	unsigned char *GetMatrix();


	/**
	* @brief Наложение гаммы
	*/
	void ImposeGamma(unsigned char* vector, unsigned long length);

};

