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

    /// ѕреобразование матрицы в вектор, используетс€ дл€ передачи в неуправл€емый код
    void MatrixToVector();

};

