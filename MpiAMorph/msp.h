#ifdef _WIN32 || _WIN64
	// Windows Includes Here
	#include <windows.h>
#endif

#define DIVISIBILITY_CRITERIA 2




void SlaveProcess(char *srcFile,char *dstFile, int myNode);
void MasterProcess(char *srcFile,char *dstFile, int totalNodes);

unsigned long *ConstructFullVector(unsigned long *src, unsigned long srcLength,
								   unsigned long dstLength);

unsigned long *ConstructShortVector(unsigned long *src, unsigned long srcLength, 
									unsigned long *dstLength);

bool InVector(unsigned long elem, unsigned long *src, unsigned long length);

void Print(int src, int dst, int tag, char *str);
void Print(int src, int dst, int tag, int str);
void Print(int src, int dst, int tag, unsigned long *v, long len);

void PrintVector(int, unsigned long *v, long len);


unsigned long *IsDivisibilityCriteria(int node, unsigned long *len);

bool AllNodesAreFree();
bool AllNodesAre(ProcessState);
void PrintNodeStatus();

void PrepareSolutionDir();


//Номер текущего процесса
extern int myNode;

//Количество процессов
extern int totalNodes; 