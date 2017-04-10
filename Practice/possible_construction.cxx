#include <iostream>

using namespace std;
struct myClass{
  myClass(double d){
    d_ = d;
  }
  double d_;
};

int main(){
  myClass  f(3.2);
  //double f(3.2);

  cout << f.d_ << endl;
  return 0;
}
