#include <iostream>
#include <vector>
#include <boost/foreach.hpp>
#include <math.h>

using namespace std;

/**
   Iterative solution for finding all subsets
*/
template <typename T>
void all_subsets(
    vector<T> input,
    vector< vector<T> > & results)
{
  results.push_back(vector<T>());

  BOOST_FOREACH(const T element, input)
  {
    vector< vector<T> > results_so_far = results;
    BOOST_FOREACH(vector<T> result, results_so_far){ 
      results.push_back(result);
      results.back().push_back(element);
    }
  }
}


/**
   Recursive solution for finding all subsets
*/
template <typename T>
vector< vector<T> > all_subsets(const vector<T> & input)
{
  if(input.empty()) {
    vector< vector<T> > to_ret;
    to_ret.push_back(vector<T>());
    return to_ret;
  }

  vector<T> local_input = input;
  const T last_element = local_input.back();
  local_input.pop_back();

  vector< vector<T> > interm_results = all_subsets(local_input);
  vector< vector<T> > results_to_add;

  BOOST_FOREACH(const vector<T> subset, interm_results){
      results_to_add.push_back(subset);
      results_to_add.back().push_back(last_element);
  }

  interm_results.insert(
      interm_results.end(), results_to_add.begin(), results_to_add.end());

  return interm_results;
}


/**
   Bit manipulation for finding all subsets.
*/
template <typename T>
vector< vector<T> > all_subsets_2(const vector<T> & input)
{
  vector< vector<T> > output;

  for(int combination_number = 0; 
      combination_number < pow(2, input.size()); 
      ++combination_number)
  {
    output.push_back(vector<T>());

    // check the j-th significant bit of the combination number.
    // If it's 1, include the j'th element of the set, otherwise don't.
    int temp = combination_number;
    for(int j = 0; j < input.size(); ++j, temp = temp >> 1)
      if(temp & 1) output.back().push_back(input[j]);
  }

  return output;
}


int main() 
{
  vector<int> input;
  input.push_back(1);
  input.push_back(2);
  input.push_back(3);
  input.push_back(4);

  vector< vector<int> > output = all_subsets_2(input);

  BOOST_FOREACH(const vector<int> subset, output) {
    if(subset.empty()) {
      cout << "' ' ";
    }
    BOOST_FOREACH(int subset_member, subset) {
      cout << "'" << subset_member << "' ";
    }

    cout << endl;
  }

  return 0;
}
