using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace AMorph
{
    public class StatisticsMatrix
    {
        private const string dll = "permutation.dll";
        private IntPtr handler;
        private bool free = false;
        private uint sizeMtx;
        private uint sizeCell;

        public StatisticsMatrix(uint sizeMtx, uint sizeCell, byte[] mtxVector)
        {
            this.sizeMtx = sizeMtx;
            this.sizeCell = sizeCell;
            this.handler = StatisticsMatrixContructorDll(sizeMtx, sizeCell, mtxVector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "StatisticsMatrix_Constructor")]
        private extern static IntPtr StatisticsMatrixContructorDll(uint matrixSize, uint cellMatrix, byte[] matrix);

        public void Run(byte options = 3)
        {
            RunDll(handler, options);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "StatisticsMatrix_Run")]
        private extern static void RunDll(IntPtr handle, byte symmetric);

        public Dictionary<uint, uint> GetColumn(uint index)
        {
            Dictionary<uint, uint> dictionary = new Dictionary<uint, uint>();
            uint arraySize = 0;
            IntPtr array = GetColumnDll(handler, index, out arraySize);

            int[] vector = new int[arraySize];

            Marshal.Copy(array, vector, 0, (int)arraySize);
            for (uint i = 0; i < vector.Length/2; ++i)
            {
                dictionary.Add((uint)vector[2 * i], (uint)vector[2 * i + 1]);
            }
            return dictionary;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "StatisticsMatrix_GetColumn")]
        private extern static IntPtr GetColumnDll(IntPtr handle, uint index, out uint arraySize);

        public Dictionary<uint, uint> GetRow(uint index)
        {
            Dictionary<uint, uint> dictionary = new Dictionary<uint, uint>();
            uint arraySize = 0;
            
            IntPtr array = GetRowDll(handler, index, out arraySize);

            int[] vector = new int[arraySize];

            Marshal.Copy(array, vector, 0, (int)arraySize);
            for (uint i = 0; i < vector.Length/2; ++i)
            {
                dictionary.Add((uint)vector[2 * i], (uint)vector[2 * i + 1]);
            }
            return dictionary;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "StatisticsMatrix_GetRow")]
        private extern static IntPtr GetRowDll(IntPtr handle, uint index, out uint arraySize);

        ~StatisticsMatrix()
        {
            if (handler != null && !free)
            {
                free = true;
                StatisticsMatrixDestructor(handler);
            }
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "StatisticsMatrix_Destructor")]
        private extern static void StatisticsMatrixDestructor(IntPtr handle);

        public void Destructor()
        {
            if (handler != null && !free)
            {
                free = true;
                StatisticsMatrixDestructor(handler);
            }
        }
    }
}
