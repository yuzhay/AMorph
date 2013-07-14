using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;

namespace AMorph
{
    public class SubstitutionCollection : ObservableCollection<Substitution>
    {
        private List<uint> iterationStats = new List<uint>();
        public List<uint> IterationStats 
        {
            get { return iterationStats; }
            set { iterationStats = value; }
        }

        public SubstitutionCollection()
        {
             
        }
    }
}
