#include <iostream>

using namespace std;

bool equiv(int a, int b){
  return (!(a<b) && !(b<a));
}

bool not_equiv(int a, int b){
  //return !(equiv(a,b));
  //return !(!(a<b) && !(b<a));
  return ((a<b) || (b<a));
}

bool less_than(int a, int b){
  //return ((a<b) && not_equiv(a,b));
  //return ((a<b) && !(!(a<b) && !(b<a))
  return ((a<b) && (a<b || b < a));
}

bool less_than_or_equiv(int a, int b){
  //return (less_than(a,b) || equiv(a,b));
  return (((a<b) && (a<b || b<a)) || (!(a<b) && !(b<a)));
}

bool greater_than_or_equiv(int a, int b){
  //return !(less_than(a,b));
  return !((a<b) && !(!(a<b) && !(b<a)));
}

bool greater_than(int a, int b){
  //return !less_than_or_equiv(a,b);
  return !(((a<b) && (a<b || b<a)) || (!(a<b) && !(b<a)));
}

int max(int a, int b){
  return greater_than(a,b)? a: b;
}

int main(){
  cout << "0 == 0: " << equiv(0,0) << endl;
  cout << "1 != 0: " << not_equiv(1,0) << endl;
  cout << "0 <  1: " << less_than(0,1) << endl;
  cout << "1 <= 1: " << less_than_or_equiv(1,1) << endl;
  cout << "1 >= 1: " << greater_than_or_equiv(1,1) << endl;
  cout << "2 >  1: " << greater_than(2,1) << endl;
  cout << "max(2,2):" << max(1,2) << endl;
  return 0;
}

