using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Media.Imaging;
using Microsoft.Research.DynamicDataDisplay.DataSources;
using System.Windows;
using System.Windows.Media;
using System.IO;
using System.Threading.Tasks;

namespace AMorph
{
    public class HadamardMtxModel: INotifyPropertyChanged
    {
        //===================Properties==========================
        #region Properties
        private SubstitutionCollection subsCollection;
        public SubstitutionCollection SubsCollection
        {
            get { return subsCollection; }
            set
            {
                subsCollection = value;
                OnPropertyChanged("SubsCollection");
            }
        }
        public ObservableDataSource<Point> values;
        public ObservableDataSource<Point> Values
        {
            get { return values; }
        }

        private HadamardMatrix mtx;
        public HadamardMatrix Mtx
        {
            get { return mtx; }
            set { mtx = value; }
        }

        private BitmapSource hadamardImage;
        public BitmapSource HadamardImage
        {
            get { return hadamardImage; }
            set
            {
                hadamardImage = value;
                OnPropertyChanged("HadamardImage");
            }
        }
        private String resultInfo;
        public String ResultInfo
        {
            get { return resultInfo; }
            set
            {
                resultInfo = value;
                OnPropertyChanged("ResultInfo");
            }
        }
        private string batchProcessDir;
        private List<HadamardMatrix> batchMatrixCollection;
        public List<HadamardMatrix> BatchMatrixCollection
        {
            get { return batchMatrixCollection; }
            set { batchMatrixCollection = value; }
        }
        public Dictionary<string, uint> BatchResults { get; set; }
        #endregion

        //=======================MainMethods==========================
        #region MainMethods
        public HadamardMtxModel()
        {
            values = new ObservableDataSource<Point>();
            subsCollection = new SubstitutionCollection();
            batchMatrixCollection = new List<HadamardMatrix>();
            BatchResults = new Dictionary<string, uint>();

            BitmapImage src = new BitmapImage();
            src.BeginInit();
            src.UriSource = new Uri(Directory.GetCurrentDirectory() + @"\img\no.png", UriKind.Absolute);
            src.EndInit();
            hadamardImage = src;
        }
        public void InitHadamardMatrix(uint sizeMtx, sbyte[] mtxVector)
        {
            mtx = new HadamardMatrix(sizeMtx, mtxVector);
        }

        #region PropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(String propertyName)
        {
            if (this.PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
        #endregion

        #endregion
        //====================Load Save====================
        #region SaveLoad
        public void SaveMatrix(String fileName)
        {
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(fileName))
            {
                for (int i = 0; i < Mtx.SizeMtx; i++)
                {
                    StringBuilder line = new StringBuilder();
                    for (int j = 0; j < Mtx.SizeMtx; j++)
                    {
                        line.Append((Mtx[i, j] > 0) ? "+" : "-");
                    }
                    file.WriteLine(line);
                }

                file.Close();
            }
        }
        public void SaveBatchResult(string dir)
        {
            dir = Path.GetDirectoryName(dir);
            dir += @"\Results.txt";
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(dir))
            {
                StringBuilder line = new StringBuilder();
                foreach (var item in BatchResults)
	            {
		            line.Append(item.Value + " " + item.Key + '\n');
	            }
                file.WriteLine(line);
                file.Close();
            }

        }
        public void LoadMatrix(String fileName)
        {
            sbyte [] vector = ParseMatrixTxt(fileName);
            uint sizeMtx = (uint)Math.Sqrt(vector.Length);
            InitHadamardMatrix((uint)sizeMtx, vector);
        }
        public sbyte[] ParseMatrixTxt(String fileName)
        {
            try
            {
                // Create an instance of StreamReader to read from a file.
                // The using statement also closes the StreamReader.
                using (StreamReader sr = new StreamReader(fileName))
                {
                    String line = "";
                    // Read and display lines from the file until the end of
                    // the file is reached.
                    while (!sr.EndOfStream)
                    {
                        line += sr.ReadLine();
                        Console.WriteLine(line);
                    }
                    sr.Close();
                    line = line.Trim();
                    line = line.Replace("\r", string.Empty).Replace("\n", string.Empty);
                    int sizeMtx = (int)Math.Sqrt(line.Length);
                    if (sizeMtx != 1 && sizeMtx != 2 && sizeMtx % 4 != 0)
                    {
                        throw new Exception("Неверный формат строки");
                    }
                    sbyte[] vector = new sbyte[line.Length];
                    switch (line[0])
                    {
                        case '1':
                        case '0':
                            for (int i = 0; i < line.Length; i++)
                            {
                                vector[i] = (line[i] == '1') ? ((sbyte)1) : ((sbyte)-1);
                            }
                            break;
                        case '+':
                        case '-':
                            for (int i = 0; i < line.Length; i++)
                            {
                                vector[i] = (line[i] == '+') ? ((sbyte)1) : ((sbyte)-1);
                            }
                            break;
                        default:
                            throw new Exception("Неверный формат строки");

                    }
                    return vector;
                    
                    //CurrentFile = fileName;

                }



            }
            catch (Exception e)
            {
                // Let the user know what went wrong.
                Console.WriteLine("The file could not be read:");
                Console.WriteLine(e.Message);
                return null;
            }
        }
        public void ParseAnyMatrixTxt(String fileName)
        {
            
        }
       
