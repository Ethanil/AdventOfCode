#include <fstream>
#include <iostream>
#include <bitset>
#include "day10.hpp"
namespace day10 {
	const int LINEWIDTH = 140; //5 140
	const int NUMBER_OF_LINES = 140; //5 140
	int counter = 0;
	enum class  Direction :uint8_t
	{
		UP, RIGHT, DOWN, LEFT, NONE
	};
	struct pipe {
	public:
		unsigned int leftConnection : 1;
		unsigned int rightConnection : 1;
		unsigned int upConnection : 1;
		unsigned int downConnection : 1;
		unsigned int pipeLength : 28;
	};
	struct pipe2 {
	public:
		Direction firstConnection : 3;
		Direction secondConnection : 3;
		uint8_t gotVisited : 1;
		uint8_t isMainLoop : 1;
	};
	long long calc1(const char* filename) {
		FILE* stream;
		pipe pipes[NUMBER_OF_LINES][LINEWIDTH];
		int c;
		fopen_s(&stream, filename, "r");
		int s_lineNumber;
		int s_pipeNumber;
		for (int line = 0; line < NUMBER_OF_LINES; ++line) {
			for (int pipe = 0; pipe < LINEWIDTH; ++pipe) {
				c = _fgetc_nolock(stream);
				switch (c) {
				case '.':
					pipes[line][pipe] = { 0, 0, 0, 0, 0 };
					break;
				case '7':
					pipes[line][pipe] = { 1, 0, 0, 1, 0 };
					break;
				case '|':
					pipes[line][pipe] = { 0, 0, 1, 1, 0 };
					break;
				case 'J':
					pipes[line][pipe] = { 1, 0, 1, 0, 0 };
					break;
				case '-':
					pipes[line][pipe] = { 1, 1, 0, 0, 0 };
					break;
				case 'L':
					pipes[line][pipe] = { 0, 1, 1, 0, 0 };
					break;
				case 'F':
					pipes[line][pipe] = { 0, 1, 0, 1, 0 };
					break;
				case 'S':
					s_lineNumber = line;
					s_pipeNumber = pipe;
					pipes[line][pipe] = { 0, 0, 0, 0, 0 };
					break;
				default:
					std::cout << "we found something strange!!!!" << c << "\n";
				}
			}
			_fgetc_nolock(stream);
		}
		_fclose_nolock(stream);
		bool setLastDirection = false;
		unsigned int linePos = s_lineNumber;
		unsigned int pipePos = s_pipeNumber;
		bool lastDirectionWasLeft = 0;
		bool lastDirectionWasRight = 0;
		bool lastDirectionWasUp = 0;
		bool lastDirectionWasDown = 0;
		if (s_pipeNumber > 0 && pipes[s_lineNumber][s_pipeNumber - 1].rightConnection) {
			pipes[s_lineNumber][s_pipeNumber].leftConnection = 1;
			lastDirectionWasLeft = 1;
			setLastDirection = true;
			--pipePos;
		}
		if (s_pipeNumber < LINEWIDTH - 1 && pipes[s_lineNumber][s_pipeNumber + 1].leftConnection) {
			pipes[s_lineNumber][s_pipeNumber].rightConnection = 1;
			if (!setLastDirection) {
				lastDirectionWasRight = 1;
				setLastDirection = true;
				++pipePos;
			}
		}
		if (s_lineNumber > 0 && pipes[s_lineNumber - 1][s_pipeNumber].downConnection) {
			pipes[s_lineNumber][s_pipeNumber].upConnection = 1;
			if (!setLastDirection) {
				lastDirectionWasUp = 1;
				setLastDirection = true;
				--linePos;
			}
		}
		if (s_lineNumber < NUMBER_OF_LINES - 1 && pipes[s_lineNumber + 1][s_pipeNumber].upConnection) {
			pipes[s_lineNumber][s_pipeNumber].downConnection = 1;
		}
		pipes[linePos][pipePos].pipeLength = 1;
		unsigned int curPipeLength = 1;
		while (linePos != s_lineNumber || pipePos != s_pipeNumber) {
			bool nextDirectionIsRight = pipes[linePos][pipePos].rightConnection - lastDirectionWasLeft;
			bool nextDirectionIsLeft = pipes[linePos][pipePos].leftConnection - lastDirectionWasRight;
			bool nextDirectionIsDown = pipes[linePos][pipePos].downConnection - lastDirectionWasUp;
			bool nextDirectionIsUp = pipes[linePos][pipePos].upConnection - lastDirectionWasDown;
			lastDirectionWasRight = nextDirectionIsRight;
			lastDirectionWasLeft = nextDirectionIsLeft;
			lastDirectionWasUp = nextDirectionIsUp;
			lastDirectionWasDown = nextDirectionIsDown;
			pipePos += lastDirectionWasRight;
			pipePos -= lastDirectionWasLeft;
			linePos += lastDirectionWasDown;
			linePos -= lastDirectionWasUp;
			pipes[linePos][pipePos].pipeLength == ++curPipeLength;
		}



		return curPipeLength / 2;
	}
	static Direction oppositeDirection(Direction direction) {
		return static_cast<Direction>((static_cast<unsigned int>(direction) + 2) % 4);
	}

