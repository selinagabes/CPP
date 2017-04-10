#include <iostream>

using namespace std;

struct A { A(){cout << "int a;\n";} int a; };
struct B { B(){cout << "int b;\n";} int b; };
struct C : A { C(){cout << "int c;\n";} int c; };
struct D : B { D(){cout << "int d;\n";} int d; };
struct E : B, C { E(){cout << "int e;\n";}int e; };
struct F : D, E { F(){cout << "int f;\n";}int f; };


//If struct F was written AS AS SINGLE STRUCT
//Preserve its data members' memory
//IE NO DUPLICATES K
//

struct F_as_one_struct{
  int b;
  int d;
  //int b; //memory layout
  int a;
  int c;
  int e;
  int f;
};

int main(){
	F f;
}

