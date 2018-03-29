#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "CharacterCard.h"
using namespace std;
class CharacterCardReader
{
public:
	CharacterCardReader();
	virtual ~CharacterCardReader();
	std::vector<CharacterCard> read_file(std::string);

};

