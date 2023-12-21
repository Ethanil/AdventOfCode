#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
namespace day18{
	const unsigned int NUMBER_OF_LINES = 14; //
	const unsigned int LINEWIDTH = 13; //
	struct Location {
		uint8_t dug:1;
		uint8_t inDirection : 3;
		uint8_t outDirection : 3;
	};

	int digColumn = 150;
	int digRow = NUMBER_OF_LINES - 1;
	int maxColumn = 150;
	int maxRow = NUMBER_OF_LINES - 1;
	int minColumn = INT_MAX;
	int minRow = INT_MAX;

	bool isInner(Location(&field)[NUMBER_OF_LINES * 2][700], int row, int column) {
		int countUpConnections = 0;
		while (column >= minColumn) {
			const auto& f = field[row][column];
			if (f.inDirection == 3 || f.outDirection == 3) ++countUpConnections;
			--column;
		}
		return (countUpConnections % 2) == 1;
	}

	long long calc1(const char* filename) {
		FILE* stream;
		int c;
		Location field[NUMBER_OF_LINES*2][700] = { 0 };
		//Location field[NUMBER_OF_LINES * 2][LINEWIDTH*2] = { 0 };
		//int startColumn = LINEWIDTH;
		//int startRow = NUMBER_OF_LINES - 1;
		//int digColumn = startColumn;
		//int digRow = startRow;
		//int maxColumn = startColumn;
		//int maxRow = startRow;
		//int minColumn = INT_MAX;
		//int minRow = INT_MAX;
		//bool setFirst = false;
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				 _getc_nolock(stream);
				 int k = _getc_nolock(stream);
				 int num = 0;
				 while (k >= '0' && k <= '9') {
					 num = num * 10 + k - '0';
					 k = _getc_nolock(stream);
				 }
				switch (c) {
				case 'R':
					for (int i = 0; i < num; ++i) {
						field[digRow][digColumn].outDirection = 2;
						field[digRow][digColumn].dug = 1;
						digColumn += 1;
						field[digRow][digColumn].inDirection = 1;
					}
					break;
				case 'L':
					for (int i = 0; i < num; ++i) {
						field[digRow][digColumn].outDirection = 1;
						field[digRow][digColumn].dug = 1;
						digColumn -= 1;
						field[digRow][digColumn].inDirection = 2;
					}
					break;
				case 'U':
					for (int i = 0; i < num; ++i) {
						field[digRow][digColumn].outDirection = 4;
						field[digRow][digColumn].dug = 1;
						digRow -= 1;
						field[digRow][digColumn].inDirection = 3;
					}
					break;
				case 'D':
					for (int i = 0; i < num; ++i) {
						field[digRow][digColumn].outDirection = 3;
						field[digRow][digColumn].dug = 1;
						digRow += 1;
						field[digRow][digColumn].inDirection = 4;
					}
					break;
				}
				maxColumn = std::max(maxColumn, digColumn);
				maxRow = std::max(maxRow, digRow);
				minColumn = std::min(minColumn, digColumn);
				minRow = std::min(minRow, digRow);
				for (int i = 0; i < 9; ++i) _getc_nolock(stream);
				break;


			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);

		//int difRow = digRow - startRow;
		//int difColumn = digColumn - startColumn;
		//if (difRow == 1) {
		//	field[digRow][digColumn].outDirection = 3;
		//	field[startRow][startColumn].inDirection = 3;
		//} else if (difRow == -1) {
		//	field[digRow][digColumn].outDirection = 4;
		//	field[startRow][startColumn].inDirection = 4;
		//}
		//if (difColumn == 1) {
		//	field[digRow][digColumn].outDirection = 1;
		//	field[startRow][startColumn].inDirection = 1;
		//}
		//else {
		//	field[digRow][digColumn].outDirection = 2;
		//	field[startRow][startColumn].inDirection = 2;
		//}


		std::ofstream outfile("test.txt");
		for (int row = minRow; row <= maxRow; ++row) {
			for (int column = minColumn; column <= maxColumn; ++column) {
				if (field[row][column].dug) {
					outfile <<"#";
				}
				else {
					outfile << ".";
				}
			}
			outfile << "\n";
		}
		outfile.close();
		long long result = 0;

		for (int row = minRow; row <= maxRow; ++row) {
			for (int column = minColumn; column <= maxColumn; ++column) {
				if (field[row][column].dug) {
					++result;
				}
				else if (isInner(field, row, column)) {
					++result;
				}
			}
		}



		//int count = 0;
		//for (int row = minRow; row <= maxRow; ++row) {
		//	count = 0;
		//	for (int column = minColumn; column <= maxColumn; ++column) {
		//		if (field[row][column].dug) {
		//			const auto& curField = field[row][column];
		//			if (!count && (curField.inDirection == 3 || curField.inDirection == 4)) count = curField.inDirection;
		//			else if(!count && (curField.outDirection == 3 || curField.outDirection == 4)) count = curField.outDirection;
		//			else if (count && ((count == 3 && 4 == curField.outDirection) || ((count == 4 && 3 == curField.inDirection)) || (count == 3 && 4 == curField.inDirection) || ((count == 4 && 3 == curField.outDirection)))) { count = 0; }
		//			++result;
		//		}
		//		else if (count) {
		//			++result; 
		//		}
		//	}
		//}







