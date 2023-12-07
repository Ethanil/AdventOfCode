#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
namespace day7 {
	const int NUMBEROFHANDS = 1000;
	struct Hand {
	public:
		uint16_t rank;
		char cards[5];
		uint16_t bid;
	};
	bool cmp(const Hand& a, const Hand& b) {
		if (a.rank == b.rank) {
			for (int i = 0; i < 5; ++i) {
				if (a.cards[i] == b.cards[i]) continue;
				return a.cards[i] < b.cards[i];
			}
			//std::cout << "we should never be here!\n";
		}
		return a.rank < b.rank;
	}
	long long calc1(const char* filename){
		Hand hands[NUMBEROFHANDS];
		FILE* stream;
		fopen_s(&stream,filename, "r");
		int c;
		int index = 0;
		while ((c = _fgetc_nolock(stream)) != -1 ) {
			hands[index] = Hand();
			for (int indexOfCard = 0; indexOfCard < 5; ++indexOfCard) {
				switch (c) {
				case 'T':
					hands[index].cards[indexOfCard] = 'A';
					break;
				case 'J':
					hands[index].cards[indexOfCard] = 'B';
					break;
				case 'Q':
					hands[index].cards[indexOfCard] = 'C';
					break;
				case 'K':
					hands[index].cards[indexOfCard] = 'D';
					break;
				case 'A':
					hands[index].cards[indexOfCard] = 'E';
					break;
				default:
					hands[index].cards[indexOfCard] = c;
				}
				c = _fgetc_nolock(stream);
			}
			for (int indexOfCard = 0; indexOfCard < 5; ++indexOfCard) {
				for (int indexToCheck = indexOfCard + 1; indexToCheck < 5; ++indexToCheck) {
					if (hands[index].cards[indexOfCard] == hands[index].cards[indexToCheck]) ++hands[index].rank;
				}
			}

			while ((c = _fgetc_nolock(stream)) >= '0' && c <= '9') {
				hands[index].bid = hands[index].bid*10 + (c - '0');
			}
			++index;
		}
		fclose(stream);
		std::sort(std::begin(hands), std::end(hands), cmp);
		int result = 0;
		for (int i = 0; i < NUMBEROFHANDS; ++i) {
			//std::cout << "rank: " << hands[i].rank << " cards: " << hands[i].cards[0] << hands[i].cards[1] << hands[i].cards[2] << hands[i].cards[3] << hands[i].cards[4] << " bid: " << hands[i].bid << "\n";
			result += hands[i].bid * (i+1);
		}
		return result;
	}
	long long calc2(const char* filename) { 
		Hand hands[NUMBEROFHANDS];
		FILE* stream;
		fopen_s(&stream, filename, "r");
		int c;
		int index = 0;
		while ((c = _fgetc_nolock(stream)) != -1) {
			hands[index] = Hand();
			for (int indexOfCard = 0; indexOfCard < 5; ++indexOfCard) {
				switch (c) {
				case 'T':
					hands[index].cards[indexOfCard] = 'A';
					break;
				case 'J':
					hands[index].cards[indexOfCard] = '0' - 1;
					break;
				case 'Q':
					hands[index].cards[indexOfCard] = 'C';
					break;
				case 'K':
					hands[index].cards[indexOfCard] = 'D';
					break;
				case 'A':
					hands[index].cards[indexOfCard] = 'E';
					break;
				default:
					hands[index].cards[indexOfCard] = c;
				}
				c = _fgetc_nolock(stream);
			}
			int numberOfJokers = 0;
			for (int indexOfCard = 0; indexOfCard < 5; ++indexOfCard) {
				if (hands[index].cards[indexOfCard] == '0' - 1) {
					++numberOfJokers;
					continue;
				}
				for (int indexToCheck = indexOfCard + 1; indexToCheck < 5; ++indexToCheck) {
					if (hands[index].cards[indexOfCard] == hands[index].cards[indexToCheck] && hands[index].cards[indexToCheck] != '0' - 1) ++hands[index].rank;
				}
			}
			if (numberOfJokers == 5) hands[index].rank = 10;
			else {
				for (int i = 0; i < numberOfJokers; ++i) {
					if (hands[index].rank == 2) hands[index].rank = 4;
					else {
						hands[index].rank += (hands[index].rank / 2.0 + 0.5) + 1;
					}
				}
			}
			while ((c = _fgetc_nolock(stream)) >= '0' && c <= '9') {
				hands[index].bid = hands[index].bid * 10 + (c - '0');
			}
			++index;
		}
		fclose(stream);
		std::sort(std::begin(hands), std::end(hands), cmp);
		int result = 0;
		for (int i = 0; i < NUMBEROFHANDS; ++i) {
			//std::cout << "rank: " << hands[i].rank << " cards: " << hands[i].cards[0] << hands[i].cards[1] << hands[i].cards[2] << hands[i].cards[3] << hands[i].cards[4] << " bid: " << hands[i].bid << "\n";
			result += hands[i].bid * (i + 1);
		}
		return result;
	}
	long long calc3(const char* filename){ return -1; }
	long long calc4(const char* filenamek){ return -1; }
}