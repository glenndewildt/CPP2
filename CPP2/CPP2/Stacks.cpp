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

Stacks::Stacks()
{
}

Stacks::~Stacks()
{
}

void Stacks::initBuildingCards()
{
	BuildingCardsReader buildReader;
	vector<BuildingCard> carts = buildReader.read_file("bouwkaarten.csv");
	for (BuildingCard cart : carts) {
		buildingCards.push_front(cart);
	}
}

void Stacks::initCharacterCards()
{
	CharacterCardReader charReader;
	vector<CharacterCard> carts = charReader.read_file("karakterkaarten.csv");
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