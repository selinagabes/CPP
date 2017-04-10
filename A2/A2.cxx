#include <string>
#include <locale>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator> 
#include <fstream>

using namespace std;
template <typename TrimPredicate>
auto string_range_trim(std::string& str, TrimPredicate p) ->
std::pair<std::string::iterator,std::string::iterator>
{
auto last_non_ws_iter = find_if_not(rbegin(str), rend(str), p).base();
auto first_non_ws_iter = find_if_not(begin(str), last_non_ws_iter, p);
return std::make_pair(first_non_ws_iter, last_non_ws_iter);

}
auto read_next_termcap_entry(istream& is) ->
std::pair<bool, string>
{
ostringstream buffer; 
auto loc = is.getloc();
buffer.imbue(loc);
bool data = false;
bool continued_line = true; 
for (
	string line;
	(!data | continued_line) 
		&& getline(is,line);
	)
{
	continued_line = false; 
	if(line.empty())
		continue;
	auto trimmed_range = string_range_trim(line, 
		[loc](auto ch){
			return isspace(ch, loc); } 
			);
	if(trimmed_range.first == trimmed_range.second)
		continue;		
	if(*trimmed_range.first == '#')
		continue;

	auto last_char_iter = trimmed_range.second;
	--last_char_iter;
	if (*last_char_iter == '\\'){
		continued_line = true;
		if(trimmed_range.first == last_char_iter)
			continue;	

		trimmed_range.second = last_char_iter;
	}
	bool const header_line = isalpha(*trimmed_range.first, loc);
	bool const capability_line = find(begin(line), trimmed_range.first,'\t') != trimmed_range.first;
	if (header_line || capability_line)
	{
		data = true;
		copy(
			trimmed_range.first,
			trimmed_range.second,
			ostreambuf_iterator<char>(buffer));
	}
}



return { data, buffer.str() };

}
int main(int argc, char *argv[]){

if (argc != 2)
{
	cerr << "Usage: " << argv[0] << " <termcap_db_file>\n";
	return 1;
}
ifstream in(argv[1]);
if (!in)
{
	cout << "Unable to open file. Aborting.\n";
	return 2;
}
in.imbue(std::locale::classic());
for (
	auto result = read_next_termcap_entry(in);
	result.first;
	result = read_next_termcap_entry(in)
	)
	cout << result.second << '\n';
return 0;
}