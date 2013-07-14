using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace AMorph
{
    class MatrixFile
    {
        private string path;
        public string Path
        {
            get
            {
                return this.path;
            }
            set
            {
                this.path = value;
            }
        }

        private const string stamp = "@M=";
        public string Stamp
        {
            get
            {
                return stamp;
            }
        }

        private short matrixSize = 2;
        public short MatrixSize
        {
            get
            {
                return this.matrixSize;
            }
            set
            {
                if (value > 1)
                    this.matrixSize = value;
                else
                    throw new Exception("Matrix size must be great than 1");
            }
        }

        private short cellSize = 1;
        public short CellSize
        {
            get
            {
                return cellSize;
            }
            set
            {
                if (value > 0)
                    this.cellSize = value;
                else
                    throw new Exception("Cell size must be great than 0");
            }
        }

        private string description;
        public string Description
        {
            get
            {
                return this.description;
            }
            set
            {
                if (!String.IsNullOrEmpty(value))
                    this.description = value;
                else
                    throw new Exception("Matrix must have some description.");
            }
        }

        private byte[,,] matrix;
        public byte[,,] Matrix
        {
            get
            {
                return matrix;
            }
            set
            {
                this.matrix = Matrix;
            }
        }

        private byte[] matrixVector;
        public byte[] MatrixVector
        {
            get
            {
                return this.matrixVector;
            }
        }

        public void MatrxFile(string path)
        {
            this.Path = path;
        }

        /// <summary>
        /// Загрузка матрицы из бинарного файла
        /// </summary>
        public void Load()
        {
            FileStream src = File.OpenRead(this.Path);

            byte[] stamp_read = new byte[3];
            src.Read(stamp_read, 0, 3);

            byte[] stamp_original = System.Text.Encoding.ASCII.GetBytes(this.Stamp);
            if (!stamp_original.SequenceEqual(stamp_read))
            {
                throw new Exception("Uknown file format");
            }

            long pos = src.Position;
            while (src.ReadByte() != 0) { }

            long len = src.Position - pos;
            byte[] description_b = new byte[len - 1];
            src.Seek(pos, SeekOrigin.Begin);
            src.Read(description_b, 0, (int)(len - 1));

            this.Description = Encoding.UTF8.GetString(description_b, 0, (int)(len - 1));

            src.Seek(1, SeekOrigin.Current);

            byte[] val = new byte[2];
            src.Read(val, 0, 2);
            this.MatrixSize = BitConverter.ToInt16(val, 0);

            src.Read(val, 0, 2);
            this.CellSize = BitConverter.ToInt16(val, 0);

            int b;
            int index = 0;
            int column = 0;
            int row = 0;

            matrix = new Byte[this.MatrixSize,this.MatrixSize,this.CellSize];

            while ((b = src.ReadByte()) != -1)
            {
                matrix[row, column, index] = (byte)b;
                if (index < this.CellSize-1)
                {
                    index++;
                }
                else
                {
                    index = 0;
                    column++;
                    if (column == this.MatrixSize)
                    {
                        column = 0;
                        row++;
                    }
                }
            }

            src.Close();
        }


        public void Save()
        {
            new NotImplementedException();
        }

        /// <summary>
        /// Преобразование матрицы в вектор, используется для передачи в неуправляемый код
        /// </summary>
        public void MatrixToVector()
        {
            this.matrixVector = new byte[this.matrixSize * this.matrixSize * this.cellSize];

            int index = 0;
            for (int i = 0; i < this.matrixSize; i++)
            {
                for (int j = 0; j < this.matrixSize; j++)
                {
                    for (int k = 0; k < this.cellSize; k++)
                    {
                        this.matrixVector[index] = matrix[i,j,k];
                        index++;
                    }
                }
            }
        }
    }
}
