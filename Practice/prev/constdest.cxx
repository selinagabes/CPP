#include <iostream>

using namespace std;


template<class T>
class A{
  public:
    A::A(){ cout << "A " ;}
    A::~A(){cout << "~A ";}
};


template<class T>
class B : A<T>{
  public:
    B::B() {cout << "B ";}
    virtual B::~B(){ cout << "~B " ;}
};



template<class T>

class C : A<T>{
  public:
    C::C(){cout << "C ";}
    C::~C(){cout << "~C ";}
};



int main (){

  //Qu1
  //A a; B b;
  //Qu2


  A* a();


  //Qu3
  //Qu4


}