        public void LoadMatrices(String [] files)
        {
         
        }
        public sbyte[] ParseFineMatrixTxt(String fileName)
        {
            try
            {
                // Create an instance of StreamReader to read from a file.
                // The using statement also closes the StreamReader.
                using (StreamReader sr = new StreamReader(fileName))
                {
                    String line = "";
                    // Read and display lines from the file until the end of
                    // the file is reached.
                    while (!sr.EndOfStream)
                    {
                        line += sr.ReadLine();
                        Console.WriteLine(line);
                    }
                    sr.Close();
                    line = line.Trim();
                    line = line.Replace("\r", string.Empty).Replace("\n", string.Empty);
                    int sizeMtx = (int)Math.Sqrt(line.Length);
                    if (sizeMtx != 1 && sizeMtx != 2 && sizeMtx % 4 != 0)
                    {
                        throw new Exception("Неверный формат строки");
                    }
                    sbyte[] vector = new sbyte[line.Length];
                    for (int i = 0; i < line.Length; i++)
                    {
                        vector[i] = (line[i] == '+') ? ((sbyte)1) : ((sbyte)-1);
                    }
                    return vector;

                }



            }
            catch (Exception e)
            {
                // Let the user know what went wrong.
                Console.WriteLine("The file could not be read:");
                Console.WriteLine(e.Message);
                return null;
            }
        }
        #endregion
        //====================Graphics=====================
        #region Draw
        public void ReDrawMatrixImage()
        {
            int length = 200;
            int sizeMtx = (int)mtx.SizeMtx;
            int dx = length / sizeMtx;
            var wb = new WriteableBitmap(dx * sizeMtx, dx * sizeMtx, 96, 96,
                              PixelFormats.Pbgra32, null);
            wb.Lock();
            uint[] pixels = new uint[dx * dx];
            for (int y = 0; y < sizeMtx; y++)
            {

                for (int x = 0; x < sizeMtx; x++)
                {
                    byte[] components = new byte[4];

                    int value = mtx[x, y] + 255;
                    components[0] = (byte)(value % 255);/// blue
                    components[1] = (byte)(value % 255);/// green
                    components[2] = (byte)(value % 255);/// red
                    components[3] = (byte)255;

                    uint pixel = BitConverter.ToUInt32(components, 0);
                    for (int k = 0; k < dx * dx; ++k)
                    {
                        pixels[k] = pixel;
                    }


                    Int32Rect rect = new Int32Rect(x * dx, y * dx, dx, dx);
                    int stride = dx * wb.Format.BitsPerPixel / 8;

                    wb.WritePixels(rect, pixels, stride, 0);
                    //count++;//test
                }
            }

            wb.Unlock();
            HadamardImage = wb;

        }
        public void RenderGrapth()
        {
            Values.Collection.Clear();
            int count = 1;
            foreach (var elem in SubsCollection)
            {
                Values.Collection.Add(new Point(count++, elem.Partitions));
            }
        }
        public void RenderSecondGrapth()
        {
            Values.Collection.Clear();
            int count = 1;
            foreach (var elem in SubsCollection.IterationStats)
            {
                Values.Collection.Add(new Point(count++, elem));
            }
        }
        #endregion
        //====================Logic=====================
        #region Logic
        public void BatchProcess(string dir)
        {
            batchProcessDir = dir;
            string[] filePaths = Directory.GetFiles(dir);
            
            foreach (string file in filePaths)
            {
                sbyte[] vector = ParseFineMatrixTxt(file);
                //uint sizeMtx = (uint)Math.Sqrt(vector.Length);
                SearchAutomorphism(vector, Path.GetFileName(file), filePaths.Length);
            }
           


        }
        public void SearchAutomorphism()
        {
            SubsCollection.Clear();
            SubsCollection.IterationStats.Clear();

            uint[] t1substitution = new uint[Mtx.SizeMtx];
            for (int i = 0; i < t1substitution.Length; ++i)
            {
                t1substitution[i] = (uint)i;
            }
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            Task t = mtx.SearchAutomorph(1, t1substitution);
            t.ContinueWith((state) =>
            {
                uint count = mtx.GetSubstitutionsCount();
                string s = "";
                for (uint i = 0; i < count; i++)
                {
                    Substitution sub = new Substitution((int)mtx.SizeMtx);
                    sub.Position = mtx.GetSubstitutions(i);
                    sub.Partitions = (int)mtx.GetSubstitutionsParts(i);
                    sub.Sign = mtx.GetSigns(i);
                    subsCollection.Add(sub);

                }
                uint cnt = 0;
                foreach (var elem in subsCollection)
                {
                    cnt += (uint)elem.Partitions;
                }
                uint[] a = mtx.GetSubstitutions(0);
                for (uint i = 0; i < Mtx.SizeMtx; i++)
                {
                    SubsCollection.IterationStats.Add(Mtx.GetIterationDepthValue(i));
                }
                s += String.Join(",", a) + "\n";
                s = "AutomorphMatrix Test:"
                    + GetInfo()
                    + "\nВсего найдено: " + count
                    + "\nВсего перебрано: " + cnt;
                   // + "\nПервый Вектор: " + s;
                MessageBox.Show(s);
                ResultInfo = s;
                
            }, ts);
        }
        public void SearchAutomorphism(sbyte [] vector, string file, int numOfFiles)
        {
            uint size = (uint)Math.Sqrt(vector.Length);
            HadamardMatrix hMtx = new HadamardMatrix(size, vector);

            uint[] t1substitution = new uint[size];
            for (int i = 0; i < t1substitution.Length; ++i)
            {
                t1substitution[i] = (uint)i;
            }
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            Task t = hMtx.SearchAutomorph(1, t1substitution);
            t.ContinueWith((state) =>
            {
                uint count = hMtx.GetSubstitutionsCount();
                BatchResults.Add(file, count);
                string s = "";
                s = "AutomorphMatrix Test:"
                     + "\nВсего найдено: " + count;
                //MessageBox.Show(s);
                ResultInfo = s;
                if (BatchResults.Count == numOfFiles)
                {
                    SaveBatchResult(batchProcessDir);
                    MessageBox.Show("BatchProcess Finish");
                }
            }, ts);

        }
        public void GenerateExtMatrix()
        {
            SubsCollection.Clear();

            int size = (int)Mtx.SizeMtx * 2;

            sbyte[] vector = new sbyte[size * size];

            for (int i = 0; i < (int)Mtx.SizeMtx; i++)
            {
                vector[i + i * size] = 2;
            }
            for (int i = (int)Mtx.SizeMtx; i < size; i++)
            {
                for (int j = 0; j < (int)Mtx.SizeMtx; j++)
                {
                    vector[i + j * size] = Mtx[i - (int)Mtx.SizeMtx, j];
                }
                
            }

            Mtx = new HadamardMatrix((uint)size, vector);
        }
        #endregion
        //====================Information=====================
        #region Information
        public String GetInfo()
        {
            int plusElemes = 0;
            for (int i = 0; i < Mtx.SizeMtx; i++ )
            {
                if (Mtx[i, i] == 1) plusElemes++;
            }
            return "\nРазмер матрицы: " + Mtx.SizeMtx.ToString()  
                   + "\nНа диагонали" 
                   + "\n    Плюсов: " + plusElemes.ToString()
                   + "\n    Минусов: " + (Mtx.SizeMtx - plusElemes).ToString();

        }

        #endregion

    }
}
