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
	std::for_each(clients.begin(), clients.end(), [](std::shared_ptr<ClientInfo>& elem) {
		if (elem->get_player().id == 1) elem->get_player().setKing();
	});

	while (running) {
		if (clients.size() >= 2) {
			if (clients[0]->get_player().getBuildingCards().size() >= BuildingLimitToEndGame || clients[1]->get_player().getBuildingCards().size() >= BuildingLimitToEndGame) {
				gameStage = ENDING;
			}
		}
		continueGame();
	}
}

void GameController::continueGame()
{
	round++;
	int count;

	switch (gameStage) {
	case PREPARATION:
		execPrep();

		gameStage = PICKING_CHARACTERS;
		break;
	case PICKING_CHARACTERS:
		sendMessageToClients("\r\nStarting the next round picking chars\r\n", -1);

		count = 1;
		while (count <= 4) {
			execPickChar(count);
			count++;
		}
		
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
			player.addBuildingCard(stacks.getBuildingCard());
		}

		socket.write("\r\nYou have been given 2 pieces of gold and 4 building cards\r\n");
	}
}

void GameController::execPickChar(int turnCount)
{
	std::vector<std::shared_ptr<ClientInfo>>::iterator it;

	for (it = clients.begin(); it != clients.end(); it++)
	{
		auto &socket = it->get()->get_socket();
		auto &player = it->get()->get_player();

		if (turnCount == 1) {
			if (player.isKing()) {
				socket.write("You're the king, so you may pick first\r\n");
				socket.write("This card will now be removed: \r\n");
				socket.write(stacks.removeCharacterCard(1) + "\r\n");
				socket.write("Those cards are left to pick:\r\n");
				socket.write(stacks.getCharacterCardOptions());

				int answer = recieveAnswerFromPlayer(stacks.getAmountOfCharacterCards());
				
				player.addCharCard(stacks.getCharacterCard(answer));
				socket.write("You took " + player.getCharCards().back().get_kind() + " as your card\r\n");

				switchTurn();
				return;
			}
			else {
				// TODO: This scenario may not happen. come up with a catch
			}
		}
		else if (player.id == currentTurnPlayerId)
		{
			socket.write("You other player has chosen a card to take and a card to remove, now it's your turn");
			socket.write("Pick a card to remove: \r\n");
			socket.write(stacks.getCharacterCardOptions());

			const int answer = recieveAnswerFromPlayer(stacks.getAmountOfCharacterCards());

			socket.write(stacks.removeCharacterCard(answer) + "\r\n");
			socket.write("Now pick a card to take for yourself\r\n");
			socket.write(stacks.getCharacterCardOptions());

			const int secAnswer = recieveAnswerFromPlayer(stacks.getAmountOfCharacterCards());

			player.addCharCard(stacks.getCharacterCard(secAnswer));
			socket.write("You took " + player.getCharCards().back().get_kind() + "\r\n");

			switchTurn();
			return;
		}
	}
}

const int GameController::recieveAnswerFromPlayer(const int optionCount) {
	bool waiting = true;
	int answer = 0;

	while (waiting && running) {
		if (!playerCommand.first.empty() && playerCommand.second == currentTurnPlayerId) {
			if (isInteger(playerCommand.first)) {
				answer = stoi(playerCommand.first);

				if (answer > 0 && answer <= optionCount)
					waiting = false;
				else
					sendMessageToClients("Incorrect id\r\n", currentTurnPlayerId);
			}
			else
			{
				sendMessageToClients("Input is invalid, did you use the ID?\r\n", currentTurnPlayerId);
			}

			playerCommand.first = "";
			playerCommand.second = 0;
		}
	}

	return answer;
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