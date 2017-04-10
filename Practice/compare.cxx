#include <iostream>
#include <cstring>
#define compare_m(a,b) (!((a)<(b)) - !((b)<(a)))


using namespace std;
template <typename T>
inline int compare(T const& a, T const& b){
  return !(a <b) - !(b <a);
}

inline int compare(const char* c_string1, const char* c_string2){
  using std::strcmp;
  return strcmp(c_string1, c_string2);
}

int main(){
  int e1 =1;
  int e2 =2;
  cout << compare_m(e1+e1, e2) << endl;
  return 0;
}
