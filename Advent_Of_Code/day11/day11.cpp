#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>
namespace day11 {
	const unsigned int NUMBER_OF_LINES = 140; //10 140
	const unsigned int LINEWIDTH = 140; //10 140
	struct Galaxy {
	public:
		int row;
		int column;
	};
	long long calc1(const char* filename) { 
		FILE* stream;
		int c;
		std::vector<Galaxy> galaxies;
		std::bitset<LINEWIDTH> rowToExpand;
		rowToExpand.set();
		std::bitset<NUMBER_OF_LINES> columnToExpand;
		columnToExpand.set();
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				if (c == '#') {
					galaxies.emplace_back(Galaxy{ row, column });
					rowToExpand[row] = 0;
					columnToExpand[column] = 0;
				}
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);
		long long result = 0;
		for (auto firstIndex = 0; firstIndex < galaxies.size(); ++firstIndex) {
			for (auto secondIndex = firstIndex + 1; secondIndex < galaxies.size(); ++secondIndex) {
				const auto& firstGalaxy = galaxies[firstIndex];
				const auto& secondGalaxy = galaxies[secondIndex];
				std::bitset<LINEWIDTH> temp;
				temp.set();
				temp <<= firstGalaxy.row;
				std::bitset<LINEWIDTH> temp2;
				temp2.set();
				temp2 <<= secondGalaxy.row;
				std::bitset<LINEWIDTH> bitMask = temp ^ temp2;
				std::bitset<LINEWIDTH> customRows = rowToExpand & bitMask;
				temp.set();
				temp <<= firstGalaxy.column;
				temp2.set();
				temp2 <<= secondGalaxy.column;
				std::bitset<LINEWIDTH> bitMask2 = temp ^ temp2;
				std::bitset<LINEWIDTH> customColumns = columnToExpand & bitMask2;
				result += customRows.count() + customColumns.count() + std::abs(firstGalaxy.row - secondGalaxy.row) + abs(firstGalaxy.column - secondGalaxy.column);
				}
		}
		return result; 
	}

	long long calc2(const char* filename) {
		FILE* stream;
		int c;
		std::vector<Galaxy> galaxies;
		std::bitset<LINEWIDTH> rowToExpand;
		rowToExpand.set();
		std::bitset<NUMBER_OF_LINES> columnToExpand;
		columnToExpand.set();
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				if (c == '#') {
					galaxies.emplace_back(Galaxy{ row, column });
					rowToExpand[row] = 0;
					columnToExpand[column] = 0;
				}
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);
		long long result = 0;
		for (auto firstIndex = 0; firstIndex < galaxies.size(); ++firstIndex) {
			for (auto secondIndex = firstIndex + 1; secondIndex < galaxies.size(); ++secondIndex) {
				const auto& firstGalaxy = galaxies[firstIndex];
				const auto& secondGalaxy = galaxies[secondIndex];
				std::bitset<LINEWIDTH> temp;
				temp.set();
				temp <<= firstGalaxy.row;
				std::bitset<LINEWIDTH> temp2;
				temp2.set();
				temp2 <<= secondGalaxy.row;
				std::bitset<LINEWIDTH> bitMask = temp ^ temp2;
				std::bitset<LINEWIDTH> customRows = rowToExpand & bitMask;
				temp.set();
				temp <<= firstGalaxy.column;
				temp2.set();
				temp2 <<= secondGalaxy.column;
				std::bitset<LINEWIDTH> bitMask2 = temp ^ temp2;
				std::bitset<LINEWIDTH> customColumns = columnToExpand & bitMask2;
				result += customRows.count() * 999999 + customColumns.count() * 999999 + std::abs(firstGalaxy.row - secondGalaxy.row) + abs(firstGalaxy.column - secondGalaxy.column);
				}
		}
		return result;
	}
	long long calc3(const char* filename) {
		FILE* stream;
		int c;
		std::vector<Galaxy> galaxies;
		std::bitset<LINEWIDTH> rowToExpand;
		rowToExpand.set();
		std::bitset<NUMBER_OF_LINES> columnToExpand;
		columnToExpand.set();
		int galaxyIndex = 0;
		int leftestGalaxy = LINEWIDTH + 1;
		int leftestGalaxyIndex;
		int uppestGalaxy = NUMBER_OF_LINES + 1;
		int uppestGalaxyIndex;
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				if (c == '#') {
					galaxies.emplace_back(Galaxy{ row, column });
					rowToExpand[row] = 0;
					columnToExpand[column] = 0;
					if (column < leftestGalaxy) {
						leftestGalaxyIndex = galaxyIndex;
						leftestGalaxy = column;
					}
					if (row < uppestGalaxy) {
						uppestGalaxyIndex = galaxyIndex;
						uppestGalaxy = row;
						}
					++galaxyIndex;
				}
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);
		long long result = 0;
		auto firstIndex = 0;
		std::vector<int> widths;
		for (auto index = 0; index < galaxies.size(); ++index) {
			if (index == leftestGalaxyIndex) continue;
			const auto& firstGalaxy = galaxies[leftestGalaxyIndex];
			const auto& secondGalaxy = galaxies[index];
			std::bitset<LINEWIDTH> temp;
			std::bitset<LINEWIDTH> temp2;
			temp.set();
			temp <<= firstGalaxy.column;
			temp2.set();
			temp2 <<= secondGalaxy.column;
			std::bitset<LINEWIDTH> bitMask = temp ^ temp2;
			std::bitset<LINEWIDTH> customColumns = columnToExpand & bitMask;
			widths.emplace_back(customColumns.count() + abs(firstGalaxy.column - secondGalaxy.column));
		}
		std::vector<int> heights;
		for (auto index = 0; index < galaxies.size(); ++index) {
			if (index == uppestGalaxyIndex) continue;
			const auto& firstGalaxy = galaxies[uppestGalaxyIndex];
			const auto& secondGalaxy = galaxies[index];
			std::bitset<LINEWIDTH> temp;
			std::bitset<LINEWIDTH> temp2;
			temp.set();
			temp <<= firstGalaxy.row;
			temp2.set();
			temp2 <<= secondGalaxy.row;
			std::bitset<LINEWIDTH> bitMask = temp ^ temp2;
			std::bitset<LINEWIDTH> customRows = rowToExpand & bitMask;
			heights.emplace_back(customRows.count() + abs(firstGalaxy.row - secondGalaxy.row));
		}
		std::sort(widths.begin(), widths.end());
		std::sort(heights.begin(), heights.end());
		auto temp = widths.back();
		result += temp * widths.size();
		widths.pop_back();
		while (widths.size()>1) {
				temp = widths.back() - widths.front();
				widths.pop_back();
				result += temp * widths.size();
				widths.erase(widths.begin());
		}
		temp = heights.back();
		result += temp * heights.size();
		heights.pop_back();
		while (heights.size()>1) {
				temp = heights.back() - heights.front();
				heights.pop_back();
				result += temp * heights.size();
				heights.erase(heights.begin());
			
		}
		return result;
	}
	long long calc4(const char* filename) {
		FILE* stream;
		int c;
		std::vector<Galaxy> galaxies;
		std::bitset<LINEWIDTH> rowToExpand;
		rowToExpand.set();
		std::bitset<NUMBER_OF_LINES> columnToExpand;
		columnToExpand.set();
		int galaxyIndex = 0;
		int leftestGalaxy = LINEWIDTH + 1;
		int leftestGalaxyIndex;
		int uppestGalaxy = NUMBER_OF_LINES + 1;
		int uppestGalaxyIndex;
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				if (c == '#') {
					galaxies.emplace_back(Galaxy{ row, column });
					rowToExpand[row] = 0;
					columnToExpand[column] = 0;
					if (column < leftestGalaxy) {
						leftestGalaxyIndex = galaxyIndex;
						leftestGalaxy = column;
					}
					if (row < uppestGalaxy) {
						uppestGalaxyIndex = galaxyIndex;
						uppestGalaxy = row;
					}
					++galaxyIndex;
				}
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);
		long long result = 0;
		auto firstIndex = 0;
		std::vector<int> widths;
		for (auto index = 0; index < galaxies.size(); ++index) {
			if (index == leftestGalaxyIndex) continue;
			const auto& firstGalaxy = galaxies[leftestGalaxyIndex];
			const auto& secondGalaxy = galaxies[index];
			std::bitset<LINEWIDTH> temp;
			std::bitset<LINEWIDTH> temp2;
			temp.set();
			temp <<= firstGalaxy.column;
			temp2.set();
			temp2 <<= secondGalaxy.column;
			std::bitset<LINEWIDTH> bitMask = temp ^ temp2;
			std::bitset<LINEWIDTH> customColumns = columnToExpand & bitMask;
			widths.emplace_back(999999 * customColumns.count() + abs(firstGalaxy.column - secondGalaxy.column));
		}
		std::vector<int> heights;
		for (auto index = 0; index < galaxies.size(); ++index) {
			if (index == uppestGalaxyIndex) continue;
			const auto& firstGalaxy = galaxies[uppestGalaxyIndex];
			const auto& secondGalaxy = galaxies[index];
			std::bitset<LINEWIDTH> temp;
			std::bitset<LINEWIDTH> temp2;
			temp.set();
			temp <<= firstGalaxy.row;
			temp2.set();
			temp2 <<= secondGalaxy.row;
			std::bitset<LINEWIDTH> bitMask = temp ^ temp2;
			std::bitset<LINEWIDTH> customRows = rowToExpand & bitMask;
			heights.emplace_back(999999*customRows.count() + abs(firstGalaxy.row - secondGalaxy.row));
		}
		std::sort(widths.begin(), widths.end());
		std::sort(heights.begin(), heights.end());
		auto temp = widths.back();
		result += temp * widths.size();
		widths.pop_back();
		while (widths.size() > 1) {
			temp = widths.back() - widths.front();
			widths.pop_back();
			result += temp * widths.size();
			widths.erase(widths.begin());
		}
		temp = heights.back();
		result += temp * heights.size();
		heights.pop_back();
		while (heights.size() > 1) {
			temp = heights.back() - heights.front();
			heights.pop_back();
			result += temp * heights.size();
			heights.erase(heights.begin());

		}
		return result;
	}
}