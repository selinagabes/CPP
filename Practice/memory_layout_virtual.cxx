#include <iostream>

using namespace std;

struct A { A(){cout << "int a;\n";} int a; };
struct B { B(){cout << "int b;\n";} int b; };
struct C : A { C(){cout << "int c;\n";} int c; };
struct D : virtual B { D(){cout << "int d;";} int d; };
struct E : virtual B { E(){cout << "int e;\n";}int e; };
struct F : D, E { F(){cout << "int f;\n";}int f; };


//If struct F was written AS AS SINGLE STRUCT
//Preserve its data members' memory
//IE NO DUPLICATES K
//

struct F_as_one_struct{
  int b;
  int d;
  int e;
  int f;
};

int main(){
	F f;
}

