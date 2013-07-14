using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace AMorph
{
    public class IsomorphMatrices
    {
        private const string dll = "permutation.dll";
        private IntPtr handler;
        private bool free = false;
        private uint sizeMtx;

        public IsomorphMatrices(uint sizeMtx, uint sizeCell, byte[] mtxVectorOrig, byte[] mtxVectorModified)
        {
            this.sizeMtx = sizeMtx;
            this.handler = IsomorphMatricesContructorDll(sizeMtx, sizeCell, mtxVectorOrig, mtxVectorModified);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_Constructor")]
        private extern static IntPtr IsomorphMatricesContructorDll(uint sizeMtx, uint sizeCell, byte[] mtxVectorOrig, byte[] mtxVectorModified);

        public void TranspositionLeft(uint[] vector)
        {
            TranspositionLeftDll(handler, vector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_TranspositionLeft")]
        private extern static void TranspositionLeftDll(IntPtr handle, uint[] vector);

        public void TranspositionRight(uint[] vector)
        {
            TranspositionRightDll(handler, vector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_TranspositionRight")]
        private extern static void TranspositionRightDll(IntPtr handle, uint[] vector);

        public void Transposition(uint[] vector)
        {
            TranspositionDll(handler, vector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_Transposition")]
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
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_GetSubstitutions")]
        private extern static IntPtr GetSubstitutionsDll(IntPtr handle, uint index);

        public uint GetParts(uint index)
        {
            return GetPartsDll(handler, index);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_GetParts")]
        private extern static uint GetPartsDll(IntPtr handle, uint index);

        public uint GetSubstitutionsCount()
        {
            return GetSubstitutionsCountDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_GetSubstitutionsCount")]
        private extern static uint GetSubstitutionsCountDll(IntPtr handle);


        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_SearchIsomorph")]
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

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_SearchStatisticsIsomorph")]
        private extern static void SearchStatisticsIsomorphDll(IntPtr handle, byte options);

        public Task SearchStatisticsIsomorph(byte options = 3)
        {
            Task t = new Task(() =>
            {
                SearchStatisticsIsomorphDll(handler, options);
            }
            );
            t.Start();
            return t;

        }

        public float GetProgress()
        {
            return GetProgressDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_GetProgress")]
        private extern static float GetProgressDll(IntPtr handle);

        ~IsomorphMatrices()
        {
            if (handler != null && !free)
            {
                free = true;
                IsomorphMatricesDestructor(handler);
            }
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IsomorphMatrices_Destructor")]
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
