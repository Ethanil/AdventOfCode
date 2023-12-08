#include "day1.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
namespace day1 {
	long long calc1(const char* file) {
		std::ifstream infile(file);
		std::string line;
		int sum = 0;
		int firstDigitInLine = -1;
		size_t firstDigitOccuredAt = std::string::npos;
		int lastDigitInLine = -1;
		size_t lastDigitOccuredAt = std::string::npos;
		size_t firstOccurences[10];
		size_t lastOccurences[10];
		std::string digitWords[10] = { "zero","one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
		int lineNumber = 1;
		while (std::getline(infile, line)) {
			for (int i = 0; i < 10; ++i) {
				size_t digitAsWord = line.find(digitWords[i]);
				size_t digitAsNumber = line.find(std::to_string(i));
				firstOccurences[i] = std::min(digitAsNumber, digitAsWord);
				if (digitAsWord != std::string::npos)
				{
					digitAsWord = line.rfind(digitWords[i]);
				}
				if (digitAsNumber != std::string::npos)
				{
					digitAsNumber = line.rfind(std::to_string(i));
				}
				if (digitAsNumber != std::string::npos || digitAsWord != std::string::npos) {
					if (digitAsNumber == std::string::npos) {
						lastOccurences[i] = digitAsWord;
					}
					else if (digitAsWord == std::string::npos) {
						lastOccurences[i] = digitAsNumber;
					}
					else {
						lastOccurences[i] = std::max(digitAsNumber, digitAsWord);
					}
					if (lastDigitOccuredAt == std::string::npos || lastOccurences[i] > lastDigitOccuredAt) {
						lastDigitOccuredAt = lastOccurences[i];
						lastDigitInLine = i;
					}

				}
				else {
					lastOccurences[i] = std::string::npos;
				}
				if (firstOccurences[i] < firstDigitOccuredAt) {
					firstDigitOccuredAt = firstOccurences[i];
					firstDigitInLine = i;
				}
			}
			sum += firstDigitInLine * 10 + lastDigitInLine;
			//std::cout << lineNumber << ": " << firstDigitInLine * 10 + lastDigitInLine << " : "<<sum << "\n";
			lineNumber++;
			firstDigitOccuredAt = std::string::npos;
			lastDigitOccuredAt = std::string::npos;
		}
		return sum;
	}
	long long calc2(const char* filename) { return -1; }
	long long calc3(const char* filename) { return -1; }
	long long calc4(const char* filename) { return -1; }
}