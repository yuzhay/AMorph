using System;
using System.Windows;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
namespace AMorph
{
    public class ImageConverter
    {
        #region ImageToBytes Conversions
        /// <summary>
        /// Возвращает одномерный массив байтов исходного изображения
        /// </summary>
        /// <returns>массив байтов исходного изображения</returns>
        static public byte[] ImageToByteArray(BitmapSource imgSrc)
        {
            byte[] pixels = new byte[imgSrc.PixelHeight * imgSrc.PixelWidth * imgSrc.Format.BitsPerPixel / 8];
            imgSrc.CopyPixels(pixels, imgSrc.PixelWidth * imgSrc.Format.BitsPerPixel / 8, 0);

            return pixels;
        }
        /// <summary>
        /// Возвращает WritableBitmap сформированный из массива байтов
        /// </summary>
        /// <param name="pixels"></param>
        /// <returns></returns>
        static public WriteableBitmap ByteArrayToWriteableBitmap(byte[] pixels, int pixelWidth, int pixelHeight, PixelFormat format)
        {
            WriteableBitmap wb = new WriteableBitmap(pixelWidth, pixelHeight, 96, 96, format, null);

            wb.Lock();
            Int32Rect rect = new Int32Rect(0, 0, pixelWidth, pixelHeight);
            int stride = pixelWidth * wb.Format.BitsPerPixel / 8;
            wb.WritePixels(rect, pixels, stride, 0);
            wb.Unlock();

            return wb;
        }
        #endregion
    }
}
