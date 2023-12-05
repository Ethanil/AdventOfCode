#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
namespace day4 {
	const int NUMBER_OF_CARDS = 199;
	struct Card {
	public:
		int winningNumbers[10];
		int numbersYouHave[25];
	};
	std::ostream& operator<<(std::ostream& os, const Card& card) {
		os << "winningNumbers: { ";
		for (const auto& win : card.winningNumbers) {
			os << win << " ";
		}
		os << "} numbers You have: {";
		for (const auto& num : card.numbersYouHave) {
			os << num << " ";
		}
		os << "}";
		return os;
	}
	//int calc1(const char* filename) {
	//	std::ifstream in(filename);
	//	int c;
	//	bool readingWinningNumbers = true;
	//	Card cards[NUMBER_OF_CARDS];
	//	int cardNumber = 0;
	//	int curNumber = 0;
	//	while ((c = in.get()) != -1) {
	//		switch (c) {
	//		case 'C':
	//			//std::cout << "we here\n";
	//			while (in.get() != ':');
	//			cards[cardNumber] = Card{ std::vector<int>(),std::vector<int>()};
	//			break;
	//		case '|':
	//			readingWinningNumbers = false;
	//			break;
	//		case ' ':
	//			break;
	//		default:
	//			curNumber = c - '0';
	//			while((c = in.get()) != -1 && c != ' ' && c != '\n') {
	//				curNumber = curNumber * 10 + (c - '0');
	//			}
	//			if (readingWinningNumbers) {
	//				cards[cardNumber].winningNumbers.emplace_back(curNumber);
	//			}
	//			else {
	//				cards[cardNumber].numbersYouHave.emplace_back(curNumber);
	//			}
	//			if (c == '\n') {
	//				readingWinningNumbers = true;
	//				cardNumber++;
	//				break;
	//			}
	//			curNumber = 0;
	//		}
	//	}
	//	int points = 0;
	//	std::vector<Card> cardsToPlay(NUMBER_OF_CARDS-1);
	//	std::copy(&cards[0], &cards[NUMBER_OF_CARDS - 1], cardsToPlay.begin());
	//	while(!cardsToPlay.empty()){
	//		Card card = cardsToPlay.back();
	//		cardsToPlay.pop_back();
	//		std::sort(card.numbersYouHave.begin(), card.numbersYouHave.end());
	//		std::sort(card.winningNumbers.begin(), card.winningNumbers.end());
	//		std::vector<int> res;
	//		auto end = std::set_intersection(card.winningNumbers.cbegin(), card.winningNumbers.cend(), card.numbersYouHave.cbegin(), card.numbersYouHave.cend(), std::back_inserter(res));
	//		for (int i = 0; i < res.size() ; ++i) {
	//			//if (card.cardNumber + i <= NUMBER_OF_CARDS) {
	//			//	cardsToPlay.emplace_back(cards[card.cardNumber + i]);
	//			//}
	//		}
	//		if (res.size() > 0) {
	//			points += std::pow(2, res.size() - 1);

	//		}
	//		//for (const int& i : res) {
	//		//	std::cout << i << " ";
	//		//}
	//		//std::cout << "\n";
	//	}
	//	return cardsToPlay.size();
	//}
	int calc2(const char* filename) { 
		std::ifstream in(filename);
		int c;
		int cardNumber = 0;
		int curNumber = 0;
		int winIndex = 0;
		int youHaveIndex = 0;
		bool readingWinningNumbers = true;
		Card cards[NUMBER_OF_CARDS];
		while ((c = in.get()) != -1) {
			switch (c) {
			case 'C':
				while (in.get() != ':');
				cards[cardNumber] = Card();
				break;
			case '|':
				readingWinningNumbers = false;
				break;
			case ' ':
				break;
			default:
				curNumber = c - '0';
				while ((c = in.get()) != -1 && c != ' ' && c != '\n') {
					curNumber = curNumber * 10 + (c - '0');
				}
				if (readingWinningNumbers) {
					cards[cardNumber].winningNumbers[winIndex++] = curNumber;
				}
				else {
					cards[cardNumber].numbersYouHave[youHaveIndex++] = curNumber;
				}
				if (c == '\n') {
					readingWinningNumbers = true;
					cardNumber++;
					winIndex = 0;
					youHaveIndex = 0;
					break;
				}
				curNumber = 0;
			}
		}
		int points = 0;
		int amountOfCard[NUMBER_OF_CARDS] = { 0 };
		for (int i = 0; i < NUMBER_OF_CARDS; ++i) {
			amountOfCard[i] += 1;
			auto& card = cards[i];
			std::sort(std::begin(card.numbersYouHave), std::end(card.numbersYouHave));
			std::sort(std::begin(card.winningNumbers), std::end(card.winningNumbers));
			std::vector<int> intersection;
			auto end = std::set_intersection(std::begin(card.numbersYouHave), std::end(card.numbersYouHave), std::begin(card.winningNumbers), std::end(card.winningNumbers), std::back_inserter(intersection));
			for (int j = 0; j < intersection.size(); ++j) {
				if (i + j + 1 < NUMBER_OF_CARDS) {
					amountOfCard[i + j + 1]+=amountOfCard[i];
				}
			}
			points += amountOfCard[i];
		}
		return points;
	}

