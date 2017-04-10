#include <iostream>

using namespace std;

int foo(char c){return 3;}

int main (){

  decltype(foo('a')) value = foo('a');
cout << value << endl;
}
