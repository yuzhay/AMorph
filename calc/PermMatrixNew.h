#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include "PermVectorTrunk.h"
#include "Stats.h"
#include <string>


#define CLASS_DECLSPEC    __declspec(dllexport)

class CLASS_DECLSPEC PermMatrixNew
{
private:
	unsigned long matrixSize;
	unsigned long cellSize;
	unsigned char ***matrix;
	unsigned char *matrixVector;

	unsigned long origCellSize;
	unsigned long encrCellSize;

	unsigned long originalMtxSize;
	unsigned long encryptMtxSize;
		
	unsigned char ***originalMtx;
	unsigned char ***encryptMtx;

	unsigned char *originalMtxVector;
	unsigned char *encryptMtxVector;


	unsigned long *currRows;
	unsigned long *prevRows;
	unsigned long *currColumns;
	unsigned long *prevColumns;

	PermVectorTrunk *automorphTree;
	PermVectorTrunk *isomorphTree;
	Stats			*substitutionStats;
	///VectorToMatrix Методы
	int VectorToMatrix(void);
	int VectorToOriginalMtx(void);
	int VectorToEncryptMtx(void);
	///
public:
	PermMatrixNew(unsigned long matrixSize, unsigned long cellSize, unsigned char *matrix);
	PermMatrixNew(unsigned long matrixSize, unsigned long cellSize, unsigned char *matrix,
				  unsigned long originalMtxSize, unsigned long origCellSize, unsigned char *originalMtx,
				  unsigned long encryptMtxSize, unsigned long encrCellSize, unsigned char *ecnryptMtx);
	~PermMatrixNew(void);

	/////InitMatrix
	/////
	void InitAutoMorphMatrix(unsigned long matrixSize, unsigned long cellSize, unsigned char *matrix);
	void InitOriginalMatrix(unsigned long matrixSize, unsigned long cellSize, unsigned char *matrix);
	void InitEncryptMatrix(unsigned long matrixSize, unsigned long cellSize, unsigned char *matrix);
	////Setters
	////Only for Tests
	void SetMatrix(unsigned char ***Mtx);
	void SetOriginalMtx(unsigned char ***originalMtx);
	void SetEncryptMtx(unsigned char ***encryptMtx);
	
	///////////////
	////Getters
	////Only for Tests
	unsigned char *** GetMatrix(void);
	unsigned char *** GetOriginalMtx(void);
	unsigned char *** GetEncryptMtx(void);
	unsigned char *** GetTranspositionMtx(void);	
	///////////////

	void LeftTransposition(unsigned long vector[]);
	void RightTransposition(unsigned long vector[]);


	//ф-ция для получения статистики из класса
	void GetSubstitutionVector(int substitutionVector[]);
	void GetTimeStatsVector(int timeVector[]);
	int GetTimeStatsSize();


	void Transposition(unsigned long vector[]);

	void ResetTransposition(); //временная
	/*Elem *ReverseArray(Elem*orig,unsigned long b);*/

	/**
	* Поиск автоморфизмов
	*
	* @param depth Глубина 1
	* @vector Вектор преобразования
	*/
	void SearchAutomorph(unsigned long length, unsigned long vector[]);
	bool CompareAutomorph(unsigned long length, unsigned long vector[]);
	//Поиск изоморфизмов//
	void SearchIsomorph(unsigned long length, unsigned long vector[]);
	bool CompareIsomorph(unsigned long length, unsigned long vector[]);


	void TimeAndNumAuthGrapthPointsMaker();

	void Print();
	friend std::ostream& operator<<(std::ostream& os, const PermMatrixNew &obj);

};