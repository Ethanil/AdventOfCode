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
#include "day9/day9.hpp"
#include "day10/day10.hpp"
#include "day11/day11.hpp"
#include "day12/day12.hpp"
#include "day13/day13.hpp"
#include "day14/day14.hpp"
#include "day15/day15.hpp"
#include "day16/day16.hpp"
#include "day17/day17.hpp"
#include "day18/day18.hpp"
#include "day19/day19.hpp"
#include "day20/day20.hpp"

using namespace day17;
double getTime(long long (*func)(const char*), const char* filename, int times = 1) {
	double sum = 0;
	for (int i = 0; i < times; ++i) {
		auto start = std::chrono::high_resolution_clock::now();
		func(filename);
		auto stop = std::chrono::high_resolution_clock::now();
		sum += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	}
	return sum / times;
}
void timeCode(int day, const char* filename) {
	std::cout << "calc1: " << getTime(calc1, filename) << " microseconds\n";
	std::cout << "calc2: " << getTime(calc2, filename) << " microseconds\n";
	std::cout << "calc3: " << getTime(calc3, filename) << " microseconds\n";
	std::cout << "calc4: " << getTime(calc4, filename) << " microseconds\n";
}


int main() {
	int day = 17;
	auto filename = std::format("day{}/day{}.txt", day, day);
	//std::cout << calc2(filename.c_str());
	timeCode(day, filename.c_str());
}
