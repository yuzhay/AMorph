using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace AMorph
{
    public class BasicMatrix
    {
        private const string dll = "permutation.dll";
        private IntPtr handler;
        private bool free = false;
        private uint sizeMtx;
        private uint sizeCell;

        public BasicMatrix(uint sizeMtx, uint sizeCell, byte[] mtxVector)
        {
            this.sizeMtx = sizeMtx;
            this.sizeCell = sizeCell;
            this.handler = BasicMatrixContructorDll(sizeMtx, sizeCell, mtxVector);
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "BasicMatrix_Constructor")]
        private extern static IntPtr BasicMatrixContructorDll(uint sizeMtx, uint sizeCell, byte[] mtxVector);

        public Task TranspositionLeft(uint[] vector)
        {
            Task t = new Task(() =>
                {
                    TranspositionLeftDll(handler, vector);
                }
            );
            t.Start();
            return t;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "BasicMatrix_TranspositionLeft")]
        private extern static void TranspositionLeftDll(IntPtr handle, uint[] vector);

        public Task TranspositionRight(uint[] vector)
        {   
            Task t = new Task(() =>
                {
                    TranspositionRightDll(handler, vector);
                }
            );
            t.Start();
            return t;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "BasicMatrix_TranspositionRight")]
        private extern static void TranspositionRightDll(IntPtr handle, uint[] vector);

        public Task Transposition(uint[] vector)
        {
            Task t = new Task(() =>
                {
                    TranspositionDll(handler, vector);
                }
            );
            t.Start();
            return t;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "BasicMatrix_Transposition")]
        private extern static void TranspositionDll(IntPtr handle, uint[] vector);

        public void ImposeGamma(byte[] vector)
        {
            //Task t = new Task(() =>
            //{
                ImposeGammaDll(handler, vector, (uint)vector.Length);
            //}
            //);

            //return t;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "BasicMatrix_ImposeGamma")]
        private extern static void ImposeGammaDll(IntPtr handle, byte[] vector, uint length);


        ~BasicMatrix()
        {
            if (handler != null && !free)
            {
                free = true;
                BasicMatrixDestructor(handler);
            }
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "BasicMatrix_Destructor")]
        private extern static void BasicMatrixDestructor(IntPtr handle);

        public void Destructor()
        {
            if (handler != null && !free)
            {
                free = true;
                BasicMatrixDestructor(handler);
            }
        }

        public byte[] GetMatrix()
        {
            IntPtr ptr = GetMatrixDll(handler);
            byte[] vector = new byte[this.sizeMtx*this.sizeMtx*this.sizeCell];
            Marshal.Copy(ptr, vector, 0, (int)(this.sizeMtx * this.sizeMtx * this.sizeCell));
            return vector;
        }
        [DllImport(dll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "BasicMatrix_GetMatrix")]
        private extern static IntPtr GetMatrixDll(IntPtr handle);

    }
}
