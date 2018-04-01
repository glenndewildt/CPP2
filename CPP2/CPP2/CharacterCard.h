#pragma once

#include <iostream>
using namespace std;
class CharacterCard
{
public:

	
	CharacterCard(const int id, const std::string name);
	~CharacterCard() {};

	int getId();
	std::string getName();

	void setId(int);
	void setName(std::string);

	enum CharType { Moordenaar, Dief, Magier, Koning, Prediker, Koopman, Bouwmeester, Condottiere, None };

private:
	int id;
	std::string name;

	CharType characterType;
};
