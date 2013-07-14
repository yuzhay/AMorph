#pragma once
#include <map>
#include <string>

#define statmap map<unsigned long,unsigned long>


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
* @brief Класс для подсчета статистики по матрице
*/
class CLASS_DECLSPEC StatisticsMatrix
{
private:
	statmap *columns;
	statmap *rows;
	unsigned long *tmpArray;
	unsigned long sizeMtx;
	unsigned long sizeCell;
	unsigned char *mtx;

public:
	/* 
	* @brief Конструктор
	* @param sizeMtx Размер матриц
	* @param sizeCell Размер ячеек матриц
	* @param mtx  Матрицы
	*/
	StatisticsMatrix(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtx);

	/* 
	* @brief Запускает сбор статистики
	* @param options  Битовая маска: 3 - сбор статистики по колонкам и строкам, 1 - сбор статистики по колонкам, 2 - сбор статистики по строкам
	*/
	void Run(char options = 3);

	/* 
	* @brief Получить массив статистики для колоноки
	* @return Возвращает массив статистики для колонок
	*/
	statmap GetColumn(unsigned long);

	/* 
	* @brief Получить массив статистики для строки
	* @return Возвращает массив статистики для строк
	*/
	statmap GetRow(unsigned long);

	/* 
	* @brief Получить массив статистики для строк
	* @return Возвращает массив статистики для строк
	*/
	statmap *GetRows();

		/* 
	* @brief Получить массив статистики для колонок
	* @return Возвращает массив статистики для колонок
	*/
	statmap *GetColumns();

	/* 
	* @brief Получить массив статистики для колонок
	* @return Возвращает массив статистики для колонок
	*/
	unsigned long *GetColumnArray(unsigned long, unsigned long*);

	/* 
	* @brief Получить массив статистики для строк
	* @return Возвращает массив статистики для строк
	*/
	unsigned long *GetRowArray(unsigned long, unsigned long*);

	/* 
	* @brief Деструктор
	*/
	~StatisticsMatrix(void);
};

