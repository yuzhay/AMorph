#include "StdAfx.h"
#include <vector>
#include "PermVectorTrunk.h"

using namespace std;

PermVectorTrunk::PermVectorTrunk(unsigned long Size)
{
	if(Size>0) this->size = Size; else this->size = 1;
	
}

PermVectorTrunk::~PermVectorTrunk(void)
{
	for(unsigned long i=0;i<trunk.size();++i)
	{
		free(trunk[i]);
	}
}

void PermVectorTrunk::Add(unsigned long *vector, unsigned long counter)
{
	bool lt = false; //less flag
	bool gt = false; //great flag

	unsigned long * tmp = (unsigned long*)malloc(sizeof(long)*this->size);
	memcpy(tmp,vector,sizeof(long)*this->size);

	//trunk.push_back(tmp);
	parts.push_back(counter);
	
	if(trunk.size()==0)
	{
		trunk.push_back(tmp);
	}else
	{
		this->it = trunk.begin();
		this->it += (trunk.size() / 2);
		while(true)
		{
			if(it == trunk.end() && gt)
			{
				trunk.push_back(tmp);
				return;
			}

			if(VectorCmp(tmp,*it)>0)
			{
				//TMP > it
				if(lt)
				{
					trunk.insert(++it,tmp);
					lt = false;
					return;
				}else{
					it++;
					gt = true;
				}
			}else if(VectorCmp(tmp,*it)<0)
			{
				//TMP < it
				if(gt)
				{
					trunk.insert(it,tmp);
					gt = false;
					return;
				}else if(this->it == trunk.begin())
				{
					trunk.insert(it,tmp);
					return;
				}else{
					it--;
					lt = true;
				}

			}else{
				MessageBoxA(0,"Duplicate","",0);
				return;
			}
		}

	}
	
}

int PermVectorTrunk::VectorCmp(const unsigned long *v1, const unsigned long* v2)
{
	for(unsigned long i=0;i<this->size;++i)
	{
		if(v1[i]>v2[i]) 
			return 1;
		else if(v1[i]<v2[i])
			return -1;
	}
	return 0;
}

unsigned long* PermVectorTrunk::operator[](const unsigned long index)
{
	if(index >= trunk.size()) return NULL;
	return this->trunk[index];
}

bool PermVectorTrunk::Find(unsigned long *vector)
{
	unsigned long *last = NULL;
	unsigned long *beforeLast = NULL;
	this->it = trunk.begin();
	this->it += (trunk.size() / 2);
	
	int result;
	bool lf,gf = false;
	int i = 0;
	while(this->it != trunk.end())
	{
		result = VectorCmp(vector,*it);
		if(result == 0)
		{
			return true;
		}
		else if(result>0)
		{
			beforeLast = last;
			last = *it;
			result = trunk.end() - this->it;
			this->it += (result>1)?((result)/2):(result);

			if(*it == beforeLast) return false;
			if((trunk.end() - this->it)/2==0 && gf) return false;
			if((trunk.end() - this->it)/2==0) gf = true;

		}else if(result<0)
		{
			beforeLast = last;
			last = *it;
			result = this->it - trunk.begin();
			this->it -= (result>1)?((result)/2):(result);
			if(*it == beforeLast) return false;
		}
		

	}
	return false;
}

long PermVectorTrunk::GetSize()
{
	return trunk.size();
}

unsigned long PermVectorTrunk::GetParts(unsigned long index)
{
	return parts[index];
}