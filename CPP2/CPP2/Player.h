//
//  Player.hpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <string>
#include<vector>
#include "BuildingCard.h"
#include "CharacterCard.h"

class Player {
public:
	Player(const int id, const std::string& name) : name{ name }, id{ id } {}

    std::string get_name() const { return name; }
    void set_name(const std::string& new_name) { name = new_name; }

	int get_gold() const { return gold; }
	void set_gold(const int& new_gold) { gold = new_gold; }


	std::vector<CharacterCard> get_character_carts() const  { return character_carts; }
	void add_character_cart(const CharacterCard& new_char_cart) { character_carts.push_back(new_char_cart); }
	//void delete_character_cart(const CharacterCard& new_char_cart) { character_carts.erase(new_char_cart); }

	std::vector<BuildingCard> get_building_carts() const { return building_carts; }
	void add_building_cart(const BuildingCard& new_char_cart) { building_carts.push_back(new_char_cart); }
	//void delete_building_cart(const BuildingCard& new_char_cart) { building_carts.erase(new_char_cart); }



	const int id;

private:
    std::string name;
	int gold;
	std::vector<CharacterCard> character_carts;
	std::vector<BuildingCard> building_carts;

};

#endif /* Player_hpp */
