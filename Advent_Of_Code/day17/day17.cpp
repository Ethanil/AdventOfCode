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
	struct GridLocation {
		int x;
		int y;    

		friend bool operator<(const GridLocation& l, const GridLocation& r)
		{
			return std::tie(l.x, l.y)
				< std::tie(r.x, r.y); // keep the same order
		}
		friend bool operator==(const GridLocation& l, const GridLocation& r)
		{
			return l.x == r.x && l.y == r.y;
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

		bool in_bounds(GridLocation location) {
			return 0 <= location.x && location.x < width && 0 <= location.y && location.y < height;
		}

		std::vector<GridLocation> neighbors(GridLocation location) {
			std::vector<GridLocation> res;
			for (const auto& dir : directions) {
				GridLocation neighbor{ location.x + dir.x, location.y + dir.y };
				if (in_bounds(neighbor)) res.push_back(neighbor);
			}
			return res;
		}


	};

	struct GridWithWeights : SquareGrid {
		GridWithWeights(int width, int height) : SquareGrid(width, height) {};
		std::map<GridLocation, int> locToCostMap;
		inline void addCost(GridLocation loc, int cost) {
			locToCostMap.emplace(loc, cost);
		}
		int cost(GridLocation to) const {
			return locToCostMap.find(to)->second;
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

	};

	inline int heuristic(GridLocation loc1, GridLocation loc2) {
		return std::abs(loc1.x - loc2.x) + std::abs(loc1.y - loc2.y);
	}

	typedef int Direction;

	template<typename Location, typename Graph>
	void a_star_search(
		Graph graph,
		Location start,
		Location goal,
		std::map<Location, Location>& came_from,
		std::map<Location, int>& cost_so_far,
		std::map<Location, std::pair<Direction, int>>& moves_in_same_direction) {
		ownPriorityQueue<Location, int> frontier;
		frontier.put(start, heuristic(start, goal));

		came_from[start] = start;
		cost_so_far[start] = 0;

		while (!frontier.empty()) {
			Location current = frontier.get();

			if (current == goal) {
				break;
			}

			for (int i = 0; i < graph.neighbors(current).size(); ++i) {
				Location next = graph.neighbors(current)[i];
				auto cur_moves_node = moves_in_same_direction.extract(current);
				auto next_moves_node = moves_in_same_direction.extract(next);
				auto cur_moves_pair = cur_moves_node.mapped();
				int moves_into_current = 0;
				if (cur_moves_pair.first == i) moves_into_current = cur_moves_pair.second;
				moves_in_same_direction.insert(std::move(cur_moves_node));
				if (moves_into_current == 3) { 
					moves_in_same_direction.insert(std::move(next_moves_node));
					continue; 
				}
				int new_cost = cost_so_far[current] + graph.cost(next);
				if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
					cost_so_far[next] = new_cost;
					int priority = new_cost + heuristic(next, goal);
					frontier.put(next, priority);
					came_from[next] = current;
					next_moves_node.mapped() = { i, moves_into_current + 1 };
				}
				moves_in_same_direction.insert(std::move(next_moves_node));

			}

		}
	}


	const unsigned int NUMBER_OF_LINES = 13; //
	const unsigned int LINEWIDTH = 13; //
	long long calc1(const char* filename) {
		GridWithWeights grid{LINEWIDTH, NUMBER_OF_LINES};
		GridLocation start{ 0, 0 }, goal{ LINEWIDTH - 1, NUMBER_OF_LINES - 1};
		std::map<GridLocation, GridLocation> came_from;
		std::map<GridLocation, int> cost_so_far;
		std::map<GridLocation, std::pair<Direction, int>> moves_in_same_direction;
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		for (int row = 0; row < NUMBER_OF_LINES; ++row) {
			for (int column = 0; column < LINEWIDTH; ++column) {
				c = _getc_nolock(stream);
				GridLocation loc(column, row);
				grid.addCost(loc, c - '0');
				moves_in_same_direction.emplace(loc, std::make_pair(0,0));
			}
			_getc_nolock(stream);
		}
		_fclose_nolock(stream);
		a_star_search(grid, start, goal, came_from, cost_so_far, moves_in_same_direction);
		GridLocation curLocation(goal.x, goal.y);
		while (!(curLocation == start)) {
			std::cout << "(" << curLocation.x << ", " << curLocation.y << "): "<<cost_so_far[curLocation]<<"\n";
			curLocation = came_from.find(curLocation)->second;
		}
		return cost_so_far[goal];
	}


	long long calc2(const char* filename) {
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