using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AMorph
{
    public class Substitution
    {
        public int Partitions { get; set; }
        public String Info { get; set; }
        public sbyte[] Sign { get; set; }
        public uint[] Position { get; set; }
        public int Length { get; set; }
        public Substitution(int num)
        {
            Partitions = 0;
            Info = "None";
            Length = num;
            Position = new uint[num];
            Sign = new sbyte[num];
            for (int i = 0; i < num; i++)
            {
                Position[i] = (uint)i;
                Sign[i] = 1;
            }
        }

        public uint this[int i]
        {
            set
            {
                Position[i] = value;
            }
            get
            {
                return Position[i];
            }
        }
        public override string ToString()
        {
            StringBuilder strPosition = new StringBuilder();
            foreach (var elem in Position)
            {
                strPosition.Append(elem.ToString() + " ");
            }

            StringBuilder strSign = new StringBuilder();
            foreach (var elem in Sign)
            {
                strSign.Append(elem.ToString() + " ");
            }

            return "[Position: " + strPosition.ToString() + "]\n[Sign: " + strSign.ToString() + "]\n[Переборов: " + Partitions.ToString() + "]";
        }
    }
}
