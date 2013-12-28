#pragma once

#include <vector>
#include <iostream>

using namespace std;

struct Vertex{
	unsigned long *vector;
	long length;
};

class VertexContainer
{
private:
	vector<Vertex> pool;
	bool Equals(Vertex *v1,Vertex *v2);
	bool VertexContainer::DepthEquals(Vertex *v1,Vertex *v2, long depth);

public:
	VertexContainer(void);
	void Add(unsigned long *v, long length);
	void Add(Vertex v);

	void Remove(unsigned long *v, long length);
	void Remove(Vertex v);

	bool Find(unsigned long *v, long length, long depth);

	void Print();
	void Clear();

	~VertexContainer(void);
};

