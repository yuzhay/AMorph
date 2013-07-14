using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Converter
{
    class Program
    {
        static public void Usage()
        {
            Console.WriteLine("Usage: Converter.exe ascii2bin|bin2ascii source destination");
        }

        static void Main(string[] args)
        {

            if (args.Count() == 3)
            {
                try
                {
                    MatrixConvertor mc = new MatrixConvertor();
                    mc.SrcPath = args[1];
                    mc.DstPath = args[2];

                    MatrixConverterMethod mcm = MatrixConverterMethod.AsciiToBinary;
                    if (args[1] == "ascii2bin")
                        mcm = MatrixConverterMethod.AsciiToBinary;
                    else if (args[0] == "bin2ascii")
                    {
                        mcm = MatrixConverterMethod.BinaryToAscii;
                    }
                    else
                    {
                        Usage();
                    }
                    mc.Convert(mcm);
                    Console.WriteLine("Done!");
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }
            }
            else
            {
                Usage();
            }
        }
    }
}
