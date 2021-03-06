#pragma once
#include <iostream>
using namespace std;

class BuildingCard
{
public:
	BuildingCard() :kind{ "" }, cost{ "" }, color {""} {};
	BuildingCard(const string kind, const string cost,const string color) : kind{ kind }, cost{ cost }, color{color} {};
	~BuildingCard() {};

	friend ostream& operator<<(ostream& strm, BuildingCard & cart) {
		if (strm) {
			strm << cart.kind.c_str();
		}

		return strm;
	}
	friend bool operator==(BuildingCard const a, BuildingCard const b)
	{
		bool t = a.kind == b.kind;
		return t;
	}
	BuildingCard& operator=(const BuildingCard& other) {
		kind = other.kind;
		cost = other.cost;
		color = other.color;
		return *this;
		
	}
	const bool is_empty() const {
		
		return kind.empty() && cost.empty()&&color.empty();
	}


	const string get_kind() const { return kind; }
	const string get_cost() const { return cost; }
	const string get_color() const { return color; }

private:
	 string kind;
	 string cost;
	 string color;

};

