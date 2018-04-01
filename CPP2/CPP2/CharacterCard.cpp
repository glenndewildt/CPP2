#include "CharacterCard.h"

CharacterCard::CharacterCard(const int id, const std::string name) : id{ id }, name{ name }
{
	if (name == "Moordenaar")
		characterType = Moordenaar;
	else if (name == "Dief")
		characterType = Dief;
	else if (name == "Magi�r")
		characterType = Magier;
	else if (name == "Koning")
		characterType = Koning;
	else if (name == "Prediker")
		characterType = Prediker;
	else if (name == "Koopman")
		characterType = Koopman;
	else if (name == "Bouwmeester")
		characterType = Bouwmeester;
	else if (name == "Condottiere")
		characterType = Condottiere;
}


int CharacterCard::getId()
{
	return id;
}

std::string CharacterCard::getName()
{
	return name;
}

void CharacterCard::setId(int id)
{
	this->id = id;
}

void CharacterCard::setName(std::string name)
{
	this->name = name;
}