
#include <cstring>
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <iomanip>
//--Gabriele, Selina 
//--103 753 256
//-- January 2017
//***Assignment 1***
//I: List of strings
//O:  Frequency Histogram
using namespace std;
int main(){  
	//Hash map to store words and frequencies
	map<string, unsigned> frequency; 
	string word; 

	//Read input
	while(cin >> word){
		++frequency[word];
		cout << word << "\n";
	}

	//use max_element to find the largest element
	auto const longest_entry_in_map = max_element(
		begin(frequency), end(frequency),
		[](const pair<string, unsigned> & w1, const pair<string, unsigned> & w2){
			//lambda to select properly from pair in frequency
			return w1.first.size() < w2.first.size();
		}
	);
	//store the variable of the longest word
	int longestWord = (longest_entry_in_map != end(frequency))? longest_entry_in_map->first.size(): 0;
	//for output 
	if(longestWord!=0)
		cout << "\n\n";

	cout << "An unsorted frequency histogram of the input is:\n";
	for_each(
		begin(frequency), end(frequency),
		[longestWord](auto const & elem){
			cout 
			<< left 												//left justify
			<< setfill(' ') 								//fill character
			<< setw(longestWord) 						//fill width
			<< elem.first 									//word 
			<<" |" 													
			<< setfill('*') 
			<< setw(elem.second) 
			<< "" 
			<< '\n';
		});
}

