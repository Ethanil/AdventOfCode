#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
namespace day3 {
	struct GearNumber {
	public:
		int number;
		int x;
		int y;
		int length;
		inline bool operator==(GearNumber other) {
			return number == other.number && x == other.x && y == other.y && length == other.length;
		}
	};
	struct PartNumber {
	public:
		int x;
		int y;
		char symbol;
		inline bool operator==(PartNumber other) {
			return x == other.x && y == other.y && symbol == other.symbol;
		}
	};
	struct GearAndPartNumber {
	public:
		GearNumber gearNumber;
		PartNumber partNumber;
		inline bool operator==(GearAndPartNumber other) {
			return partNumber == other.partNumber && gearNumber == other.gearNumber;
		}
	};
	std::ostream& operator<<(std::ostream& os, const GearNumber& gearNumber) {

		os << gearNumber.number << " at x:"<<gearNumber.x<<" y:"<<gearNumber.y<<" length:"<<gearNumber.length;
		return os;

	}
	std::ostream& operator<<(std::ostream& os, const PartNumber& partNumber) {

		os << "x:" << partNumber.x << " y:" << partNumber.y<<" symbol:"<<partNumber.symbol;
		return os;

	}
	std::ostream& operator<<(std::ostream& os, const GearAndPartNumber& gearAndPartNumber) {

		os << "gearNumber: "<< gearAndPartNumber .gearNumber<<" with partNumber: "<< gearAndPartNumber.partNumber;
		return os;

	}

	int checkSubstring1(const char* filename) {
		std::ifstream streamToParse(filename);
		std::vector<GearNumber> gearVec;
		std::vector<PartNumber> partVec;
		int lastNumberFound = 0;
		int c;
		int x = 0;
		int y = 0;
		while((c = streamToParse.get()) != -1) {
			if (c >= '0' && c <= '9') {
				lastNumberFound = c - '0';
				int length = 1;
				while ((c = streamToParse.get()) >= '0' && c <= '9') {
					lastNumberFound = lastNumberFound * 10 + (c - '0');
					++length;
				}
				gearVec.emplace_back(GearNumber{ lastNumberFound, x, y, length});
				x += length;
			}
			if (c == '\n') {
				++y;
				x = 0;
				continue;
			}
			else if (c != '.') {
				partVec.emplace_back(PartNumber{x,y,(char)c});
			}
			++x;
		}
		int sum = 0;
		std::vector<GearAndPartNumber> resultVec;
		//for (PartNumber pn : partVec) {
		//	std::cout << "Part: " << pn << "\n";
		//}
		for (GearNumber gn : gearVec) {
			//std::cout << "checking " << gn << ":\n";
			for (PartNumber pn : partVec) {
				//std::cout << "with " << pn << ":";
				if (pn.y > gn.y + 1) break;
				if (pn.x >= gn.x - 1 && pn.x <= gn.x + gn.length && pn.y >= gn.y - 1 && pn.y <= gn.y + 1) {
					sum += gn.number;
					resultVec.emplace_back(GearAndPartNumber{gn,pn});
					//std::cout << "found a match!\n";
					break;
				}
				//std::cout << "no match!\n";
			}
		}
		//for (GearAndPartNumber gapn : resultVec) {
		//	std::cout << gapn << "\n";
		//}
		return sum;
	}

	long long calc1(const char* filename) {
		return checkSubstring1(filename);
	}
	int checkSubstring2(const char* filename) {
		std::ifstream streamToParse(filename);
		GearNumber numberArray[140][140];
		std::vector<PartNumber> gearVec;
		PartNumber partArray[140][140];
		for (int i = 0; i < 140; ++i) {
			for (int j = 0; j < 140; ++j) {
				numberArray[i][j] = GearNumber{ 0,0,0,0 };
			}
		}
		int lastNumberFound = 0;
		int c;
		int x = 0;
		int y = 0;
		int longestLength = 0;
		while ((c = streamToParse.get()) != -1) {
			if (c >= '0' && c <= '9') {
				lastNumberFound = c - '0';
				int length = 1;
				while ((c = streamToParse.get()) >= '0' && c <= '9') {
					lastNumberFound = lastNumberFound * 10 + (c - '0');
					++length;
				}
				numberArray[x][y] = GearNumber{ lastNumberFound, x, y, length };
				longestLength = std::max(longestLength, length);
				x += length;
			}
			if (c == '\n') {
				++y;
				x = 0;
				continue;
			}
			else if (c == '*') {
				gearVec.emplace_back(PartNumber{ x,y,(char)c });
			}
			++x;
		}
		int sum = 0;
		for (auto gear : gearVec) {
			bool foundOne = false;
			bool foundTwo = false;
			bool foundMore = false;
			GearNumber first;
			GearNumber second;
			for (y = std::max(gear.y - 1, 0); y < std::min(gear.y + 2, 140); ++y) {
				for (x = std::max(gear.x - longestLength, 0); x < std::min(gear.x + 2, 140);++x) {
					if (!foundMore && !(numberArray[x][y] == GearNumber{ 0,0,0,0 }) && gear.x <= x + numberArray[x][y].length) {
						if (!foundOne) {
							first = numberArray[x][y];
							foundOne = true;
						}
						else if (!foundTwo) {
							second = numberArray[x][y];
							foundTwo = true;
						}
						else {
							foundMore = true;
						}
					}
				}
			}
			if (foundTwo && !foundMore) {
				//std::cout <<"Gear: " << gear << " first Number: " << first << " second Number: " << second << "\n";
				sum += first.number * second.number;
			}
		}



		//std::vector<GearAndPartNumber> resultVec;
		////for (PartNumber pn : partVec) {
		////	std::cout << "Part: " << pn << "\n";
		////}
		//for (GearNumber gn : gearVec) {
		//	//std::cout << "checking " << gn << ":\n";
		//	for (PartNumber pn : partVec) {
		//		//std::cout << "with " << pn << ":";
		//		if (pn.y > gn.y + 1) break;
		//		if (pn.x >= gn.x - 1 && pn.x <= gn.x + gn.length && pn.y >= gn.y - 1 && pn.y <= gn.y + 1) {
		//			//sum += gn.number;
		//			resultVec.emplace_back(GearAndPartNumber{ gn,pn });
		//			//std::cout << "found a match!\n";
		//			break;
		//		}
		//		//std::cout << "no match!\n";
		//	}
		//}
		//while (!resultVec.empty()) {
		//	GearAndPartNumber secondGear;
		//	bool foundSecond = false;
		//	bool foundTooMany = false;
		//	resultVec.pop_back();
		//	for (auto outerit = resultVec.begin(); outerit != resultVec.end();) {
		//		for (auto it = resultVec.begin(); it != resultVec.end();) {
		//			GearAndPartNumber potentialGear = resultVec.front();
		//			it = resultVec.erase(it);
		//			if ((*it).partNumber == potentialGear.partNumber) {
		//				if (!foundSecond) {
		//					secondGear = *it;
		//					foundSecond = true;
		//				}
		//				else {
		//					foundTooMany = true;
		//				}
		//				it = resultVec.erase(it);
		//			}
		//			else {
		//				++it;
		//			}
		//		}
		//	}
		//}
		////for (GearAndPartNumber gapn : resultVec) {
		////	std::cout << gapn << "\n";
		////}
		return sum;
	}

	long long calc2(const char* filename) {
		return checkSubstring2(filename);
	}

	long long calc3(const char* filename) { return -1; }
	long long calc4(const char* filename) { return -1; }
}