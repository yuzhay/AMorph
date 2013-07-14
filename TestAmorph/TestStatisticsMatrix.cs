using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AMorph;

namespace TestAmorph
{
    [TestClass]
    public class TestStatisticsMatrix
    {
        [TestMethod]
        public void TestStatistics()
        {
            byte[] img = new byte[]{
                255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    3,2,1,255,
                255,255,255,255,    255,255,255,255,    2,2,2,255,          255,255,255,255,    0,1,2,255,
                1,1,1,255,          255,255,255,255,    0,0,0,255,          255,255,255,255,    255,255,255,255,
                0,1,2,255,          255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,
                255,255,255,255,    5,5,5,5,            255,255,255,255,    255,255,255,255,    0,0,0,255
            };
            StatisticsMatrix sm = new StatisticsMatrix(5, 4, img);
            sm.Run();

            Dictionary<uint, uint> column0 = new Dictionary<uint, uint>();
            column0.Add(0xFFFFFFFF, 3); //4294967295
            column0.Add(0x010101FF, 1); //16843263
            column0.Add(0x000102FF, 1); //66303

            var result = sm.GetColumn(0);
            foreach (var a in result)
            {
                Assert.AreEqual(column0[a.Key],a.Value);
            }
        }
    }
}
