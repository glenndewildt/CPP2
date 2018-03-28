#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "BuildingCard.h"


using namespace std;
class BuildingCardsReader
{
public:
	BuildingCardsReader();
	std::vector<BuildingCard> read_file(std::string);
	virtual ~BuildingCardsReader();
};

