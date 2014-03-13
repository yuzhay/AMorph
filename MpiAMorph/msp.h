

void SlaveProcess(char *srcFile,char *dstFile, int myNode);
void MasterProcess(char *srcFile,char *dstFile, int totalNodes);

unsigned long *ConstructFullVector(unsigned long *src, unsigned long srcLength,
								   unsigned long dstLength);

bool InVector(unsigned long elem, unsigned long *src, unsigned long length);

void Print(int src, int dst, int tag, char *str);
void Print(int src, int dst, int tag, int str);
void Print(int src, int dst, int tag, unsigned long *v, long len);

void PrintVector(unsigned long *v, long len);
