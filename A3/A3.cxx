#include <iostream>

using namespace std;
class ship{
	explicit ship(std::string const& name)
	:name_(name){

	}
	explicit ship(std::string&& std::move(name))
	:name_(name){
		
	}
private:
	string name_;
	ship();
	ship(const ship& that)=delete;
	ship& operator=(const ship&) = delete;

}
﻿﻿
// template <typename ShipType, typename... Args>
// std::unique_ptr<ship> create_ship(Args&&... args)
// {
//   return std::unique_ptr<ship>(new ShipType(std::forward<Args>(args)...));
// }

// template <typename ShipType, typename... Args>
// std::unique_ptr<ShipType> create_ship_as_is(Args&&... args)
// {
//   return std::unique_ptr<ShipType>(new ShipType(std::forward<Args>(args)...));
// }

// void sail_all_ships(vector<unique_ptr<ship>>& ships)
// {
//   for (auto& ship : ships)
//     ship->sail();
// }

// void fight(battle_ship& b1, battle_ship& b2)
// {
//   b1.fire_weapon();
//   b2.fire_weapon();
// }

// void sail(unique_ptr<ship> const& s)
// {
//   s->sail();
// }

int main (){

	return 0;
}