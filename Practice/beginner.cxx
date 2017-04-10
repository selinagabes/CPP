#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

int main(){
  vector<string> values;

  std::string s;

  while( cin >> s)
    values.push_back(s);

  for(auto const& o : values)
    cout << o << endl;

  return 0;
}


