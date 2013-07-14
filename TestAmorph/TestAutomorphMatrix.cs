using System;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using AMorph;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace TestAmorph
{
    [TestClass]
    public class TestAutomorphMatrix
    {
        [TestMethod]
        public void TestAutomorph()
        {
            byte[] matrix = new byte[]{
                 0,1,0,0,1,
                 1,0,1,0,0,
                 0,1,0,1,0,
                 0,0,1,0,1,
                 1,0,0,1,0};
            
            AutomorphMatrix am = new AutomorphMatrix(5, 1, matrix);
            
            uint[] t1substitution = new uint[] { 0, 1, 2, 3, 4 };
            Task t = am.SearchAutomorph(1, t1substitution);
            t.Wait();
            uint count = am.GetSubstitutionsCount();

            Assert.AreEqual(count, (uint)10);

            uint[][] result = new uint[10][]{
                new uint[5]{0,1,2,3,4},
                new uint[5]{0,4,3,2,1},
                new uint[5]{1,0,4,3,2},
                new uint[5]{1,2,3,4,0},
                new uint[5]{2,1,0,4,3},
                new uint[5]{2,3,4,0,1},
                new uint[5]{3,2,1,0,4},
                new uint[5]{3,4,0,1,2},
                new uint[5]{4,0,1,2,3},
                new uint[5]{4,3,2,1,0},
            };
            
            for (uint i = 0; i < count; i++)
            {
                uint[] a = am.GetSubstitutions(i);
                bool b = a.SequenceEqual(result[i]);
                Assert.IsTrue(b);               
            }

        }

        [TestMethod]
        public void TestIsomorph()
        {
            ////IsoMorph
            byte[] matrixOrg = new byte[]{
                 (byte)'a',(byte)'b',(byte)'c',(byte)'d',(byte)'e',
                 (byte)'f',(byte)'g',(byte)'h',(byte)'i',(byte)'j',
				 (byte)'k',(byte)'l',(byte)'m',(byte)'n',(byte)'o',
				 (byte)'p',(byte)'q',(byte)'r',(byte)'s',(byte)'t',
				 (byte)'u',(byte)'v',(byte)'w',(byte)'x',(byte)'y'};


            //IsomorphMatrices im = new IsomorphMatrices(5,1,matrixOrg,matrixMod);
            uint[] substitution = new uint[] { 1, 0, 3, 4, 2 };

            BasicMatrix bm = new BasicMatrix(5, 1, matrixOrg);
            Task t = bm.Transposition(substitution);
            t.Wait();
            byte[] matrixMod = bm.GetMatrix();

            IsomorphMatrices im = new IsomorphMatrices(5, 1, matrixOrg, matrixMod);
            uint[] substitution2 = new uint[] { 0, 1, 2, 3, 4 };
            Task tt = im.SearchIsomorph(1, substitution2);
            tt.Wait();
            uint count = im.GetSubstitutionsCount();
            Assert.AreEqual(count, (uint)1);

            uint[] a = im.GetSubstitutions(0);
            Assert.IsTrue(a.SequenceEqual(new uint[] { 1, 0,3, 4, 2 }));
        }

        [TestMethod]
        public void TestIsomophImg()
        {
            ////IsoMorph

            MainWindow mw = new MainWindow();
            byte[] matrixOrg =  mw.ImageSourceToByteArray();
            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);

            //IsomorphMatrices im = new IsomorphMatrices(5,1,matrixOrg,matrixMod);
            uint[] substitution = new uint[mtxSize];
            for (int i = 0; i < substitution.Length; ++i)
            {
                substitution[i] = (uint)((i + 1) % (substitution.Length));
            }

            BasicMatrix bm = new BasicMatrix(mtxSize, 4, matrixOrg);
            Task t = bm.Transposition(substitution);
            t.Wait();
            byte[] matrixMod = bm.GetMatrix();
            IsomorphMatrices im = new IsomorphMatrices(mtxSize, 4, matrixOrg, matrixMod);
            uint[] substitution2 = new uint[mtxSize];
            for (int i = 0; i < substitution2.Length; ++i)
            {
                substitution2[i] = (uint)i;
            }

            Task tt = im.SearchIsomorph(1, substitution2);
            tt.Wait();
            uint count = im.GetSubstitutionsCount();
            string s = "";
            for (uint i = 0; i < count; i++)
            {
                uint[] a = im.GetSubstitutions(i);
                s += String.Join(",", a) + "\n"; ;

            }
            s = "IsomorphMatrices Test:\n" + s;
            MessageBox.Show(s);
        }
    }
}
