#pragma once

#include <vector>

#define CLASS_DECLSPEC    __declspec(dllimport)

using namespace std;

class CLASS_DECLSPEC PermVectorTrunk
{
private:
	unsigned long size;
	vector<unsigned long*> trunk;

	//Begin iterator
	vector<unsigned long*>::iterator it;
	
	//Vector comporator
	int VectorCmp(const unsigned long *v1, const unsigned long* v2);
public:
	PermVectorTrunk(unsigned long Size);
	~PermVectorTrunk(void);
	unsigned long* operator[](const unsigned long index);

	void Add(unsigned long *vector);
	bool Find(unsigned long *vector);
	long GetSize();
};
