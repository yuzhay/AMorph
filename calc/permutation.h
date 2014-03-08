#include <iostream>
#include <ostream>
#include "AutomorphMatrix.h"
#include "IsomorphMatrices.h"
#include "DecryptMatrix.h"
#include "BasicMatrix.h"
#include "StatisticsMatrix.h"
#include "HadamardMatrix.h"
#include "IncidenceMatrix.h"
#include "Files.h"

//#define DEBUG_MODE 0

extern "C" __declspec(dllexport) void DoEverything(void);
extern "C" __declspec(dllexport) void CalculcateInt(int **matrix, int **permutation, unsigned int size, int **result);

//Ёкспортируемые функции класс AutomorphMatrix
extern "C" __declspec(dllexport) int AutomorphMatrix_Constructor(unsigned long matrixSize, unsigned long cellSize, unsigned char *matrix);
extern "C" __declspec(dllexport) void AutomorphMatrix_Destructor(AutomorphMatrix *);
extern "C" __declspec(dllexport) void AutomorphMatrix_SearchAutomorph(AutomorphMatrix *handler, unsigned long length, unsigned long vector[]);
extern "C" __declspec(dllexport) unsigned long *AutomorphMatrix_GetSubstitutions(AutomorphMatrix *handler, unsigned long index);
extern "C" __declspec(dllexport) unsigned long AutomorphMatrix_GetSubstitutionsCount(AutomorphMatrix *handler);
extern "C" __declspec(dllexport) double AutomorphMatrix_GetProgress(AutomorphMatrix *handler);

//Ёкспортируемые функции класс IsomorphMatrices
extern "C" __declspec(dllexport) int IsomorphMatrices_Constructor(unsigned long matrixSize, unsigned long cellSize, unsigned char *mtxVectorOrig, unsigned char *mtxVectorModified);
extern "C" __declspec(dllexport) void IsomorphMatrices_Destructor(IsomorphMatrices *);
extern "C" __declspec(dllexport) void IsomorphMatrices_LeftTransposition(IsomorphMatrices *,unsigned long vector[]);
extern "C" __declspec(dllexport) void IsomorphMatrices_RightTransposition(IsomorphMatrices *,unsigned long vector[]);
extern "C" __declspec(dllexport) void IsomorphMatrices_Transposition(IsomorphMatrices *,unsigned long vector[]);
extern "C" __declspec(dllexport) void IsomorphMatrices_SearchIsomorph(IsomorphMatrices *handler, unsigned long length, unsigned long vector[]);
extern "C" __declspec(dllexport) unsigned long *IsomorphMatrices_GetSubstitutions(IsomorphMatrices *handler, unsigned long index);
extern "C" __declspec(dllexport) unsigned long IsomorphMatrices_GetSubstitutionsCount(IsomorphMatrices *handler);
extern "C" __declspec(dllexport) double IsomorphMatrices_GetProgress(IsomorphMatrices *handler);
extern "C" __declspec(dllexport) void IsomorphMatrices_SearchStatisticsIsomorph(IsomorphMatrices *, char);
extern "C" __declspec(dllexport) unsigned long IsomorphMatrices_GetParts(IsomorphMatrices *, unsigned long);

//Ёкспортируемые функции класс BasicMatrix
extern "C" __declspec(dllexport) int BasicMatrix_Constructor(unsigned long matrixSize, unsigned long cellSize, unsigned char *matrix);
extern "C" __declspec(dllexport) void BasicMatrix_Destructor(BasicMatrix *);
extern "C" __declspec(dllexport) void BasicMatrix_TranspositionLeft(BasicMatrix *,unsigned long vector[]);
extern "C" __declspec(dllexport) void BasicMatrix_TranspositionRight(BasicMatrix *,unsigned long vector[]);
extern "C" __declspec(dllexport) void BasicMatrix_Transposition(BasicMatrix *,unsigned long vector[]);
extern "C" __declspec(dllexport) unsigned char *BasicMatrix_GetMatrix(BasicMatrix *handler);
extern "C" __declspec(dllexport) void BasicMatrix_ImposeGamma(BasicMatrix *,unsigned char *vector, unsigned long);


