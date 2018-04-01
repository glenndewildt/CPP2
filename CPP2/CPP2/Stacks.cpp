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

const BuildingCard Stacks::getBuildingCard() {
	if (buildingCards.size() == 0) {
		buildingCards = discardedBuildings;
		discardedBuildings.clear();
	}

	const BuildingCard rv = buildingCards.front();
	buildingCards.pop_front();
	return rv;
}

const int Stacks::getAmountOfCharacterCards() {
	return characterCards.size();
}

const CharacterCard Stacks::getCharacterCard(const int optionID)
{
	CharacterCard rv = characterCards[optionID - 1];
	characterCards.erase(characterCards.begin() + optionID - 1);
	return rv;
}

void Stacks::addCharacterCard(const CharacterCard card)
{
	characterCards.push_back(card);
}

void Stacks::undiscardCharacterCards()
{
	deque<CharacterCard>::iterator it;

	for (it = discardedCharacters.begin(); it != discardedCharacters.end(); it++)
	{
		characterCards.push_back(*it);
	}

	discardedCharacters.clear();
}

const std::string Stacks::removeCharacterCard(const int optionID)
{
	discardedCharacters.push_back(characterCards[optionID - 1]);
	characterCards.erase(characterCards.begin() + optionID - 1);

	return discardedCharacters.back().getName();
}

const std::string Stacks::getCharacterCardOptions()
{
	std::string options{ "" };

	deque<CharacterCard>::iterator it;

	for (it = characterCards.begin(); it != characterCards.end(); it++)
	{
		options.append(std::to_string(it - characterCards.begin() + 1) + ": ");
		options.append(it->getName() + "\r\n");
	}

	return options;
}