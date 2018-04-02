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

		for each (const std::shared_ptr<ClientInfo> client in clients)
		{
			auto &socket = client->get_socket();
			auto &player = client->get_player();
			socket.write("\r\n GameInfo:" + player.get_name());
			socket.write("\r\n Gold:" + std::to_string(player.get_gold()));
			socket.write("\r\n Building carts:" + std::to_string(player.getBuildingCards().size()));
			socket.write("\r\n Charcater carts:" + std::to_string(player.getCharCards().size()));

		}
		

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
		while (stacks.getCharacterCardOptions().size() > 0) {
			execPickChar(count);
			count++;
		}
		
		gameStage = CALLING_CHARACTERS;
		break;
	case CALLING_CHARACTERS:
		sendMessageToClients("Characters are chosen, now we start calling them", 3);
		execCallChar();
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
			 auto bc = stacks.getBuildingCard();
			// assign bc to player
			 player.addBuildingCard(bc);
		}
		player.set_gold(2);

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
				socket.write("You took " + player.getCharCards().back().getName() + " as your card\r\n");

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
			socket.write("You took " + player.getCharCards().back().getName() + "\r\n");
			switchTurn();

		
			return;
		}
	}
}

void GameController::execCallChar() {
	int callCount = 1;
	while (callCount <= 8)
	{
		for each (const std::shared_ptr<ClientInfo> client in clients)
		{
			auto &player = client->get_player();

			std::vector<CharacterCard>::iterator it;

			auto cards = player.getCharCards();
			for (it = cards.begin(); it != cards.end(); it++)
			{
				if (it->getId() == callCount) {
					if (it->getType() != killedChar) {
						execPlayerTurn(player, *it);
					}
				}
			}
		}

		callCount++;
	}

	sendMessageToClients("The chars have been called, end of the round\r\n", 3);
	cleanRound();
}

void GameController::execPlayerTurn(Player &player, const CharacterCard charCard) {
	currentTurnPlayerId = player.id;
	sendMessageToClients(player.get_name() + " is now on turn with " + charCard.getName(), 3);

	if (charCard.getType() == stolenChar) {
		for each (auto client in clients) {
			if (client->get_player().id == thiefPlayerId) {
				auto& thiefPlayer = client->get_player();
				thiefPlayer.add_gold(player.get_gold());

				sendMessageToClients("\r\nThe thief stole from " + charCard.getName() + ". " + thiefPlayer.get_name() + " now has " + std::to_string(thiefPlayer.get_gold()) + "\r\n", 3);
			}
		}

		player.set_gold(0);
	}

	bool firstStage{ true };
	bool secondStage{ true };

	int countBuilded{ 0 };
	bool usedCharCard{ false };
	bool tookSomething{ false };
	bool buildedbuilding{ false };

	while (firstStage) {
		sendMessageToClients("\r\nChoose what to do next: \r\n", player.id);

		int countOptions{ 0 };
		int indexPower{ 0 };
		int indexGoldOrBuilding{ 0 };
		int indexShowStats{ 0 };

		std::string message{ "" };

		if (!usedCharCard)
		{
			countOptions++;
			message.append(std::to_string(countOptions) + ": Use character power.\r\n");
			indexPower = countOptions;
		}

		if (tookSomething)
		{
			countOptions++;
			message.append(std::to_string(countOptions) + ": Recieve 2 gold or a building card.\r\n ");
			indexGoldOrBuilding = countOptions;
		}

		countOptions++;
		message.append(std::to_string(countOptions) + ": Show info \r\n");
		indexShowStats = countOptions;

		sendMessageToClients(message, player.id);

		int answer = recieveAnswerFromPlayer(countOptions);

		if (answer == 0)
			return;
		else if (answer == indexPower)
		{
			useCard(player, charCard);
			usedCharCard = true;
		}
		else if (answer == indexGoldOrBuilding)
		{
			getGoldOrBuilding(player);
			tookSomething = true;
			firstStage = false;
		}
		else if (answer == indexShowStats)
		{
			showPlayerStats(player);
		}
	}

	while (secondStage)
	{
		sendMessageToClients("\r\nChoose what to do next: \r\n", player.id);
	}
}

void GameController::useCard(Player& player, CharacterCard charCard)
{
	switch (charCard.getType()) {
	case CharacterCard::CharType::Moordenaar:
		executeMoordenaar(player);
		break;
	case CharacterCard::CharType::Dief:
		executeDief(player);
		break;
	case CharacterCard::CharType::Magier:
		executeMagier(player);
		break;
	case CharacterCard::CharType::Koning:
		executeKoning(player);
		break;
	case CharacterCard::CharType::Prediker:
		executePrediker(player);
		break;
	case CharacterCard::CharType::Koopman:
		executeKoopman(player);
		break;
	case CharacterCard::CharType::Bouwmeester:
		executeBouwmeester(player);
		break;
	case CharacterCard::CharType::Condottiere:
		executeCondottiere(player);
		break;
	}
}

