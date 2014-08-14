#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <vector>

using namespace std;

/**
   Determines how many addition signs need to be inserted into `input` in order
   to produce a result of `sum`. 
   \param input is a string of positive integers
   \param sum is an integer up to 10 digits long.
   \return int the number of additions needed to be inserted into string to make
   it equate to sum, or -1 if its not possible to form the sum with any amount 
   of additions
*/
int min_sums(const string & input, const int sum)
{
  const int max_num_additions = (int)pow(2, input.size()-1);

  int min_additions = -1;
  bool found_sum = false;

  for(int i = 0; i < max_num_additions; ++i)
  {
    int temp_i = i;
    int current_sum = 0;
    int current_operand = 0;
    int num_additions = 0;

    for(int j = 0; j < input.size(); ++j)
    {
      current_operand = (current_operand*10) + atoi(input.substr(j,1).c_str());
      if(temp_i & 1) {
        current_sum += current_operand;
        current_operand = 0;
        ++num_additions;
      }
      temp_i = temp_i >> 1;
    }

    current_sum+=current_operand;

    if(current_sum == sum && (num_additions < min_additions || !found_sum)) 
    {
      found_sum = true;
      min_additions = num_additions;
    }
  }

  return min_additions;
}


int main()
{
  assert(min_sums("99999", 45) == 4);
  assert(min_sums("1110", 3) == 3);
  assert(min_sums("0123456789", 45) == 8);
  assert(min_sums("99999", 100) == -1);
  assert(min_sums("382834", 100) == 2);
  assert(min_sums("9230560001", 71) == 4);
}
