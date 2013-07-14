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
* @brief Класс для поиска Изоморфизмов на матрицах
*/
class CLASS_DECLSPEC IsomorphMatrices
{
private:
	/**
	* Размер матриц
	*/
	unsigned long sizeMtx;

	/**
	* Счетчик сравнений
	*/
	unsigned long counter;

	/**
	* Размер ячейки матриц
	*/
	unsigned long sizeCell;

	/**
	* Указатель на трехмерную матрицу оригинал 
	*/
	unsigned char ***mtxOriginal;

	/**
	* Указатель на трехмерную матрицу модифицированную
	*/
	unsigned char ***mtxModified;

	/**
	* Указатель на оригинальную матрицу развернутую в вектор
	*/
	unsigned char *mtxVectorOriginal;

	/**
	* Указатель на модифицированную матрицу развернутую в вектор
	*/
	unsigned char *mtxVectorModified;

	
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

	/**
	* Массив найденых перестановок
	*/
	PermVectorTrunk *substitutions;

	/**
	* Массив информации для построения графика
	*/
	vector<unsigned long> parts;

	/**
	* Статистика по исходной матрице
	*/
	StatisticsMatrix *mtxStatOriginal;

	/**
	* Статистика по измененной матрице
	*/
	StatisticsMatrix *mtxStatModified;

	/**
	* Индекатор проверенных вершин при поиска изоморфизмов по статистике
	*/
	bool *passed;

	/**
	* Индекатор прогресса
	*/
	double  progress;

	/**
	* @brief Сравнение изоморфизмов
	* @param depth Глубина поиска изоморфизмов
	* @param vector Вектор перестановок
	* @return Если изоморфизм есть, тогда TRUE. Иначе FALSE.
	*/
	bool CompareIsomorph(unsigned long depth, unsigned long vector[]);

	/**
	* @brief Поиск изоморфизмов используя собранную статистику, рекурсивная функция
	* @param depth Глубина поиска изоморфизмов
	* @param vector Вектор перестановок
	*/
	void SearchStatisticsIsomorphRecursive(unsigned int depth, unsigned long *vector,statmap *mtxOriginalColumns, statmap *mtxModifiedColumns);

	/**
	* @brief Поиск изоморфизмов используя собранную статистику, рекурсивная функция
	* @param depth Глубина поиска изоморфизмов
	* @param vector Вектор перестановок
	* @param mtxOriginalColumns Статистика по колонкам оригинала
	* @param mtxModifiedColumns Статистика по колонкам шифра
	* @param mtxOriginalRows Статистика по строкам оригинала
	* @param mtxModifiedRows Статистика по строка шифра
	*/
	void SearchStatisticsIsomorphRecursiveFull(unsigned int depth, unsigned long *vector,statmap *mtxOriginalColumns, statmap *mtxModifiedColumns, statmap *mtxOriginalRows, statmap *mtxModifiedRows);

	/**
	* @brief Получение трехмерных матрицы из одномерных представлений матриц
	*/
	/*void VectorsToMatrices();*/

public:
	/**
	* @brief Конструктор
	* @param sizeMtx Размер матриц
	* @param sizeCell Размер ячеек матриц
	* @param mtxVectorOrig  Вектор оригинальной матрицы
	* @param mtxVectorModif Вектор модифицированной матрицы
	*/
	IsomorphMatrices(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVectorOrig, unsigned char *mtxVectorModif);

	/**
	* @brief Деструктор
	*/
	~IsomorphMatrices(void);
	

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

	/**
	* @brief Поиск изоморфизмов
	* @param depth Глубина поиска изоморфизмов
	* @param vector Вектор перестановок
	*/
	void SearchIsomorph(unsigned long depth, unsigned long vector[]);

	/**
	* @brief Поиск изоморфизмов используя собранную статистику
	* @param options Битовая маска: 0000 0001 - сбор статистики по колонкам, 0000 0010 - сбор статистики по строкам
	*/
	void SearchStatisticsIsomorph(char options);



	/**
	* @brief Получить список векторов перестановок 
	* @return Возвращает список векторов перестановок
	*/
	unsigned long *GetSubstitutions(unsigned long index);

	/**
	* @brief Получить колическтво подстановок которые перебрались, до того как нашлась нужная подстановка
	* @return Возвращает колическтво подстановок которые перебрались, до того как нашлась нужная подстановка
	*/
	unsigned long GetSubstitutionsParts(unsigned long index);

	/**
	* @brief Получить количество найденых векторов перестановок
	* @return Возвращает количество найденых векторов перестановок
	*/
	unsigned long GetSubstitutionsCount();

	/**
	* @brief Получить индекатор прогресса
	* @return Возвращает индекатор прогресса
	*/
	double  GetProgress();

	/**
	* @brief Получить индекатор прогресса
	* @return Возвращает индекатор прогресса
	*/
	unsigned long IsomorphMatrices::GetParts(unsigned long index);
};
