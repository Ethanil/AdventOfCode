#include <fstream>
#include <iostream>
#include <vector>
namespace day13 {
	const unsigned int NUMBER_OF_LINES = 10; //
	const unsigned int LINEWIDTH = 10; //
	long long calc1(const char* filename) {
		FILE* stream;
		int c;
		long long result = 0;
		fopen_s(&stream, filename, "r");
		while (true) {
			std::vector<std::vector<int>> lines;
			c = _getc_nolock(stream);
			while (true) {
				lines.emplace_back(std::vector<int>());
				while (c != '\n' && c != -1) {
					lines.back().emplace_back(c);
					c = _getc_nolock(stream);
				}
				if (c == '\n') c = _getc_nolock(stream);
				if (c == '\n' || c == -1) break;
			}
			//end Of Pattern
			//check vertical
			int verticalLine = lines.back().size() - 2;
			while (verticalLine >= 0) {
				bool foundAMatch = true;
				for (const auto& lineToCheck : lines) {
					int columnsToCheck = std::max(1,std::min(static_cast<int>(lines.back().size()) - 1 - verticalLine, verticalLine));
					for (int columnOffset = 0; columnOffset < columnsToCheck; ++columnOffset) {
						if (lineToCheck[verticalLine - columnOffset] != lineToCheck[verticalLine + columnOffset + 1]) {
							--verticalLine;
							foundAMatch = false;
							break;
						}
					}
					if (!foundAMatch) break;
				}
				if (foundAMatch) {
					break;
				}
			}
			++verticalLine;
			//check horizontal
			int horizontalLine = 0;
			if (!verticalLine) {

				horizontalLine = lines.size() - 2;
				while (horizontalLine >= 0) {
					bool foundAMatch = true;
					for (int column = 0; column < lines.back().size(); ++column) {
						int linesToCheck = std::max(1,std::min(static_cast<int>(lines.size()) - 1 - horizontalLine, horizontalLine));
						for (int lineOffset = 0; lineOffset < linesToCheck; ++lineOffset) {
							if (lines[horizontalLine - lineOffset][column] != lines[horizontalLine + lineOffset + 1][column]) {
								--horizontalLine;
								foundAMatch = false;
								break;
							}
						}
						if (!foundAMatch) break;
					}
					if (foundAMatch) {
						++horizontalLine;
						break;
					}
				}
			}
			result += verticalLine + 100 * horizontalLine;
			if (c == -1) break;
		}
		_fclose_nolock(stream);

		return result;
	}
	long long calc2(const char* filename) {
		FILE* stream;
		int c;
		long long result = 0;
		fopen_s(&stream, filename, "r");
		while (true) {
			std::vector<std::vector<int>> lines;
			c = _getc_nolock(stream);
			while (true) {
				lines.emplace_back(std::vector<int>());
				while (c != '\n' && c != -1) {
					lines.back().emplace_back(c);
					c = _getc_nolock(stream);
				}
				if (c == '\n') c = _getc_nolock(stream);
				if (c == '\n' || c == -1) break;
			}
			//end Of Pattern
			//check vertical
			int verticalLine = lines.back().size() - 2;
			while (verticalLine >= 0) {
				bool foundAMatch = true;
				bool fixedOneSmudge = false;
				for (const auto& lineToCheck : lines) {
					int columnsToCheck = std::min(static_cast<int>(lines.back().size()) - 1 - verticalLine, verticalLine + 1);
					for (int columnOffset = 0; columnOffset < columnsToCheck; ++columnOffset) {
						if (lineToCheck[verticalLine - columnOffset] != lineToCheck[verticalLine + columnOffset + 1]) {
							if (!fixedOneSmudge) {
								fixedOneSmudge = true;
								continue;
							}
							--verticalLine;
							foundAMatch = false;
							break;
						}
					}
					if (!foundAMatch) break;
				}
				if (foundAMatch && fixedOneSmudge) {
					break;
				}
				else if (!fixedOneSmudge) { --verticalLine; }
			}
			++verticalLine;
			//check horizontal
			int horizontalLine = 0;
			if (!verticalLine) {

				horizontalLine = lines.size() - 2;
				while (horizontalLine >= 0) {
					bool foundAMatch = true;
					bool fixedOneSmudge = false;
					for (int column = 0; column < lines.back().size(); ++column) {
						int linesToCheck = std::min(static_cast<int>(lines.size()) - 1 - horizontalLine, horizontalLine + 1);
						for (int lineOffset = 0; lineOffset < linesToCheck; ++lineOffset) {
							if (lines[horizontalLine - lineOffset][column] != lines[horizontalLine + lineOffset + 1][column]) {
								if (!fixedOneSmudge) {
									fixedOneSmudge = true;
									continue;
								}
								--horizontalLine;
								foundAMatch = false;
								break;
							}
						}
						if (!foundAMatch) break;
					}
					if (foundAMatch && fixedOneSmudge) {
						break;
					} if (!fixedOneSmudge) { --horizontalLine; }
				}
				++horizontalLine;
			}
			result += verticalLine + 100 * horizontalLine;
			if (c == -1) break;
		}
		_fclose_nolock(stream);

		return result;
	}
	long long calc3(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		_fclose_nolock(stream);

		return -1;
	}
	long long calc4(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		_fclose_nolock(stream);

		return -1;
	}
}