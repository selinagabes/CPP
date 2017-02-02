#include <algorithm>
#include <iostream>
#include <string>
#include <list> // doubly-linked list
#include <sstream> // string stream (i.e., memory buffer)
#include <cctype> // <ctype.h> => <cctype>
#include <iterator> // ostream_iterator<T>



template <typename TrimPredicate>
auto string_range_trim(std::string& str, TrimPredicate p) ->
std::pair<std::string::iterator,std::string::iterator>
{
	auto last_non_ws_iter = end(str);
	auto first_non_ws_iter = find_if_not(begin(str), last_non_ws_iter, p);
	return std::make_pair(first_non_ws_iter, last_non_ws_iter);

}
using namespace std;

int main(){
	string word= " I am trimming this "; 

	auto stringTrim = string_range_trim(word, [](auto ch) {
		return isspace(ch, cin.getloc()); } 
		);
	copy(
		stringTrim.first, stringTrim.second,
		ostream_iterator<char>(cout)
		);
	cout << '\n';
}