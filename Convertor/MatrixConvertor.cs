using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace Converter
{
    enum MatrixConverterMethod {AsciiToBinary, BinaryToAscii};
    

    class MatrixConvertor
    {
        private const string stamp = "@M=";
        public string Stamp
        {
            get
            {
                return stamp;
            }
        }

        private string srcPath;
        public string SrcPath
        {
            get
            {
                return this.srcPath;
            }
            set
            {
                if (File.Exists(value))
                    this.srcPath = value;
                else
                    throw new FileNotFoundException("File not found "+value);
            }
        }

        private string dstPath;
        public string DstPath
        {
            get
            {
                return this.dstPath;
            }
            set
            {
                this.dstPath = value;
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

        public void MatrxFile(string srcPath, string dstPath)
        {
            this.DstPath = dstPath;
            this.SrcPath = srcPath;
        }

        //Конвертирование файла матрицы (текстовое представление <-> бинарное представление)
        public void Convert(MatrixConverterMethod mcm)
        {
            if(mcm == MatrixConverterMethod.AsciiToBinary){
                ConvertAsciiToBinary();
            }else if(mcm == MatrixConverterMethod.BinaryToAscii){
                ConvertBinaryToASCII();
            }else{
                throw new Exception("Unknown convert method");
            }
            
        }

        /// <summary>
        /// Конвертирование файла матрицы из текстового представления в бинарное
        /// </summary>
        private void ConvertAsciiToBinary(){
            StreamReader src = File.OpenText(this.srcPath);
            
            string line;
            string section="";
            Regex reg;

            while ((line = src.ReadLine()) != null)
            {

                reg = new Regex(@"^\s*\[\s*(?<section>([a-z]+))\s*\]\s*$", RegexOptions.IgnoreCase);
                var m =reg.Match(line);
                if (m.Success)
                {
                    section = m.Groups["section"].ToString().ToLower();
                    continue;
                }

                if (section == "properties")
                {
                    reg = new Regex(@"^\s*(?<key>([a-z]+))\s*=\s*(?<value>([a-z0-9':! ]+))$", RegexOptions.IgnoreCase);
                    m = reg.Match(line);
                    if (m.Success)
                    {
                        switch (m.Groups["key"].ToString().ToLower())
                        {
                            case "description": this.Description = m.Groups["value"].ToString(); break;
                            case "matrixsize": this.MatrixSize = short.Parse(m.Groups["value"].ToString().Trim()); break;
                            case "cellsize": this.CellSize = short.Parse(m.Groups["value"].ToString().Trim()); break;
                        }
                    }
                    continue;
                }
                else if (section == "matrix")
                {
                    FileStream dst = new FileStream(this.dstPath, FileMode.Create);
                    int fileOffset = 0;
                    
                    //Write stamp to file
                    byte[] stamp_b = System.Text.Encoding.ASCII.GetBytes(this.Stamp);
                    dst.Write(stamp_b, 0, stamp_b.Length);
                    dst.Seek(stamp_b.Length, SeekOrigin.Begin);

                    byte[] description_b = System.Text.Encoding.ASCII.GetBytes(this.description+'\0');
                    dst.Write(description_b, 0, description_b.Length);
                    //dst.Seek(description_b.Length, SeekOrigin.Current);

                    byte[] matrixSize_b = BitConverter.GetBytes(this.matrixSize);
                    dst.Write(matrixSize_b, 0, matrixSize_b.Length);
                    //dst.Seek(matrixSize_b.Length, SeekOrigin.Current);

                    byte[] cellSize_b = BitConverter.GetBytes(this.cellSize);
                    dst.Write(cellSize_b, 0, cellSize_b.Length);
                    //dst.Seek(cellSize_b.Length, SeekOrigin.Current);

                    do
                    {
                        reg = new Regex("(?<cell>(([0-9.-]+,*)+))", RegexOptions.IgnoreCase);
                        var matches = reg.Matches(line);
                        foreach (Match mm in matches)
                        {
                            string[] values = mm.Groups["cell"].ToString().Trim().Split(',');
                            foreach(string s in values)
                            {
                                byte a = byte.Parse(s.Trim());
                                dst.WriteByte(a);
                            }
                        }
                    } while ((line = src.ReadLine()) != null);
                    dst.Close();
                    continue;
                }
            }

            src.Close();
            
        }

        /// <summary>
        /// Конвертирование файла матрицы из бинарного представления в текстовое
        /// </summary>
        private void ConvertBinaryToASCII()
        {
            FileStream src = File.OpenRead(this.srcPath);
            FileStream dstFile = new FileStream(this.dstPath, FileMode.Create);
            StreamWriter dst = new StreamWriter(dstFile);

            byte[] stamp_read = new byte[3];
            src.Read(stamp_read,0,3);

            byte[] stamp_original = System.Text.Encoding.ASCII.GetBytes(this.Stamp);
            if (!stamp_original.SequenceEqual(stamp_read))
            {
                throw new Exception("Uknown file format");
            }

            long pos = src.Position;
            while(src.ReadByte()!=0){}

            long len = src.Position - pos;
            byte[] description_b = new byte[len-1];
            src.Seek(pos, SeekOrigin.Begin);
            src.Read(description_b, 0, (int)(len-1));


             
            dst.WriteLine("[Properties]");
            this.Description = Encoding.UTF8.GetString(description_b, 0, (int)(len - 1));
            dst.WriteLine("Description = "+this.Description);
            src.Seek(1, SeekOrigin.Current);

            byte[] val = new byte[2];
            src.Read(val, 0, 2);
            //Array.Reverse(val);
            this.MatrixSize= BitConverter.ToInt16(val, 0);
            dst.WriteLine("MatrixSize = " + BitConverter.ToInt16(val, 0));

            src.Read(val, 0, 2);
            //Array.Reverse(val);
            this.CellSize = BitConverter.ToInt16(val, 0);
            dst.WriteLine("CellSize = " + BitConverter.ToInt16(val, 0));

            int b;
            int index = 1;
            int columns = 0;
            dst.WriteLine("[Matrix]");
            while((b=src.ReadByte())!=-1)
            {
                dst.Write(b.ToString());
                if (index < this.CellSize) {                 
                    dst.Write(',');
                    index++;
                }else {
                    index = 1;
                    dst.Write('\t');
                    columns++;
                    if (columns == this.MatrixSize)
                    {
                        dst.Write('\n');
                        columns = 0;
                    }
                }
            }

            dst.Flush();
            dst.Close();
            src.Close();
        }
    }
}
