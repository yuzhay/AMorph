using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Microsoft.Research.DynamicDataDisplay;
using Microsoft.Research.DynamicDataDisplay.DataSources;
using Microsoft.Win32;
using QuickGraph;
using Microsoft.Research.DynamicDataDisplay.PointMarkers;
using System.Diagnostics;



namespace AMorph
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private ObservableCollection<ChartPoint> chartData;
        public ObservableCollection<ChartPoint> ChartData
        {
            get
            {
                int b;
                return chartData;
            }
        }
        private ObservableDataSource<Point> chartValues = new ObservableDataSource<Point>();
        public ObservableDataSource<Point> ChartValues
        {
            get { return chartValues; }
        }

        private uint[] substitution;
        public uint[] Substitution
        {
            get { return substitution; }
            set 
            { 
                substitution = value;
                OnPropertyChanged("Substitution");
            }
        }

        private byte[] gamma;
        public byte[] Gamma
        {
            get { return gamma; }
            set
            {
                gamma = value;
                OnPropertyChanged("Gamma");
            }
        }

        public int ImagePixelWidth { get; set; }

        private ImageSource srcImageSource ;
        public ImageSource SrcImageSource
        {
            get{
                if (srcImageSource == null)
                {
                    BitmapImage src = new BitmapImage();
                    src.BeginInit();
                    src.UriSource = new Uri(Directory.GetCurrentDirectory()+ @"\img\no.png", UriKind.Absolute);
                    src.EndInit();
                    this.srcImageSource = src;
                }
                return srcImageSource;
            }
            set
            {
                this.srcImageSource = value;
                srcImageLoaded = true;
                OnPropertyChanged("SrcImageSource");
            }
        }

        private ImageSource dstImageSource;
        public ImageSource DstImageSource
        {
            get
            {
                if (dstImageSource == null)
                {
                    BitmapImage dst = new BitmapImage();
                    dst.BeginInit();
                    dst.UriSource = new Uri(Directory.GetCurrentDirectory() + @"\img\no.png", UriKind.Absolute);
                    dst.EndInit();
                    this.dstImageSource = dst;
                }
                return dstImageSource;
            }
            set
            {
                this.dstImageSource = value;
                dstImageLoaded = true;
                OnPropertyChanged("DstImageSource");
            }
        }

        private ObservableCollection<ImageSource> imagesListBox = new ObservableCollection<ImageSource>();
        public ObservableCollection<ImageSource> ImagesListBox 
        { 
            get
            {
                return this.imagesListBox;
            }
            set
            {
                this.imagesListBox = value;
                OnPropertyChanged("ImageListBox");
            }
        }


        private float progress = 0;
        public float Progress
        {
            get
            {
                return this.progress;
            }
            set
            {
                this.progress = value;
                OnPropertyChanged("Progress");
            }
        }

        private bool srcImageLoaded = false;
        private bool dstImageLoaded = false;

        private IBidirectionalGraph<object, TaggedEdge<object, object>> _graphToVisualize;
        public IBidirectionalGraph<object, TaggedEdge<object, object>> GraphToVisualize
        {
            get
            {
                return _graphToVisualize;
            }
            set
            {
                this._graphToVisualize = value;
                //OnPropertyChanged("GraphToVisualize");
            }
        }

        public void drawGraph()
        {

            BitmapImage bi = SrcImageSource as BitmapImage;

            byte[] pixels = new byte[bi.PixelHeight * bi.PixelWidth * bi.Format.BitsPerPixel / 8];
            bi.CopyPixels(pixels, bi.PixelWidth * bi.Format.BitsPerPixel / 8, 0);



            //this.state = new ObservableCollection<State>(ad.States);
            //this.edges = new ObservableCollection<Edge>(ad.Edges);

            Dictionary<object, object> v = new Dictionary<object, object>();
            var g = new BidirectionalGraph<object, TaggedEdge<object, object>>(true);
            for (uint i = 0; i < bi.PixelHeight; i++)
            {
                v.Add(i, i);

                g.AddVertex(i);
            }
            //g.AddEdge(new TaggedEdge<object, object>(v[1], v[0], "aaaa"));
            //g.AddEdge(new TaggedEdge<object, object>(v[3], v[5], "bbb"));

            GraphToVisualize = g;

        }

        public MainWindow()
        {
            InitializeComponent();



            hadMtxModel = new HadamardMtxModel();
            xHadamardTab.DataContext = hadMtxModel;

            gModel = new GraphModel();
            xGraphTab.DataContext = gModel;

            DataContext = this;
            logTextBox.Text = ">Начало рабочей сессии\n";
        }

        private void test3()
        {
            ////IsoMorph

            byte[] matrixOrg = ImageSourceToByteArray();
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

        #region menu


        private void MenuFileLoadMatrix(object sender, RoutedEventArgs e)
        {
            //OpenFileDialog ofd = new OpenFileDialog();
            //ofd.Multiselect = false;
            //ofd.Filter = "Matrix (*.mtx)|*.mtx";
            //if (ofd.ShowDialog() == true)
            //{
            //    try
            //    {
            //        FileStream fs = File.Open(ofd.FileName, FileMode.Open);
            //        BinaryFormatter bf = new BinaryFormatter();
            //        srcMatrix = (byte[,])bf.Deserialize(fs);
            //        //System.Xml.Serialization.XmlSerializer x = new System.Xml.Serialization.XmlSerializer(typeof(string[][]));
            //        //string2dArray = (string[][])x.Deserialize(fs);
            //        fs.Close();
            //        //statusBar1.Text = "Matrix loaded successful";
            //    }
            //    catch (InvalidOperationException ex)
            //    {
            //        //statusBar1.Text = ex.Message;
            //    }
            //}
            
        }

        private void MenuFileSaveMatrix(object sender, RoutedEventArgs e)
        {
            //SaveFileDialog sfd = new SaveFileDialog();
            //sfd.Filter = "Matrix (*.mtx)|*.mtx";
            //sfd.DefaultExt = ".mtx";
            //if (sfd.ShowDialog() == true)
            //{
            //    try
            //    {
            //        FileStream fs = File.Open(sfd.FileName, FileMode.Create);
            //        BinaryFormatter bf = new BinaryFormatter();
            //        bf.Serialize(fs, srcMatrix);
            //        //System.Xml.Serialization.XmlSerializer x = new System.Xml.Serialization.XmlSerializer(typeof(string[][]));
            //        //x.Serialize(fs, string2dArray);
            //        fs.Close();
            //        //statusBar1.Text = "Matrix saved successful";
            //    }
            //    catch (InvalidOperationException ex)
            //    {
            //        //statusBar1.Text = ex.Message;
            //    }
            //}

        }

        private void MenuControlPermute(object sender, RoutedEventArgs e)
        {

        }


        #region Commands
        private void CommandTranspositionLeft(object sender, ExecutedRoutedEventArgs e)
        {
            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);

            BasicMatrix bm = new BasicMatrix(mtxSize, 4, matrixOrg);
            logTextBox.Text += "Запуск левой транспозиции\n";
            Task t = bm.TranspositionLeft(Substitution);
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            t.ContinueWith((state) =>
                {
                    byte[] matrixMod = bm.GetMatrix();
                    DstImageSource = ImageConverter.ByteArrayToWriteableBitmap(matrixMod,
                                                                                ((BitmapSource)SrcImageSource).PixelWidth,
                                                                                ((BitmapSource)SrcImageSource).PixelHeight,
                                                                                ((BitmapSource)SrcImageSource).Format
                                                                                );
                    logTextBox.Text += "Левая транспозиция закончена\n";
                },ts
            );

        }

        private void CommandTranspositionRight(object sender, ExecutedRoutedEventArgs e)
        {
            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);
            BasicMatrix bm = new BasicMatrix(mtxSize, 4, matrixOrg);
            logTextBox.Text += "Запуск правой транспозиции\n";
            Task t = bm.TranspositionRight(Substitution);
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            t.ContinueWith((state) =>
            {
                byte[] matrixMod = bm.GetMatrix();
                DstImageSource = ImageConverter.ByteArrayToWriteableBitmap(matrixMod,
                                                                            ((BitmapSource)SrcImageSource).PixelWidth,
                                                                            ((BitmapSource)SrcImageSource).PixelHeight,
                                                                            ((BitmapSource)SrcImageSource).Format
                                                                            );
                logTextBox.Text += "Правая транспозиция закончена\n";
            }, ts);

        }

        private void CommandTranspositionFull(object sender, ExecutedRoutedEventArgs e)
        {
            Busy.IsBusy = true;
            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);

            BasicMatrix bm = new BasicMatrix(mtxSize, 4, matrixOrg);
            Task t = bm.Transposition(Substitution);

            logTextBox.Text += "Запуск полной транспозиции\n";
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            t.ContinueWith((state) =>
                {
                    Busy.IsBusy = false;
                    byte[] matrixMod = bm.GetMatrix();
                    DstImageSource = ImageConverter.ByteArrayToWriteableBitmap(matrixMod,
                                                                                ((BitmapSource)SrcImageSource).PixelWidth,
                                                                                ((BitmapSource)SrcImageSource).PixelHeight,
                                                                                ((BitmapSource)SrcImageSource).Format
                                                                                );
                    logTextBox.Text += "Полная транспозиция закончена\n";
                },ts);

            
        }

        private void CommandAutomorphismSearch(object sender, ExecutedRoutedEventArgs e)
        {
            logTextBox.Text += "> Поиск автоморфизмов по методу расширяющегося подграфа\n";

            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);
            AutomorphMatrix am = new AutomorphMatrix(mtxSize, 4, matrixOrg);
            uint[] t1substitution = new uint[mtxSize];
            for (int i = 0; i < t1substitution.Length; ++i)
            {
                t1substitution[i] = (uint)i;
            }
            Task t = am.SearchAutomorph(1, t1substitution);
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            t.ContinueWith((state) =>
            {
                uint[] a;
                uint count = am.GetSubstitutionsCount();
                logTextBox.Text += "> Общее количество решений: " + count + "\n";
                logTextBox.Text += "> Решения: \n";
                for (uint i = 0; i < count; i++)
                {
                    a = am.GetSubstitutions(i);
                    logTextBox.Text += ("> " + String.Join(",", a) + "\n");
                }

                MessageBox.Show("Решение не найдено");
            }, ts);


        }

        private void CommandIsomorphismSearch(object sender, ExecutedRoutedEventArgs e)
        {
            ////IsoMorph
            logTextBox.Text += "> Поиск изоморфизамов по методу расширяющегося подграфа\n";

            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
            byte[] matrixMod = ImageConverter.ImageToByteArray((BitmapSource)DstImageSource); //ImageDestinationToByteArray();


            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);


            IsomorphMatrices im = new IsomorphMatrices(mtxSize, 4, matrixOrg, matrixMod);
            uint[] substitution2 = new uint[mtxSize];
            for (int i = 0; i < substitution2.Length; ++i)
            {
                substitution2[i] = (uint)i;
            }


            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            Stopwatch stop = new Stopwatch();
            stop.Start();
            Task t = im.SearchIsomorph(1, substitution2);

           

            uint elem = 0;
            t.ContinueWith((state) =>
            {
                int c = 0;
                if (im.GetSubstitutionsCount() > 0)
                {
                    uint[] a;
                    uint count = im.GetSubstitutionsCount();
                    //now = DateTime.Now.Ticks - now;
                    logTextBox.Text += "> Общее количество решений: " + count + "\n";
                    logTextBox.Text += "> Решения: \n";

                    chartValues.Collection.Clear();
                    for (uint i = 0; i < count; i++)
                    {
                        a = im.GetSubstitutions(i);
                        elem = im.GetParts(i);
                        logTextBox.Text += ("> " + String.Join(",", a) + "\n");
                        chartValues.Collection.Add(new Point(c++, elem));
                    }

                    processPlotter.RemoveUserElements();
                    processPlotter.RightPanel.Children.Clear();
                    processPlotter.AddLineGraph(chartValues, 1, "переборы");

                    ;
                    MessageBox.Show("Найдено решение\n" + stop.ElapsedMilliseconds + " миллесекунд");
                }
                else
                {
                    logTextBox.Text += "Решение не найдено\n";
                    MessageBox.Show("Решение не найдено");
                }
            }, ts);
        }

        private void CommandIsomorphismStatisticsSearch(object sender, ExecutedRoutedEventArgs e)
        {
            ////IsoMorph
            logTextBox.Text += "> Поиск изоморфизамов по статистическому методу\n";

            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
            byte[] matrixMod = ImageConverter.ImageToByteArray((BitmapSource)DstImageSource); //ImageDestinationToByteArray();


            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);


            IsomorphMatrices im = new IsomorphMatrices(mtxSize, 4, matrixOrg, matrixMod);
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            Stopwatch stop = new Stopwatch();
            stop.Start();

            Task t = im.SearchStatisticsIsomorph();
            //Task p = new Task((state) =>
            //{

            //},ts);
            //p.Start();
            
            t.ContinueWith((state) =>
            {
                if (im.GetSubstitutionsCount() > 0)
                {
                    uint[] a;
                    uint count = im.GetSubstitutionsCount();
                    logTextBox.Text += "> Общее количество решений: " + count + "\n";
                    logTextBox.Text += "> Решениея: \n";
                    for (uint i = 0; i < count; i++)
                    {
                        a = im.GetSubstitutions(i);
                        logTextBox.Text += ("> " + String.Join(",", a) + "\n");
                    }

                    MessageBox.Show("Решение найдено\n"+stop.ElapsedMilliseconds + " миллесекунд");
                }
                else
                {
                    MessageBox.Show("Решений не найдено");
                }
            }, ts);
        }

        private void CommandIsomorphismStatisticsSearchColumns(object sender, ExecutedRoutedEventArgs e)
        {
            ////IsoMorph
            logTextBox.Text += "> Поиск изоморфизамов по статистическому методу колонок\n";

            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
            byte[] matrixMod = ImageConverter.ImageToByteArray((BitmapSource)DstImageSource); //ImageDestinationToByteArray();


            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);


            IsomorphMatrices im = new IsomorphMatrices(mtxSize, 4, matrixOrg, matrixMod);
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            Task t = im.SearchStatisticsIsomorph(1);
            Task p = new Task((state) =>
            {

            }, ts);
            p.Start();

            t.ContinueWith((state) =>
            {
                if (im.GetSubstitutionsCount() > 0)
                {
                    uint[] a;
                    uint count = im.GetSubstitutionsCount();
                    logTextBox.Text += "> Общее количество решений: " + count + "\n";
                    logTextBox.Text += "> Решения: \n";
                    for (uint i = 0; i < count; i++)
                    {
                        a = im.GetSubstitutions(i);
                        logTextBox.Text += ("> " + String.Join(",", a) + "\n");
                    }

                    MessageBox.Show("Решение найдено");
                }
                else
                {
                    MessageBox.Show("Решений не найдено");
                }
            }, ts);
        }

        private void CommandIsomorphismStatisticsSearchRows(object sender, ExecutedRoutedEventArgs e)
        {
            ////IsoMorph
            logTextBox.Text += ">  Поиск изоморфизамов по статистическому методу строк\n";

            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
            byte[] matrixMod = ImageConverter.ImageToByteArray((BitmapSource)DstImageSource); //ImageDestinationToByteArray();


            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);


            IsomorphMatrices im = new IsomorphMatrices(mtxSize, 4, matrixOrg, matrixMod);
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            Task t = im.SearchStatisticsIsomorph(2);
            Task p = new Task((state) =>
            {

            }, ts);
            p.Start();

            t.ContinueWith((state) =>
            {
                if (im.GetSubstitutionsCount() > 0)
                {
                    uint[] a;
                    uint count = im.GetSubstitutionsCount();
                    logTextBox.Text += "> Общее количество решений: " + count + "\n";
                    logTextBox.Text += "> Решения: \n";
                    for (uint i = 0; i < count; i++)
                    {
                        a = im.GetSubstitutions(i);
                        logTextBox.Text += ("> " + String.Join(",", a) + "\n");
                    }

                    MessageBox.Show("Решение найдено");
                }
                else
                {
                    MessageBox.Show("Решений не найдено");
                }
            }, ts);
        }


        private void SrcImageLoadedCanExecute(object sender, System.Windows.Input.CanExecuteRoutedEventArgs e)
        {
            if (srcImageLoaded)
            {
                e.CanExecute = true;
            }
            else
            {
                e.CanExecute = false;
            }
        }

        private void SrcDstImageLoadedCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            if (srcImageLoaded && dstImageLoaded)
            {
                e.CanExecute = true;
            }
            else
            {
                e.CanExecute = false;
            }
        }
        
        private void CommandClose(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void CommandShowSubstitution(object sender, ExecutedRoutedEventArgs e)
        {
            logTextBox.Text += (">" + String.Join(",", Substitution) + "\n");
        }

        private void CommandImposeGamma(object sender, ExecutedRoutedEventArgs e)
        {
                Busy.IsBusy = true;
                System.Threading.Thread.Sleep(50);

                byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageSourceToByteArray();
                logTextBox.Text += "Запуск наложения гаммы\n";
                uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);
                BasicMatrix bm = new BasicMatrix(mtxSize, 4, matrixOrg);
                bm.ImposeGamma(Gamma);
                byte[] matrixMod = bm.GetMatrix();
                DstImageSource = ImageConverter.ByteArrayToWriteableBitmap(matrixMod,
                                                                            ((BitmapSource)SrcImageSource).PixelWidth,
                                                                            ((BitmapSource)SrcImageSource).PixelHeight,
                                                                            ((BitmapSource)SrcImageSource).Format
                                                                            );

                logTextBox.Text += "Наложение гаммы закончено\n";
                Busy.IsBusy = false;
            //set the IsBusy before you start the thread

        }

        private void CommandShowGamma(object sender, ExecutedRoutedEventArgs e)
        {
            logTextBox.Text += ("> Гамма = " + String.Join(",", Gamma) + "\n");
        }

        private void CommandGenerateGamma(object sender, ExecutedRoutedEventArgs e)
        {

        }

        private void CommandGenerateSubstitution(object sender, ExecutedRoutedEventArgs e)
        {

        }
        #endregion



        #endregion


        public int[,] ArrayToMatrix(int[] arr)
        {
            uint length = (uint)Math.Sqrt(arr.Length);
            int[,] tmp = new int[length,length];
            for(int i=0;i<length;i++)
            {
                for(int j=0;j<length;j++)
                {
                    tmp[i,j] = arr[i*length+j];
                }
            }
            return tmp;
        }



        #region Images load&save buttons
        private void Button_Load_Dst_Image_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = false;
            ofd.Filter = "Image (*.bmp)|*.bmp";
            if (ofd.ShowDialog() == true)
            {
                try
                {
                    BitmapImage dst = new BitmapImage();
                    dst.BeginInit();
                    dst.UriSource = new Uri(ofd.FileName, UriKind.Absolute);
                    dst.EndInit();
                    
                    //Проверка загружаемого изображения
                    bool isEqualHeightAndWidth = (dst.PixelWidth == dst.PixelHeight);
                    bool isEqualSrcAndDstWidth = (((BitmapSource)SrcImageSource).PixelWidth == dst.PixelWidth);

                    if (isEqualHeightAndWidth && isEqualSrcAndDstWidth)
                    {
                        this.DstImageSource = dst;
                        //statusBar1.Text = "Destination image loaded";
                    }
                    else
                    {
                        String errMsg = "";
                        if (!isEqualHeightAndWidth)
                        {
                            errMsg += "Загружаемое изображение должно иметь одинаковые высоту и ширину;\n";
                        }
                        if (!isEqualSrcAndDstWidth)
                        {
                            errMsg += "Загружаемое изображение должно совпадать размерами с исходным;\n";
                        }

	                   MessageBox.Show(errMsg);
                    }
                }
                catch (InvalidOperationException ex)
                {
                    //statusBar1.Text = ex.Message;
                }
            }
        }
        private void Button_Load_Src_Image_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = false;
            ofd.Filter = "Image (*.bmp)|*.bmp";
            if (ofd.ShowDialog() == true)
            {
                try
                {
                    BitmapImage src = new BitmapImage();
                    src.BeginInit();
                    src.UriSource = new Uri(ofd.FileName, UriKind.Absolute);
                    src.EndInit();

                    //Проверка загружаемого изображения
                    bool isEqualHeightAndWidth = (src.PixelWidth == src.PixelHeight);
                    if (isEqualHeightAndWidth)
                    {
                        this.SrcImageSource = src;

                        //statusBar1.Text = "Source image loaded";
                        ///
                        ImagePixelWidth = src.PixelWidth;
                        Substitution = new uint[src.PixelWidth];
                        Gamma = new byte[(int)GammaSize.Value * (src.Format.BitsPerPixel / 8)];
                        for (int i = 0; i < Substitution.Length; i++)
                        {
                            Substitution[i] = (uint)i;
                        }
                        DrawGradientLine(Substitution, SubsImage);
                        Gamma = GammaShuffle(gamma);
                        DrawGammaLine(Gamma, GammaImage);
                        srcImageLoaded = true;

                        drawGraph();
                    }
                    else
                    {
                        String errMsg = "";
                        if (!isEqualHeightAndWidth)
                        {
                            errMsg += "Изображение должно иметь одинаковые высоту и ширину\n";
                        }
                        MessageBox.Show(errMsg);
                    }

                }
                catch (InvalidOperationException ex)
                {
                    //statusBar1.Text = ex.Message;
                }
            }
        }
        private void Button_Save_Src_Image_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "Image (*.bmp)|*.bmp";
            sfd.DefaultExt = ".bmp";
            if (sfd.ShowDialog() == true)
            {
                try
                {
                    FileStream fs = File.Open(sfd.FileName, FileMode.Create);
                    BmpBitmapEncoder encoder = new BmpBitmapEncoder();
                    //encoder.Palette = ((BitmapSource)SrcImageSource).Palette;
                    encoder.Frames.Add(BitmapFrame.Create((BitmapSource)SrcImageSource));
                    encoder.Save(fs);
                    fs.Close();
                    //statusBar1.Text = "Source image saved";
                    
                }
                catch (InvalidOperationException ex)
                {
                    //statusBar1.Text = ex.Message;
                }
            }
        }
        private void Button_Save_Dst_Image_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "Image (*.bmp)|*.bmp";
            sfd.DefaultExt = ".bmp";
            if (sfd.ShowDialog() == true)
            {
                try
                {
                    FileStream fs = File.Open(sfd.FileName, FileMode.Create);
                    BmpBitmapEncoder encoder = new BmpBitmapEncoder();
                   // encoder.Palette = ((BitmapSource)SrcImageSource).Palette;
                    encoder.Frames.Add(BitmapFrame.Create((BitmapSource)DstImageSource));
                    encoder.Save(fs);
                    fs.Close();
                    //statusBar1.Text = "Destination image saved";

                }
                catch (InvalidOperationException ex)
                {
                    ///statusBar1.Text = ex.Message;
                }
            }
        }
        private void Button_Load_Images_ListBox_Click(object sender, RoutedEventArgs e)
        {
            int imageWidth = 0;
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = true;
            ofd.Filter = "Image (*.bmp)|*.bmp";
            if (ofd.ShowDialog() == true)
            {
                try
                {
                    String errMsg = "";

                    foreach (string f in ofd.FileNames)
                    {
                        BitmapImage img = new BitmapImage();
                        img.BeginInit();
                        img.UriSource = new Uri(f, UriKind.Absolute);
                        img.EndInit();

                        bool isEqualHeightAndWidth = (img.PixelWidth == img.PixelHeight);
                        bool isEqualImagesSize = (imageWidth == 0) ? (true) : (imageWidth == img.PixelWidth);
                        if (isEqualHeightAndWidth && isEqualImagesSize)
                        {
                            ImagesListBox.Add(img);
                            if (ImagesListBox.Count == 1)
                            {
                                imageWidth = img.PixelWidth;
                                Substitution = new uint[img.PixelWidth];
                                for (int i = 0; i < Substitution.Length; i++)
                                {
                                    Substitution[i] = (uint)i;
                                }
                                DrawGradientLine(Substitution, SubsImage);
                            }
                        }
                        else
                        {
                            errMsg += "Изображение: " + f + "\n";
                            if (!isEqualHeightAndWidth)
                            {
                                errMsg += "-должно иметь одинаковые высоту и ширину\n";
                            }
                            if (!isEqualImagesSize)
                            {
                                errMsg += "-должно иметь размер как и у других изображений\n";
                            }
                        }
                    }

                    if (errMsg != "")
                    {
                        MessageBox.Show(errMsg);
                    }
                }
                catch (InvalidOperationException ex)
                {
                    //statusBar1.Text = ex.Message;
                }
            }
        }
        private void Button_Save_Images_ListBox_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "Image (*.bmp)|*.bmp";
            sfd.DefaultExt = ".bmp";
            if (sfd.ShowDialog() == true)
            {
                try
                {
                    int cnt = 0;
                    foreach (var elem in imagesListBox)
                    {

                        int index = sfd.FileName.IndexOf(sfd.SafeFileName);
                        String tmp = sfd.FileName.Insert(index, cnt + "_");


                        FileStream fs = File.Open(tmp, FileMode.Create);
                        BmpBitmapEncoder encoder = new BmpBitmapEncoder();
                        // encoder.Palette = ((BitmapSource)SrcImageSource).Palette;
                        encoder.Frames.Add(BitmapFrame.Create((BitmapSource)elem));
                        encoder.Save(fs);
                        fs.Close();
                        cnt++;
                    }

                    //statusBar1.Text = "Series image saved";

                }
                catch (InvalidOperationException ex)
                {
                    //statusBar1.Text = ex.Message;
                }
            }
        }
        #endregion

        protected void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        public event PropertyChangedEventHandler PropertyChanged;

        #region ImageToBytes Conversions
        /// <summary>
        /// Возвращает одномерный массив байтов исходного изображения
        /// </summary>
        /// <returns>массив байтов исходного изображения</returns>
        public byte[] ImageSourceToByteArray()
        {
            BitmapImage bi = SrcImageSource as BitmapImage;
            WriteableBitmap wb = new WriteableBitmap(bi);

            byte[] pixels = new byte[wb.PixelHeight * wb.PixelWidth * wb.Format.BitsPerPixel / 8];
            wb.CopyPixels(pixels, wb.PixelWidth * wb.Format.BitsPerPixel / 8, 0);

            return pixels;
        }
        /// <summary>
        /// Возвращает одномерный массив байтов исходного изображения
        /// </summary>
        /// <returns>массив байтов исходного изображения</returns>
        private byte[] ImageDestinationToByteArray()
        {

            BitmapImage bi = DstImageSource as BitmapImage;
            WriteableBitmap wb;
            if (bi != null)
            {
                wb = new WriteableBitmap(bi);
            }
            else
            {
                wb = DstImageSource as WriteableBitmap;
            }
            byte[] pixels = new byte[wb.PixelHeight * wb.PixelWidth * wb.Format.BitsPerPixel / 8];
            wb.CopyPixels(pixels, wb.PixelWidth * wb.Format.BitsPerPixel / 8, 0);

            return pixels;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="pixels"></param>
        /// <returns></returns>
        private WriteableBitmap ByteArrayToWriteableBitmap(byte[] pixels)
        {
            //TODO
            BitmapImage bi = SrcImageSource as BitmapImage;
            WriteableBitmap wbtmp = new WriteableBitmap(bi);
            ///////
            WriteableBitmap wb = new WriteableBitmap(wbtmp.PixelWidth,wbtmp.PixelHeight,wbtmp.Width, wbtmp.Height,wbtmp.Format,null);

            wb.Lock();
            Int32Rect rect = new Int32Rect(0, 0, wbtmp.PixelWidth, wbtmp.PixelHeight);
            int stride = wbtmp.PixelWidth * wb.Format.BitsPerPixel / 8;
            wb.WritePixels(rect, pixels, stride, 0);
            wb.Unlock();

            return wb;
        }
        #endregion
        #region Substitutions Func

        /// <summary>
        /// Перемешать перестановку случайным образом
        /// </summary>
        /// <param name="subs">вектор перетановки</param>
        /// <returns>вектор перестановки</returns>
        private uint[] VectorShuffle(uint[] subs)
        {
            Random rand = new Random();
            List<uint> list = new List<uint>();

            for (int i = 0; i < subs.Length; i++)
            {
                list.Add(subs[i]);                    
            }
            for (int i = 0; i < list.Count; i++)
            {
                uint tmp = list[i];
                list.RemoveAt(i);
                list.Insert(rand.Next(0, list.Count), tmp);
            }
            return list.ToArray();

        }

        private byte[] GammaShuffle(byte[] gamma)
        {
            Random rand = new Random((int)DateTime.Now.Ticks);
            for (int i = 0; i < gamma.Length; i++)
            {
                gamma[i] = (byte)(rand.Next(0, 255));

            }
            return gamma;
        }

        private void ShuffleSubs_Click(object sender, RoutedEventArgs e)
        {
            if (Substitution != null)
            {
                Substitution = VectorShuffle(Substitution);
            }
        }


        private void DrawGradientLine(uint[] vector, Image img)
        {
 
            int width = (SrcImageSource as BitmapSource).PixelWidth;//stub
            int height = (int)25;
            int dx = 1;
            int dy = height;
            var wb = new WriteableBitmap(width, height, 96, 96,PixelFormats.Pbgra32, null);

            wb.Lock();
            uint pixel = 0;
            uint[] pixels = new uint[dx * dy];
            for (int x = 0; x < width; x++)
            {
                //byte[] components = new byte[4];

                //components[0] = (byte)(Substitution[x] % 255);/// blue
                //components[1] = (byte)(Substitution[x] % 255);/// green
                //components[2] = (byte)(Substitution[x] % 255);/// red
                //components[3] = (byte)255;

                //uint pixel = BitConverter.ToUInt32(components, 0);
                pixel = vector[x];
                for (int k = 0; k < dy; ++k)
                {
                    pixels[k] = pixel;
                }


                Int32Rect rect = new Int32Rect(x , 0, dx, dy);
                int stride = wb.Format.BitsPerPixel / 8;

                wb.WritePixels(rect, pixels, stride, 0);
                    
            }
           

            wb.Unlock();
            img.Source = wb;

        }


        private void DrawGammaLine(byte[] vector, Image img)
        {

            int width = vector.Length;
            int height = (int)25;
            int dx = 1;
            int dy = height;
            var wb = new WriteableBitmap(width, height, 96, 96, PixelFormats.Pbgra32, null);

            wb.Lock();
            uint pixel = 0;
            uint[] pixels = new uint[dx * dy];
            for (int x = 0; x < width; x++)
            {
                //byte[] components = new byte[4];

                //components[0] = (byte)(Substitution[x] % 255);/// blue
                //components[1] = (byte)(Substitution[x] % 255);/// green
                //components[2] = (byte)(Substitution[x] % 255);/// red
                //components[3] = (byte)255;

                //uint pixel = BitConverter.ToUInt32(components, 0);
                pixel = vector[x];
                for (int k = 0; k < dy; ++k)
                {
                    pixels[k] = pixel;
                }


                Int32Rect rect = new Int32Rect(x, 0, dx, dy);
                int stride = wb.Format.BitsPerPixel / 8;

                wb.WritePixels(rect, pixels, stride, 0);

            }
            wb.Unlock();
            img.Source = wb;

        }


        private void SubsSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (Substitution != null)
            {
                Substitution = VectorShuffle(Substitution);
                DrawGradientLine(Substitution, SubsImage);    
            }
            
        }

        private void GammaSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (Gamma != null)
            {
                Gamma = GammaShuffle(Gamma);
                DrawGammaLine(Gamma, GammaImage);
                
            }
        }

        public void SubstitutionPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Substitution")
            {
                DrawGradientLine(Substitution, SubsImage);
            }
        }
        #endregion
        private void FirstImgGetBytesButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] img = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource);
            int bytesPerPixel = ((BitmapSource)SrcImageSource).Format.BitsPerPixel / 8;
            /*
            int brightestPixel = ImageTools.GetBrightestPixel(img, bytesPerPixel);
            int darkestPixel = ImageTools.GetDarkestPixel(img, bytesPerPixel);

            MessageBox.Show("brightestPixel=" + brightestPixel.ToString(), ", darkestPixel=" + darkestPixel);
            //test3();

            int width = ((BitmapSource)SrcImageSource).PixelWidth;
            int height = ((BitmapSource)SrcImageSource).PixelHeight;
            int pSumm1 = 0;
            int pSumm2 = 0;
            int tSumm = 0;
            int hSumm = 0;
            for (int k = 0; k < height-1; k++)
            {
                tSumm = 0;
                for (int i = 0; i < width * bytesPerPixel; i += bytesPerPixel)
                {
                    pSumm1 = 0;
                    pSumm2 = 0;
                    for (int j = 0; j < bytesPerPixel; j++)
                    {
                        pSumm1 += (int)img[k*width+i + j];
                        pSumm2 += (int)img[(k+1) * width + i + j];
                    }
                    tSumm += Math.Abs(pSumm1 - pSumm2);

                }
                hSumm += (tSumm/width);

            }
            MessageBox.Show("ABS by row: "+(hSumm / height).ToString());

            pSumm1 = 0;
            pSumm2 = 0;
            tSumm = 0;
            hSumm = 0;
            for (int k = 0; k < width - 1; k++)
            {
                tSumm = 0;
                for (int i = 0; i < height * bytesPerPixel; i += bytesPerPixel)
                {
                    pSumm1 = 0;
                    pSumm2 = 0;
                    for (int j = 0; j < bytesPerPixel; j++)
                    {
                        pSumm1 += (int)img[k * height + i + j];
                        pSumm2 += (int)img[(k + 1) * height + i + j];
                    }
                    tSumm += Math.Abs(pSumm1 - pSumm2);

                }
                hSumm += (tSumm / height);

            }
            MessageBox.Show("ABS by column: " + (hSumm / width).ToString());*/

            int width = ((BitmapSource)SrcImageSource).PixelWidth;

            StatisticsMatrix sm = new StatisticsMatrix((uint)width, (uint)bytesPerPixel, img);
            long t = DateTime.Now.Ticks;
            sm.Run();
            t = DateTime.Now.Ticks - t;
            MessageBox.Show("Ticks:" + t);

            var result = sm.GetColumn(1);
            foreach (var a in result)
                MessageBox.Show(a.ToString());
            
            //Color c = Color.FromArgb(0, 0, 0, 0);

            //result = sm.GetColumn(1);
            //foreach (var a in result)
            //    MessageBox.Show(a.ToString());
        }

        #region Encrypt&Descrypt images
        private void Button_Encrypt_Images_ListBox_Click(object sender, RoutedEventArgs e)
        {

                    List<WriteableBitmap> tmp = new List<WriteableBitmap>();
                    int PixelWidth = ((BitmapSource)ImagesListBox[0]).PixelWidth;
                    int PixelHeight = ((BitmapSource)ImagesListBox[0]).PixelHeight;
                    PixelFormat Format = ((BitmapSource)SrcImageSource).Format;

                    var ui = TaskScheduler.FromCurrentSynchronizationContext();
                    List<Task<Tuple<ImageSource, int>>> tasks = new List<Task<Tuple<ImageSource, int>>>();
                    for (int i = 0; i < ImagesListBox.Count; i++)
                    {
                        int j = i;
                        var t = Task.Factory.StartNew<Tuple<ImageSource, int>>(() =>
                        {
                            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)ImagesListBox[j]);
                            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);
                            BasicMatrix bm = new BasicMatrix(mtxSize, 4, matrixOrg);
                            Task task = bm.Transposition(Substitution);
                            task.Wait();

                            byte[] matrixMod = bm.GetMatrix();
                            ImageSource encodeImg = ImageConverter.ByteArrayToWriteableBitmap(matrixMod, PixelWidth, PixelHeight, Format);
                            //return encodeImg;
                            var tu = Tuple.Create(encodeImg, j);
                            return tu;
                        }, CancellationToken.None, TaskCreationOptions.None, ui);


                        var display = t.ContinueWith(
                            resultTask =>
                            {
                                ImageSource wb = resultTask.Result.Item1;
                                int? k = resultTask.Result.Item2;
                                if (k == null) k = 0;
                                ImagesListBox[(int)k] = wb;
                            }, ui);
                        tasks.Add(t);
                    }

            //MessageBox.Show("12312");
            ///*imagesListBox.Clear();
            //foreach (var elem in tmp)
            //{
            //    ImagesListBox.Add(elem);
            //}*/
            //List<WriteableBitmap> tmp = new List<WriteableBitmap>();
            ////TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            ////Task t = new Task((state) =>
            ////{
            /////ImageSource[] imgs = new ImageSource[ImagesListBox.Count];
            ////for (int i = 0; i < ImagesListBox.Count; i++)
            ////{


            ////}
            //Parallel.For(0, ImagesListBox.Count, (i, loopState) =>
            ////  {
            ////for (int i = 0; i < ImagesListBox.Count;i++ )
            ////foreach (var elem in ImagesListBox)
            ////imgs[i] = ImagesListBox[i].Clone();
            //{
            //    //MessageBox.Show( (ImageConverter.f(i)).ToString());


            //    byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)elem); //ImageSourceToByteArray();

            //    ///ImageConverter.ImageToByteArray methos
            //    //byte[] matrixOrg = new byte[((BitmapSource)imgs[i]).PixelHeight * ((BitmapSource)imgs[i]).PixelWidth * ((BitmapSource)imgs[i]).Format.BitsPerPixel / 8];
            //    //((BitmapSource)imgs[i]).CopyPixels(matrixOrg, ((BitmapSource)imgs[i]).PixelWidth * ((BitmapSource)imgs[i]).Format.BitsPerPixel / 8, 0);
            //    ////////

            //    uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);
            //    //
            //    BasicMatrix bm = new BasicMatrix(mtxSize, 4, matrixOrg);
            //    Task task = new Task(() => bm.Transposition(Substitution));
            //    task.Start();
            //    task.Wait();

            //    //MessageBox.Show("Hello");
            //    byte[] matrixMod = bm.GetMatrix();

            //    WriteableBitmap encodeImg = ImageConverter.ByteArrayToWriteableBitmap(matrixMod,
            //                                                                ((BitmapSource)SrcImageSource).PixelWidth,
            //                                                                ((BitmapSource)SrcImageSource).PixelHeight,
            //                                                                ((BitmapSource)SrcImageSource).Format
            //                                                                );
            //    tmp.Add((WriteableBitmap)encodeImg);
            //    //ImagesListBox[i] = encodeImg;

            //}
            //imagesListBox.Clear();
            //foreach (var elem in tmp)
            //{
            //    ImagesListBox.Add(elem);
            //}
            ////},ts);
            ////});
            ////t.Start();       
        }
        private void Button_Decrypt_Images_ListBox_Click(object sender, RoutedEventArgs e)
        {

            byte[] matrixOrg = ImageConverter.ImageToByteArray((BitmapSource)DstImageSource); //ImageSourceToByteArray();
            byte[] matrixMod = ImageConverter.ImageToByteArray((BitmapSource)SrcImageSource); //ImageDestinationToByteArray();


            uint mtxSize = (uint)Math.Sqrt(matrixOrg.Length / 4);


            IsomorphMatrices im = new IsomorphMatrices(mtxSize, 4, matrixOrg, matrixMod);
            uint[] subs = new uint[mtxSize];
            for (int i = 0; i < subs.Length; ++i)
            {
                subs[i] = (uint)i;
            }

            
            Task t = im.SearchIsomorph(1, subs);
            Task p = new Task(() =>
            {
                //Так делать нельзя, руки бы себе по отрывал. Юра.
                //Подписываюсь. И себе бы вырвал за использование этого. Максим.
                while (true)
                {
                    //progressBar.Value = im.GetProgress();

                    Progress = im.GetProgress();

                    Thread.Sleep(50);
                }
            });
            p.Start();
            t.ContinueWith((state) =>
            {
                if (im.GetSubstitutionsCount() > 0)
                {
                    uint count = im.GetSubstitutionsCount();
                    string s = "";
                    uint[] a = im.GetSubstitutions(0);
                    s += String.Join(",", a) + "\n";
                    s = "IsomorphMatrices Test:"
                        + "\nВсего найдено:" + count
                        + "\nПервый Вектор:" + s;
                    MessageBox.Show(s);
                }
                else
                {
                    MessageBox.Show("Нет векторов");
                }
            });
            t.Wait();
            if (im.GetSubstitutionsCount() > 0)
            {
                List<WriteableBitmap> tmp = new List<WriteableBitmap>();
                uint[] findSubs = im.GetSubstitutions(0);
                foreach (var elem in ImagesListBox)
                {
                    byte[] matrixEncr = ImageConverter.ImageToByteArray((BitmapSource)elem); //ImageSourceToByteArray();

                    //
                    BasicMatrix bm = new BasicMatrix(mtxSize, 4, matrixEncr);
                    Task task = new Task(() => bm.Transposition(findSubs));
                    task.Start();
                    task.Wait();
                    //MessageBox.Show("Hello");
                    byte[] matrixDecr = bm.GetMatrix();
                    WriteableBitmap decodeImg = ImageConverter.ByteArrayToWriteableBitmap(matrixDecr,
                                                                                ((BitmapSource)SrcImageSource).PixelWidth,
                                                                                ((BitmapSource)SrcImageSource).PixelHeight,
                                                                                ((BitmapSource)SrcImageSource).Format
                                                                                );
                    tmp.Add((WriteableBitmap)decodeImg);
                }
                imagesListBox.Clear();
                foreach (var elem in tmp)
                {
                    ImagesListBox.Add(elem);
                }

            }
        }
        #endregion


        private void MenuAbout(object sender, RoutedEventArgs e)
        {
            AboutWindow ab = new AboutWindow();
            ab.ShowDialog();
        }

        private void CtxMenuImageLBox_Click(object sender, RoutedEventArgs e)
        {
            String menuItemName = (sender as MenuItem).Name;
            ImageSource selectedImg = (ImagesLBox.SelectedItem as ImageSource);
            switch (menuItemName)
            {
                case "AddSrcImageFromLB":
                    SrcImageSource = selectedImg;
                    break;
                case "AddDstImageFromLB":
                    DstImageSource = selectedImg;
                    break;
            }
        }

        #region HadamardMatrix

        //================HadamardProperties======================
        #region HadamardProperties
        private HadamardMtxModel hadMtxModel;
        public HadamardMtxModel HadMtxModel
        {
            get { return hadMtxModel; }
            set
            {
                hadMtxModel = value;
                OnPropertyChanged("HadMtxModel");
            }
        }
        #endregion
        //================HadamardLoadSave======================
        #region HadamardLoadSave
        private void GenerateExtHadamardMtxButton_Click(object sender, RoutedEventArgs e)
        {
            hadMtxModel.GenerateExtMatrix();
        }

        private void LoadMatrixButton_Click(object sender, RoutedEventArgs e)
        {
            #region Stub
            //int[,] dataTestFiveDim = new int[5, 5] {
            //        {1,0,0,1,0},
            //        {0,-1,0,-1,0},
            //        {0,0,-1,0,0},
            //        {0,-1,0,-1,0},
            //        {0,1,0,0,1},
            //        };
            //HadamardLogic.Matrix mtx = new HadamardLogic.Matrix(5, 5);
            //for (int i = 0; i < 5; i++)
            //{
            //    for (int j = 0; j < 5; j++)
            //    {
            //        mtx[i, j] = dataTestFiveDim[i, j];
            //    }
            //}
            //vModel.Mtx.InitOriginalMatrix(mtx);
            #endregion
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.DefaultExt = ".txt";
            dlg.Filter = "txt documents (.txt)|*.txt";

            Nullable<bool> result = dlg.ShowDialog();
            if (result == true)
            {
                string filename = dlg.FileName;
                hadMtxModel.LoadMatrix(filename);
            }
        }

        private void SaveMatrixButton_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog dlg = new Microsoft.Win32.SaveFileDialog();
            dlg.DefaultExt = ".txt";
            dlg.Filter = "txt documents (.txt)|*.txt";

            if (dlg.ShowDialog() == true)
            {
                string filename = dlg.FileName;
                hadMtxModel.SaveMatrix(dlg.FileName);
            }
        }
        #endregion
        //================Hadamard_UI_Logic======================
        #region Hadamard_UI_Logic
        private void ReDrawButton_Click(object sender, RoutedEventArgs e)
        {
            hadMtxModel.ReDrawMatrixImage();
        }

        private void SearchAutomorphismButton_Click(object sender, RoutedEventArgs e)
        {
            hadMtxModel.SearchAutomorphism();
        }

        private void RenderGrapthButton_Click(object sender, RoutedEventArgs e)
        {
            //hadMtxModel.RenderGrapth();
            hadMtxModel.RenderSecondGrapth();
            ChartPlotterHadamard.RemoveUserElements();
            ChartPlotterHadamard.RightPanel.Children.Clear();
            ChartPlotterHadamard.AddLineGraph(hadMtxModel.Values, Colors.Blue, 2, "переборы");
       }

        #endregion

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            logTextBox.ScrollToEnd();
        }
         
        #endregion

        #region Graph

        private GraphModel gModel;
        public GraphModel GModel
        {
            get { return gModel; }
            set { gModel = value; }
        }
        private void SearchGraphAutomorphButton_Click(object sender, RoutedEventArgs e)
        {
            gModel.SearchAutomorphism();
        }
        private void RenderGraphGrapthButton_Click(object sender, RoutedEventArgs e)
        {
            gModel.RenderGrapth();
            ChartPlotterGraph.RemoveUserElements();
            ChartPlotterGraph.RightPanel.Children.Clear();
            ChartPlotterGraph.UseLayoutRounding = true;

            ChartPlotterGraph.AddLineGraph(gModel.Values, Colors.Blue, 2, "переборы");
        
        }
        private void LoadGraphButton_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.DefaultExt = ".scd";
            dlg.Filter = "scd documents (.scd)|*.scd";

            Nullable<bool> result = dlg.ShowDialog();
            if (result == true)
            {
                string filename = dlg.FileName;
                gModel.SetMatrix(filename);
            }
        }
        #endregion

        private void HadamardBatchProcessButton_Click(object sender, RoutedEventArgs e)
        {
            using (System.Windows.Forms.FolderBrowserDialog dlg = new System.Windows.Forms.FolderBrowserDialog())
            {
                dlg.Description = "Select a folder";
                System.Windows.Forms.DialogResult result = dlg.ShowDialog();
                if (result == System.Windows.Forms.DialogResult.OK)
                {
                    MessageBox.Show("You selected: " + dlg.SelectedPath);
                    HadMtxModel.BatchProcess(dlg.SelectedPath);
                }
            }
        }

        private void ImagesLBox_Drop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent("myImage"))
            {
                ImageSource contact = e.Data.GetData("myImage") as ImageSource;
                ImagesListBox.Add(contact);
            }
        }
        private void srcdstImage_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
                {
                    Image ims = sender as Image;
                    DataObject dragData = new DataObject("myImage", ims.Source);
                    DragDrop.DoDragDrop(ims, dragData, DragDropEffects.Copy);
                } 
        }

        private void ImagesLBox_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                ListBox list = sender as ListBox;
                ImageSource listItem = (ImageSource)list.SelectedItem;
                if (listItem != null)
                {
                    DataObject dragData = new DataObject("myImage", listItem);
                    DragDrop.DoDragDrop(list, dragData, DragDropEffects.Copy);
                }
            } 
        }

        private void srcImage_Drop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent("myImage"))
            {
                Image im = sender as Image;
                ImageSource contact = e.Data.GetData("myImage") as ImageSource;
                SrcImageSource = contact;
            }
        }

        private void dstImage_Drop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent("myImage"))
            {
                Image im = sender as Image;
                ImageSource contact = e.Data.GetData("myImage") as ImageSource;
                DstImageSource = contact;
            }
        }

        private void DeleteFromImagesList_Click(object sender, RoutedEventArgs e)
        {
            String menuItemName = (sender as MenuItem).Name;
            ImageSource selectedImg = (ImagesLBox.SelectedItem as ImageSource);
            ImagesListBox.Remove(selectedImg);
        }



        private void GammaSize_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            Gamma = new byte[(int)GammaSize.Value * (((BitmapSource)SrcImageSource).Format.BitsPerPixel / 8)];
            Gamma = GammaShuffle(Gamma);
            DrawGammaLine(Gamma, GammaImage);
        }



    }

    public static class Commands
    {
        public static readonly RoutedUICommand TranspositionLeft = new RoutedUICommand("TranspositionLeft", "TranspositionLeft", typeof(Commands));
        public static readonly RoutedUICommand TranspositionRight = new RoutedUICommand("TranspositionRight", "TranspositionRight", typeof(Commands));
        public static readonly RoutedUICommand TranspositionFull = new RoutedUICommand("TranspositionFull", "TranspositionFull", typeof(Commands));

        public static readonly RoutedUICommand AutomorphismSearch = new RoutedUICommand("AutomorphismSearch", "AutomorphismSearch", typeof(Commands));

        public static readonly RoutedUICommand IsomorphismSearch = new RoutedUICommand("IsomorphismSearch", "IsomorphismSearch", typeof(Commands));
        public static readonly RoutedUICommand IsomorphismStatisticsSearch = new RoutedUICommand("IsomorphismStatisticsSearch", "IsomorphismStatisticsSearch", typeof(Commands));
        public static readonly RoutedUICommand IsomorphismStatisticsSearchColumns = new RoutedUICommand("IsomorphismStatisticsSearchColumns", "IsomorphismStatisticsSearchColumns", typeof(Commands));
        public static readonly RoutedUICommand IsomorphismStatisticsSearchRows = new RoutedUICommand("IsomorphismStatisticsSearchRows", "IsomorphismStatisticsSearchRows", typeof(Commands));

        public static readonly RoutedUICommand ShowGamma = new RoutedUICommand("ShowGamma", "ShowGamma", typeof(Commands));
        public static readonly RoutedUICommand GenerateGamma = new RoutedUICommand("GenerateGamma", "GenerateGamma", typeof(Commands));
        public static readonly RoutedUICommand ImposeGamma = new RoutedUICommand("ImposeGamma", "ImposeGamma", typeof(Commands));

        public static readonly RoutedCommand ShowSubstitution = new RoutedUICommand("ShowSubstitution", "ShowSubstitution", typeof(Commands));
        public static readonly RoutedCommand GenerateSubstitution = new RoutedUICommand("GenerateSubstitution", "GenerateSubstitution", typeof(Commands));

    }
}
