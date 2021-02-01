using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Sign
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args == null)
            {
                Console.WriteLine("ERROR"); // Check for null array
                return;
            }
            if (args.Length != 3)
            {
                Console.WriteLine("ERROR"); // Check for null array
                return;
            }
            TrySign t = new TrySign(args[0], args[1], args[2]);
            string zik;
            zik = t.MakePotpis();
            Console.WriteLine(zik);
        }
    }
}
