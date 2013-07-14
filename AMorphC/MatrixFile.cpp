#include "MatrixFile.h"
#include <iostream>
#include <fstream>

MatrixFile::MatrixFile(string path):Stamp("@M=")
{
	this->Path = path;
	this->Matrix = NULL;
	this->MatrixVector = NULL;
}

MatrixFile::MatrixFile():Stamp("@M=")
{
	this->Matrix = NULL;
	this->MatrixVector = NULL;
}

MatrixFile::~MatrixFile(void)
{
	if(this->Matrix != NULL)
	{
		for(int i=0;i<MatrixSize;i++){
			for(int j=0;j<MatrixSize;j++){
				free(this->Matrix[i][j]);
			}
			free(this->Matrix[i]);
		}
		free(this->Matrix);
	}
	if(this->MatrixVector != NULL)
	{
		free(MatrixVector);
	}

}

void MatrixFile::MatrixToVector()
{
	long index = 0;
	this->MatrixVector = (unsigned char *)malloc(this->MatrixSize*this->MatrixSize*this->CellSize);
	for (int i = 0; i < this->MatrixSize; i++)
    {
        for (int j = 0; j < this->MatrixSize; j++)
        {
            for (int k = 0; k < this->CellSize; k++)
            {
                this->MatrixVector[index] = this->Matrix[i][j][k];
                index++;
            }
        }
    }
}

void MatrixFile::Save()
{
	//new NotImplementedException();
}

void MatrixFile::Load()
{
	char stamp_read[3];
	//long position = 0;
	 fpos_t position = 0;

	FILE * file = fopen(this->Path.c_str(),"r");
	if(file == NULL){
		//throw new Excep
		exit(1);
	}

	fread(stamp_read,1,3,file);

	if(strcmp(stamp_read, this->Stamp.c_str())!=0)
	{
		//throw new Exception();
	}

	fgetpos(file,&position);
	while(fgetc(file)!=0){}

	fpos_t len;
	fgetpos(file,&len);

	len = len - position;
	fsetpos(file,&position);

	char *description = (char*) malloc (len+1);
	fread(description,1,len,file);
	this->Description = description;
	free(description);

	char matrixSize[2];
	fread(matrixSize,1,2,file);
	this->MatrixSize = (short)(matrixSize[0]+((short)matrixSize<<16));

	char cellSize[2];
	fread(cellSize,1,2,file);
	this->CellSize = (short)(cellSize[0]+((short)cellSize<<16));


    int b;
    int index = 0;
    int column = 0;
    int row = 0;


	//this->Matrix = new char[MatrixSize][MatrixSize][CellSize];
	//this->Matrix[MatrixSize][MatrixSize][CellSize];
	this->Matrix = (unsigned char***)malloc(sizeof(char**)*MatrixSize);
	for(int i=0;i<MatrixSize;i++){
		this->Matrix[i] = (unsigned char**)malloc(sizeof(char**)*MatrixSize);
		for(int j=0;j<MatrixSize;j++){
			this->Matrix[i][j] = (unsigned char*)malloc(CellSize);
		}
	}

 ////   //matrix = new Byte[this.MatrixSize,this.MatrixSize,this.CellSize];
	int c;
	while(!feof(file))
	{
		c = fgetc(file);
		if(c==EOF) break;
		this->Matrix[row][column][index] = (unsigned char)c;
		if (index < this->CellSize-1)
        {
            index++;
        }
        else
        {
            index = 0;
            column++;
            if (column == this->MatrixSize)
            {
                column = 0;
                row++;
            }
        }
	}
	

		fclose(file);

}