#include <iostream>

using namespace std;


template <class T>
class Selina{
  T first, second;
public:
  Selina(T a, T b){
  first = a;
  second = b;
  }
  T bigger();
};

template <class T>
T Selina<T>::bigger(){
  return (first>second?first:second);
}



int main(){
  Selina<int> bo(69, 105);
  cout << bo.bigger() << endl;
}
