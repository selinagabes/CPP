#include <iostream>
using namespace std;

template <typename T>
struct A {
  A() { cout << "A "; }
  ~A() { cout << "~A ";}

};

template <typename T>
struct B : A<T>{
  B(){ cout << "B "; }
  virtual ~B(){ cout << "~B "; }
};


template <typename T>
struct C : A<T>{
  C(){ cout << "C ";}
  ~C(){cout << "~C ";}
};

int main(){
    cout << "QUESTION 1:" << endl << "A<int> a; \nB<int> b;" << endl;
      A<int> a1;
      B<int> b1;
      cout << endl;

  /*
     cout << "QUESTION 2:" << endl << "A<int>* a= new B<int>" << endl;
     A<int>* a2 =  new B<int>;
     cout << endl;

*/
  /*
     cout << "QUESTION 3:" << endl << "A<int>* a = new A<int>; \n delete a;" << endl;
     A<int>* a3 = new A<int>;
     delete a3;
     cout << endl;
     */

  /*
     cout << "QUESTION 3:" << endl << "A<int>* a = new B<int>; \n delete a;" << endl;
     A<int>* a4 = new B<int>;
     delete a4;
     cout << endl;
     */



}
