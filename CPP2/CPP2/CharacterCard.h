#pragma once

#include <iostream>
using namespace std;
class CharacterCard
{
public:

	
	CharacterCard(const int id, const std::string name);
	~CharacterCard() {};
	
	enum CharType { Moordenaar, Dief, Magier, Koning, Prediker, Koopman, Bouwmeester, Condottiere, None };

	int getId() const;
	std::string getName() const;
	CharType getType() const;

	void setId(int);
	void setName(std::string);
	void setType(CharType);

private:
	int id;
	std::string name;

	CharType charType;
};
