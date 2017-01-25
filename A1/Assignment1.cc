
#include <cstring>
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;
int main(){  

	map<string, unsigned> frequency; 
	string word; 
	while(cin >> word){
		++frequency[word];
		cout << word << "\n";
	}
	auto const longest_entry_in_map = max_element(
		begin(frequency), end(frequency),
		[](const pair<string, unsigned> & w1, const pair<string, unsigned> & w2){
			return w1.first.size() < w2.first.size();
		}
		);
	int longestWord = (longest_entry_in_map != end(frequency))? longest_entry_in_map->first.size(): 0;
	if(longestWord!=0)
		cout << "\n\n";

	cout << "An unsorted frequency histogram of the input is:\n";
	for_each(
		begin(frequency), end(frequency),
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

