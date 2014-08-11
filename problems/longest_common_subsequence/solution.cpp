#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

/**
   Dynamic programming implementation of a solution for the longest common
   sub-sequence problem.
*/
int
longest_common_subsequence(const string & lhs, const string & rhs)
{
  if(lhs.empty() || rhs.empty())
    return 0;

  vector< vector<int> > dp_table(
      lhs.size()+1, vector<int>(rhs.size() + 1, 0));

  for(int i = 1; i <= lhs.size(); ++i) 
  {
    for(int j = 1; j <= rhs.size(); ++j) 
    {
      if(lhs[i-1] == rhs[j-1])
        dp_table[i][j] = dp_table[i-1][j-1] + 1;
      else
        dp_table[i][j] = max(dp_table[i-1][j], dp_table[i][j-1]);
    }
  }

  stack<char> subsequence;
  int i = lhs.size();
  int j = rhs.size();
  while(i > 0 && j > 0)
  {
    if(dp_table[i][j] == (dp_table[i-1][j-1] + 1) && lhs[i-1] == rhs[j-1]) {
      i -= 1;
      j -= 1;
      subsequence.push(lhs[i]);
    }
    else if(dp_table[i-1][j] > dp_table[i][j-1]) {
      --i;
    }
    else {
      --j;
    }
  }


  while(!subsequence.empty()){
    cout << subsequence.top();
    subsequence.pop();
  }
  cout << endl;


  return dp_table[lhs.size()][rhs.size()];
}

int main() 
{
  cout << 
      longest_common_subsequence(
          "abgbcdf",
          "abbcdeeeeeeeeeeeeeeeeeeeeeeeeef")
       << endl;

  return 0;
}
