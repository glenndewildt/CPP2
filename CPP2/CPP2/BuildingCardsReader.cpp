#include "BuildingCardsReader.h"



BuildingCardsReader::BuildingCardsReader()
{
}


BuildingCardsReader::~BuildingCardsReader()
{
}


std::vector<BuildingCard> BuildingCardsReader::read_file(std::string path) {
	std::vector<BuildingCard> data;

	ifstream ip(path);
	if (!ip.is_open()) cout << "ERROR: FIle Open" << endl;

	//loops untill there are no lines in document
	while (ip) {
		char line[200];

		ip.getline(line, 200, '\n');

	

		stringstream sentence{ line };

		char line1[100];
		char line2[100];
		char line3[100];


		sentence.getline(line1, 100, ';');
		sentence.getline(line2, 100, ';');
		sentence.getline(line3, 100, ';');



		BuildingCard element{ line1, line2,line3 };


		// make cart object 
		if (!element.is_empty())
			data.push_back(element);
	}

	return data;
}