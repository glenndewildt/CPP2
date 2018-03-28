#pragma once
#include <iostream>
using namespace std;

class building_cart
{
public:
	building_cart() :kind{ "" }, cost{ "" }, color {""} {};
	building_cart(const string kind, const string cost,const string color) : kind{ kind }, cost{ cost }, color{color} {};
	~building_cart() {};

/*	friend ostream& operator<<(ostream& strm, building_cart & cart) {
		if (strm) {
			strm << cart.kind;
		}

		return strm;
	}
*/
	const bool is_empty() const {
		
		return kind.empty() && cost.empty()&&color.empty();
	}


	const string get_kind() const { return kind; }
	const string get_cost() const { return cost; }
	const string get_color() const { return color; }

private:
	const string kind;
	const string cost;
	const string color;


};

