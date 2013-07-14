using System;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows;
using System.IO;

namespace AMorph
{
    public class AutomorphMatrix
    {
        private const string dll = "permutation.dll";
        private IntPtr handler;
        private bool free = false;
        private uint sizeMtx;

        public AutomorphMatrix(uint sizeMtx, uint sizeCell, byte[] mtxVector)
        {
            this.sizeMtx = sizeMtx;
            this.handler = AutomorphMatrixContructorDll(sizeMtx, sizeCell, mtxVector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "AutomorphMatrix_Constructor")]
        private extern static IntPtr AutomorphMatrixContructorDll(uint matrixSize, uint cellMatrix, byte[] matrix);

        public Task SearchAutomorph(uint length, uint[] vector)
        {
            Task t = new Task(() =>
            {
                SearchAutomorphDll(handler, length, vector);
            });
            t.Start();
            return t;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "AutomorphMatrix_SearchAutomorph")]
        private extern static void SearchAutomorphDll(IntPtr handle, uint length, uint[] vector);

        public uint[] GetSubstitutions(uint index)
        {
            IntPtr array = GetSubstitutionsDll(handler, index);
            int[] vector = new int[this.sizeMtx];
            uint[] uvector = new uint[this.sizeMtx];
            Marshal.Copy(array, vector, 0, (int)this.sizeMtx);
            for (int i = 0; i < this.sizeMtx; i++)
                uvector[i] = (uint)vector[i];
            return uvector;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "AutomorphMatrix_GetSubstitutions")]
        private extern static IntPtr GetSubstitutionsDll(IntPtr handle, uint index);


        public uint GetSubstitutionsCount()
        {
            return GetSubstitutionsCountDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "AutomorphMatrix_GetSubstitutionsCount")]
        private extern static uint GetSubstitutionsCountDll(IntPtr handle);

        public float GetProgress()
        {
            return GetProgressDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "AutomorphMatrix_GetProgress")]
        private extern static float GetProgressDll(IntPtr handle);

        ~AutomorphMatrix()
        {
            if (handler != null && !free)
            {
                free = true;
                AutomorphMatrixDestructor(handler);
            }
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "AutomorphMatrix_Destructor")]
        private extern static void AutomorphMatrixDestructor(IntPtr handle);

        public void Destructor()
        {
            if (handler != null && !free)
            {
                free = true;
                AutomorphMatrixDestructor(handler);
            }
        }
    }
}
