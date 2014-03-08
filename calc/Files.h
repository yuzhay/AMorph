#ifdef _WIN32 || _WIN64
	// Windows Includes Here
	#define NOMINMAX
	#include <windows.h>
	#define CLASS_DECLSPEC    __declspec(dllexport)
#endif

#ifdef __linux__
  // Linux Includes Here
  #define CLASS_DECLSPEC
#endif

int CLASS_DECLSPEC LoadVectorFromBmp(char *fielname, unsigned char **vector, unsigned long *sizeMtx, unsigned long *sizeCell);
bool CLASS_DECLSPEC FileExists(char *filename);
