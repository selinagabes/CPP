//Nikola Zjalic
//a04.cxx
//Assignment 4


#include <map>
#include <stdexcept>
#include <memory>
#include <tuple>
#include <limits>
#include <iostream>

using namespace std;
using uint_type = unsigned long long;




class ack {
public:
	using tuple_type = std::tuple<uint_type, uint_type>;
	uint_type operator ()(uint_type const& m, uint_type const& n) const{
		return calculate({m,n});
	}

	uint_type calculate(tuple_type const& args) const{
		auto hasCache = cache_.find(args);
		
		if(hasCache != end(cache_))
			return hasCache->second;
		
		uint_type retval;
		uint_type const m = std::get<0>(args);
		uint_type const n = std::get<1>(args);

		//ack function

		if (m==0){
			if (n < std::numeric_limits<uint_type>::max())
				retval = n+1;
			else 
				throw std::range_error("ack::uint_type overflow");



		} else if (m > 0 && n==0){
			retval = calculate({m-1,1});
		} else if (m > 0 && n > 0){
			retval = calculate({m-1, calculate({m,n-1})});
		}
		return retval;
	}

private:
	static std::map<tuple_type, uint_type> cache_;
};

std::map<ack::tuple_type, uint_type> ack::cache_;

int main(){
	using std::cout; 
	ack f;	
	for(uint_type m = 0; m!=5; ++m){
		for(uint_type n=0; n!=10; ++n){
			cout << "f("<< m << ',' << n << ") = ";
			try{
				cout << f(m,n) << '\n';
			}
			catch(std::range_error &e){
				cout << e.what() << '\n';
			}
			catch(std::bad_alloc &e){
				cout << e.what() << '\n';
			}
			catch(...){
				cout << "<UNKNOWN ERROR>\n";
			}
		}
		cout << '\n';
	}

	return 0;
}