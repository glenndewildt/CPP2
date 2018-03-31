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
	enum GameStage { PREPARATION, PICKING_CHARACTERS, CALLING_CHARACTERS, ENDING};
	GameStage gameStage{ PREPARATION };

	void switchTurn() { currentTurnPlayerId = ( currentTurnPlayerId == 1 ? 2 : 1); };

	void execPrep();
	void execPickChar();
	void execCallChar();
	void execPlayerTurn();
	void cleanRound();

	void sendMessageToClients(const std::string message, const int playerId);

	std::pair<std::string, int> playerCommand;

	Stacks stacks;
	int currentTurnPlayerId{ 1 };
	int round{ 0 };
	int firstWinPlayerId{ 0 };
};