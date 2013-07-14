#include "StdAfx.h"
#include <vector>
#include "SubstitutionCollection.h"

using namespace std;

SubstitutionCollection::SubstitutionCollection(unsigned long Size)
{
	if(Size>0) this->size = Size; else this->size = 1;
	InversionIsEnabled = false;

	this->iterations = new unsigned long[this->size];
	for(int i = 0; i < this->size; i++)
	{
		this->iterations[i] = 0;
	}
	
}

SubstitutionCollection::~SubstitutionCollection(void)
{
	for(unsigned long i=0;i<trunk.size();++i)
	{
		if(InversionIsEnabled)
		{
			if(trunk[i]->signs) delete [] trunk[i]->signs;
			if(trunk[i]->vector) delete [] trunk[i]->vector;
			if(trunk[i]) delete trunk[i];
		}
		else
		{
			if(trunk[i]->vector) delete [] trunk[i]->vector;
			if(trunk[i]) delete trunk[i];
		}
		
	}
	if(this->iterations) delete [] this->iterations;
}
void SubstitutionCollection:: IncrementDepthValue(unsigned long depth)
{
	if(depth >= 0)
	{
		this->iterations[depth]++;
	}
}
unsigned long SubstitutionCollection::GetIterationDepthValue(unsigned long depth)
{
	if(depth >=0 )
	{
		return this->iterations[depth];
	}
	return -1;
}
void SubstitutionCollection::Add(Substitution *subs)
{
	Substitution *newSub = new Substitution;
	newSub->signs = new char[this->size];
	newSub->vector = new unsigned long[this->size];

	memcpy(newSub->signs,subs->signs,sizeof(char)*this->size);
	memcpy(newSub->vector,subs->vector,sizeof(long)*this->size);

	trunk.push_back(newSub);
}

void SubstitutionCollection::Add(unsigned long* vector, char* signs, unsigned long parts)
{
	Substitution *newSub = new Substitution;
	newSub->signs = new char[this->size];
	newSub->vector = new unsigned long[this->size];

	memcpy(newSub->signs,signs,sizeof(char)*this->size);
	memcpy(newSub->vector,vector,sizeof(long)*this->size);

	newSub->partitions = parts;
	newSub->size = this->size;

	trunk.push_back(newSub);
}
void SubstitutionCollection::Add(unsigned long* vector, unsigned long parts)
{
	Substitution *newSub = new Substitution;
	newSub->vector = new unsigned long[this->size];

	memcpy(newSub->vector,vector,sizeof(long)*this->size);
	newSub->partitions = parts;
	newSub->size = this->size;

	trunk.push_back(newSub);
}
Substitution* SubstitutionCollection::operator[](const unsigned long index)
{
	if(index >= trunk.size()) return NULL;
	return this->trunk[index];
}


long SubstitutionCollection::GetSize()
{
	return trunk.size();
}

bool SubstitutionCollection:: GetInversionIsEnabled()
{
	return this->InversionIsEnabled;
}
void SubstitutionCollection:: SetInversionIsEnabled(bool arg)
{
	InversionIsEnabled = arg;
}