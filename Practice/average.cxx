#include <iostream>
#include <limits>
using namespace std;

template <typename T>
T average(std::initializer_list<T> const & il, T const& error=T{}){
  if(il.size() > 0){
    double sum = 0.0;
    for(auto i : il)
      sum += i;


    return sum/il.size();
  }
  return error;
}

int main(){
 cout << average({}, std::numeric_limits<double>::quiet_NaN()) << endl;
 cout << average({1.0}, std::numeric_limits<double>::quiet_NaN()) << endl;
 cout << average({1.0,2.0}, std::numeric_limits<double>::quiet_NaN()) << endl;
 cout << average({1.0,2.0,3.0}, std::numeric_limits<double>::quiet_NaN()) << endl;
 cout << average({1.0,2.0,3.0,4.0}, std::numeric_limits<double>::quiet_NaN()) << endl;
}
