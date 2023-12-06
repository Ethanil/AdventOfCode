#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <limits>
namespace day6 {

	struct Race {
	public:
		long long time;
		long long distance;
	};
	struct TwoHoldTimes {
		long HighBorder;
		long lowBorder;
	};
	long long calculateDistance(long long raceTime, long long buttonHeldTime) {
		return (raceTime - buttonHeldTime) * buttonHeldTime;
	}
	TwoHoldTimes calculateHoldTimeFromDistance(long long racetime, long long distance) {
		double squareRoot = sqrtl(0.25 * racetime * racetime - distance);
		return { long(squareRoot + 0.5 * racetime), long( - squareRoot + 0.5 * racetime)};
	}
	long long optimalHoldTime(long long raceTime) {
		return raceTime / 2 + 0.5;
	}

	long long calc1(const char* filename) {
		std::ifstream in(filename);
		int c;
		Race races[] = {
			Race{},
			Race{},
			Race{},
			Race{} };
		int index = 0;
		bool readingTime = true;
		while (in.get() != ':');
		while ((c = in.get()) != -1) {
			switch (c) {
			case ' ':
				break;
			default:
				int res = c - '0';
				while ((c = in.get()) != ' ' && c != '\n' && c != -1) {
					res = res * 10 + (c - '0');
				}
				if (readingTime) {
					races[index++].time = res;
				}
				else {
					races[index++].distance = res;
				}
				if (c == '\n') {
					while (in.get() != ':');
					readingTime = false;
					index = 0;
				}
			}
		}
		in.close();
		int result = 1;
		for (const auto& race : races) {
			TwoHoldTimes hTime = calculateHoldTimeFromDistance(race.time, race.distance);
			result *= hTime.HighBorder - hTime.lowBorder;
		}
		return result;
	}	
	long long calc2(const char* filename) {
		std::ifstream in(filename);
		int c;
		Race race{};
		bool readingTime = true;
		while (in.get() != ':');
		long long res = -1;
		while ((c = in.get()) != -1) {
			switch (c) {
			case ' ':
				break;
			default:
				if (res == -1) {
					res = c - '0';
				}
				else {
					res = res * 10 + (c - '0');
				}
				while ((c = in.get()) != ' ' && c != '\n' && c != -1) {
					res = res * 10 + (c - '0');
				}
				if (c == '\n' || c == -1) {
					if (c == '\n') {
						while (in.get() != ':');
					}
					if (readingTime) {
						race.time = res;
					}
					else {
						race.distance = res;
					}
					res = -1;
					readingTime = false;
				}
			}
		}
		in.close();
		TwoHoldTimes hTime = calculateHoldTimeFromDistance(race.time, race.distance);
		return hTime.HighBorder - hTime.lowBorder;
	}
}