
#include <cstring>
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;


class Histogram{
public:
  map<string, unsigned> frequency; 
  void operator()(const string & item){
    ++frequency[item];
  }

  operator map<string, unsigned>(){return frequency;}
};

void printFile(int longestWord, Histogram & histogram){
 cout << "\n\n" << "An unsorted frequency histogram of the input is:\n";
 for_each(
   begin(histogram.frequency), end(histogram.frequency),
   [longestWord](auto const & elem){
    cout 
      << left 
      << setfill(' ') 
      << setw(longestWord) 
      << elem.first 
      <<" |" 
      << setfill('*') 
      << setw(elem.second) 
      << "" 
      << '\n';
  });
}

void processFile(Histogram & histogram){

 auto const longest_entry_in_map = max_element(
     begin(histogram.frequency), end(histogram.frequency),
     [](const pair<string, unsigned> & w1, const pair<string, unsigned> & w2){
      return w1.first.size() < w2.first.size();
    }
    );
 int longestWord = (longest_entry_in_map != end(histogram.frequency))? longest_entry_in_map->first.size(): 0;

 printFile(longestWord, histogram);

}
void readInput(){
  Histogram histogram; 
  string word; 
  while(cin >> word){
    histogram(word);
    cout << word << "\n";
  }
  processFile(histogram);
}
void writeFile(  ifstream  & file, Histogram  & histogram  ){
  string word; 
  while(file >> word){
    histogram(word);
    cout << word << "\n";
  }
}

void readFile(){
  ifstream file; 
  file.open("input.txt");  
  if(!file) 
    cout << "there is not file!\n";
  Histogram histogram;
  writeFile(file, histogram);
  processFile(histogram);
  
}
void run(){
  readInput();
  //readFile();
}
int main(){  
  run();
}






