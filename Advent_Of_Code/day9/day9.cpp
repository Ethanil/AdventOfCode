#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>
#include "day9.hpp"
#include "../myFileReader.hpp"
namespace day9 {
	const int LENGTHOFLINE = 21; //6 21
	const int NUMBEROFLINES = 200; //3 200
	long long calc1(const char* filename){
		FILE* stream;
		int c;
		long line[LENGTHOFLINE - 1];
		long lastNumbers[LENGTHOFLINE - 1] = { 0 };
		fopen_s(&stream,filename, "r");
		long long result = 0;
		while ((c = _fgetc_nolock(stream)) != -1) {
			for (int curLine = 0; curLine < NUMBEROFLINES; ++curLine) {
				int lastNumberIndex = 0;
				bool finished = false;
				long lastNumber;
				bool NumberIsNegative = false;
				if (c != '-') lastNumber = c - '0';
				else 
				{
					lastNumber = _fgetc_nolock(stream) - '0';
					NumberIsNegative = true;
				}
				while ((c = _fgetc_nolock(stream)) != ' ' && c != '\n')lastNumber = lastNumber * 10 + c - '0';
				if (NumberIsNegative) lastNumber *= -1;
				while (!finished) {
					finished = true;
					for (int curNumberIndex = 0; curNumberIndex < LENGTHOFLINE - 1 - lastNumberIndex; ++curNumberIndex) {
						long curNumber = 0;
						NumberIsNegative = false;
						if (lastNumberIndex == 0) while ((c = _fgetc_nolock(stream)) != ' ' && c != '\n' && c != -1)
						{
							if (c == '-')
							{
								c = _fgetc_nolock(stream);
								curNumber = curNumber * 10 + c - '0';
								NumberIsNegative = true;
							}
							else curNumber = curNumber * 10 + c - '0';
						}
						else curNumber = line[curNumberIndex + 1];
						if (NumberIsNegative) curNumber *= -1;
						line[curNumberIndex] = curNumber - lastNumber;
						lastNumber = curNumber;
						if (finished && curNumberIndex != 0) finished = (line[curNumberIndex] - line[curNumberIndex - 1] == 0);
					}
					lastNumbers[lastNumberIndex++] = lastNumber;
					lastNumbers[lastNumberIndex] = line[LENGTHOFLINE - 1 - lastNumberIndex];
					lastNumber = line[0];
				}
				for (int i = LENGTHOFLINE - 2; i > 0; --i) {
					lastNumbers[i - 1] = lastNumbers[i - 1] + lastNumbers[i];
					lastNumbers[i] = 0;
				}
				result += lastNumbers[0];
				lastNumbers[0] = 0;
				if (c != -1) c = _fgetc_nolock(stream);
			}
			 
		}
		fclose(stream);
		
		return result;
	}

	long long calc2(const char* filename) {
		FILE* stream;
		int c;
		long line[LENGTHOFLINE - 1];
		long firstNumbers[LENGTHOFLINE - 1] = { 0 };
		fopen_s(&stream, filename, "r");
		long long result = 0;
		while ((c = _fgetc_nolock(stream)) != -1) {
			for (int curLine = 0; curLine < NUMBEROFLINES; ++curLine) {
				int lastNumberIndex = 0;
				bool finished = false;
				long lastNumber;
				bool NumberIsNegative = false;
				long firstNumber;
				if (c != '-') lastNumber = c - '0';
				else
				{
					lastNumber = _fgetc_nolock(stream) - '0';
					NumberIsNegative = true;
				}
				while ((c = _fgetc_nolock(stream)) != ' ' && c != '\n')lastNumber = lastNumber * 10 + c - '0';
				if (NumberIsNegative) lastNumber *= -1;
				while (!finished) {
					firstNumbers[lastNumberIndex] = lastNumber;
					finished = true;
					for (int curNumberIndex = 0; curNumberIndex < LENGTHOFLINE - 1 - lastNumberIndex; ++curNumberIndex) {
						long curNumber = 0;
						NumberIsNegative = false;
						if (lastNumberIndex == 0) while ((c = _fgetc_nolock(stream)) != ' ' && c != '\n' && c != -1)
						{
							if (c == '-')
							{
								c = _fgetc_nolock(stream);
								curNumber = curNumber * 10 + c - '0';
								NumberIsNegative = true;
							}
							else curNumber = curNumber * 10 + c - '0';
						}
						else curNumber = line[curNumberIndex + 1];
						if (NumberIsNegative) curNumber *= -1;
						line[curNumberIndex] = curNumber - lastNumber;
						lastNumber = curNumber;
						if (finished && curNumberIndex != 0) finished = (line[curNumberIndex] - line[curNumberIndex - 1] == 0);
					}
					firstNumbers[++lastNumberIndex] = line[0];
					lastNumber = line[0];
				}
				for (int i = LENGTHOFLINE - 2; i > 0; --i) {
					firstNumbers[i - 1] =  firstNumbers[i - 1] - firstNumbers[i];
					firstNumbers[i] = 0;
				}
				result += firstNumbers[0];
				firstNumbers[0] = 0;
				if (c != -1) c = _fgetc_nolock(stream);
			}

		}
		fclose(stream);

		return result;
	}
	long long calc3(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		while ((c = _fgetc_nolock(stream)) != -1) {

		
		}
		fclose(stream);
		int result = 0;

		return result;
	}
	long long calc4(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		while ((c = _fgetc_nolock(stream)) != -1) {

		}
	
		fclose(stream);
		int result = 0;

		return result;
	}
}