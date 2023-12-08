#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>
#include "day8.hpp"
#include "../myFileReader.hpp"
namespace day8 {
	const int NUMBEROFLINES = 802;
	const int NUMBEROFDIRECTIONS = 263;
	struct entry {
		int left;
		int right;
	};
	struct entry2 {
		uint16_t left;
		uint16_t right;
	};
	long long calc1(const char* filename){
		FILE* stream;
		fopen_s(&stream,filename, "r");
		int c;
		bool directions[NUMBEROFDIRECTIONS];
		entry entries[17576];
		while ((c = _fgetc_nolock(stream)) != -1) {
			int directionIndex = 0;
			directions[directionIndex++] = c - 'L';
			while ((c = _fgetc_nolock(stream)) != '\n') {
				directions[directionIndex++] = c - 'L';
			}
			 _fgetc_nolock(stream);
			 for (int line = 0; line < NUMBEROFLINES; ++line) {
				 int index = 0;
				 for(int position = 0; position < 3 ;++position){
					 index = index * 26 + _fgetc_nolock(stream) - 'A';
				 }
				 for (int position = 0; position < 4; ++position) _fgetc_nolock(stream);
				 int left = 0;
				 for (int position = 0; position < 3; ++position) {
					 left = left * 26 + _fgetc_nolock(stream) - 'A';
				 }
				 for (int position = 0;position < 2; ++position) _fgetc_nolock(stream);
				 int right = 0;
				 for (int position = 0; position < 3; ++position) {
					 right = right * 26 + _fgetc_nolock(stream) - 'A';
				 }
				 for (int position = 0; position < 2; ++position) _fgetc_nolock(stream);
				 entries[index] = entry{ left,right };
				 //std::cout << index << ": " << left << ", " << right << "\n";
			 }
		}
		fclose(stream);
		int result = 0;
		int curPos = 0;
		while (curPos != 17575) {
			//std::cout << pos << "\n";
			if (directions[result % NUMBEROFDIRECTIONS]) {
				curPos = entries[curPos].right;
			}
			else {
				curPos = entries[curPos].left;
			}
			++result;
		}
		return result;
	}
	long long calc2(const char* filename) { 
		FILE* stream;
		fopen_s(&stream, filename, "r");
		int c;
		bool directions[NUMBEROFDIRECTIONS];
		entry entries[17576];
		std::vector<int> startpoints;
		while ((c = _fgetc_nolock(stream)) != -1) {
			int directionIndex = 0;
			directions[directionIndex++] = c - 'L';
			while ((c = _fgetc_nolock(stream)) != '\n') {
				directions[directionIndex++] = c - 'L';
			}
			_fgetc_nolock(stream);
			for (int line = 0; line < NUMBEROFLINES; ++line) {
				int index = 0;
				for (int position = 0; position < 3; ++position) {
					index = index * 26 + (c=_fgetc_nolock(stream)) - 'A';
				}
				if (c == 'A') {
					startpoints.emplace_back(index);
				}
				for (int position = 0; position < 4; ++position) _fgetc_nolock(stream);
				int left = 0;
				for (int position = 0; position < 3; ++position) {
					left = left * 26 + _fgetc_nolock(stream) - 'A';
				}
				for (int position = 0; position < 2; ++position) _fgetc_nolock(stream);
				int right = 0;
				for (int position = 0; position < 3; ++position) {
					right = right * 26 + _fgetc_nolock(stream) - 'A';
				}
				for (int position = 0; position < 2; ++position) _fgetc_nolock(stream);
				entries[index] = entry{ left,right };
				//std::cout << index << ": " << left << ", " << right << "\n";
			}
		}
		fclose(stream);
		long long result = -1;
		for (auto& startpoint : startpoints) {
			int steps = 0;
			int curPos = startpoint;
			while (curPos % 26 != 25) {
				if (directions[steps % NUMBEROFDIRECTIONS]) {
					curPos = entries[curPos].right;
				}
				else {
					curPos = entries[curPos].left;
				}
				++steps;
			}
			startpoint = steps;
			if (result == -1) result = startpoint;
			else result = std::lcm(startpoint, result);
		}
		return result;
	}
	long long calc3(const char* filename){
		FILE* stream;
		fopen_s(&stream, filename, "r");
		int c;
		bool directions[NUMBEROFDIRECTIONS];
		entry2 entries[17576];
		std::vector<int> startpoints;
		while ((c = _fgetc_nolock(stream)) != -1) {
			int directionIndex = 0;
			directions[directionIndex++] = c - 'L';
			while ((c = _fgetc_nolock(stream)) != '\n') {
				directions[directionIndex++] = c - 'L';
			}
			_fgetc_nolock(stream);
			for (int line = 0; line < NUMBEROFLINES; ++line) {
				uint16_t index = 0;
				for (int position = 0; position < 3; ++position) {
					index = index * 26 + (c = _fgetc_nolock(stream)) - 'A';
				}
				if (c == 'A') {
					startpoints.emplace_back(index);
				}
				for (int position = 0; position < 4; ++position) _fgetc_nolock(stream);
				uint16_t left = 0;
				for (int position = 0; position < 3; ++position) {
					left = left * 26 + _fgetc_nolock(stream) - 'A';
				}
				for (int position = 0; position < 2; ++position) _fgetc_nolock(stream);
				uint16_t right = 0;
				for (int position = 0; position < 3; ++position) {
					right = right * 26 + _fgetc_nolock(stream) - 'A';
				}
				for (int position = 0; position < 2; ++position) _fgetc_nolock(stream);
				entries[index] = entry2{ left,right };
				//std::cout << index << ": " << left << ", " << right << "\n";
			}
		}
		fclose(stream);
		long long result = -1;
		for (auto& startpoint : startpoints) {
			int steps = 0;
			uint16_t curPos = startpoint;
			while (curPos % 26 != 25) {
				if (directions[steps % NUMBEROFDIRECTIONS]) {
					curPos = entries[curPos].right;
				}
				else {
					curPos = entries[curPos].left;
				}
				++steps;
			}
			startpoint = steps;
			if (result == -1) result = startpoint;
			else result = std::lcm(startpoint, result);
		}
		return result;
	}
	long long calc4(const char* filenamek){ return -1; }
}