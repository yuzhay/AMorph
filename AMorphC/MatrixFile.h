#pragma once

#include <string>

using namespace std;

class MatrixFile
{
public:
	MatrixFile(string path);
	MatrixFile();
	~MatrixFile(void);

	string Path;
	const string Stamp;
	short MatrixSize;
	short CellSize;
	string Description;
	unsigned char *** Matrix;
	unsigned char* MatrixVector;

    void Load();
	void Save();

    /// �������������� ������� � ������, ������������ ��� �������� � ������������� ���
    void MatrixToVector();

};