//Ёкспортируемые функции класс DecryptMatrix
extern "C" __declspec(dllexport) int DecryptMatrix_Constructor(unsigned long matrixSize, unsigned long cellSize, unsigned char *mtxVectorOrig, unsigned char *mtxVectorModified);
extern "C" __declspec(dllexport) void DecryptMatrix_Destructor(DecryptMatrix *);
extern "C" __declspec(dllexport) void DecryptMatrix_LeftTransposition(DecryptMatrix *,unsigned long vector[]);
extern "C" __declspec(dllexport) void DecryptMatrix_RightTransposition(DecryptMatrix *,unsigned long vector[]);
extern "C" __declspec(dllexport) void DecryptMatrix_Transposition(DecryptMatrix *,unsigned long vector[]);
extern "C" __declspec(dllexport) void DecryptMatrix_SearchIsomorph(DecryptMatrix *handler, unsigned long length, unsigned long vector[]);
extern "C" __declspec(dllexport) unsigned long *DecryptMatrix_GetSubstitutions(DecryptMatrix *handler, unsigned long index);
extern "C" __declspec(dllexport) unsigned long DecryptMatrix_GetSubstitutionsCount(DecryptMatrix *handler);
extern "C" __declspec(dllexport) double DecryptMatrix_GetProgress(DecryptMatrix *handler);


//Ёкспортируемые функции класс StatisticsMatrix
extern "C" __declspec(dllexport) int StatisticsMatrix_Constructor(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtx);
extern "C" __declspec(dllexport) void StatisticsMatrix_Run(StatisticsMatrix*, bool);
extern "C" __declspec(dllexport) unsigned long *StatisticsMatrix_GetColumn(StatisticsMatrix*,unsigned long, unsigned long*);
extern "C" __declspec(dllexport) unsigned long *StatisticsMatrix_GetRow(StatisticsMatrix*,unsigned long, unsigned long*);
extern "C" __declspec(dllexport) void StatisticsMatrix_Destructor(StatisticsMatrix*);


//Ёкспортируемые функции класс HadamardMatrix
extern "C" __declspec(dllexport) int HadamardMatrix_Constructor(unsigned long matrixSize, char *matrix);
extern "C" __declspec(dllexport) void HadamardMatrix_Destructor(HadamardMatrix*);
extern "C" __declspec(dllexport) void HadamardMatrix_SearchAutomorph(HadamardMatrix *handler, unsigned long length, unsigned long vector[]);
extern "C" __declspec(dllexport) unsigned long *HadamardMatrix_GetSubstitutions(HadamardMatrix *handler, unsigned long index);
extern "C" __declspec(dllexport) unsigned long HadamardMatrix_GetSubstitutionsParts(HadamardMatrix *handler, unsigned long index);
extern "C" __declspec(dllexport) char* HadamardMatrix_GetSigns(HadamardMatrix *handler, unsigned long index);
extern "C" __declspec(dllexport) unsigned long HadamardMatrix_GetSubstitutionsCount(HadamardMatrix *handler);
extern "C" __declspec(dllexport) double HadamardMatrix_GetProgress(HadamardMatrix *handler);
extern "C" __declspec(dllexport) unsigned long HadamardMatrix_GetIterationDepthValue(HadamardMatrix *handler, unsigned long depth);

//Ёкспортируемые функции класс IncidenceMatrix
extern "C" __declspec(dllexport) int IncidenceMatrix_Constructor(unsigned long matrixSize, char *matrix);
extern "C" __declspec(dllexport) void IncidenceMatrix_Destructor(IncidenceMatrix*);
extern "C" __declspec(dllexport) void IncidenceMatrix_SearchAutomorph(IncidenceMatrix *handler, unsigned long length, unsigned long vector[]);
extern "C" __declspec(dllexport) unsigned long *IncidenceMatrix_GetSubstitutions(IncidenceMatrix *handler, unsigned long index);
extern "C" __declspec(dllexport) unsigned long IncidenceMatrix_GetSubstitutionsParts(IncidenceMatrix *handler, unsigned long index);
extern "C" __declspec(dllexport) unsigned long IncidenceMatrix_GetSubstitutionsCount(IncidenceMatrix *handler);
extern "C" __declspec(dllexport) double IncidenceMatrix_GetProgress(IncidenceMatrix *handler);
extern "C" __declspec(dllexport) unsigned long IncidenceMatrix_GetIterationDepthValue(IncidenceMatrix *handler, unsigned long depth);
