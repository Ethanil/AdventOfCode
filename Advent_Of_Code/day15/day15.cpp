#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <array>
namespace day15 {
	const unsigned int NUMBER_OF_LINES = 10; //
	const unsigned int LINEWIDTH = 10; //
	long long calc1(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		long long result = 0;
		int intermediateResult = 0;
		while ((c = _getc_nolock(stream)) != -1) {
			if (c == '\n') continue;
			if (c == ',') {
				result += intermediateResult;
				intermediateResult = 0;
				continue;
			}
			intermediateResult += c;
			intermediateResult *= 17;
			intermediateResult %= 256;
		}
		result += intermediateResult;
		_fclose_nolock(stream);

		return result;
	}
	long long calc2(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		std::array<std::vector<std::pair<std::string, int>>,256> map;
		int intermediateResult = 0;
		std::string curString = "";
		while ((c = _getc_nolock(stream)) != -1) {
			switch (c) {
			case '\n': continue;
			case '-': 
			{
				for (auto it = map[intermediateResult].begin(); it != map[intermediateResult].end(); ++it) {
					if (it->first == curString) {
						map[intermediateResult].erase(it); 
						break;
					}
				}
				intermediateResult = 0;
				curString = "";
				break; 
			}
			case '=': 
			{
				c = _getc_nolock(stream);
				bool foundPair = false;
				for (auto it = map[intermediateResult].begin(); it != map[intermediateResult].end(); ++it) {
					if (it->first == curString) {
						it->second = c - '0';
						foundPair = true;
					}
				}
				if (!foundPair) map[intermediateResult].emplace_back(std::make_pair(curString, c - '0'));
				intermediateResult = 0;
				curString = "";
				break; 
			}
			case ',': continue;
			default:
				intermediateResult += c;
				intermediateResult *= 17;
				intermediateResult %= 256;
				curString += c;
			}
		}
		_fclose_nolock(stream);
		long long result = 0;
		for (size_t arrayIndex = 0; arrayIndex < map.size(); ++arrayIndex) {
			for (int vecIndex = 0; vecIndex < map[arrayIndex] .size(); ++vecIndex) {
				result += map[arrayIndex][vecIndex].second * (vecIndex+1) * (arrayIndex+1);
			}
		}
		return result;
	}
	long long calc3(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);

			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);

		return -1;
	}
	long long calc4(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);

			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);

		return -1;
	}
}