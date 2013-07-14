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
struct Substitution
{
	unsigned long size;

	unsigned long partitions;
	char *signs;
	unsigned long* vector;
};
class CLASS_DECLSPEC SubstitutionCollection
{
private:
	unsigned long size;
	bool InversionIsEnabled;
	unsigned long *iterations;
	vector<Substitution*> trunk;

	//Begin iterator
	vector<Substitution*>::iterator it;
	
public:
	/**
	* @brief Конструктор
	* @param Size длина вектора для хранения
	*/
	SubstitutionCollection(unsigned long Size);

	/**
	* @brief Деструктор
	*/
	~SubstitutionCollection(void);

	void IncrementDepthValue(unsigned long);
	unsigned long  GetIterationDepthValue(unsigned long);
	/**
	* @brief Оператор доступа к сохраненным векторам перестановки
	* @param index Индекс
	*/
	Substitution *operator[](const unsigned long index);

	/**
	* @brief Добавление нового вектора перестановки для хранения
	* @param vector Вектор перестановки
	*/
	void Add(Substitution *sub);
	void Add(unsigned long*, char*, unsigned long);
	void Add(unsigned long*, unsigned long);

	/**
	* @brief Поиск вектора перестановки среди имеющихся
	* @param vector Вектор перестановки
	* @return Возвращает TRUE, если такой вектор найден. Иначе FALSE.
	*/
	bool GetInversionIsEnabled();
	void SetInversionIsEnabled(bool arg);

	long GetSize();
};

