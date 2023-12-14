#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
namespace day14 {
	const unsigned int NUMBER_OF_LINES = 100; //
	const unsigned int LINEWIDTH = 100; //
	long long calc1(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		unsigned int nextNorthestPositions[NUMBER_OF_LINES];
		std::fill(std::begin(nextNorthestPositions), std::end(nextNorthestPositions), NUMBER_OF_LINES);
		long long result = 0;
		for (int row = NUMBER_OF_LINES; row > 0; --row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				switch (c) {
				case '#':
					nextNorthestPositions[column] = row - 1;
					break;
				case 'O':
					result += nextNorthestPositions[column];
					--nextNorthestPositions[column];
					break;
				default:
					break;
				}
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);

		return result;
	}
	static void tiltNorth(std::array<std::array<int, LINEWIDTH>, NUMBER_OF_LINES>& platform) {
		unsigned int nextNorthestPositions[NUMBER_OF_LINES];
		std::fill(std::begin(nextNorthestPositions), std::end(nextNorthestPositions), 0);
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				switch (platform[row][column]) {
				case 'O':
					platform[row][column] = '.';
					platform[nextNorthestPositions[column]][column] = 'O';
					++nextNorthestPositions[column];
					break;
				case '#':
					nextNorthestPositions[column] = row + 1;
					break;
				case '.':
					break;
				}
			}
		}
	}
	static void tiltWest(std::array<std::array<int, LINEWIDTH>, NUMBER_OF_LINES>& platform) {
		unsigned int nextWestestPositions[NUMBER_OF_LINES];
		std::fill(std::begin(nextWestestPositions), std::end(nextWestestPositions), 0);
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				switch (platform[row][column]) {
				case 'O':
					platform[row][column] = '.';
					platform[row][nextWestestPositions[row]] = 'O';
					++nextWestestPositions[row];
					break;
				case '#':
					nextWestestPositions[row] = column + 1;
					break;
				case '.':
					break;
				}
			}
		}
	}
	static void tiltSouth(std::array<std::array<int, LINEWIDTH>, NUMBER_OF_LINES>& platform) {
		unsigned int nextSouthestPositions[NUMBER_OF_LINES];
		std::fill(std::begin(nextSouthestPositions), std::end(nextSouthestPositions), NUMBER_OF_LINES - 1);
		for (int row = NUMBER_OF_LINES - 1; row >= 0; --row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				switch (platform[row][column]) {
				case 'O':
					platform[row][column] = '.';
					platform[nextSouthestPositions[column]][column] = 'O';
					--nextSouthestPositions[column];
					break;
				case '#':
					nextSouthestPositions[column] = row - 1;
					break;
				case '.':
					break;
				}
			}
		}

	}
	static void tiltEast(std::array<std::array<int, LINEWIDTH>, NUMBER_OF_LINES>& platform) {
		unsigned int nextEastestPositions[NUMBER_OF_LINES];
		std::fill(std::begin(nextEastestPositions), std::end(nextEastestPositions), LINEWIDTH - 1);
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = LINEWIDTH - 1; column >= 0; --column) {
				switch (platform[row][column]) {
				case 'O':
					platform[row][column] = '.';
					platform[row][nextEastestPositions[row]] = 'O';
					--nextEastestPositions[row];
					break;
				case '#':
					nextEastestPositions[row] = column - 1;
					break;
				case '.':
					break;
				}
			}
		}
	}
	static void cycle(std::array<std::array<int, LINEWIDTH>, NUMBER_OF_LINES>& platform) {
		tiltNorth(platform);
		tiltWest(platform);
		tiltSouth(platform);
		tiltEast(platform);
	}

	long long calculateLoad(std::array<std::array<int, LINEWIDTH>, NUMBER_OF_LINES>& platform) {
		long long load = 0;
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				if (platform[row][column] == 'O') {
					load += NUMBER_OF_LINES - row;
				}
			}
		}
		return load;
	}


	long long calc2(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		std::array<std::array<int, LINEWIDTH>, NUMBER_OF_LINES> platform;
		long long result = 0;
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				platform[row][column] = c;

			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);
		std::vector<std::array<std::array<int, LINEWIDTH>, NUMBER_OF_LINES>> history;
		for (int i = 0; i < 1000000000; ++i) {
			history.emplace_back(platform);
			cycle(platform);
			for (int j = 0; j < history.size();++j) {
				if (history[j] == platform) {
					int cycleLength = i + 1 - j;
					return calculateLoad(history[j + ((1000000000 - j) % cycleLength)]);
				}
			}
		}
		return calculateLoad(platform);
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