void GameController::getGoldOrBuilding(Player& player)
{

}

void GameController::showPlayerStats(Player& player)
{

}

void GameController::cleanRound() {
	std::vector<std::shared_ptr<ClientInfo>>::iterator clientIt;

	for (clientIt = clients.begin(); clientIt != clients.end(); clientIt++)
	{
		auto &player = clientIt->get()->get_player();

		std::vector<CharacterCard>::iterator charIt;
		for (charIt = player.getCharCards().begin(); charIt != player.getCharCards().end(); charIt++)
		{
			stacks.addCharacterCard(*charIt);
		}

		player.getCharCards().clear();
	}

	stacks.undiscardCharacterCards();
	stacks.shuffleCharacterCards();
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

void GameController::executeMoordenaar(Player & player)
{
	sendMessageToClients("\r\You're using Moordenaar, who would you like to kill?\r\n", player.id);
	std::string message{ "" };

	message.append("1: Dief\r\n");
	message.append("2: Magier\r\n");
	message.append("3: Koning\r\n");
	message.append("4: Prediker\r\n");
	message.append("5: Koopman\r\n");
	message.append("6: Bouwmeester\r\n");
	message.append("7: Condotierre\r\n");

	sendMessageToClients(message, player.id);

	const int answer = recieveAnswerFromPlayer(7);
	if (answer == 0)return;

	switch (answer) {
	case 1:
		killedChar = CharacterCard::CharType::Dief;
		message = " Dief ";
		break;
	case 2:
		killedChar = CharacterCard::CharType::Magier;
		message = " Magier ";
		break;
	case 3:
		killedChar = CharacterCard::CharType::Koning;
		message = " Koning ";
		break;
	case 4:
		killedChar = CharacterCard::CharType::Prediker;
		message = " Prediker ";
		break;
	case 5:
		killedChar = CharacterCard::CharType::Koopman;
		message = " Koopman ";
		break;
	case 6:
		killedChar = CharacterCard::CharType::Bouwmeester;
		message = " Bouwmeester ";
		break;
	case 7:
		killedChar = CharacterCard::CharType::Condottiere;
		message = " Condottiere ";
		break;
	}

	sendMessageToClients("\r\nThe " + message + " has been killed and he can not be used this turn!\r\n", 3);
}

void GameController::executeDief(Player & player)
{
	sendMessageToClients("\r\You're using dief, who would you like to rob?\r\n", player.id);
	std::string message{ "" };

	message.append("1: Moordenaar\r\n");
	message.append("2: Magier\r\n");
	message.append("3: Koning\r\n");
	message.append("4: Prediker\r\n");
	message.append("5: Koopman\r\n");
	message.append("6: Bouwmeester\r\n");
	message.append("7: Condotierre\r\n");

	sendMessageToClients(message, player.id);

	const int answer = recieveAnswerFromPlayer(7);
	if (answer == 0)return;

	switch (answer) {
	case 1:
		stolenChar = CharacterCard::CharType::Moordenaar;
		message = " Moordernaar ";
		break;
	case 2:
		stolenChar = CharacterCard::CharType::Magier;
		message = " Magier ";
		break;
	case 3:
		stolenChar = CharacterCard::CharType::Koning;
		message = " Koning ";
		break;
	case 4:
		stolenChar = CharacterCard::CharType::Prediker;
		message = " Prediker ";
		break;
	case 5:
		stolenChar = CharacterCard::CharType::Koopman;
		message = " Koopman ";
		break;
	case 6:
		stolenChar = CharacterCard::CharType::Bouwmeester;
		message = " Bouwmeester ";
		break;
	case 7:
		stolenChar = CharacterCard::CharType::Condottiere;
		message = " Condottiere ";
		break;
	}

	sendMessageToClients("\r\nThe " + message + " Has been chosen to steel from. you will steel his gold when his turn begins!\r\n", 3);
}
void GameController::executeMagier(Player & player)
{
	sendMessageToClients("\r\You're using magieer, what do you want to do?\r\n", player.id);
	std::string message{ "" };

	message.append("1: switch hand with someone else\r\n");
	message.append("2: naar keuze een aantal handkaarten afleggen en een gelijk aantal gebouwenkaarten trekken \r\n");


	sendMessageToClients(message, player.id);

	const int answer = recieveAnswerFromPlayer(7);
	if (answer == 0)return;

	switch (answer) {
	case 1:
		message = " switched with other player ";
		break;
	case 2:
		for (CharacterCard& charCard : player.getCharCards) {
			sendMessageToClients("\r\1:"+ charCard.getName, player.id);
		}
		message = "  ";
	}
}
void GameController::executeKoning(Player & player)
{
}
void GameController::executePrediker(Player & player)
{
}
void GameController::executeKoopman(Player & player) {
}
void GameController::executeBouwmeester(Player & player)
{
}
void GameController::executeCondottiere(Player & player)
{
}