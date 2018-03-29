#include "CharacterCardReader.h"



CharacterCardReader::CharacterCardReader()
{
}


CharacterCardReader::~CharacterCardReader()
{
}

std::vector<CharacterCard> CharacterCardReader::read_file(std::string path) {
	std::vector<CharacterCard> data;

	ifstream ip(path);
	if (!ip.is_open()) cout << "ERROR: FIle Open" << endl;

	//loops untill there are no lines in document
	while (ip) {
		char line[200];

		ip.getline(line, 200, '\n');



		stringstream sentence{ line };

		char line1[100];
		char line2[100];


		sentence.getline(line1, 100, ';');
		sentence.getline(line2, 100, ';');



		CharacterCard element{ line1, line2 };


		// make cart object 
		if (!element.is_empty())
			data.push_back(element);
	}

	return data;
}