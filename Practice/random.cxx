#include <iostream>
#include <random>

using namespace std;

int main(){
  std::random_device rd;
  std::default_random_engine eng;
  std::uniform_int_distribution<unsigned> ud{5,45};
  for(int i =0; i < 10; ++i)
    cout << ud(eng) <<' ';
}
