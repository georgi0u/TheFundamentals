#include <iostream>
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>

using namespace std;

void longest_increasing_subsequence(
    const vector<int> & input,
    const int end,
    vector<int> & solutions)
{
  if(input.empty())
    return;

  if(end == 0) {
    solutions.push_back(1);
    return;
  }

  if(end > solutions.size())
    longest_increasing_subsequence(input, end-1, solutions);

  int previous_max_sequence = 0;
  for(int i = end-1; i >=0; --i)
  {
    if(input[i] >= input[end]) continue;
    if(solutions[i] > previous_max_sequence)
      previous_max_sequence = solutions[i];
  }

  solutions.push_back(previous_max_sequence+1);
}


int longest_increasing_subsequence(const vector<int> & input)
{
  vector<int> dp_solutions;
  longest_increasing_subsequence(input, input.size()-1, dp_solutions);

  int max = 0;
  BOOST_FOREACH(const int solution, dp_solutions) {
    cout << solution << " ";
    if(solution > max) max = solution;
  }
  cout << endl;
  return max;
}


int main() 
{
  const vector<int> a = boost::assign::list_of(2)(4)(3)(5)(1)(7)(6)(9)(8);
  cout << longest_increasing_subsequence(a) << endl;

  return 0;
}
