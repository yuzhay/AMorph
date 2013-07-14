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
* @brief Класс для поиска автоморфизмов на матрицах
*/
class CLASS_DECLSPEC IncidenceMatrix
{
private:
	/**
	* Размер матрицы
	*/
	unsigned long sizeMtx;
	unsigned long counter;
	/**
	* Указатель на трехмерную матрицу
	*/
	char **mtx;

	/**
	* Указатель на матрицу развернутую в вектор
	*/
	char *mtxVector;

	/**
	* Массив найденых перестановок
	*/
	SubstitutionCollection *substitutions;

	/**
	* Индекатор прогресса
	*/
	double progress;

	/**
	* @brief Сравнение автоморфизмов
	* @param depth Глубина поиска автоморфизмов
	* @param vector Вектор перестановок
	* @return Если автоморфизм есть, тогда TRUE. Иначе FALSE.
	*/
	bool CompareAutomorph(unsigned long depth, unsigned long vector[]);

	/**
	* @brief Получение трехмерной матрицы из одномерного представления матрицы
	*/
	/*void VectorToMatrix();*/
public:
	/**
	* @brief Конструктор
	* @param sizeMtx Размер матрицы
	* @param sizeCell Размер ячейки матрицы
	* @param mtxVector Вектор матрицы
	*/
	IncidenceMatrix(unsigned long sizeMtx, char *mtxVector);

	/**
	* @brief Деструктор
	*/
	~IncidenceMatrix(void);

	/**
	* @brief Поиск автоморфизмов
	* @param depth Глубина поиска автоморфизмов
	* @param vector Вектор перестановок
	*/
	void SearchAutomorphRecursive(unsigned long depth, unsigned long vector[]);

		/**
	* @brief Поиск автоморфизмов
	* @param depth Глубина поиска автоморфизмов
	* @param vector Вектор перестановок
	*/
	void SearchAutomorph(unsigned long depth, unsigned long vector[]);

	/**
	* @brief Получить список векторов перестановок 
	* @return Возвращает список векторов перестановок
	*/
	unsigned long *GetSubstitutions(unsigned long index);

	unsigned long GetSubstitutionsParts(unsigned long index);
		
	/**
	* @brief Получить количество найденых векторов перестановок
	* @return Возвращает количество найденых векторов перестановок
	*/
	unsigned long GetSubstitutionsCount();
	unsigned long GetIterationDepthValue(unsigned long depth);
	/**
	* @brief Получить индекатор прогресса
	* @return Возвращает индекатор прогресса
	*/
	double GetProgress();
};

