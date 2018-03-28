#include "building_carts_reader.h"



building_carts_reader::building_carts_reader()
{
}


building_carts_reader::~building_carts_reader()
{
}


std::vector<building_cart> building_carts_reader::read_file(std::string path) {
	std::vector<building_cart> data;

	ifstream ip(path);
	if (!ip.is_open()) cout << "ERROR: FIle Open" << endl;
	int counter1 = 0;

	//loops untill there are no lines in document
	while (ip) {
		char line[200];

		if (counter1 == 24) {
			int i = 1;
		}

		ip.getline(line, 200, '\r');

		if (line[0] == '\000') {
			break;
		}

		stringstream sentence{ line };

		char line1[100];
		char line2[100];
		char line3[100];


		sentence.getline(line1, 100, ';');
		sentence.getline(line2, 100, ';');
		sentence.getline(line3, 100, ';');



		building_cart element{ line1, line2,line1 };

		counter1++;

		// make cart object 
		if (!element.is_empty())
			data.push_back(element);
	}

	return data;
}