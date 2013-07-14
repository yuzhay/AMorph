using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using GraphSharp.Controls;
using System.Text.RegularExpressions;
using System.IO;
using System.Windows;
using Microsoft.Research.DynamicDataDisplay.DataSources;
using System.Threading.Tasks;

namespace AMorph
{
    public class GraphModel : INotifyPropertyChanged
    {
        private IncidenceMatrix incMtx;
        public IncidenceMatrix IncidenceMatrix
        {
            get { return incMtx; }
            set { incMtx = value; }
        }

        private List<int>[] graphEdges;
        public List<int>[] GraphEdges
        {
            get { return graphEdges; }
            set { graphEdges = value; }
        }

        private byte[] graphEdgesScd;
        public byte[] GraphEdgesScd
        {
            get { return graphEdgesScd; }
            set { graphEdgesScd = value; }
        }

        private int dimension;
        public int Dimension
        {
            get { return dimension; }
            set { dimension = value; }
        }

        private int degree;
        public int Degree
        {
            get { return degree; }
            set { degree = value; }
        }
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

        public GraphModel()
        {
            //SetMatrix(@"C:\Users\Dales3D\Dropbox\Егоров\genreg\20_3_3-U.scd");
            values = new ObservableDataSource<Point>();
            subsCollection = new SubstitutionCollection();
        }

         private void Init(int dim, int deg)
        {
            dimension = dim;
            degree = deg;

            graphEdges = new List<int>[dimension];
            for (int i = 0; i < graphEdges.Length; i++)
            {
                graphEdges[i] = new List<int>();
            }
        }
        private void InitValues(byte[] bytes)
        {
            int index = 1;
            for (int i = 0; i < dimension; i++)
            {
                while (graphEdges[i].Count < degree)
                {
                    int value = bytes[index];
                    graphEdges[i].Add(value - 1);
                    graphEdges[value - 1].Add(i);
                    index++;
                }
            }
            ///
            byte[,] mtx = GraphToMatrix();
            string ans = "";
            for (int i = 0; i < dimension; i++)
            {
                for (int j = 0; j < dimension; j++)
                {
                    ans += " " + mtx[i, j].ToString() + " ";
                    
                }
                ans += "\n";
            }

            System.Windows.MessageBox.Show(ans);
            sbyte[] vec = GraphToVector();
            incMtx = new IncidenceMatrix((uint)dimension, GraphToVector());
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
        /// <summary>
        /// Получает файл и формирует матрицу
        /// </summary>
        /// <param name="filePath"></param>
        public void SetMatrix(String filePath)
        {
            Tuple<int,int> dimDeg = ParseFileName(filePath); //return dimension|degree
            if (dimDeg != null)
            {
                Init(dimDeg.Item1, dimDeg.Item2);
                byte[] bytes = ReadBytesFromScd(filePath);
                InitValues(bytes);
            }
        }

        private Tuple<int,int> ParseFileName(string filePath)
        {
            String fileName = Path.GetFileNameWithoutExtension(filePath);
            String pattern = @"[0]?(?<dimension>\d*)_(?<degree>\d)_(?<part3>\d)[-U]?";
            Regex regex = new Regex(pattern);
                        
            //bool isMatch = regex.IsMatch(fileName);
            Match match = regex.Match(fileName);
            Console.WriteLine("[Status String Match] - {0}", match.Success);
            if (match.Success)
            {
                foreach (Group g in match.Groups)
                {
                    Console.WriteLine("[Value: {0}, Length: {1}]", g.Value, g.Length);
                }

                int dim = Int32.Parse(match.Groups["dimension"].Value);
                int deg = Int32.Parse(match.Groups["degree"].Value);
                
                return new Tuple<int, int>(dim,deg);
            }

            return null;

        }

        private byte[] ReadBytesFromScd(string filePath)
        {
           byte[] bytes = File.ReadAllBytes(filePath);
            ///TODO #1
           graphEdgesScd = bytes;
           return bytes;
        }

        public byte[,] GraphToMatrix()
        {
            byte[,] mtx = new byte[dimension, dimension];

            for (int v1Num = 0; v1Num < dimension; v1Num++)
            {
                for (int v2Num = 0; v2Num < degree; v2Num++)
                {
                    mtx[v1Num, graphEdges[v1Num][v2Num]] = 1;
                    mtx[graphEdges[v1Num][v2Num], v1Num] = 1;
                }
            }
            return mtx;
        }
        public sbyte[] GraphToVector()
        {
            sbyte[] smtx = new sbyte[dimension * dimension];

            byte[,] mtx = GraphToMatrix();
            for (int i = 0; i < dimension; i++)
            {
                for (int j = 0; j < dimension; j++)
                {
                    smtx[i * dimension + j] = (sbyte)mtx[i,j];
                    
                }
            }
            return smtx;
        }
        public void SearchAutomorphism()
        {
            SubsCollection.Clear();

            uint[] t1substitution = new uint[incMtx.SizeMtx];
            for (int i = 0; i < t1substitution.Length; ++i)
            {
                t1substitution[i] = (uint)i;
            }
            TaskScheduler ts = TaskScheduler.FromCurrentSynchronizationContext();
            Task t = incMtx.SearchAutomorph(1, t1substitution);
            t.ContinueWith((state) =>
            {
                uint count = incMtx.GetSubstitutionsCount();
                string s = "";
                for (uint i = 0; i < count; i++)
                {
                    Substitution sub = new Substitution((int)incMtx.SizeMtx);
                    sub.Position = incMtx.GetSubstitutions(i);
                    sub.Partitions = (int)incMtx.GetSubstitutionsParts(i);
                    subsCollection.Add(sub);

                }
                uint cnt = 0;
                foreach (var elem in subsCollection)
                {
                    cnt += (uint)elem.Partitions;
                }
                uint[] a = incMtx.GetSubstitutions(0);
                s += String.Join(",", a) + "\n";
                s = "AutomorphMatrix Test:"
                //    + GetInfo()
                    + "\nВсего найдено: " + count
                    + "\nВсего перебрано: " + cnt;
                // + "\nПервый Вектор: " + s;
                MessageBox.Show(s);
                //ResultInfo = s;

            }, ts);
        }



        public override String ToString()
        {
            String str = "";

            str += "GraphEdges:\n";
            for (int i = 0; i < dimension; i++)
            {
                str += "vertex " + i + ":";
                for (int j = 0; j < degree; j++)
                {
                    str += graphEdges[i][j] + ", "; 
                }
                str += ";\n";
            }

            return str;
        }

    }
    
}
