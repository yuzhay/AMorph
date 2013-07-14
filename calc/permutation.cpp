// calc.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PermMatrix.h"
#include "permutation.h"
#include <windows.h>
#include "PermVectorTrunk.h"
#include <iostream>
#include <fstream>

using namespace std;

void DoEverything(void)
{
	#ifdef DEBUG_MODE
		MessageBoxA(NULL,"DoEverything","Caption",NULL);
	#endif
	//int data[10][10] = {
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9},
	//				{0,1,2,3,4,5,6,7,8,9}
	//				};

	//PermMatrix<int> *matrix = new PermMatrix<int>(10,(int**)data);

	//ofstream outfile("permutation.txt");
	////outfile << (matrix->Print());

	//outfile.close();
	//delete matrix;

	//unsigned long v1[]={1,2,3,4,5,6};
	//unsigned long v2[]={3,2,1,6,4,5};
	//unsigned long v3[]={2,6,3,1,5,4};
	//unsigned long v4[]={3,6,2,1,4,5};
	//unsigned long v5[]={2,5,3,1,6,4};
	//unsigned long v6[]={4,6,3,2,5,1};

	//PermVectorTrunk *pvt = new PermVectorTrunk(6);
	//pvt->Add(v1);
	//pvt->Add(v2);
	//pvt->Add(v3);
	//pvt->Add(v4);
	//pvt->Add(v5);
	//pvt->Add(v6);
	//
	//if(pvt->Find(v2))
	//	MessageBoxA(0,"vector 2 found {3,2,1,6,4,5}","",0);
	//MessageBoxA(0,"testmsg","",0);
	//if(pvt->Find(v3))
	//	MessageBoxA(0,"vector 3 found {2,6,3,1,5,4}","",0);

	//if(pvt->Find(v4))
	//	MessageBoxA(0,"vector 4 found {3,6,2,1,4,5}","",0);

	//v1[0] = 3;
	//	if(pvt->Find(v1))
	//	MessageBoxA(0,"vector 1 found {3,2,3,4,5,6}","",0);

	//#ifdef DEBUG_MODE
	//	MessageBoxA(0,"DoEverything DONE","",0);
	//#endif
	//delete pvt;
}

void CalculcateInt(int **matrix, int **permutation, unsigned int size, int **result)
{
	#ifdef DEBUG_MODE
		MessageBoxA(NULL,"CalculateInt","Caption",NULL);
	#endif
}

//========================AutomorphMatrix===========================
int AutomorphMatrix_Constructor(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVector)
{
	AutomorphMatrix *am = new AutomorphMatrix(sizeMtx,sizeCell,mtxVector);
	#ifdef DEBUG_MODE
		MessageBoxA(0,"AutomorphMatrix_Contructor","",0);
	#endif
	return (int)am;
}

void AutomorphMatrix_Destructor(AutomorphMatrix *handler)
{
	delete handler;
	#ifdef DEBUG_MODE
		MessageBoxA(0,"AutomorphMatrix_Destructor","",0);
	#endif
}

void AutomorphMatrix_SearchAutomorph(AutomorphMatrix *handler, unsigned long depth, unsigned long vector[])
{
	#ifdef DEBUG_MODE
	MessageBoxA(0,"AutomorphMatrix_SearchAutomorph","",0);
	#endif
	handler->SearchAutomorph(depth, vector);
}

unsigned long *AutomorphMatrix_GetSubstitutions(AutomorphMatrix *handler, unsigned long index)
{
	return handler->GetSubstitutions(index);
}

unsigned long AutomorphMatrix_GetSubstitutionsCount(AutomorphMatrix *handler)
{
	return handler->GetSubstitutionsCount();
}

double AutomorphMatrix_GetProgress(AutomorphMatrix *handler)
{
	return handler->GetProgress();
}

//========================IsomorphMatrices===========================
int IsomorphMatrices_Constructor(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVectorOrig, unsigned char *mtxVectorModified)
{
	IsomorphMatrices *im = new IsomorphMatrices(sizeMtx,sizeCell,mtxVectorOrig, mtxVectorModified);
	#ifdef DEBUG_MODE
		MessageBoxA(0,"IsomorphMatrices_Contructor","",0);
	#endif
	return (int)im;
}

void IsomorphMatrices_Destructor(IsomorphMatrices *handler)
{
	delete handler;
	#ifdef DEBUG_MODE
		MessageBoxA(0,"IsomorphMatrices_Destructor","",0);
	#endif
}

void IsomorphMatrices_TranspositionLeft(IsomorphMatrices *handler,unsigned long vector[])
{
	handler->TranspositionLeft(vector);
}

