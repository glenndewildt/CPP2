#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "building_cart.h"


using namespace std;
class building_carts_reader
{
public:
	building_carts_reader();
	std::vector<building_cart> read_file(std::string);
	virtual ~building_carts_reader();
};