	static void changePositions(Direction& direction, unsigned int& linePos, unsigned int& pipePos) {
		switch (direction) {
		case Direction::DOWN:
			++linePos;
			break;
		case Direction::UP:
			--linePos;
			break;
		case Direction::RIGHT:
			++pipePos;
			break;
		case Direction::LEFT:
			--pipePos;
			break;
		}
	}
	static bool isOuter(pipe2(&pipes)[NUMBER_OF_LINES][LINEWIDTH], unsigned int linePos, unsigned int pipePos) {
		if (pipes[linePos][pipePos].isMainLoop) return false;
		unsigned int startPipe = pipePos;
		//check to the right
		unsigned int rightMainPipes = 0;
		while (++pipePos < LINEWIDTH) {
			if (pipes[linePos][pipePos].isMainLoop && (pipes[linePos][pipePos].firstConnection == Direction::UP || pipes[linePos][pipePos].secondConnection == Direction::UP)) ++rightMainPipes;
		}
		return rightMainPipes % 2 == 1;
		//check to the left
		pipePos = startPipe;
		unsigned int leftMainPipes = 0;
		while (--pipePos < LINEWIDTH) {
			if (pipes[linePos][pipePos].isMainLoop && (pipes[linePos][pipePos].firstConnection == Direction::UP || pipes[linePos][pipePos].secondConnection == Direction::UP)) ++leftMainPipes;
		}
		if (leftMainPipes % 2 == 1) return false;
	}

