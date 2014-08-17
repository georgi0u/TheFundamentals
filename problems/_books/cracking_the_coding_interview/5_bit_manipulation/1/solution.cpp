#include <iostream>
#include <bitset>
#include <string>

using namespace std;

void 
print(unsigned int N)
{
  for(int i = 0; i < 32; ++i) {
    cout << ((N & (1 << 31)) >> 31);
    N = N << 1;
  }

  cout << endl;
}

void
insert(int N, const int M, const int i, const int j)
{
  print(N);
  print(M);
  cout << endl;

  const int length = j - i;
  const int front_pad_length = 31-j;
  const int rear_pad_length = i;

  const int to_or = M << rear_pad_length;
  unsigned int mask = (-1 << (31-length));
  mask = mask  >> (31-length);
  mask = mask << rear_pad_length;
  mask = ~mask;

  N = N & mask;
  N = N | (M << i);

  print(N);
}


int main(){
  insert(1000, 19, 2, 6);
  

  return 0;
}
