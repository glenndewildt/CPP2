#include "GameController.h"

#include <deque>
#include <vector>
#include <iterator>
#include <thread>
#include <exception>
#include <utility>
#include <chrono>
#include <algorithm>

GameController::GameController()
{
}

GameController::~GameController()
{
}

void GameController::init() {
	stacks.initBuildingCards();
	stacks.initCharacterCards();

	stacks.shuffleBuildingCards();
	stacks.shuffleCharacterCards();
}

void GameController::startGame()
{
	sendMessageToClients("\r\n2 Players have been found starting the game!\r\n", -1);

	//Assign Player 1 with the king
	//std::for_each(clients.begin(), clients.end(), [](std::shared_ptr<ClientInfo>& elem) {
	//	if (elem->get_player().id == 1) elem->get_player().king = true;
	//});

	while (running) {
		if (clients.size() >= 2) {
			//if (clients[0]->get_player().BuildingCount >= BuildingLimit || Client[1] does eat sleep rave repeat) {
			//	gameStage = ending state;
			//}
		}
		continueGame();
	}
}

void GameController::continueGame()
{
	round++;
	switch (gameStage) {
	case PREPARATION:
		execPrep();
		gameStage = PICKING_CHARACTERS;
		break;
	case PICKING_CHARACTERS:
		gameStage = CALLING_CHARACTERS;
		break;
	case CALLING_CHARACTERS:
		gameStage = PICKING_CHARACTERS;
		break;
	case ENDING:
		break;
	}
}

void GameController::execPrep()
{
	for each (const std::shared_ptr<ClientInfo> client in clients)
	{
		auto &socket = client->get_socket();
		auto &player = client->get_player();

		for (int i = 0; i < 4; i++)
		{
			// auto bc = stacks.getBuildingCard();
			// assign bc to player
		}

		socket.write("\r\nYou have been given 2 pieces of gold and 4 building cards\r\n");
	}
}

void GameController::sendMessageToClients(const std::string message, const int playerId)
{
	std::vector<std::shared_ptr<ClientInfo>>::iterator it;

	for (it = clients.begin(); it != clients.end(); it++)
	{
		const auto &socket = it->get()->get_socket();
		const auto &player = it->get()->get_player();

		if (playerId == player.id && running) {
			socket.write(message);
		}
		else if (playerId < 0 && running) // When ID is negative everyone gets the message
		{
			socket.write(message);
		}
	}
}

void GameController::handleClientInput(const ClientCommand command)
{
	auto clientInfo = command.get_client_info().lock();
	auto &client = clientInfo->get_socket();
	auto &player = clientInfo->get_player();

	if (command.get_cmd() == "stats") {
		return;
	}

	if (player.id == currentTurnPlayerId) {
		playerCommand.first = command.get_cmd();
		playerCommand.second = player.id;
	}
	else
	{
		if (client.is_open()) {
			client << "\r\nIt's not your turn right now. Wait for the other player to finish his turn...\r\n";
		}
	}
}

