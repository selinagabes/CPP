#include <iostream>

typedef int (*func_ptr)(int, int);

int mult(int a, int b){return a*b;}

int main(){
  func_ptr fptr = &mult;
  int num = (*fptr)(2,5);
  std::cout << num << std::endl;
  return 0;
}
