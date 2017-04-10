#include <iostream>
template<typename T>
struct A{
  A(){std::cout << "A "; }
  explicit A(int i){std::cout << "A " << i << ' ';}
  ~A(){std::cout << "~A "; }
  explicit operator int() {return 5;}
};
int main(){

  //A<int> a = 42;//Does not compile, missing template arguments before 'a'

  //  A a{13};    //output A 13 ~ A
  A<int> a;
  std::cout << static_cast<int>(a) << '\n';
  return 0;
}
