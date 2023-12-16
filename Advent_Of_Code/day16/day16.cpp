#include <fstream>
#include <iostream>
#include <map>
#include <vector>
namespace day16 {
	const unsigned int NUMBER_OF_LINES = 110; // 10 110
	const unsigned int LINEWIDTH = 110; // 10 110
	enum class Tile {
		Horizontal,	//	|
		Vertical,	//	-
		Forward,	//	/ 
		Backward,	//	\ 
		Empty		//	.
	};
	enum class Direction {
		Right,
		Up,
		Left,
		Down
	};
	std::map < Tile, std::map < Direction, std::vector< Direction> >> moveMap{
		{Tile::Horizontal,
			{
				{Direction::Left,	{Direction::Up, Direction::Down}},
				{Direction::Right,	{Direction::Up, Direction::Down}},
				{Direction::Up,		{Direction::Up }},
				{Direction::Down,	{Direction::Down }}
			}
		},
		{Tile::Vertical,
			{
				{Direction::Left,	{Direction::Left}},
				{Direction::Right,	{Direction::Right}},
				{Direction::Up,		{Direction::Left, Direction::Right}},
				{Direction::Down,	{Direction::Left, Direction::Right}}
			}
		},
		{Tile::Forward,
			{
				{Direction::Left,	{Direction::Down}},
				{Direction::Right,	{Direction::Up}},
				{Direction::Up,		{Direction::Right}},
				{Direction::Down,	{Direction::Left} }
			}
		},
		{ Tile::Backward,
			{
				{Direction::Left,	{Direction::Up}},
				{Direction::Right,	{Direction::Down} },
				{Direction::Up,		{Direction::Left}},
				{Direction::Down,	{Direction::Right}}
			}
		},
		{ Tile::Empty,
			{
				{Direction::Left,	{Direction::Left}},
				{Direction::Right,	{Direction::Right} },
				{Direction::Up,		{Direction::Up }},
				{Direction::Down,	{Direction::Down }}
			}
		}
	};
	struct TileWithMemory{
	public:	
		Tile tile;
		bool visitedOnce;
		std::map<Direction, bool> visited;
	};
	struct MemoizationElement {
		std::map<Direction, long long> directionToValue;
		std::map<Direction, bool> usedThisIteration;
	};
	struct Position {
		int x;
		int y;
		Direction direction;
		TileWithMemory& memTile;
	};
	static std::pair<int,int> getNextCoordinates(const Position& pos, const Direction& dir) {
		switch (dir) {
		case Direction::Left:
			return { pos.x, pos.y - 1 };
		case Direction::Right:
			return { pos.x , pos.y + 1 };
		case Direction::Up:
			return { pos.x - 1, pos.y };
		case Direction::Down:
			return { pos.x + 1, pos.y };
		}
	}
	static std::pair<int, int> getNextCoordinates(const int row, const int column, const Direction& dir) {
		switch (dir) {
		case Direction::Left:
			return { row, column - 1 };
		case Direction::Right:
			return { row , column + 1 };
		case Direction::Up:
			return { row - 1, column };
		case Direction::Down:
			return { row + 1, column };
		}
	}
	long long calc1(const char* filename) {
		FILE* stream;
		int c;
		TileWithMemory tiles[NUMBER_OF_LINES][LINEWIDTH];
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				switch (c) {
				case '|':
					tiles[row][column] = { Tile::Horizontal, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '-':
					tiles[row][column] = { Tile::Vertical, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '/':
					tiles[row][column] = { Tile::Forward, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '\\':
					tiles[row][column] = { Tile::Backward, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '.':
					tiles[row][column] = { Tile::Empty, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				default:
					std::cout << "we should not be here\n";
				}
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);

		std::vector<Position> curPositions = { {0, 0, Direction::Right, tiles[0][0]}};
		bool finished = false;
		long long result = 0;
		while (!finished) {
			finished = true;
			std::vector<Position> nextPositions;
			for (const auto& pos : curPositions) {
				auto it = pos.memTile.visited.find(pos.direction);
				if (it->second) continue; //we already visited from that direction
				if (!pos.memTile.visitedOnce) {
					++result;
					pos.memTile.visitedOnce = true;
				}
				finished = false;
				it->second = true;
				auto nextDirections = moveMap.find(pos.memTile.tile)->second.find(pos.direction)->second;
				for (const auto& dir : nextDirections) {
					auto nextCoordinates = getNextCoordinates(pos, dir);
					if (nextCoordinates.first < 0 || nextCoordinates.first >= LINEWIDTH || nextCoordinates.second < 0 || nextCoordinates.second >= NUMBER_OF_LINES) continue;
					nextPositions.emplace_back(nextCoordinates.first, nextCoordinates.second, dir, tiles[nextCoordinates.first][nextCoordinates.second]);
				}
			}
			curPositions.swap(nextPositions);
		}

		return result;
	}



	long long shootBeam(int row, int column, Direction direction, TileWithMemory(&tiles)[NUMBER_OF_LINES][LINEWIDTH], MemoizationElement(&memoizationArray)[NUMBER_OF_LINES][LINEWIDTH], int curId) {
		if (row < 0 || row >= NUMBER_OF_LINES || column < 0 || column >= LINEWIDTH) return 0;
		auto it = memoizationArray[row][column].directionToValue.find(direction);
		if (it != memoizationArray[row][column].directionToValue.end()) { 
			//auto usedIt = memoizationArray[row][column].directionToValue.find()
			return it->second; 
		}
		long long res = 1;
		auto nextDirections = moveMap.find(tiles[row][column].tile)->second.find(direction)->second;
		for (const auto& dir : nextDirections) {
			auto nextCoordinates = getNextCoordinates(row, column, dir);
			res += shootBeam(nextCoordinates.first, nextCoordinates.second, dir, tiles, memoizationArray, curId);
		}
		memoizationArray[row][column].directionToValue.emplace(direction, res);
		if (tiles[row][column].tile == Tile::Horizontal || tiles[row][column].tile == Tile::Vertical) {
			memoizationArray[row][column].directionToValue.emplace(static_cast<Direction>((static_cast<int>(direction) + 2) % 4), res);
		}
		return res;
	}


	long long calc2(const char* filename) {
		FILE* stream;
		int c;
		TileWithMemory tiles[NUMBER_OF_LINES][LINEWIDTH];
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				switch (c) {
				case '|':
					tiles[row][column] = { Tile::Horizontal, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '-':
					tiles[row][column] = { Tile::Vertical, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '/':
					tiles[row][column] = { Tile::Forward, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '\\':
					tiles[row][column] = { Tile::Backward, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '.':
					tiles[row][column] = { Tile::Empty, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				default:
					std::cout << "we should not be here\n";
				}
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);

		MemoizationElement memoizationArray[NUMBER_OF_LINES][LINEWIDTH];
		long long result = shootBeam(0, 0, Direction::Right, tiles, memoizationArray, 0);

		return result;
	}
	long long calc3(const char* filename) {
		FILE* stream;
		int c;
		TileWithMemory tiles[NUMBER_OF_LINES][LINEWIDTH];
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				switch (c) {
				case '|':
					tiles[row][column] = { Tile::Horizontal, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '-':
					tiles[row][column] = { Tile::Vertical, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '/':
					tiles[row][column] = { Tile::Forward, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '\\':
					tiles[row][column] = { Tile::Backward, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				case '.':
					tiles[row][column] = { Tile::Empty, false, {{Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false}} };
					break;
				default:
					std::cout << "we should not be here\n";
				}
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);

		long long endResult = 0;
		int curColumn = 0;
		int curRow = 0;
		while (curRow < NUMBER_OF_LINES && curColumn < LINEWIDTH) {
			//specialCase for corners
			for (int i = 0; i < 2; ++i) {
				if (i == 1 && !((curRow == 0 || curRow == NUMBER_OF_LINES - 1) && (curColumn == 0 || curColumn == LINEWIDTH - 1)))continue;
				std::vector<Position> curPositions;
				if ((curRow == 0 || curRow == NUMBER_OF_LINES - 1) && (curColumn == 0 || curColumn == LINEWIDTH - 1)) {
					if (i == 0) {
						if (curRow == 0) {
							curPositions.emplace_back(curRow, curColumn, Direction::Down, tiles[curRow][curColumn]);
						}
						if (curRow == NUMBER_OF_LINES - 1) {
							curPositions.emplace_back(curRow, curColumn, Direction::Up, tiles[curRow][curColumn]);
						}
					}
					else {
						if (curColumn == 0) {
							curPositions.emplace_back(curRow, curColumn, Direction::Right, tiles[curRow][curColumn]);
						}
						if (curColumn == LINEWIDTH - 1) {
							curPositions.emplace_back(curRow, curColumn, Direction::Left, tiles[curRow][curColumn]);
						}
					}
				}
				else {
					if (curRow == 0) {
						curPositions.emplace_back(curRow, curColumn, Direction::Down, tiles[curRow][curColumn]);
					}
					else if (curRow == NUMBER_OF_LINES - 1) {
						curPositions.emplace_back(curRow, curColumn, Direction::Up, tiles[curRow][curColumn]);
					}
					else if (curColumn == 0) {
						curPositions.emplace_back(curRow, curColumn, Direction::Right, tiles[curRow][curColumn]);
					}
					else if (curColumn == LINEWIDTH - 1) {
						curPositions.emplace_back(curRow, curColumn, Direction::Left, tiles[curRow][curColumn]);
					}
				}



				bool finished = false;
				long long result = 0;
				while (!finished) {
					finished = true;
					std::vector<Position> nextPositions;
					for (const auto& pos : curPositions) {
						auto it = pos.memTile.visited.find(pos.direction);
						if (it->second) continue; //we already visited from that direction
						if (!pos.memTile.visitedOnce) {
							++result;
							pos.memTile.visitedOnce = true;
						}
						finished = false;
						it->second = true;
						auto nextDirections = moveMap.find(pos.memTile.tile)->second.find(pos.direction)->second;
						for (const auto& dir : nextDirections) {
							auto nextCoordinates = getNextCoordinates(pos, dir);
							if (nextCoordinates.first < 0 || nextCoordinates.first >= LINEWIDTH || nextCoordinates.second < 0 || nextCoordinates.second >= NUMBER_OF_LINES) continue;
							nextPositions.emplace_back(nextCoordinates.first, nextCoordinates.second, dir, tiles[nextCoordinates.first][nextCoordinates.second]);
						}
					}
					curPositions.swap(nextPositions);
				}
				for (auto& row : tiles) {
					for (auto& tile : row) {
						tile.visited = { {Direction::Left, false},{Direction::Right, false},{Direction::Up, false},{Direction::Down, false} };
						tile.visitedOnce = false;
					}
				}
				endResult = std::max(endResult, result);
			}
			if (curRow == 0 || curRow == NUMBER_OF_LINES - 1) {
				++curColumn;
			}
			else if (curColumn >= LINEWIDTH) {
				curColumn = 0;
				++curRow;
			}
		}
			

		return endResult;
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