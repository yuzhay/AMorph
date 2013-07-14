using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace AMorph
{
    public class IncidenceMatrix
    {
        private const string dll = "permutation.dll";
        private IntPtr handler;
        private bool free = false;

        private sbyte[] matrix;
        public sbyte[] Matrix
        {
            get { return matrix; }
            
        }

        private uint sizeMtx;
        public uint SizeMtx
        {
            get { return sizeMtx; }
            set
            {
                sizeMtx = value;
            }
        }

        public IncidenceMatrix(uint sizeMtx, sbyte[] mtxVector)
        {
            matrix = mtxVector;
            this.sizeMtx = sizeMtx;
            this.handler = IncidenceMatrixContructorDll(sizeMtx, mtxVector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IncidenceMatrix_Constructor")]
        private extern static IntPtr IncidenceMatrixContructorDll(uint matrixSize, sbyte[] matrix);

        public Task SearchAutomorph(uint length, uint[] vector)
        {
            Task t = new Task(() =>
            {
                SearchAutomorphDll(handler, length, vector);
            });
            t.Start();
            return t;
        }

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IncidenceMatrix_SearchAutomorph")]
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
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IncidenceMatrix_GetSubstitutions")]
        private extern static IntPtr GetSubstitutionsDll(IntPtr handle, uint index);

        public uint GetSubstitutionsParts(uint index)
        {
            uint num = GetSubstitutionsPartsDll(handler, index);
            
            return num;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IncidenceMatrix_GetSubstitutionsParts")]
        private extern static uint GetSubstitutionsPartsDll(IntPtr handle, uint index);

        public uint GetSubstitutionsCount()
        {
            return GetSubstitutionsCountDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IncidenceMatrix_GetSubstitutionsCount")]
        private extern static uint GetSubstitutionsCountDll(IntPtr handle);

        public float GetProgress()
        {
            return GetProgressDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IncidenceMatrix_GetProgress")]
        private extern static float GetProgressDll(IntPtr handle);

        ~IncidenceMatrix()
        {
            if (handler != null && !free)
            {
                free = true;
                IncidenceMatrixDestructor(handler);
            }
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "IncidenceMatrix_Destructor")]
        private extern static void IncidenceMatrixDestructor(IntPtr handle);

        public void Destructor()
        {
            if (handler != null && !free)
            {
                free = true;
                IncidenceMatrixDestructor(handler);
            }
        }


        public sbyte this[int i, int j]
        {
            set
            {
                matrix[i + j * sizeMtx] = value;
            }
            get
            {
                return matrix[i + j * sizeMtx];
            }
        }

    }
}
