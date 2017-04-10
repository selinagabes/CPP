
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;



template <typename Iter>
Iter iter_min(Iter a, Iter b){
  //  cout << *a << ' ' << *b;

  // auto ret = ((*a<*b) && !(!(a<b) && !(b<a))) ? a : b;
  // cout << "min:"<< *ret << endl;
  return ((*a<*b) && !(!(a<b) && !(b<a))) ? a : b;
}

template <typename Iter>
Iter iter_max(Iter a, Iter b){
  //cout << *a << '>' << *b;
  //auto ret = !(((*a<*b)&&(*a<*b || *b<*a)) || (!(a<b) && !(b<a))) ? a : b;
  //cout << "max:" << *ret << endl;
  return !(((*a<*b)&&(*a<*b || *b<*a)) || (!(a<b) && !(b<a))) ? a : b;
}

template <typename Iter>
auto find_pivot_iter(Iter first, Iter last){
  auto d = distance(first,last);

  switch(d){
    case 0: return last;
            break;
    case 1: return first;
            break;
    case 2: return iter_max(first,prev(last));
            break;
    default: return iter_min(iter_max(first,first+(d/2)), prev(last));
             break;

  }
}

template <typename Iter>
void quick_sort(Iter const& first, Iter const& last){


  using std::iter_swap;
  using std::partition;
  using std::prev;

  if(first == last) return; //empty list

  //Determine the pivot, returns an iterator
  auto const pivot = find_pivot_iter(first, last);

  //Swap the pivot with the first position
  iter_swap(first, pivot);

  //Partition excluding the pivot value now in *first
  auto pos_after_pivot = partition(next(first), last,
      [&first](auto const& v) {return v < *first;}
      );

  //If the first hald is not empty, move the pivot value back to
  //the correct palce; otherwise the pivot is in the correct place
  if(first != prev(pos_after_pivot)){
    auto prev_to_pos_after_pivot = prev(pos_after_pivot);
    iter_swap(first, prev_to_pos_after_pivot);

    //Sort the first half before the pivot
    quick_sort(first, prev_to_pos_after_pivot);
  }
  //Sort the second half after the pivot
  quick_sort(pos_after_pivot, last);
}


int main(){
  std::vector<int> unsorted{0,4,2,4,6,7,2,1,12,5,0};
  for(auto i = begin(unsorted); i!= end(unsorted);++i)
    cout << *i << ' ';

  cout << endl;
  quick_sort(begin(unsorted),end(unsorted));

  for(auto i = begin(unsorted); i!= end(unsorted);++i)
    cout << *i << ' ';
  cout << endl;
  return 0;
}

