using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AMorph
{

    public class ImageTools
    {
        public static int GetBrightestPixel(byte[] image, int bytesPerPixel = 3)
        {
            int summ = 0;
            int max = 0;
            for (int i = 0; i < image.Length; i += bytesPerPixel)
            {
                summ = 0;
                for (int j = 0; j < bytesPerPixel; j++)
                {
                    summ += image[i + j];
                }
                if (summ > max)
                    max = summ;
            }
            return max;
        }

        public static int GetDarkestPixel(byte[] image,int bytesPerPixel = 3)
        {
            int summ = 0;
            int min = 0;
            for (int i = 0; i < image.Length; i += bytesPerPixel)
            {
                summ = 0;
                for (int j = 0; j < bytesPerPixel; j++)
                {
                    summ += image[i + j];
                }
                if (summ < min)
                    min = summ;
            }
            return min;
        }

        public static void GetStatistic(byte[] image, int bytesPerPixel = 3)
        {

        }
    }
}
