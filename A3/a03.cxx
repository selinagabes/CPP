//Nikola Zjalic
//Assign3
//ao3.cxx

#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <memory>

using namespace std;


class ship{
private:
	std::string name_;

public:

		ship() = delete; //delete default, copy and copy assign op
		virtual ~ship(){};

		ship(ship const&) = delete;
		ship& operator = (ship const&) = delete;


		ship(ship&& s) : name_(s.name_){}
		ship& operator = (ship&&){return *this;}


		//explicitly defined copy and move

		explicit ship(std::string const& name): name_(name){}
		explicit ship(std::string&& name): name_(std::move(name)){}

		virtual void sail()=0;

		virtual std::string get_ship_type() const{
			return std::string("ship");
		}

		std::string const& get_name() const{
			return name_;
		}

		std::string get_type_name_addr() const{
			std::ostringstream buf;
			buf << get_ship_type() << ' ' << get_name() << " [" << this << ']';
			return buf.str();
		}

	protected:
		void do_ship_only_move(ship&& s){
			name_ = std::move(s.name_);
		}
	};

	class battle_ship : public virtual ship {
	public:

	//delete default, copy and copy assign
		battle_ship() = delete;
		battle_ship(battle_ship const&) = delete;
		battle_ship& operator = (battle_ship const&) = delete;

		battle_ship(battle_ship&& bs) : ship::ship(move(bs)){}

	battle_ship& operator = (battle_ship&& bs){ //move constructor
		ship::operator=(std::move(bs));
		do_battle_ship_only_move(std::move(bs));
		return *this;
	}	

	explicit battle_ship(std::string&& name): ship(name){}  //move constructor forn name
	explicit battle_ship(std::string const& name): ship(name){} // constructor for name


	std::string get_ship_type() const override{
		return std::string("battle_ship");
	}

	void sail() override{
		std::cout << get_type_name_addr() << " off to war!\n";
	}


	virtual void fire_weapon(){
		cout << get_type_name_addr() << " firing cannon!\n";
	}
	

protected:
	void do_battle_ship_only_move(battle_ship&&){}

};

template <typename T>
class cargo_ship : public virtual ship 
{
public:
	//delete these guys again
	cargo_ship() = delete;
	cargo_ship(cargo_ship const&) = delete;
	cargo_ship& operator =(cargo_ship const&) = delete;

	cargo_ship(cargo_ship&& cs):ship(std::move(cs)) {

		this->cargo_(move(cs.cargo_));
	};

	cargo_ship & operator = (cargo_ship && cs){
		ship::operator=(std::move(cs));
		do_cargo_ship_only_move(std::move(cs));
		return *this;
	}

	explicit cargo_ship(string const& name): ship(name) {}
	explicit cargo_ship(string && name): ship(move(name)){}




  template<typename Iter>
	explicit cargo_ship(string name, Iter first, Iter last): ship(name), cargo_(first, last) {}

	string get_ship_type() const override {
		return std::string("cargo_ship");
	}

	void sail() override {
		cout << get_type_name_addr() << " setting sail!\n";
	}



	std::vector<T>& get_cargo() {
		return cargo_;
	}

	std::vector<T> const& get_cargo() const{ //
		return cargo_;
	}
protected:
	std::vector<T> cargo_;
	void do_cargo_ship_only_move(cargo_ship&& cs)
	{
		this->cargo_(std::move(cs.cargo_));
	}
};

template<typename T>
class smuggler_ship : public virtual cargo_ship<T> , public virtual battle_ship{
public:

	//delete all the tings!!!

	smuggler_ship() = delete;
	smuggler_ship(smuggler_ship const&) = delete;
	smuggler_ship & operator = (smuggler_ship const&) = delete;

  smuggler_ship(smuggler_ship&& ss) : ship(std::move(ss)), cargo_ship<T>(std::move(ss)), battle_ship(std::move(ss)){}
	smuggler_ship& operator=(smuggler_ship&& ss) {
			ship::operator=(std::move(ss));
		do_smuggler_ship_only_move(move(ss));
		return *this;
	}

    //make 2 so that the compiler knows which class to reference
	explicit smuggler_ship(std::string&& name): ship(name), cargo_ship<T>(name), battle_ship(name){}//move constructor forn name
	explicit smuggler_ship(std::string const& name): ship(name), cargo_ship<T>(name), battle_ship(name){}// constructor for name


	std::string get_ship_type() const override {
		return std::string("smuggler_ship");
	}

