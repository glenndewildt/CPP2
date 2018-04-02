#include "CharacterCard.h"

CharacterCard::CharacterCard(const int id, const std::string name) : id{ id }, name{ name }
{
	if (name == "Moordenaar")
		charType = Moordenaar;
	else if (name == "Dief")
		charType = Dief;
	else if (name == "Magiër")
		charType = Magier;
	else if (name == "Koning")
		charType = Koning;
	else if (name == "Prediker")
		charType = Prediker;
	else if (name == "Koopman")
		charType = Koopman;
	else if (name == "Bouwmeester")
		charType = Bouwmeester;
	else if (name == "Condottiere")
		charType = Condottiere;
}


int CharacterCard::getId() const
{
	return id;
}

std::string CharacterCard::getName() const
{
	return name;
}

CharacterCard::CharType CharacterCard::getType() const
{
	return charType;
}

void CharacterCard::setId(int id)
{
	this->id = id;
}

void CharacterCard::setName(std::string name)
{
	this->name = name;
}

void CharacterCard::setType(CharType type)
{
	this->charType = type;
}