		//bool foundPrev = false;
		//bool countFieldSinceLast = false;
		//bool addFieldsSinceLast = true;
		//int fieldsSinceLast = 0;
		//for (int row = minRow; row <= maxRow; ++row) {
		//	foundPrev = false;
		//	countFieldSinceLast = false;
		//	fieldsSinceLast = 0;
		//	addFieldsSinceLast = true;
		//	for (int column = minColumn; column <= maxColumn; ++column) {
		//		if (field[row][column].dug) {
		//			foundPrev = true;
		//			++result;
		//			if (countFieldSinceLast) {
		//				if (addFieldsSinceLast) {
		//					result += fieldsSinceLast;
		//				}
		//				addFieldsSinceLast = !addFieldsSinceLast;
		//				countFieldSinceLast = false;
		//				fieldsSinceLast = 0;
		//			}
		//		}
		//		else {
		//			if (foundPrev) { 
		//				countFieldSinceLast = true;
		//			}
		//			foundPrev = false;
		//		}
		//		if (countFieldSinceLast) ++fieldsSinceLast;
		//	}
		//}





		//bool count = false;
		//bool goingHorizontal = false;
		//for (int row = minRow; row <= maxRow; ++row) {
		//	count = false;
		//	for (int column = minColumn; column <= maxColumn; ++column) {
		//		if (field[row][column].dug) {
		//			if (field[row][column].outDirection == 3 || field[row][column].outDirection == 4) {
		//				count = !count;
		//				goingHorizontal = false;
		//			}
		//			else { 
		//				goingHorizontal = true;
		//				count = true; 
		//			}
		//			++result;
		//		}else if (count) {
		//			if (goingHorizontal) {
		//				goingHorizontal = false;
		//				count = false;
		//			}else ++result;
		//		}
		//	}
		//}
		//for (int row = minRow; row <= maxRow; ++row) {
		//	for (int column = maxColumn; column >= minColumn; --column) {
		//		if (!field[row][column]) {
		//			++result;
		//		}
		//		else {
		//			break;
		//		}
		//	}
		//}
		//for (int column = minColumn + 1; column <= maxColumn - 2; ++column) {
		//	for (int row = minRow; row < maxRow + 1; ++row) {
		//		if (!field[row][column]) {
		//			++result;
		//		}
		//		else {
		//			break;
		//		}
		//	}
		//}
		//for (int column = minColumn + 1; column <= maxColumn - 2; ++column) {
		//	for (int row = maxRow + 1 - 1; row >= minRow; --row) {
		//		if (!field[row][column]) {
		//			++result;
		//		}
		//		else {
		//			break;
		//		}
		//	}
		//}
		//return (maxRow+1-minRow) * (maxColumn+1-minColumn)- result;
		return result;
	}
	
	struct Vertex {
		long long x;
		long long y;
	};
	
	long long calculatePolygon(std::vector<Vertex> vertices) {
		long long result = 0;
		for (int i = 0; i < vertices.size();++i) {
			int j = (i + 1) % vertices.size();
			result = result + vertices[i].x * vertices[j].y;
			result = result - vertices[i].y * vertices[j].x;
		}

		return result;
	}
	
	long long calc2(const char* filename) {
		std::ifstream infile(filename);
		std::string line;
		std::vector<Vertex> vertices;
		long long curX = 0;
		long long curY = 0;
		long long fullLength = 0;
		while (std::getline(infile, line)) {
			vertices.emplace_back(curX, curY);
			std::string hex = line.substr(line.find('#') + 1, line.find(')') - line.find('#') - 2);
			int direction = std::stoi(line.substr(line.find(')') - 1, 1));
			unsigned int length;
			std::stringstream ss;
			ss << std::hex << hex;
			ss >> length;
			switch (direction) {
			case 0:
				curX += length;
				break;
			case 1:
				curY += length;
				break;
			case 2:
				curX -= length;
				if (curX < 0) { 
					std::for_each(vertices.begin(), vertices.end(), [=](Vertex& vertex) {vertex.x -= curX; });
					curX = 0;
				}
				break;
			case 3:
				curY -= length;
				if (curY < 0) {
					std::for_each(vertices.begin(), vertices.end(), [=](Vertex& vertex) {vertex.y -= curY; });
					curY = 0;
				}
				break;
			}
			fullLength += length;
		}
		auto area = calculatePolygon(vertices) / 2;
		return area + 0.5 * fullLength + 1;
	}
	long long calc3(const char* filename) {
		std::ifstream infile(filename);
		std::string line;
		std::vector<Vertex> vertices;
		long long curX = 0;
		long long curY = 0;
		long long fullLength = 0;
		while (std::getline(infile, line)) {
			vertices.emplace_back(curX, curY);
			auto tempDirection = line.substr(0, 1);
			auto length = std::stoi(line.substr(2, line.find('('))) ;
			unsigned int direction;
			if (tempDirection == "R") direction = 0;
			else if (tempDirection == "D") direction = 1;
			else if (tempDirection == "L") direction = 2;
			else direction = 3;
			switch (direction) {
			case 0:
				curX += length;
				break;
			case 1:
				curY += length;
				break;
			case 2:
				curX -= length;
				if (curX < 0) {
					std::for_each(vertices.begin(), vertices.end(), [=](Vertex& vertex) {vertex.x -= curX; });
					curX = 0;
				}
				break;
			case 3:
				curY -= length;
				if (curY < 0) {
					std::for_each(vertices.begin(), vertices.end(), [=](Vertex& vertex) {vertex.y -= curY; });
					curY = 0;
				}
				break;
			}
			fullLength += length;
		}
		auto area = calculatePolygon(vertices) / 2;
		return area - 0.5 * fullLength + 1 + fullLength;
	}
	long long calc4(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
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