using System;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows;
using System.IO;


namespace AMorph
{
    public class HadamardMatrix
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

        public HadamardMatrix(uint sizeMtx, sbyte[] mtxVector)
        {
            matrix = mtxVector;
            this.sizeMtx = sizeMtx;
            this.handler = HadamardMatrixContructorDll(sizeMtx, mtxVector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_Constructor")]
        private extern static IntPtr HadamardMatrixContructorDll(uint matrixSize, sbyte[] matrix);

        public Task SearchAutomorph(uint length, uint[] vector)
        {
            Task t = new Task(() =>
            {
                SearchAutomorphDll(handler, length, vector);
            });
            t.Start();
            return t;
        }

        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_SearchAutomorph")]
        private extern static void SearchAutomorphDll(IntPtr handle, uint length, uint[] vector);

        public sbyte[] GetSigns(uint index)
        {
            IntPtr array = GetSignsDll(handler, index);
            byte[] vector = new byte[this.sizeMtx];
            sbyte[] uvector = new sbyte[this.sizeMtx];
            Marshal.Copy(array, vector, 0, (int)this.sizeMtx);
            for (int i = 0; i < this.sizeMtx; i++)
                uvector[i] = (sbyte)vector[i];
            return uvector;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_GetSigns")]
        private extern static IntPtr GetSignsDll(IntPtr handle, uint index);


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
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_GetSubstitutions")]
        private extern static IntPtr GetSubstitutionsDll(IntPtr handle, uint index);

        public uint GetSubstitutionsParts(uint index)
        {
            uint num = GetSubstitutionsPartsDll(handler, index);
            
            return num;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_GetSubstitutionsParts")]
        private extern static uint GetSubstitutionsPartsDll(IntPtr handle, uint index);

        public uint GetIterationDepthValue(uint index)
        {
            uint num = GetIterationDepthValueDll(handler, index);

            return num;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_GetIterationDepthValue")]
        private extern static uint GetIterationDepthValueDll(IntPtr handle, uint index);

        public uint GetSubstitutionsCount()
        {
            return GetSubstitutionsCountDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_GetSubstitutionsCount")]
        private extern static uint GetSubstitutionsCountDll(IntPtr handle);

        public float GetProgress()
        {
            return GetProgressDll(handler);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_GetProgress")]
        private extern static float GetProgressDll(IntPtr handle);

        ~HadamardMatrix()
        {
            if (handler != null && !free)
            {
                free = true;
                HadamardMatrixDestructor(handler);
            }
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "HadamardMatrix_Destructor")]
        private extern static void HadamardMatrixDestructor(IntPtr handle);

        public void Destructor()
        {
            if (handler != null && !free)
            {
                free = true;
                HadamardMatrixDestructor(handler);
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
