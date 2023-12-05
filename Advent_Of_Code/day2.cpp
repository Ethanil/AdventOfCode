#include "day2.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
namespace day2 {

	bool checkSubstring1(std::string stringToCheck, int numberOfAllowedReds = 12, int numberOfAllowedGreens = 13, int numberOfAllowedBlues = 14) {
		int lastNumberFound = 0;
		for (int i = 0; i < stringToCheck.length(); ++i) {
			char c = stringToCheck[i];
			switch (c) {
			case ',':case ' ':case ';':
				break;
			case 'G':
				i += 4;
				while (stringToCheck[i] >= '0' && stringToCheck[i] <= '9') ++i;
			case 'g':
				if (lastNumberFound > numberOfAllowedGreens) return false;
				lastNumberFound = 0;
				i += 4;
				break;
			case 'r':
				if (lastNumberFound > numberOfAllowedReds) return false;
				lastNumberFound = 0;
				i += 2;
				break;
			case 'b':
				if (lastNumberFound > numberOfAllowedBlues) return false;
				lastNumberFound = 0;
				i += 3;
				break;
			default:
				lastNumberFound = lastNumberFound * 10 + (c - '0');
				break;
			}
		}
		return true;
	}

	int calc1(const char* filename) {
		std::ifstream in(filename);
		std::string line;
		int gameNumber = 1;
		int sum = 0;
		while (std::getline(in, line)) {
			if(checkSubstring1(line)) sum += gameNumber;
			++gameNumber;
		}
		return sum;
	}

	struct SetOfCubes {
	public:
		int redCubes;
		int greenCubes;
		int blueCubes;
		void merge(SetOfCubes other, const int& (*mergeFunction)(const int&, const int&)) {
			redCubes = mergeFunction(other.redCubes, redCubes);
			greenCubes = mergeFunction(other.greenCubes, greenCubes);
			blueCubes = mergeFunction(other.blueCubes, blueCubes);
		}
		int power() {
			return redCubes * greenCubes * blueCubes;
		}
	};
	std::ostream& operator<<(std::ostream& os, const SetOfCubes& cubes){

		os << "{r:"<<cubes.redCubes<<" g:"<<cubes.greenCubes<<" b:"<<cubes.blueCubes<<"}";
		return os;

	}

	SetOfCubes checkSubstring2(std::string stringToCheck) {
		int lastNumberFound = 0;
		SetOfCubes result{ 0,0,0 };
		SetOfCubes cubes{ 0,0,0 };
		for (int i = 0; i < stringToCheck.length(); ++i) {
			char c = stringToCheck[i];
			switch (c) {
			case ',': case' ':case':':
				break;
			case 'G':
				i += 3;
				while (stringToCheck[i] >= '0' && stringToCheck[i] <= '9') ++i;
			case 'g':
				cubes.greenCubes = lastNumberFound;
				lastNumberFound = 0;
				i += 4;
				break;
			case 'r':
				cubes.redCubes = lastNumberFound;
				lastNumberFound = 0;
				i += 2;
				break;
			case 'b':
				cubes.blueCubes = lastNumberFound;
				lastNumberFound = 0;
				i += 3;
				break;
			case ';':
				std::cout << cubes<< "\n";
				result.merge(cubes,&std::max);
				cubes={ 0,0,0 };
				break;
			default:
					lastNumberFound = lastNumberFound * 10 + (c - '0');
				break;
			}
		}
		std::cout << cubes << "\n";
		result.merge(cubes, &std::max);
		return result;
	}

	int calc2(const char* filename) {
		std::ifstream in(filename);
		std::string line;
		int sum = 0;
		while (std::getline(in, line)) {
			sum += checkSubstring2(line).power();
		}
		return sum;
	}
}