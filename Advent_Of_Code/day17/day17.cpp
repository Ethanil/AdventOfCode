#include <fstream>
#include <iostream>
#include <deque>
#include <functional>
#include <map>
#include <set>
#include <queue>
#include <unordered_set>
#include < unordered_map >
namespace day17 {
	using Direction = uint8_t;
	struct Coordinates {
		int16_t x;
		int16_t y;
		friend bool operator<(const Coordinates& l, const Coordinates& r)
		{
			return std::tie(l.x, l.y)
				< std::tie(r.x, r.y); // keep the same order
		}
		friend bool operator==(const Coordinates& l, const Coordinates& r)
		{
			return l.x == r.x && l.y == r.y;
		}
	};
	struct GridLocation {
		Coordinates coordinates;
		Direction direction;
		uint8_t runlength;
		friend bool operator<(const GridLocation& l, const GridLocation& r)
		{
			return std::tie(l.coordinates, l.runlength, l.direction)
				< std::tie(r.coordinates, r.runlength, r.direction); // keep the same order
		}
		friend bool operator==(const GridLocation& l, const GridLocation& r)
		{
			return l.coordinates == r.coordinates && l.direction == r.direction && l.runlength == r.runlength;
		}
	};
	struct SquareGrid {
		const GridLocation directions[4] = {
			{1,0},
			{0,1},
			{-1,0},
			{0,-1}
		};

		int width;
		int height;

		SquareGrid(int width, int height) : width(width), height(height) {};

		virtual bool in_bounds(GridLocation location) {
			return 0 <= location.coordinates.x && location.coordinates.x < width && 0 <= location.coordinates.y && location.coordinates.y < height && location.runlength <= 3;
		}

		virtual std::vector<GridLocation> neighbors(GridLocation location) {
			std::vector<GridLocation> res;
			for (Direction direction = 0; direction < 4; ++direction) {
				const auto& dir = directions[direction];
				GridLocation neighbor{ location.coordinates.x + dir.coordinates.x, location.coordinates.y + dir.coordinates.y, direction, location.direction == direction ? location.runlength + 1 : 1 };
				if (in_bounds(neighbor) && (location.direction + 2) % 4 != direction) res.emplace_back(neighbor);
			}
			return res;
		}
		virtual bool checkCosts(GridLocation nextLocation, int64_t new_cost, std::map<GridLocation, int64_t>& cost_so_far) {
			int64_t maxNewCosts = 0;
			bool finished = false;
			for (Direction dir = 0; dir < 4; ++dir) {
				for (int runlength = 1; runlength < 4; ++runlength) {
					GridLocation searchLocation = { nextLocation.coordinates, dir, runlength };
					auto search = cost_so_far.find(nextLocation);
					if (search != cost_so_far.end()) {
						maxNewCosts = std::max(maxNewCosts, search->second);
						finished = new_cost < maxNewCosts;
						if (finished) break;
					}
				}
				if (finished) break;
			}
			return finished || maxNewCosts == 0;
		}
	};


	struct GridWithWeights : SquareGrid {
		GridWithWeights(int width, int height) : SquareGrid(width, height) {};
		std::map<Coordinates, int> locToCostMap;
		inline void addCost(GridLocation loc, int cost) {
			locToCostMap.emplace(std::make_pair(loc.coordinates, cost));
		}
		int cost(GridLocation to) const {
			return locToCostMap.find(to.coordinates)->second;
		}
	};

	struct UltraSquareGrid : GridWithWeights {
		UltraSquareGrid(int width, int height) : GridWithWeights(width, height) {};
		bool in_bounds(GridLocation location) override {
			return 0 <= location.coordinates.x && location.coordinates.x < width && 0 <= location.coordinates.y && location.coordinates.y < height && location.runlength <= 10;
		}
		std::vector<GridLocation> neighbors(GridLocation location) override {
			std::vector<GridLocation> res;
			for (Direction direction = 0; direction < 4; ++direction) {
				const auto& dir = directions[direction];
				int neighborRunLength;
				if (location.direction == direction) {
					neighborRunLength = location.runlength + 1;
				}
				else if (location.runlength >= 4) neighborRunLength = 1;
				else continue;
				GridLocation neighbor{ location.coordinates.x + dir.coordinates.x, location.coordinates.y + dir.coordinates.y, direction, neighborRunLength };
				if (in_bounds(neighbor) && (location.direction + 2) % 4 != direction) res.emplace_back(neighbor);
			}
			return res;
		}
		bool checkCosts(GridLocation nextLocation, int64_t new_cost, std::map<GridLocation, int64_t>& cost_so_far) override {
			if (new_cost > 810) return false;
			int64_t maxNewCosts = 0;
			bool finished = false;
			for (Direction dir = 0; dir < 4; ++dir) {
				for (int runlength = 1; runlength < 11; ++runlength) {
					GridLocation searchLocation = { nextLocation.coordinates, dir, runlength };
					auto search = cost_so_far.find(nextLocation);
					if (search != cost_so_far.end()) {
						maxNewCosts = std::max(maxNewCosts, search->second);
						finished = new_cost < maxNewCosts;
						if (finished) break;
					}
				}
				if (finished) break;
			}
			return finished || maxNewCosts == 0;
		}
	};

