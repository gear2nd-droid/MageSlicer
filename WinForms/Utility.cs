using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IE_WinForms
{
  internal class Utility
  {
    // least common denominator
    public static int Lcm(int a, int b) {
      return a * b / Gcd(a, b);
    }

    // Euclid's reciprocal division
    public static int Gcd(int a, int b) {
      if (a < b)
        return Gcd(b, a);
      while (b != 0) {
        var remainder = a % b;
        a = b;
        b = remainder;
      }
      return a;
    }
  }
}
