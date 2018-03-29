#pragma once
#include <iostream>
using namespace std;
class CharacterCard
{
public:
	CharacterCard() :number{ "" }, kind{ "" } {};
	CharacterCard(const string kind, const string cost) : number{ kind }, kind{ cost } {};
	~CharacterCard() {};

	/*	friend ostream& operator<<(ostream& strm, BuildingCard & cart) {
	if (strm) {
	strm << cart.kind;
	}

	return strm;
	}
	*/
	const bool is_empty() const {

		return number.empty() && kind.empty();
	}


	const string get_kind() const { return number; }
	const string get_cost() const { return kind; }

private:
	const string number;
	const string kind;


};