#include "Stacks.h"

#include <string>
#include <iterator>
#include <vector>
#include <ctime>
#include <algorithm>
#include "Globals.h"
#include "BuildingCardsReader.h"
#include "CharacterCardReader.h"

using namespace std;
//stacks

Stacks::Stacks()
{
}

Stacks::~Stacks()
{
}

void Stacks::initBuildingCards()
{
	BuildingCardsReader reader;
	vector<BuildingCard> carts = reader.read_file("bouwkaarten.csv");
	for (BuildingCard cart : carts) {
		buildingCards.push_front(cart);
	}
}

void Stacks::initCharacterCards()
{
	CharacterCardReader reader1;
	vector<CharacterCard> carts = reader1.read_file("karakterkaarten.csv");
	for (CharacterCard cart : carts) {
		characterCards.push_front(cart);
	}
}

void Stacks::shuffleBuildingCards()
{
	std::random_shuffle(buildingCards.begin(), buildingCards.end());

}

void Stacks::shuffleCharacterCards()
{
	std::random_shuffle(characterCards.begin(), characterCards.end());

}