	template<typename T, typename priority_t>
	struct ownPriorityQueue {
		typedef std::pair<priority_t, T> QueueElement;
		std::priority_queue<QueueElement, std::vector<QueueElement>, std::greater<QueueElement>> elements;

		inline bool empty() const {
			return elements.empty();
		}

		inline void put(T item, priority_t priority) {
			elements.emplace(priority, item);
		}

		T get() {
			T best_item = elements.top().second;
			elements.pop();
			return best_item;
		}

		size_t size() {
			return elements.size();
		}

	};

	inline int heuristic(GridLocation loc1, GridLocation loc2) {
		return std::abs(loc1.coordinates.x - loc2.coordinates.x) + std::abs(loc1.coordinates.y - loc2.coordinates.y);
	}


	
	template<typename Location, typename Graph>
	void a_star_search(
		Graph graph,
		Location start,
		Location goal,
		std::map<Location, Location>& came_from,
		std::map<Location, int64_t>& cost_so_far) {

		ownPriorityQueue<Location, int> frontier;
		frontier.put(start, heuristic(start, goal));

		came_from[start] = start;
		cost_so_far[start] = 0;
		start.direction = 1 - start.direction;
		frontier.put(start, heuristic(start, goal));

		came_from[start] = start;
		cost_so_far[start] = 0;
		while (!frontier.empty()) {
			Location current = frontier.get();

			if (current == goal && current.runlength >= 4) {
				break;
			}

			for (const auto& nextLocation :  graph.neighbors(current)) {
				int64_t new_cost = cost_so_far[current] + graph.cost(nextLocation);
				if (graph.checkCosts(nextLocation, new_cost, cost_so_far)) {
					cost_so_far[nextLocation] = new_cost;
					int priority = new_cost + heuristic(nextLocation, goal);
					frontier.put(nextLocation, priority);
					came_from[nextLocation] = current;
				}

			}

		}
	}


	const unsigned int NUMBER_OF_LINES = 141; //
	const unsigned int LINEWIDTH = 141; //
	long long calc1(const char* filename) {
		GridWithWeights grid{LINEWIDTH, NUMBER_OF_LINES};
		GridLocation start{ 0, 0 }, goal{ LINEWIDTH - 1, NUMBER_OF_LINES - 1};
		std::map<GridLocation, GridLocation> came_from;
		std::map<GridLocation, int64_t> cost_so_far;
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		for (int16_t row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int16_t column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				GridLocation loc({ column, row });
				grid.addCost(loc, c - '0');
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);
		a_star_search(grid, start, goal, came_from, cost_so_far);
		GridLocation curLocation(goal.coordinates);
		int costs = INT_MAX;
		GridLocation goalLocation;
		for (Direction dir = 0; dir < 4; ++dir) {
			for (int runlength = 1; runlength < 4; ++runlength) {
				curLocation.direction = dir;
				curLocation.runlength = runlength;
				auto search = came_from.find(curLocation);
				if (search != came_from.end() && cost_so_far[curLocation] < costs) { 
					goalLocation = curLocation;
					costs = cost_so_far[curLocation];
				}
			}
		}
		return costs;
	}


	long long calc2(const char* filename) {
		UltraSquareGrid grid{ LINEWIDTH, NUMBER_OF_LINES };
		GridLocation start{ 0, 0 }, goal{ LINEWIDTH - 1, NUMBER_OF_LINES - 1 };
		std::map<GridLocation, GridLocation> came_from;
		std::map<GridLocation, int64_t> cost_so_far;
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		for (int16_t row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int16_t column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				GridLocation loc({ column, row });
				grid.addCost(loc, c - '0');
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);
		a_star_search(grid, start, goal, came_from, cost_so_far);
		GridLocation curLocation(goal.coordinates);
		int costs = INT_MAX;
		GridLocation goalLocation;
		for (Direction dir = 0; dir < 4; ++dir) {
			for (int runlength = 4; runlength < 11; ++runlength) {
				curLocation.direction = dir;
				curLocation.runlength = runlength;
				auto search = came_from.find(curLocation);
				if (search != came_from.end()) {
					if (cost_so_far[curLocation] < costs) {
						goalLocation = curLocation;
						costs = cost_so_far[curLocation];
					}
				}
			}
		}
		return costs;
	}
	long long calc3(const char* filename) {
		FILE* stream;
		int c;
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
	long long calc4(const char* filename) {
		FILE* stream;
		int c;
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