void IsomorphMatrices_TranspositionRight(IsomorphMatrices *handler,unsigned long vector[])
{
	handler->TranspositionRight(vector);
}

void IsomorphMatrices_Transposition(IsomorphMatrices *handler,unsigned long vector[])
{
	#ifdef DEBUG_MODE
	MessageBoxA(0,"IsomorphMatrices_Transposition","",0);
	#endif
	handler->Transposition(vector);
}

void IsomorphMatrices_SearchIsomorph(IsomorphMatrices *handler, unsigned long depth, unsigned long vector[])
{
	#ifdef DEBUG_MODE
		MessageBoxA(0,"IsomorphMatrices_SearchIsomorph","",0);
	#endif
	handler->SearchIsomorph(depth, vector);
}

unsigned long *IsomorphMatrices_GetSubstitutions(IsomorphMatrices *handler, unsigned long index)
{
	return handler->GetSubstitutions(index);
}

unsigned long IsomorphMatrices_GetSubstitutionsCount(IsomorphMatrices *handler)
{
	return handler->GetSubstitutionsCount();
}

double IsomorphMatrices_GetProgress(IsomorphMatrices *handler)
{
	return handler->GetProgress();
}

void IsomorphMatrices_SearchStatisticsIsomorph(IsomorphMatrices *handler, char options)
{
	return handler->SearchStatisticsIsomorph(options);
}


unsigned long IsomorphMatrices_GetParts(IsomorphMatrices *handler, unsigned long index)
{
	return handler->GetParts(index);
}


//Ёкспортируемые функции класс BaseMatrix
int BasicMatrix_Constructor(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVector)
{
	BasicMatrix *bm = new BasicMatrix(sizeMtx,sizeCell,mtxVector);
	#ifdef DEBUG_MODE
		MessageBoxA(0,"BaseMatrix_Contructor","",0);
	#endif
	return (int)bm;
}

void BasicMatrix_Destructor(BasicMatrix *handler)
{
	delete handler;
}

void BasicMatrix_TranspositionLeft(BasicMatrix *handler,unsigned long vector[])
{
	handler->TranspositionLeft(vector);
}

void BasicMatrix_TranspositionRight(BasicMatrix *handler,unsigned long vector[])
{
	handler->TranspositionRight(vector);
}

void BasicMatrix_Transposition(BasicMatrix *handler,unsigned long vector[])
{
	handler->Transposition(vector);
}

unsigned char *BasicMatrix_GetMatrix(BasicMatrix *handler)
{
	return handler->GetMatrix();
}


void BasicMatrix_ImposeGamma(BasicMatrix *handler,unsigned char *vector, unsigned long length)
{
	handler->ImposeGamma(vector,length);
}

//========================DecryptMatrix===========================
int DecryptMatrix_Constructor(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtxVectorOrig, unsigned char *mtxVectorModified)
{
	DecryptMatrix *im = new DecryptMatrix(sizeMtx,sizeCell,mtxVectorOrig, mtxVectorModified);
	#ifdef DEBUG_MODE
		MessageBoxA(0,"DecryptMatrix_Contructor","",0);
	#endif
	return (int)im;
}

void DecryptMatrix_Destructor(DecryptMatrix *handler)
{
	delete handler;
	#ifdef DEBUG_MODE
		MessageBoxA(0,"DecryptMatrix_Destructor","",0);
	#endif
}

void DecryptMatrix_TranspositionLeft(DecryptMatrix *handler,unsigned long vector[])
{
	handler->TranspositionLeft(vector);
}

void DecryptMatrix_TranspositionRight(DecryptMatrix *handler,unsigned long vector[])
{
	handler->TranspositionRight(vector);
}

void DecryptMatrix_Transposition(DecryptMatrix *handler,unsigned long vector[])
{
	#ifdef DEBUG_MODE
	MessageBoxA(0,"DecryptMatrix_Transposition","",0);
	#endif
	handler->Transposition(vector);
}

void DecryptMatrix_SearchIsomorph(DecryptMatrix *handler, unsigned long depth, unsigned long vector[])
{
	#ifdef DEBUG_MODE
		MessageBoxA(0,"DecryptMatrix_SearchIsomorph","",0);
	#endif
	handler->SearchIsomorph(depth, vector);
}

unsigned long *DecryptMatrix_GetSubstitutions(DecryptMatrix *handler, unsigned long index)
{
	return handler->GetSubstitutions(index);
}

unsigned long DecryptMatrix_GetSubstitutionsCount(DecryptMatrix *handler)
{
	return handler->GetSubstitutionsCount();
}

double DecryptMatrix_GetProgress(DecryptMatrix *handler)
{
	return handler->GetProgress();
}



