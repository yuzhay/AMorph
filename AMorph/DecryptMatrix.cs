using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace AMorph
{
    public class DecryptMatrix
    {
        private const string dll = "permutation.dll";
        private IntPtr handler;
        private bool free = false;
        private uint sizeMtx;

        public DecryptMatrix(uint sizeMtx, uint sizeCell, byte[] mtxVectorOrig, byte[] mtxVectorModified)
        {
            this.sizeMtx = sizeMtx;
            this.handler = IsomorphMatricesContructorDll(sizeMtx, sizeCell, mtxVectorOrig, mtxVectorModified);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_Constructor")]
        private extern static IntPtr IsomorphMatricesContructorDll(uint sizeMtx, uint sizeCell, byte[] mtxVectorOrig, byte[] mtxVectorModified);

        public void TranspositionLeft(uint[] vector)
        {
            TranspositionLeftDll(handler, vector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_TranspositionLeft")]
        private extern static void TranspositionLeftDll(IntPtr handle, uint[] vector);

        public void TranspositionRight(uint[] vector)
        {
            TranspositionRightDll(handler, vector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_TranspositionRight")]
        private extern static void TranspositionRightDll(IntPtr handle, uint[] vector);

        public void Transposition(uint[] vector)
        {
            TranspositionDll(handler, vector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_Transposition")]
        private extern static void TranspositionDll(IntPtr handle, uint[] vector);

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
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_GetSubstitutions")]
        private extern static IntPtr GetSubstitutionsDll(IntPtr handle, uint index);

        public uint GetSubstitutionsCount()
        {
            return GetSubstitutionsCountDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_GetSubstitutionsCount")]
        private extern static uint GetSubstitutionsCountDll(IntPtr handle);


        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_SearchIsomorph")]
        private extern static void SearchIsomorphDll(IntPtr handle, uint length, uint[] vector);

        public Task SearchIsomorph(uint length, uint[] vector)
        {
            Task t = new Task(() =>
            {
                SearchIsomorphDll(handler, length, vector);
            }
            );
            t.Start();
            return t;

        }


        public float GetProgress()
        {
            return GetProgressDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_GetProgress")]
        private extern static float GetProgressDll(IntPtr handle);

        ~DecryptMatrix()
        {
            if (handler != null && !free)
            {
                free = true;
                IsomorphMatricesDestructor(handler);
            }
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "DecryptMatrix_Destructor")]
        private extern static void IsomorphMatricesDestructor(IntPtr handle);



        public void Destructor()
        {
            if (handler != null && !free)
            {
                free = true;
                IsomorphMatricesDestructor(handler);
            }
        }

    }
}
