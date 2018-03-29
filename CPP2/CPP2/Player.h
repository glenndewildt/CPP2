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


	const int id;

private:
    std::string name;
	int gold;
	std::vector<CharacterCard> character_carts;
	std::vector<BuildingCard> building_carts;

};

#endif /* Player_hpp */
