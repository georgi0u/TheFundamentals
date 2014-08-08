#include <iostream>
#include <vector>
#include <boost/foreach.hpp>

using namespace std;


vector< vector<int> >
representations(const int amount, const vector<int> & available_coins)
{
  vector< vector<int> > result;
  if(amount <= 0) 
    return result;

  for(vector<int>::const_iterator itr = available_coins.begin();
      itr != available_coins.end(); ++itr)
  {
    const int coin = *itr;
    const int coin_count = amount/coin;

    cout << amount << " " <<  coin << endl;

    if(coin_count == 0)
      continue;

    const vector<int> remaining_coins(itr+1, available_coins.end());
    for(int num_coins = 1; num_coins < coin_count; ++num_coins) {
      BOOST_FOREACH(const vector<int> & sub_result,
                    representations(amount-num_coins, remaining_coins))
      {
        result.push_back(sub_result);
        for(int i = 0; i < num_coins; ++i)
          result.back().push_back(coin);
      }
    }
  }

  return result;
}

vector< vector<int> >
representations(const int amount){

  vector<int> available_coins;

  available_coins.push_back(1);
  available_coins.push_back(5);
  available_coins.push_back(10);
  available_coins.push_back(25);

  return representations(amount, available_coins);
}



int main() {

  BOOST_FOREACH(const vector<int> possibility, representations(6)){
    BOOST_FOREACH(const int coin, possibility){
      cout  << coin << " ";
    }
    cout << endl;
  }

};