	void sail() override{
		cout << get_type_name_addr() << " off to pillage!\n";
	}

	void fire_weapon() override{
		cout << get_type_name_addr() << " launching harpoon!\n";
	}

	void steal(ship& s){

		cargo_ship<T>* cs = dynamic_cast<cargo_ship<T>*>(&s);
		smuggler_ship<T>* ss= dynamic_cast<smuggler_ship<T>*>(&s);


		if(ss) {
			std::ostringstream buf;
			buf << get_type_name_addr() 
				<< " cannot steal from another" 
				<< ss->get_type_name_addr() << '!';
			throw runtime_error{buf.str()};
		}else if( cs ) {

			std::cout << get_type_name_addr() << "is stealing half of " 
			<< cs->get_type_name_addr() << "!\n";

			auto middle = begin(cs->get_cargo()) 
			+ distance(begin(cs->get_cargo()), end(cs->get_cargo())) / 2;

			for(auto e = middle; e != end(cs->get_cargo()); ++e)
			{
        this->cargo_.emplace_back(std::move(*e)); //e is what the current iterator points to
      }

      cs->get_cargo().erase(middle, end(cs->get_cargo()));

    }
    else {
    	std::ostringstream buf;
    	buf << get_type_name_addr() 
    		<< " cannot steal from a " 
    		<< s.get_type_name_addr() << "!";
    	throw runtime_error{buf.str()};
    }
  }

protected:
	void do_smuggler_ship_only_move(smuggler_ship&& ss) {
		ship::do_ship_only_move(ss);
		cargo_ship<T>::do_cargo_ship_only_move(ss);

		battle_ship::do_battle_ship_only_move(ss);
	}  

};



void sail_all_ships(vector<unique_ptr<ship>>& ships)
{
	for (auto& ship : ships)
		ship->sail();
}

void fight(battle_ship& b1, battle_ship& b2)
{
	b1.fire_weapon();
	b2.fire_weapon();
}

void sail(unique_ptr<ship> const& s)
{
	s->sail();
}

template <typename ShipType, typename... Args>
std::unique_ptr<ship> create_ship(Args&&... args)
{
	return std::unique_ptr<ship>(new ShipType(std::forward<Args>(args)...));
}

template <typename ShipType, typename... Args>
std::unique_ptr<ShipType> create_ship_as_is(Args&&... args)
{
	return std::unique_ptr<ShipType>(new ShipType(std::forward<Args>(args)...));
}

int main()
{
  // Using a unique_ptr to own each ship and ensure its eventual destruction...
	vector<unique_ptr<ship>> shipyard;
	shipyard.emplace_back(create_ship<cargo_ship<int>>("Storage Master"));
	shipyard.emplace_back(create_ship<battle_ship>("HMS Awesome"));
	shipyard.emplace_back(create_ship<smuggler_ship<int>>("Bounty Hunter"));

  // NOTE: If uncommented, the next line should compile...
	//vector<unique_ptr<ship>> shipyard_new_location(std::move(shipyard));

  // NOTE: If uncommented, the next line should NOT compile...
  //vector<unique_ptr<ship>> wont_work2(shipyard);

  // NOTE: If uncommented, the next declaration should NOT compile...
  /*vector<smuggler_ship<int>> wont_work2{
    smuggler_ship<int>("Thief 1 Ship"),
    smuggler_ship<int>("Thief 2 Ship")
  };*/

  // Anchors aweigh!
	cout << "Sail all ships...\n";
	sail_all_ships(shipyard);

  // Create a fight between two battle ships.
	cout << "\nFight between b1 and b2...\n";
	battle_ship b1("HMCS Hunter");
	battle_ship b2("SS Teacup");
	fight(b1, b2);

  // A battle ship fights a smuggler ship....
	cout << "\nFight b1 with last ship in shipyard...\n";
	fight(
		b1,
		dynamic_cast<smuggler_ship<int>&>(*shipyard.back())
		);

	cout << "\nSail one-by-one...\n";
	for (auto const& i : shipyard)
		sail(i);

	cout << "\nSteal cargo...\n";
	auto ss = create_ship_as_is<smuggler_ship<int>>("Pirate Ship");
	auto cs = create_ship<cargo_ship<int>>("Cargo Ship");
	auto bs = create_ship<battle_ship>("Non-Cargo Ship");
	ss->steal(*cs);
	try { ss->steal(*bs); } catch (exception& e) { cout << e.what() << '\n'; }

}