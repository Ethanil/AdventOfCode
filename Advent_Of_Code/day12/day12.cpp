#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <set>
namespace day12 {
	/*possible values in string:
	* # : 35 => 35 % 3 = 2
	* . : 46 => 46 % 3 = 1
	* ? : 63 => 63 % 3 = 0
	*/
	bool checkSpringlineUntilPosition(std::vector<int>& line, std::vector<int>& springs, int position) {
		int curLinePos = 0;
		for (int curSpringPos = 0; curSpringPos < springs.size(); ++curSpringPos) {
			bool foundOne = curSpringPos == 0;
			while (curLinePos < line.size() && curLinePos <= position && line[curLinePos] == 1) {
				++curLinePos;
				foundOne = true;
			}
			if (curLinePos > position) return true;
			if (!foundOne || curLinePos >= line.size() || line[curLinePos] != 2) {
				return false;
			}
			for (int i = 0; i < springs[curSpringPos]; ++i) {
				if (curLinePos > position) return true;
				if (curLinePos >= line.size() || line[curLinePos] != 2) {
					return false;
				}
				++curLinePos;
			}
		}
		while (curLinePos < line.size()) {
			++curLinePos;
			if (curLinePos >= line.size() || curLinePos > position) return true;
			if (line[curLinePos] != 1) return false;
		}
		return true;
	}
	bool checkSpringline(std::vector<int>& line, std::vector<int>& springs) {
		auto lineIt = line.cbegin();
		for (auto it = springs.cbegin(); it != springs.cend(); ++it) {
			bool foundOne = it == springs.cbegin();
			while (lineIt != line.cend() && *lineIt == 1) {
				++lineIt;
				foundOne = true;
			}
			if (!foundOne || lineIt == line.cend() || *lineIt != 2) {
				return false;
			}
			for (int i = 0; i < *it; ++i) {
				if (lineIt == line.cend() || *lineIt != 2) {
					return false;
				}
				++lineIt;
			}
		}
		while (lineIt != line.cend() && *lineIt == 1) ++lineIt;
		if (lineIt == line.cend()) {
			return true;
		}
		return false;
	}
	void backtracking(std::vector<int>& line, std::vector<int>& springs, long long& result, int curPos = 0) {
		if (curPos >= line.size()) {
			if (checkSpringline(line, springs)) {
				++result;
			}
			return;
		}
		while (line[curPos] != 0) {
			++curPos;
			if (curPos >= line.size()) {
				if (checkSpringline(line, springs)) {
					++result;
				}
				return;
			}
		}
		line[curPos] = 1;
		if(checkSpringlineUntilPosition(line, springs, curPos)) backtracking(line, springs, result, curPos + 1);
		line[curPos] = 2;
		if (checkSpringlineUntilPosition(line, springs, curPos)) backtracking(line, springs, result, curPos + 1);
		line[curPos] = 0;
	}
	long long calc1(const char* filename) {
		FILE* stream;
		int c;
		std::vector<std::vector<int>> lines;
		std::vector<std::vector<int>> springsOfLine;
		fopen_s(&stream, filename, "r");
		while ((c = _getc_nolock(stream)) != -1) {
			if (c == '\n') break;
			int curLine = 0;
			do {
				lines.emplace_back(std::vector<int>());
				springsOfLine.emplace_back(std::vector<int>());
				do {
					lines[curLine].emplace_back(c % 3);
				} while ((c = _getc_nolock(stream)) != ' ');
				c = _getc_nolock(stream);
				int curSpringNumber = 0;
				while (c != '\n' && c != -1) {
					springsOfLine[curLine].emplace_back(0);
					auto& curNumber = springsOfLine[curLine][curSpringNumber];
					while (c >= '0' && c <= '9') {
						curNumber = curNumber * 10 + c - '0';
						c = _getc_nolock(stream);
					}
					if (c == '\n') break;
					c = _getc_nolock(stream);
					++curSpringNumber;
				}
				++curLine;
			} while ((c = _getc_nolock(stream)) != '\n' && c != -1);
		}
		_fclose_nolock(stream);
		long long result = 0;
		for (int i = 0; i < lines.size(); ++i) {
			backtracking(lines[i], springsOfLine[i], result);
		}
		return result;
	}
	typedef uint8_t State;
	enum class Symbol {
		a, b
	};
	typedef std::map<std::pair<State, Symbol>, int> Transition;
	class Automata {
	public:
		Automata(std::vector<int>& springs) {
			finalState = 0;
			transitions.emplace(std::make_pair(finalState, Symbol::a), finalState);
			bool first = true;
			for (const auto spring : springs) {
				if (!first) {
					transitions.emplace(std::make_pair(finalState, Symbol::a), finalState + 1);
					states.emplace(std::make_pair( finalState, 0 ));
					++finalState;
					transitions.emplace(std::make_pair(finalState, Symbol::a), finalState);
				}
				first = false;
				for (int i = 0; i < spring; ++i) {
					transitions.emplace(std::make_pair(finalState, Symbol::b), finalState + 1);
					states.emplace(std::make_pair(finalState, 0));
					++finalState;
				}
			}
			states.emplace(std::make_pair(finalState, 0));
			transitions.emplace(std::make_pair(finalState, Symbol::a), finalState);
		}
		void reset() {
			for (auto& state : states) {
				if (state.first == 0)state.second = 1;
				else state.second = 0;
			}
		}

