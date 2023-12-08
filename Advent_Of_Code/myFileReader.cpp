#include "myFileReader.hpp"
void readNumber(FILE* stream, long long& number, const char delimiter) {
	int c;
	while ((c = _fgetc_nolock(stream)) != -1 && c != '\n' && c != delimiter) {
		number = number * 10 + (c - '0');
	}
}

void readString(FILE* stream, char* string, const char delimiter) {
	int c;
	int index = 0;
	while ((c = _fgetc_nolock(stream)) != -1 && c != '\n' && c != delimiter) {
		string[index++] = c;
	}
}