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
#include <vector>

#include "BuildingCard.h"
#include "CharacterCard.h"

class Player {
public:
	Player(const int id, const std::string& name) : name{ name }, id{ id } {}

    std::string get_name() const { return name; }
    void set_name(const std::string& new_name) { name = new_name; }

	int get_gold() const { return gold; }
	void set_gold(const int& new_gold) { gold = new_gold; }
	void add_gold(const int& toAdd) { gold += toAdd; }

	std::vector<CharacterCard> getCharCards() const { return charCards; }
	void addCharCard(const CharacterCard& newCard) { charCards.push_back(newCard); }
	//void delCharCard(const CharacterCard& new_char_cart) { character_carts.erase(new_char_cart); }

	std::vector<BuildingCard> getBuildingCards() const { return buildingCards; }
	void addBuildingCard(const BuildingCard& newCard) { buildingCards.push_back(newCard); }
	//void delBuildingCard(const BuildingCard& newCard) { building_carts.erase(newCard); }

	std::vector<BuildingCard> getBuildings() const { return buildedBuildings; }
	void buildBuildingCard(const BuildingCard& newCard) { buildedBuildings.push_back(newCard); }

	void setKing() { king = true; };
	void unsetKing() { king = false; };
	bool isKing() const { return king; };
	const int id;

private:
	int gold { 2 };
	bool king{ false };
	std::string name;
	std::vector<CharacterCard> charCards;
	std::vector<BuildingCard> buildingCards;
	std::vector<BuildingCard> buildedBuildings;
};

#endif /* Player_hpp */
