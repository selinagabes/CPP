#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;

int main(){
  vector<int> v{0,1,1,5,5,5,7,8,12};

  std::copy(v.begin(),v.end(),std::ostream_iterator<int>(std::cout, " "));
  cout << endl;
  auto lb=lower_bound(v.begin(), v.end(), 5);
  auto ub=upper_bound(v.begin(), v.end(), 5);

  cout << "lower_bound: " << *lb

    << " found at index " << distance(v.begin(), lb) <<endl;
  cout << "upper_bound: " << *ub
    << " found at index " << distance(v.begin(), ub) <<endl;
  cout << "sub vector: " ;
  std::copy(lb, ub, std::ostream_iterator<int>(std::cout, " "));
  cout << endl;
  return 0;

}
