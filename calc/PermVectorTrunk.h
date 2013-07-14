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
* @brief Класс для хранения векторов перестановок
*/
class CLASS_DECLSPEC PermVectorTrunk
{
private:
	unsigned long size;
	vector<unsigned long*> trunk;

	vector<unsigned long> parts;
	//Begin iterator
	vector<unsigned long*>::iterator it;
	
	/**
	* @brief Сравнение векторов
	* @param v1 Первый вектор для сравнения
	* @param v2 Второй вектор для сравнения
	* @return Возвращает 0 если вектора идентичны.
	*/
	int VectorCmp(const unsigned long *v1, const unsigned long* v2);
public:
	/**
	* @brief Конструктор
	* @param Size длина вектора для хранения
	*/
	PermVectorTrunk(unsigned long Size);

	/**
	* @brief Деструктор
	*/
	~PermVectorTrunk(void);

	/**
	* @brief Оператор доступа к сохраненным векторам перестановки
	* @param index Индекс
	*/
	unsigned long* operator[](const unsigned long index);

	/**
	* @brief Добавление нового вектора перестановки для хранения
	* @param vector Вектор перестановки
	* @param counter Характеристика
	*/
	void Add(unsigned long *vector, unsigned long counter);

	/**
	* @brief Поиск вектора перестановки среди имеющихся
	* @param vector Вектор перестановки
	* @return Возвращает TRUE, если такой вектор найден. Иначе FALSE.
	*/
	bool Find(unsigned long *vector);

	/**
	* @brief Получить количество имеющихся векторов.
	* @return Возвращает количество имеющихся векторов.
	*/
	long GetSize();

	/**
	* @brief Получить статистику вектора.
	* @return Возвращает статистику для вектора.
	*/
	unsigned long GetParts(unsigned long index);
};

