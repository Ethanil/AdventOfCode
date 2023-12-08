#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <limits>
namespace day5 {
	struct ValueRange {
	public:
		long long startNumber;
		long long range;
	};

	struct MapEntry {
	private:
		long long sourceStart;
		long long destinationStart;
		long long range;
		bool isInRange(const long long& number) const {
			return number >= sourceStart && number < sourceStart + range;
		}
		long long convert(const long long& number) const {
			return number - sourceStart + destinationStart;
		}
		MapEntry(long long sourceStart, long long destinationStart, long long range) : sourceStart(sourceStart), destinationStart(destinationStart), range(range) {};
		friend struct RangeMap;
	};

	struct RangeMap {
	private:
		std::vector<MapEntry> map;
	public:
		void addRange(long long sourceStart, long long destinationStart, long long range) {
			map.emplace_back(MapEntry{ sourceStart, destinationStart, range });
		}
		long long convert(const long long& number) const {
			for (const auto& entry : map) {
				if (entry.isInRange(number)) return entry.convert(number);
			}
			return number;
		}
		std::vector<ValueRange> convertRange(const ValueRange& valueRange) const{
			std::vector<ValueRange> result;
			for (long long value = valueRange.startNumber; value < valueRange.startNumber + valueRange.range;) {
				bool foundARange = false;
				const MapEntry * nextEntry = NULL;
				for (const auto& entry : map) {
					if (entry.isInRange(value)) {
						long long addedRange = std::min(entry.range-(value - entry.sourceStart), valueRange.startNumber + valueRange.range - value);
						result.emplace_back(ValueRange{entry.convert(value),addedRange});
						value += addedRange;
						foundARange = true;
						break;
					}
					if (entry.sourceStart > value && (!nextEntry || nextEntry->sourceStart > entry.sourceStart)) {
						nextEntry = &entry;
					}
				}
				if(!foundARange){
					long long addedRange;
					addedRange =nextEntry ? std::min(nextEntry->range, valueRange.startNumber + valueRange.range - value) : valueRange.startNumber + valueRange.range - value;
					result.emplace_back(ValueRange{ value,addedRange });
					value += addedRange;
				}
				
			}
			return result;
		}
	};

	std::ostream& operator<<(std::ostream& os, const ValueRange& range) {
		os << "start: " << range.startNumber << " range: " << range.range;
		return os;
	}

	static long long readNumber(std::ifstream& stream) {
		long long res = 0;
		int c;
		while ((c = stream.get()) != ' ' && c != '\n' && c != -1) {
			res = res * 10 + (c - '0');
		}
		return res;
	}

	static void fillMap(RangeMap& mapToFill, std::ifstream& stream) {
		while (stream.get() != ':');
		while (stream.get() != '\n');
		while (true) {
			long long destinationStart = readNumber(stream);
			long long sourceStart = readNumber(stream);
			long long range = readNumber(stream);
			mapToFill.addRange(sourceStart, destinationStart, range);
			if (stream.peek() == '\n' || stream.peek() == -1) break;
		}
	}

	long long calc1(const char* fileName) {
		std::vector<long long> seeds;
		RangeMap maps[] =
		{ 
			RangeMap(),
			RangeMap(),
			RangeMap(),
			RangeMap(),
			RangeMap(),
			RangeMap(),
			RangeMap()
		};

		std::ifstream in(fileName);
		while (in.get() != ' ');
		while (true) {
			seeds.emplace_back(readNumber(in));
			if (in.peek() == '\n') break;
		}
		for (int i = 0; i < 7; ++i) {
			fillMap(maps[i], in);
		}
		long long minimum = std::numeric_limits<long long>::max();
		long long nextVal;
		for (const auto& seed : seeds) {
			nextVal = seed;
			for (int i = 0; i < 7; ++i) {
				nextVal = maps[i].convert(nextVal);
			}
			minimum = std::min(minimum, nextVal);
		}
		return minimum;
	}

	long long calc2(const char* fileName) {
		ValueRange seeds[10] = { 0 };
		RangeMap maps[] =
		{
			RangeMap(),
			RangeMap(),
			RangeMap(),
			RangeMap(),
			RangeMap(),
			RangeMap(),
			RangeMap()
		};

		std::ifstream in(fileName);
		while (in.get() != ' ');
		for(int i = 0;i < 10; ++i) {
			long long seedStart = readNumber(in);
			long long seedRange = readNumber(in);
			seeds[i] = ValueRange{ seedStart, seedRange };
		}

		for (int i = 0; i < 7; ++i) {
			fillMap(maps[i], in);
		}

		long long minimum = std::numeric_limits<long long>::max();
		for (const auto& seed : seeds) {
			std::vector<ValueRange> valueRanges = { seed };
			for (int i = 0; i < 7; ++i) {
				std::vector<ValueRange> nextValueRanges;
				for (const auto& valueRange : valueRanges) {
					auto ranges = maps[i].convertRange(valueRange);
					for (const auto& range : ranges) {
						nextValueRanges.emplace_back(range);
					}
				}
				valueRanges = std::move(nextValueRanges);
			}
			for (const auto& valueRange : valueRanges) {
				minimum = std::min(minimum, valueRange.startNumber);
			}
		}
		return minimum;
	}
	long long calc3(const char* fileName) { return -1; }
	long long calc4(const char* fileName) { return -1; }
}