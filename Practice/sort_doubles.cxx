#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

/*
   template <typename InIter, typename T>
   InIter find(InIter first, InIter last, T const& value);

   template <typename FwdITer, typename T>
   FwdIter lower_bound(FwdIter first, FwdIter last, T const& value);

   template <typename FwdIter, typename T>
   FwdIter upper_bound(FwdIter first, FwdIter last, T const& value);

   template <typename RndIter>
   void sort(RndIter first, RndIter last);
   */


int main(){

  vector<double> data;
  double d;
  while(cin >> d)
    data.push_back(d);


  if(data.size() == 0)
    throw("Empty list\n");

  cout << "INPUT DONE" << endl;
  sort(data.begin(), data.end());

  auto lower = std::lower_bound(data.begin(), data.end(), 0.0);
  auto upper = std::upper_bound(data.begin(), data.end(), 100.0);

  //std::copy(lower, upper, std::ostream_iterator<double>(std::cout, "\n"));

   for(auto const& i: data)
     cout << i<< ' ';
   cout << endl;
  return 0;
}
