#pragma once
#include <memory>
#include <iostream>

#include "ClientCommand.h"
#include "Player.h"
#include "Stacks.h"
#include "ClientInfo.h"
#include "BuildingCard.h"
#include "CharacterCard.h"

#include "Globals.h"

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
	void execPickChar(int count);
	void execCallChar();
	void execPlayerTurn(Player&, const CharacterCard);
	void cleanRound();

	void useCard(Player& player, const CharacterCard charCard);
	void getGoldOrBuilding(Player& player);
	void showPlayerStats(Player& player);

	void executeMoordenaar(Player& player);
	void executeDief(Player& player);
	void executeMagier(Player& player);
	void executeKoning(Player& player);
	void executePrediker(Player& player);
	void executeKoopman(Player& player);
	void executeBouwmeester(Player& player);
	void executeCondottiere(Player& player);

	void sendMessageToClients(const std::string message, const int playerId);
	const int recieveAnswerFromPlayer(const int amountOfOptions);
	std::pair<std::string, int> playerCommand;


	Stacks stacks;
	int round{ 0 };
	int thiefPlayerId{ 0 };
	int firstWinPlayerId{ 0 };
	int currentTurnPlayerId{ 1 };

	CharacterCard::CharType stolenChar { CharacterCard::CharType::None };
	CharacterCard::CharType killedChar { CharacterCard::CharType::None };

	// STATICS
	const int BuildingLimitToEndGame { 8 };
};