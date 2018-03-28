#pragma once
#include <memory>
#include <iostream>

#include "ClientCommand.h"
#include "Player.h"
#include "Stacks.h"
#include "ClientInfo.h"
#include "BuildingCard.h"
#include "CharacterCard.h"

class GameController
{
public:
	GameController();
	~GameController();

	void init();

	void handleClientInput(const ClientCommand command);

	void startGame();

	void continueGame();

	std::vector<std::shared_ptr<ClientInfo>> clients;

	bool running{ true };
private:

	std::pair<std::string, int> playerCommand;
	Stacks stacks;
	int currentTurnPlayerId{ 1 };
	int round{ 0 };
	int firstWinPlayerId{ 0 };
};