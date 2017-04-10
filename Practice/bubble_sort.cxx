#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

template<typename FwdIter>
void bubble_sort(FwdIter first, FwdIter last){
  if(first==last) return ;
  bool swapped;
  do{
    swapped = false;
    FwdIter prev = first;

    for(FwdIter curr=next(first); curr!= last;++curr,++prev){
      if(*curr < *prev)
      {
        auto tmp = *curr;
        *curr = *prev;
        *prev = tmp;
        swapped = true;
      }
    }
  }
  while(swapped);
}

int main(){
  vector<int> data;
  int nums;
  while(cin >> nums){
    data.push_back(nums);
  }

  //Ctrl-D to end input
  for(auto i: data)
    cout << i<<' ';
  cout << endl;
  auto first = begin(data);
  auto last = end(data);
  bubble_sort(first, last);

  for(auto i: data)
    cout << i <<' ';
  cout << endl;

}
