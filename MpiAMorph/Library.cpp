#include "Library.h"
#include <iostream>

//ѕостроение полного вектора по сокращенному
unsigned long *BuildVector(unsigned long *vector, long fromLength, long toLength)
{
	unsigned long *newVector = (unsigned long*)malloc (sizeof(long)*toLength);
	unsigned long *seq = (unsigned long*)malloc (sizeof(long)*toLength);
	unsigned long j = 0;

	for (register unsigned long i = 0; i < toLength; i++)
	{
		seq[i] = i;
		if(i < fromLength )
		{
			newVector[i] = vector[i];
		}
		else
		{
			while(IsPresents(vector,fromLength, seq[j])){j++;}
			newVector[i] = seq[j];
			j++;
		}
	}

	free(seq);
	return newVector;
}

bool IsPresents(unsigned long *vector, long length, long value)
{
	for (register unsigned long i = 0; i < length; i++)
	{
		if(vector[i] == value)
			return true;
	}

	return false;
}
