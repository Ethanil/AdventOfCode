#include <iostream>
#include <chrono>
#include "day1/day1.hpp"
#include "day2/day2.hpp"
#include "day3/day3.hpp"
#include "day4/day4.hpp"
#include "day5/day5.hpp"
double getTime(long long (*func)(const char*), const char* filename, int times = 10000) {
	double sum = 0;
	for (int i = 0; i < times; ++i) {
		auto start = std::chrono::high_resolution_clock::now();
		func(filename);
		auto stop = std::chrono::high_resolution_clock::now();
		sum += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	}
	return sum / times;
}
int main() {
	std::cout << getTime(day5::calc2, "day5/day5.txt") <<" microseconds";
	//std::cout << day5::calc2("day5.txt");
}