		void handleInput(int input,int curPosition, int maxSize) {
			std::map<State, long long> newStates;
			auto symbolNode = inputToSymbolMap.find(input);
			if (symbolNode == inputToSymbolMap.end()) std::cout << "input not valid";
			auto symbols = symbolNode->second;
			for (const auto& curState : states) {
				if (curState.first + maxSize - curPosition < finalState) continue;
				for (const auto& symbol : symbols) {
					auto nextState = transitions.find(std::make_pair(curState.first, symbol));
					if (nextState != transitions.end()) {
						const auto [returnIt, success] = newStates.insert({ nextState->second, curState.second });
						if (!success) {
							auto availableState = newStates.find(nextState->second);
							availableState->second += curState.second;
						}
					}
				}
			}
			states.swap(newStates);
			
		}
		long long result() {
			auto resNode = states.find(finalState);
			if (resNode == states.end()) return 0;
			return resNode->second;
		}
	private:
		const std::map<int, std::vector<Symbol>> inputToSymbolMap = {
			{0, {Symbol::a, Symbol::b} },
			{1, {Symbol::a}},
			{2, {Symbol::b} }
		};

		Transition transitions;
		std::map<State, long long> states;
		int finalState;
	};


	long long calc2(const char* filename) {
		FILE* stream;
		int c;
		long long result = 0;
		fopen_s(&stream, filename, "r");
		while ((c = _getc_nolock(stream)) != -1) {
			if (c == '\n') break;
			do {
				std::vector<int> tempLine;
				std::vector<int> tempSprings;
				do {
					tempLine.emplace_back(c % 3);
				} while ((c = _getc_nolock(stream)) != ' ');
				c = _getc_nolock(stream);
				int curSpringNumber = 0;
				while (c != '\n' && c != -1) {
					tempSprings.emplace_back(0);
					auto& curNumber = tempSprings[curSpringNumber];
					while (c >= '0' && c <= '9') {
						curNumber = curNumber * 10 + c - '0';
						c = _getc_nolock(stream);
					}
					if (c == '\n') break;
					c = _getc_nolock(stream);
					++curSpringNumber;
				}
				std::vector<int> lines;
				std::vector<int> cummulatedSprings;
				for (int i = 0; i < 5; ++i) {
					for (const auto& l : tempLine) {
						lines.emplace_back(l);
					}
					lines.emplace_back(0);
					for (const auto& s : tempSprings) {
						cummulatedSprings.emplace_back(s);
					}
				}
				lines.pop_back();
				Automata automata(cummulatedSprings);
				automata.reset();
				int curSymbol = 0;
				for (const auto& symbol : lines) {
					automata.handleInput(symbol, curSymbol, lines.size());
					++curSymbol;
				}
				result += automata.result();
			} while ((c = _getc_nolock(stream)) != '\n' && c != -1);
		}
		_fclose_nolock(stream);
		
		return result;
	}
	long long calc3(const char* filename) {
		FILE* stream;
		int c;
		long long result = 0;
		fopen_s(&stream, filename, "r");
		while ((c = _getc_nolock(stream)) != -1) {
			if (c == '\n') break;
			do {
				std::vector<int> lines;
				std::vector<int> cummulatedSprings;
				do {
					lines.emplace_back(c % 3);
				} while ((c = _getc_nolock(stream)) != ' ');
				c = _getc_nolock(stream);
				int curSpringNumber = 0;
				while (c != '\n' && c != -1) {
					cummulatedSprings.emplace_back(0);
					auto& curNumber = cummulatedSprings[curSpringNumber];
					while (c >= '0' && c <= '9') {
						curNumber = curNumber * 10 + c - '0';
						c = _getc_nolock(stream);
					}
					if (c == '\n') break;
					c = _getc_nolock(stream);
					++curSpringNumber;
				}
				Automata automata(cummulatedSprings);
				automata.reset();
				int curSymbol = 0;
				for (const auto& symbol : lines) {
					automata.handleInput(symbol, curSymbol, lines.size());
					++curSymbol;
				}
				result += automata.result();
			} while ((c = _getc_nolock(stream)) != '\n' && c != -1);
		}
		_fclose_nolock(stream);

		return result;
	}
	long long calc4(const char* filename) {
		FILE* stream;
		int c;
		fopen_s(&stream, filename, "r");
		while ((c = _getc_nolock(stream)) != -1) {

		}
		_fclose_nolock(stream);

		return -1;
	}
}