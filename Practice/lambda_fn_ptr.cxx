#include <iostream>

void do_something(void (*callback)()){
  callback();
}

int main(){
  auto callback=[](){std::cout<<"callback called\n";};
  do_something(callback);

  return 0;
}

