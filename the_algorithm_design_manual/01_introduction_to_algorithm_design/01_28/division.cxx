// std
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

// boost
#include <boost/assert.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/timer/timer.hpp>

// A divide function's signature, takes two const int primitives and returns an 
// int representing the quotient.
typedef int(*DivideFunction)(const int, const int);

/**
   First thing that comes to mind is to just do iterative subtraction.
   However many times you can subtract the denominator from the numerator before
   the numerator goes negative is how many times the denominator goes into the
   numerator, which is the definition of integer division.

   XxX: this only works for positive numbers.
*/
int
divide_v1(const int numerator, const int denominator)
{
  int temp_numerator = numerator;
  int quotient = 0;

  while(temp_numerator >= denominator)
  {
    temp_numerator -= denominator;
    ++quotient;
  }
  return quotient;
}

/**
   However, the test taker in me knows there's probably something to the 
   explicit request for "a fast way to do it".

   First thought had me wondering if there was anything we could do with the 
   bitwise operators, but I can't think of anything, unless we can guarantee the
   denominator has a factor of 2.

   I'm guessing since we weren't disallowed modulo, there's probably a trick 
   there.

   ...time passes...

   Found this solution proposed online as being faster. However, compiled with 
   g++, and no on my machine with no command line optimizations, I get similar 
   time with divide_v1.

   Going to come back to this to see if i can understand the authors intention 
   and potentially make it better.
*/
int
divide_v2(const int numerator, const int denominator)
{
  int count = 0;
  int temp_denominator = denominator;
  int last_temp_denominator = denominator;
  bool incremented = false;
  while(numerator % temp_denominator == 0) 
  {
    incremented = true;
    last_temp_denominator = temp_denominator;
    temp_denominator += denominator;
    ++count;
  }

  if(incremented) {
    temp_denominator -= last_temp_denominator;
    --count;
  }

  int result = divide_v1(numerator, temp_denominator);
  for(int i = 0; i < count; ++i)
    result += i;

  return result;
}

/**
   A function to test an argument provided DivideFunction on a given 
   numerator/denominator pair,
*/
boost::timer::nanosecond_type
test(
    const DivideFunction & fn, 
    const int numerator, 
    const int denominator, 
    const int num_iterations)
{
  boost::timer::nanosecond_type average_wall_time = 0;
  for(unsigned int i = 0; i < num_iterations; ++i)
  {
    boost::timer::cpu_timer cpu_timer;

    const int function_quotient = fn(numerator, denominator);
    const int correct_quotient = numerator/denominator;

    if(function_quotient != correct_quotient) {
      std::cerr
          << "ERROR: Your function isn't producing the correct results. " 
          << "It thinks " << numerator << "/" << denominator << "==" 
          << function_quotient
          << std::endl;
      exit(1);
    }

    average_wall_time += cpu_timer.elapsed().wall;
  }

  return average_wall_time/num_iterations;
}

int main() 
{
  std::cout << "Starting test...\n" << std::endl;

  typedef std::vector< std::pair<const int, const int> > TestSetType;
  typedef std::map<std::string, DivideFunction> FunctionMap;

  TestSetType test_set =
      boost::assign::list_of
      (std::make_pair(10,5))
      (std::make_pair(10,3))
      (std::make_pair(100,2))
      (std::make_pair(100,1))
      (std::make_pair(1231231231,6))
      (std::make_pair(1231231231,1231231231))
      (std::make_pair(6, 123123123));

  const unsigned int num_iterations = 100;

  FunctionMap functions;
  functions["divide_v1"] = &divide_v1;
  functions["divide_v2"] = &divide_v2;

  BOOST_FOREACH(const TestSetType::value_type & values, test_set) 
  {
    std::cout 
        << "Testing " 
        << values.first << "/" << values.second
        << std::endl;

    BOOST_FOREACH(const FunctionMap::value_type & function, functions)
    {
      std::cout 
          << "* Function '" << function.first << "' average: "
          << test(function.second, values.first, values.second, num_iterations) 
          << " nanoseconds"
          << std::endl;
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;

  return 0;
}
