#include "Stats.h"

Stats::Stats()
{
	this->size = 1;
}
Stats::Stats(int size)
{
	this->sizeOfMatrixDim = size;
	this->partialSubstitution = new int[size];
	memset(this->partialSubstitution, 0, sizeof(int)*size);

}
Stats::~Stats()
{
		list.clear();
		if(this->partialSubstitution != NULL) delete [] this->partialSubstitution;
}
void Stats::Add(time_t time)
{
	list.push_back(time);
}
void Stats::Add(int num)
{
	if(this->partialSubstitution == NULL) return;

	++partialSubstitution[num];
}
void Stats::AddTimePoint()
{
	Add(time(NULL));
}
void Stats::AddSubstitutionToLevel(int num)
{
	Add(num);
}
time_t Stats::operator[](const unsigned long index)
{
	if(index >= list.size()) return NULL;
	return this->list[index];
}