	long long calc2(const char* filename) {
		FILE* stream;
		pipe2 pipes[NUMBER_OF_LINES][LINEWIDTH];
		int c;
		fopen_s(&stream, filename, "r");
		int s_lineNumber;
		int s_pipeNumber;
		for (int line = 0; line < NUMBER_OF_LINES; ++line) {
			for (int pipe = 0; pipe < LINEWIDTH; ++pipe) {
				c = _fgetc_nolock(stream);
				switch (c) {
				case '.':
					pipes[line][pipe] = { Direction::NONE, Direction::NONE, 0, 0};
					break;
				case '7':
					pipes[line][pipe] = { Direction::LEFT, Direction::DOWN, 0, 0 };
					break;
				case '|':
					pipes[line][pipe] = { Direction::UP, Direction::DOWN, 0, 0 };
					break;
				case 'J':
					pipes[line][pipe] = { Direction::UP, Direction::LEFT, 0, 0 };
					break;
				case '-':
					pipes[line][pipe] = { Direction::LEFT, Direction::RIGHT, 0, 0 };
					break;
				case 'L':
					pipes[line][pipe] = { Direction::UP, Direction::RIGHT, 0, 0 };
					break;
				case 'F':
					pipes[line][pipe] = { Direction::RIGHT, Direction::DOWN, 0, 0 };
					break;
				case 'S':
					s_lineNumber = line;
					s_pipeNumber = pipe;
					pipes[line][pipe] = { Direction::NONE, Direction::NONE, 0, 1};
					break;
				default:
					std::cout << "we found something strange!!!!" << c << "\n";
				}
				
			}
			_fgetc_nolock(stream);
		}
		_fclose_nolock(stream);
		unsigned int linePos =s_lineNumber;
		unsigned int pipePos =s_pipeNumber;
		Direction curDirection;
		//deduce S-pipe connections
		if (s_pipeNumber > 0 && (pipes[s_lineNumber][s_pipeNumber - 1].firstConnection == Direction::RIGHT || pipes[s_lineNumber][s_pipeNumber - 1].secondConnection == Direction::RIGHT)) {
			pipes[s_lineNumber][s_pipeNumber].firstConnection = Direction::LEFT;
			curDirection = Direction::LEFT;
		}
		if (s_pipeNumber < LINEWIDTH - 1 && (pipes[s_lineNumber][s_pipeNumber + 1].firstConnection == Direction::LEFT || pipes[s_lineNumber][s_pipeNumber + 1].secondConnection == Direction::LEFT)) {
			if (pipes[s_lineNumber][s_pipeNumber].firstConnection == Direction::NONE) {
				pipes[s_lineNumber][s_pipeNumber].firstConnection = Direction::RIGHT;
				curDirection = Direction::RIGHT;
			}
			else { 
				pipes[s_lineNumber][s_pipeNumber].secondConnection = Direction::RIGHT;
			}
		}
		if (s_lineNumber > 0 && (pipes[s_lineNumber - 1][s_pipeNumber].firstConnection == Direction::DOWN || pipes[s_lineNumber - 1][s_pipeNumber].secondConnection == Direction::DOWN)) {
			if (pipes[s_lineNumber][s_pipeNumber].firstConnection == Direction::NONE) {
				pipes[s_lineNumber][s_pipeNumber].firstConnection = Direction::UP;
				curDirection = Direction::UP;
			}
			else {
				pipes[s_lineNumber][s_pipeNumber].secondConnection = Direction::UP;
			}
		}
		if (s_lineNumber < NUMBER_OF_LINES - 1 && (pipes[s_lineNumber + 1][s_pipeNumber].firstConnection == Direction::UP || pipes[s_lineNumber + 1][s_pipeNumber].secondConnection == Direction::UP)) {
			pipes[s_lineNumber][s_pipeNumber].secondConnection = Direction::DOWN;

		}
		changePositions(curDirection, linePos, pipePos);
		while (s_lineNumber != linePos || s_pipeNumber != pipePos) {
			pipe2* curPipe = &pipes[linePos][pipePos];
			curPipe->isMainLoop = 1;
			if (curPipe->firstConnection == oppositeDirection(curDirection)) {
				curDirection = curPipe->secondConnection;
			}
			else {
				curDirection = curPipe->firstConnection;
			}
			changePositions(curDirection, linePos, pipePos);
		}

		long long result = 0;
		for (int line = 0; line < NUMBER_OF_LINES; ++line) {
			for (int pipe = 0; pipe < LINEWIDTH; ++pipe) {
				if (isOuter(pipes, line, pipe)) ++result;
			}
		}
		return result;

	}
	long long calc3(const char* filename) { 
		FILE* stream;
		pipe2 pipes[NUMBER_OF_LINES][LINEWIDTH];
		int c;
		fopen_s(&stream, filename, "r");
		int s_lineNumber;
		int s_pipeNumber;
		for (int line = 0; line < NUMBER_OF_LINES; ++line) {
			for (int pipe = 0; pipe < LINEWIDTH; ++pipe) {
				c = _fgetc_nolock(stream);
				switch (c) {
				case '.':
					pipes[line][pipe] = { Direction::NONE, Direction::NONE, 0, 0 };
					break;
				case '7':
					pipes[line][pipe] = { Direction::LEFT, Direction::DOWN, 0, 0 };
					break;
				case '|':
					pipes[line][pipe] = { Direction::UP, Direction::DOWN, 0, 0 };
					break;
				case 'J':
					pipes[line][pipe] = { Direction::UP, Direction::LEFT, 0, 0 };
					break;
				case '-':
					pipes[line][pipe] = { Direction::LEFT, Direction::RIGHT, 0, 0 };
					break;
				case 'L':
					pipes[line][pipe] = { Direction::UP, Direction::RIGHT, 0, 0 };
					break;
				case 'F':
					pipes[line][pipe] = { Direction::RIGHT, Direction::DOWN, 0, 0 };
					break;
				case 'S':
					s_lineNumber = line;
					s_pipeNumber = pipe;
					pipes[line][pipe] = { Direction::NONE, Direction::NONE, 0, 1 };
					break;
				default:
					std::cout << "we found something strange!!!!" << c << "\n";
				}

			}
			_fgetc_nolock(stream);
		}
		_fclose_nolock(stream);
		unsigned int linePos = s_lineNumber;
		unsigned int pipePos = s_pipeNumber;
		Direction curDirection;
		//deduce S-pipe connections
		if (s_pipeNumber > 0 && (pipes[s_lineNumber][s_pipeNumber - 1].firstConnection == Direction::RIGHT || pipes[s_lineNumber][s_pipeNumber - 1].secondConnection == Direction::RIGHT)) {
			pipes[s_lineNumber][s_pipeNumber].firstConnection = Direction::LEFT;
			curDirection = Direction::LEFT;
		}
		if (s_pipeNumber < LINEWIDTH - 1 && (pipes[s_lineNumber][s_pipeNumber + 1].firstConnection == Direction::LEFT || pipes[s_lineNumber][s_pipeNumber + 1].secondConnection == Direction::LEFT)) {
			if (pipes[s_lineNumber][s_pipeNumber].firstConnection == Direction::NONE) {
				pipes[s_lineNumber][s_pipeNumber].firstConnection = Direction::RIGHT;
				curDirection = Direction::RIGHT;
			}
			else {
				pipes[s_lineNumber][s_pipeNumber].secondConnection = Direction::RIGHT;
			}
		}
		if (s_lineNumber > 0 && (pipes[s_lineNumber - 1][s_pipeNumber].firstConnection == Direction::DOWN || pipes[s_lineNumber - 1][s_pipeNumber].secondConnection == Direction::DOWN)) {
			if (pipes[s_lineNumber][s_pipeNumber].firstConnection == Direction::NONE) {
				pipes[s_lineNumber][s_pipeNumber].firstConnection = Direction::UP;
				curDirection = Direction::UP;
			}
			else {
				pipes[s_lineNumber][s_pipeNumber].secondConnection = Direction::UP;
			}
		}
		if (s_lineNumber < NUMBER_OF_LINES - 1 && (pipes[s_lineNumber + 1][s_pipeNumber].firstConnection == Direction::UP || pipes[s_lineNumber + 1][s_pipeNumber].secondConnection == Direction::UP)) {
			pipes[s_lineNumber][s_pipeNumber].secondConnection = Direction::DOWN;

		}
		changePositions(curDirection, linePos, pipePos);
		long long result = 1;
		while (s_lineNumber != linePos || s_pipeNumber != pipePos) {
			pipe2* curPipe = &pipes[linePos][pipePos];
			curPipe->isMainLoop = 1;
			++result;
			if (curPipe->firstConnection == oppositeDirection(curDirection)) {
				curDirection = curPipe->secondConnection;
			}
			else {
				curDirection = curPipe->firstConnection;
			}
			changePositions(curDirection, linePos, pipePos);
		}


		return result/2;
	}
	long long calc4(const char* filename) { return -1; }
}