	int calc3(const char* filename) {
		std::ifstream in(filename);
		std::string line;
		Card cards[NUMBER_OF_CARDS];
		int cardNumber = 0;
		while (std::getline(in, line)) {
			cards[cardNumber] = Card();
			std::istringstream is(line);
			std::string temp;
			is >> temp >> temp;
			for (int i = 0; i < 10; ++i) {
				is >> temp;
				cards[cardNumber].winningNumbers[i] = std::stoi(temp);
			}
			is >> temp;
			for (int i = 0; i < 25; ++i) {
				is >> temp;
				cards[cardNumber].numbersYouHave[i] = std::stoi(temp);
			}
			cardNumber++;
		}
		int points = 0;
		int amountOfCard[NUMBER_OF_CARDS] = { 0 };
		for (int i = 0; i < NUMBER_OF_CARDS; ++i) {
			amountOfCard[i] += 1;
			auto& card = cards[i];
			//std::cout << card << "\n";
			std::sort(std::begin(card.numbersYouHave), std::end(card.numbersYouHave));
			std::sort(std::begin(card.winningNumbers), std::end(card.winningNumbers));
			std::vector<int> intersection;
			auto end = std::set_intersection(std::begin(card.numbersYouHave), std::end(card.numbersYouHave), std::begin(card.winningNumbers), std::end(card.winningNumbers), std::back_inserter(intersection));
			for (int j = 0; j < intersection.size(); ++j) {
				if (i + j + 1 < NUMBER_OF_CARDS) {
					amountOfCard[i + j + 1] += amountOfCard[i];
				}
			}
			points += amountOfCard[i];
		}
		return points;
	}
	int calc4(const char* filename) {
		std::ifstream in(filename);
		int c;
		
		int curNumber = 0;
		int winIndex = 0;
		int youHaveIndex = 0;
		bool readingWinningNumbers = true;
		Card cards[NUMBER_OF_CARDS];
		for (int cardNumber = 0; cardNumber < NUMBER_OF_CARDS; ++cardNumber) {
			cards[cardNumber] = Card();
			std::string temp;
			in >> temp >> temp;
			for (int i = 0; i < 10; ++i) {
				in >> temp;
				cards[cardNumber].winningNumbers[i] = std::stoi(temp);
			}
			in >> temp;
			for (int i = 0; i < 25; ++i) {
				in >> temp;
				cards[cardNumber].numbersYouHave[i] = std::stoi(temp);
			}
		}
		int points = 0;
		int amountOfCard[NUMBER_OF_CARDS] = { 0 };
		for (int i = 0; i < NUMBER_OF_CARDS; ++i) {
			amountOfCard[i] += 1;
			auto& card = cards[i];
			std::sort(std::begin(card.numbersYouHave), std::end(card.numbersYouHave));
			std::sort(std::begin(card.winningNumbers), std::end(card.winningNumbers));
			std::vector<int> intersection;
			auto end = std::set_intersection(std::begin(card.numbersYouHave), std::end(card.numbersYouHave), std::begin(card.winningNumbers), std::end(card.winningNumbers), std::back_inserter(intersection));
			for (int j = 0; j < intersection.size(); ++j) {
				if (i + j + 1 < NUMBER_OF_CARDS) {
					amountOfCard[i + j + 1] += amountOfCard[i];
				}
			}
			points += amountOfCard[i];
		}
		return points;
	}
}