//========================StatisticsMatrix===========================
int StatisticsMatrix_Constructor(unsigned long sizeMtx, unsigned long sizeCell, unsigned char *mtx)
{
	StatisticsMatrix *sm = new StatisticsMatrix(sizeMtx,sizeCell,mtx);
	#ifdef DEBUG_MODE
		MessageBoxA(0,"StatisticsMatrix_Contructor","",0);
	#endif
	return (int)sm;
}

void StatisticsMatrix_Run(StatisticsMatrix* handler, bool symmetric = false)
{
	return handler->Run(symmetric);
}

unsigned long *StatisticsMatrix_GetColumn(StatisticsMatrix* handler, unsigned long index, unsigned long* arraySize)
{
	return handler->GetColumnArray(index, arraySize);
}

unsigned long *StatisticsMatrix_GetRow(StatisticsMatrix* handler, unsigned long index, unsigned long* arraySize)
{
	return handler->GetRowArray(index, arraySize);
}

void StatisticsMatrix_Destructor(StatisticsMatrix* handler)
{
	delete handler;
	#ifdef DEBUG_MODE
		MessageBoxA(0,"StatisticsMatrix_Destructor","",0);
	#endif
}
//========================HadamardMatrix===========================
int HadamardMatrix_Constructor(unsigned long sizeMtx, char *mtxVector)
{
	HadamardMatrix *am = new HadamardMatrix(sizeMtx,mtxVector);
	#ifdef DEBUG_MODE
		MessageBoxA(0,"HadamardMatrix_Contructor","",0);
	#endif
	return (int)am;
}

void HadamardMatrix_Destructor(HadamardMatrix *handler)
{
	delete handler;
	#ifdef DEBUG_MODE
		MessageBoxA(0,"HadamardMatrix_Destructor","",0);
	#endif
}

void HadamardMatrix_SearchAutomorph(HadamardMatrix *handler, unsigned long depth, unsigned long vector[])
{
	#ifdef DEBUG_MODE
	MessageBoxA(0,"HadamardMatrix_SearchAutomorph","",0);
	#endif
	handler->SearchAutomorph(depth, vector);
}

unsigned long *HadamardMatrix_GetSubstitutions(HadamardMatrix *handler, unsigned long index)
{
	return handler->GetSubstitutions(index);
}

unsigned long HadamardMatrix_GetSubstitutionsParts(HadamardMatrix *handler, unsigned long index)
{
	return handler->GetSubstitutionsParts(index);
}

char *HadamardMatrix_GetSigns(HadamardMatrix *handler, unsigned long index)
{
	return handler->GetSigns(index);
}

unsigned long HadamardMatrix_GetSubstitutionsCount(HadamardMatrix *handler)
{
	return handler->GetSubstitutionsCount();
}

double HadamardMatrix_GetProgress(HadamardMatrix *handler)
{
	return handler->GetProgress();
}
unsigned long HadamardMatrix_GetIterationDepthValue(HadamardMatrix *handler, unsigned long depth)
{
	return handler->GetIterationDepthValue(depth);
}
//========================IncidenceMatrix===========================
int IncidenceMatrix_Constructor(unsigned long sizeMtx, char *mtxVector)
{
	IncidenceMatrix *am = new IncidenceMatrix(sizeMtx,mtxVector);
	#ifdef DEBUG_MODE
		MessageBoxA(0,"IncidenceMatrix_Contructor","",0);
	#endif
	return (int)am;
}

void IncidenceMatrix_Destructor(IncidenceMatrix *handler)
{
	delete handler;
	#ifdef DEBUG_MODE
		MessageBoxA(0,"IncidenceMatrix_Destructor","",0);
	#endif
}

void IncidenceMatrix_SearchAutomorph(IncidenceMatrix *handler, unsigned long depth, unsigned long vector[])
{
	#ifdef DEBUG_MODE
	MessageBoxA(0,"IncidenceMatrix_SearchAutomorph","",0);
	#endif
	handler->SearchAutomorph(depth, vector);
}

unsigned long *IncidenceMatrix_GetSubstitutions(IncidenceMatrix *handler, unsigned long index)
{
	return handler->GetSubstitutions(index);
}

unsigned long IncidenceMatrix_GetSubstitutionsParts(IncidenceMatrix *handler, unsigned long index)
{
	return handler->GetSubstitutionsParts(index);
}

unsigned long IncidenceMatrix_GetSubstitutionsCount(IncidenceMatrix *handler)
{
	return handler->GetSubstitutionsCount();
}

double IncidenceMatrix_GetProgress(IncidenceMatrix *handler)
{
	return handler->GetProgress();
}
unsigned long IncidenceMatrix_GetIterationDepthValue(IncidenceMatrix *handler, unsigned long depth)
{
	return handler->GetIterationDepthValue(depth);
}