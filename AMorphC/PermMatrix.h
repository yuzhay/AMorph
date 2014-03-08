#include <iostream>
#include <ostream>
#include "../calc/PermVectorTrunk.h"
#include "../calc/Stats.h"


#define CLASS_DECLSPEC    __declspec(dllimport)


/*
Elem - type of the custom element
Size - matrix line size
*/
//#define DEBUG_MODE
//


template <typename Elem>
class CLASS_DECLSPEC PermMatrix
{
private:
	Elem **data;
	Elem **matrix;
	
	unsigned long size;

	unsigned long *currRows;
	unsigned long *prevRows;
	unsigned long *currColumns;
	unsigned long *prevColumns;

	PermVectorTrunk *automorphTree;
	Stats			*timeStats;
public:
	PermMatrix(unsigned long);
	PermMatrix(unsigned long, Elem &);
	PermMatrix(unsigned long, Elem **);
	PermMatrix(unsigned long, char *);
	~PermMatrix(void);

	void Fill(Elem &elem);
	void Null(void);
	void LeftTransposition(unsigned long vector[]);
	void RightTransposition(unsigned long vector[]);
	void Trans(void);
	bool ExportToFile(char *path);
	bool ImportFromFile(char *path);
	void GetMatrix(int matrix[]);
	void SetMatrix(unsigned long, Elem **);
	//ф-ция для получения статистики из класса
	void GetSubstitutionVector(int substitutionVector[]);
	void GetTimeStatsVector(int timeVector[]);
	int GetTimeStatsSize();

	void Transposition(unsigned long vector[]);
	void SearchAutomorph(unsigned long length, unsigned long vector[]);
	bool CompareAutomorph(unsigned long length, unsigned long vector[]);
	void ResetTransposition(); //временная
	/*Elem *ReverseArray(Elem*orig,unsigned long b);*/

	void TimeAndNumAuthGrapthPointsMaker();

	void Print();
	friend std::ostream& operator<<(std::ostream& os, const PermMatrix &obj);

	//private: bool firstTransposition;

	//Import/Export
	//ONLY Eleme = INT, LONG
	bool LoadFromFile(char *path, char separator = ',');
	bool SaveToFile(char *path, char separator = ',');
};