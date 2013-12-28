#include "VertexContainer.h"


VertexContainer::VertexContainer(void)
{
	
}


VertexContainer::~VertexContainer(void)
{
}


void VertexContainer::Add(unsigned long *v, long length)
{


	//���� ����� ������� ��� ����������, ����� ��� ������������� � ���������
	if(!Find(v,length, length))
	{
		Vertex vertex;
		vertex.vector = (unsigned long *)malloc(sizeof(unsigned long)*length);
		memcpy(vertex.vector,v,sizeof(unsigned long)*length);
		vertex.length = length;

		pool.push_back(vertex);

	}
}

void VertexContainer::Add(Vertex v)
{
	//���� ����� ������� ��� ����������, ����� ��� ������������� � ���������
	if(!Find(v.vector,v.length, v.length))
	{
		Vertex vertex;
		vertex.vector = (unsigned long *)malloc(sizeof(unsigned long)*v.length);
		memcpy(vertex.vector,v.vector,sizeof(unsigned long)*v.length);
		vertex.length = v.length;

		pool.push_back(vertex);
	}
}

void VertexContainer::Remove(unsigned long *v, long length)
{
	Vertex vertex;
	vertex.vector = v;
	vertex.length = length;
	
	for (register long i = 0; i < pool.size(); i++)
	{
		if(Equals(&pool[i],&vertex))
		{
			free(pool[i].vector);
			pool.erase(pool.begin() + i);
		}
	}
}

void VertexContainer::Remove(Vertex v)
{
	for (register long i = 0; i < pool.size(); i++)
	{
		if(Equals(&pool[i],&v))
		{
			free(pool[i].vector);
			pool.erase(pool.begin() + i);
		}
	}
}

//��������� 2 �������� 
bool VertexContainer::Equals(Vertex *v1,Vertex *v2)
{
	if(v1->length != v2->length)
		return false;

	for (register long i = 0; i < v1->length; i++)
	{
		if(v1->vector[i] != v2->vector[i])
			return false;
	}

	return true;
}


//��������� 2 �������� �� ������������ �������
bool VertexContainer::DepthEquals(Vertex *v1,Vertex *v2, long depth)
{
	if(v1->length < depth || v2->length < depth)
		return false;

	for (register long i = 0; i < depth; i++)
	{
		if(v1->vector[i] != v2->vector[i])
			return false;
	}

	return true;
}

//��������� ������ ������ �� ������� ������������ �������. �.�. ������� ��������
//� ���� �������, �� ����� ������� �� ����������� ������� �� ������� �������
bool VertexContainer::Find(unsigned long *v, long length, long depth)
{
	Vertex vertex;
	vertex.vector = v;
	vertex.length = length;

	for (register long i = 0; i < pool.size(); i++)
	{
		if(DepthEquals(&pool[i],&vertex, depth))
		{
			return true;
		}
	}
	return false;
}

void VertexContainer::Print()
{
	std::cout << "Vector Pool [" << pool.size() << "]:" <<endl;
	for (long i = 0; i < pool.size(); i++)
	{
		for (long j = 0; j < pool[i].length; j++)
		{
			std::cout << pool[i].vector[j] << " ";
		}
		std::cout << endl;
	}

}

void VertexContainer::Clear()
{
	for (register long i = 0; i < pool.size(); i++)
	{
		free(pool[i].vector);
	}

	pool.clear();
}