#include <iostream>
#include <set>
#include <string>
#include <boost/foreach.hpp>
u
using namespace std;

set<string>
all_paren_pairs(const int number_of_pairs)
{
  set<string> all_pairs;
  if(number_of_pairs <= 0) {
    all_pairs.insert("");
    return all_pairs;
  }


  const set<string> previous_pairs = all_paren_pairs(number_of_pairs-1);
  BOOST_FOREACH(const string & combo, previous_pairs)
  {
    for(int i = 0; i < combo.size(); ++i)
    {
      if(combo[i] == '(') {
        string new_combo = combo.substr(0,i+1);
        new_combo += "()";
        new_combo += combo.substr(i+1, combo.size());
        all_pairs.insert(new_combo);
      }
    }
    all_pairs.insert("()" + combo);
  }

  return all_pairs;
}

set<string>
all_paren_pairs_2(const int number_of_pairs)
{
  // do
}



int main(int argc, char* argv[]) {

  BOOST_FOREACH(const string & pair, all_paren_pairs(atoi(argv[1]))){
    cout << "'" << pair << "'" << endl;
  }

  return 0;
}
