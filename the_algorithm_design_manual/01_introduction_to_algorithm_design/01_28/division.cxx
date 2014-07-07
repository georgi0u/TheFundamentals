// std
#include <iostream>
#include <cassert>

/**
   First thing that comes to mind is to just do iterative subtraction.
   However many times you can subtract the denominator from the numerator
   is how many times the denominator goes into the numerator, which is the 
   definition of integer division.

   XxX: this only works for positive numbers.
*/
int
divide_v1(int numerator, const int denominator)
{
  int quotient = 0;
  while(numerator >= 0)
  {
    numerator -= denominator;
    ++quotient;
  }
  return quotient;
}


int main() {
  
  std::assert(divide_v1(10,5) = 10/5)
  return 0;
}
