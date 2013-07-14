#include "AMorphC.h"
#include "MatrixFile.h"
#include <direct.h>

unsigned char dataTestFiveDim[5][5] = {
					/*{0,1,1,1,1},
					{1,0,1,1,1},
					{1,1,0,1,1},
					{1,1,1,0,1},
					{1,1,1,1,0},*/
					{0,1,0,0,1},
					{1,0,1,0,0},
					{0,1,0,1,0},
					{0,0,1,0,1},
					{1,0,0,1,0},
					};

//Текущая директория
char *curDir = NULL;

//Размер пути текущей директории
int curDirSize;


int main(int argc, char* argv[]){

	//We are cool guys, that's why we are using green color, like in movies about hackers
	system("color 0A");

	//Получение текущей директории
	if((curDir = getcwd(curDir, curDirSize))==NULL){
		perror("Ошибка getcwd");
	}


	////Usage example:
	//PermVectorTrunk *pmt = new PermVectorTrunk(10);
	//delete pmt;
	////End usage example
	//	//5x5 Matrix Test Start
	//PermMatrix<int> *matrixTest5 = new PermMatrix<int>(5,(int**)dataTestFiveDim);
	//unsigned long vector5[5] = {0,1,2,3,4};

	////matrixTest5->Print();
	//matrixTest5->SearchAutomorph(1,vector5);
	////5x5 Matrix Test End
	//delete matrixTest5;


	MatrixFile *mx = new MatrixFile();
	//mx->Path = curDir;
	mx->Path.append("samples\\sample.mxb");

    mx->Load();
    mx->MatrixToVector();
	
	//PermMatrixNew *pmn = new PermMatrixNew(mx->MatrixSize,mx->CellSize,mx->MatrixVector);
	//
	//PermMatrixNew *pmn = new PermMatrixNew(mx->MatrixSize,mx->CellSize,mx->MatrixVector,
	//										mx->MatrixSize,mx->CellSize,mx->MatrixVector,
	//										mx->MatrixSize,mx->CellSize,mx->MatrixVector);
	//delete pmn;

	//delete mx;
	//string s;
	//cin>>s;

	/////Test1
	/////Mtx Size = 5
	/////Cell Size = 1
	/////Test1.1 AutoMorph

	//unsigned char ***t1Mtx;
	//t1Mtx = (unsigned char***)malloc(sizeof(char**)*5);
	//for(int i=0;i<5;i++){
	//	t1Mtx[i] = (unsigned char**)malloc(sizeof(char**)*5);
	//	for(int j=0;j<5;j++){
	//		t1Mtx[i][j] = (unsigned char*)malloc(1);
	//	}
	//}
	//for(int i = 0; i < 5; ++i)
	//{
	//	for(int j = 0; j < 5; ++j)
	//	{
	//		t1Mtx[i][j][0] = dataTestFiveDim[i][j];
	//	}
	//}
	//for(int i = 0; i < 5; ++i)
	//{
	//	cout<<endl;
	//	for(int j = 0; j < 5; ++j)
	//	{
	//		cout<<"["<<t1Mtx[i][j][0]<<"]";
	//	}
	//}

	//PermMatrixNew *t1Pmn = new PermMatrixNew(5,1,NULL,
	//										 5,1,NULL,
	//										 5,1,NULL);
	//t1Pmn->SetMatrix(t1Mtx);
	//unsigned long *t1substitution = new unsigned long[5];
	//for(int i = 0; i < 5; ++i)
	//{
	//	t1substitution[i] = i;
	//}
	//t1Pmn->SearchAutomorph(1, t1substitution);
	//cin>>s;


	///////test1.2 IsoMorph
	//unsigned char ***t2Mtx;
	//t2Mtx = (unsigned char***)malloc(sizeof(char**)*5);
	//for(int i=0;i<5;i++){
	//	t2Mtx[i] = (unsigned char**)malloc(sizeof(char**)*5);
	//	for(int j=0;j<5;j++){
	//		t2Mtx[i][j] = (unsigned char*)malloc(1);
	//	}
	//}
	//char z = 'a';
	//for(int i = 0; i < 5; ++i)
	//{
	//	for(int j = 0; j < 5; ++j)
	//	{
	//		t2Mtx[i][j][0] = z++;
	//	}
	//}
	//for(int i = 0; i < 5; ++i)
	//{
	//	cout<<endl;
	//	for(int j = 0; j < 5; ++j)
	//	{
	//		cout<<"["<<t2Mtx[i][j][0]<<"]";
	//	}
	//}
	//t1Pmn->SetMatrix(t2Mtx);
	//unsigned long *t2substitution = new unsigned long[5];
	//t2substitution[0] = 1;
	//t2substitution[1] = 0;
	//t2substitution[2] = 3;
	//t2substitution[3] = 4;
	//t2substitution[4] = 2;
	//t1Pmn->Transposition(t2substitution);

	//unsigned char ***t2EncrMtx = t1Pmn->GetTranspositionMtx();
	//	for(int i = 0; i < 5; ++i)
	//{
	//	cout<<endl;
	//	for(int j = 0; j < 5; ++j)
	//	{
	//		cout<<"["<<t2EncrMtx[i][j][0]<<"]";
	//	}
	//}


	//for(int i = 0; i < 5; ++i)
	//{
	//	t2substitution[i] = i;
	//}
	//t1Pmn->SetOriginalMtx(t2Mtx);
	//t1Pmn->SetEncryptMtx(t2EncrMtx);
	//t1Pmn->ResetTransposition();
	//t1Pmn->SearchIsomorph(1,t2substitution);
	///////
	//
	//cin>>s;

	////Очистить переменную с текущей директорией
	//free(curDir);

	return 0;
}