#include <iostream>
#include <chrono>
#include <string>
#include <format>
#include "day1/day1.hpp"
#include "day2/day2.hpp"
#include "day3/day3.hpp"
#include "day4/day4.hpp"
#include "day5/day5.hpp"
#include "day6/day6.hpp"
#include "day7/day7.hpp"
#include "day8/day8.hpp"
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
void timeCode() {
	int day = 3;
	using namespace day3;
	auto filename = std::format("day{}/day{}.txt", day, day);
	std::cout << "calc1: " << getTime(calc1, filename.c_str()) << " microseconds\n";
	std::cout << "calc2: " << getTime(calc2, filename.c_str()) << " microseconds\n";
	std::cout << "calc3: " << getTime(calc3, filename.c_str()) << " microseconds\n";
	std::cout << "calc4: " << getTime(calc4, filename.c_str()) << " microseconds\n";
}

int main() {
	timeCode();
	//std::cout << day8::calc2("day8/day8.txt");
}
