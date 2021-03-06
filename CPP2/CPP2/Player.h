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
#include <algorithm>   


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
	void pay_gold(const int& toPay) { gold -= toPay; }

	std::vector<CharacterCard>& getCharCards()  { return charCards; }
	void addCharCard(const CharacterCard& newCard) { charCards.push_back(newCard); }
	//void delCharCard(const CharacterCard& new_char_cart) { character_carts.erase(new_char_cart); }

	std::vector<BuildingCard>& getBuildingCards() { return buildingCards; }
	void addBuildingCard(const BuildingCard& newCard) { buildingCards.push_back(newCard); }
	void deleteBuildingCard(const BuildingCard& newCard) { 
	//	int counter = 0;
	//	for (BuildingCard& card: buildingCards) {
	//		if (card == newCard) {
	//			buildingCards.erase(buildingCards.begin() + counter);
	//		}
	//		counter++;
	//	}
	//	buildingCards.pop_back();
		//std::remove(buildingCards.begin(), buildingCards.end(), newCard); 
	}
	void switchCards(const vector<BuildingCard>& buidlings, const vector<CharacterCard>& charaCards) {
		buildingCards = buidlings;
		charCards = charaCards;
	
	}



	std::vector<BuildingCard>& getBuildings() { return buildedBuildings; }
	void buildBuildingCard(const BuildingCard& newCard) { buildedBuildings.push_back(newCard); }
	void deletebuildBuildingCard(const BuildingCard& newCard) { std::remove(buildedBuildings.begin(), buildedBuildings.end(), newCard); }


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
