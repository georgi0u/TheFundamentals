#include <algorithm>
#include <limits.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

int edit_distance(const string & lhs, const string & rhs)
{
  // Initialize DP table
  vector< vector<int> > edit_distances(
      lhs.size() + 1, vector<int>(rhs.size()+1, 0));
  for(int i = 0; i <= lhs.size(); ++i)
    edit_distances[i][0] = i;
  for(int j = 0; j <= rhs.size(); ++j)
    edit_distances[0][j] = j;

  // Populate DP table
  vector<int> costs(3, 0);
  for(int i = 1; i < lhs.size(); ++i) {
    for(int j = 1; j < rhs.size(); ++j) {

      costs[0] = edit_distances[i-1][j-1] + (lhs[i-1] == rhs[j-1] ? 0 : 1);
      costs[1] = edit_distances[i][j-1] + 1;
      costs[2] = edit_distances[i-1][j] + 1;

      edit_distances[i][j] = *min_element(costs.begin(), costs.end());
    }
  }

  // Return edit distance
  return edit_distances[lhs.size()][rhs.size()];
}


bool is_k_palindrome(const string & word, const int k)
{
  const string reverse_word(word.rbegin(), word.rend());

  // Initialize DP table
  vector< vector<int> > edit_distances(
      word.size()+1, vector<int>(word.size()+1, 0));

  for(int i = 0; i <= word.size(); ++i)
    edit_distances[i][0] = i;
  for(int j = 0; j <= reverse_word.size(); ++j)
    edit_distances[0][j] = j;


  // Populate DP table
  vector<int> costs(3, 0);
  for(int i = 1; i <= word.size(); ++i) {
    for(int j = 1; j <= word.size()); ++j) {

      costs[0] = (word[i-1] == reverse_word[j-1] ? edit_distances[i-1][j-1] : INT_MAX);
      costs[1] = edit_distances[i][j-1] + 1;
      costs[2] = edit_distances[i-1][j] + 1;

      edit_distances[i][j] = *min_element(costs.begin(), costs.end());
    }
  }

  return edit_distances[word.size()][word.size()] <= 2*k;
}

int main(int argc, char *argv[])
{
  string test = "";
  for(int i = 0; i < 10000; ++i) {
    test.push_back((char)((rand() % 26) + 97));
  }
  string test_reverse(test.rbegin(), test.rend());
  
  cout << test+test_reverse << endl;
  cout << (is_k_palindrome(test+test_reverse+"asdasd", 1) ? "true" : "false") << endl;

  return 0;
}
