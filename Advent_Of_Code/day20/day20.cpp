#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
namespace day20 {
	bool gotcha = false;
	std::set<std::string> names;
	enum class moduleSignal{LOW, HIGH, LAST};
	enum class moduleState { ON, OFF};
	const unsigned int NUMBER_OF_LINES = 10; //
	const unsigned int LINEWIDTH = 10; //
	struct moduleInput {
		std::string moduleName;
		moduleSignal signal;
		friend bool operator<(const moduleInput& l, const moduleInput& r)
		{
			return l.moduleName
				< r.moduleName;
		}
	};
	class MyModule {
	public:
		MyModule() {};
		MyModule(char moduleType, std::string moduleName) : moduleType(moduleType), moduleName(moduleName) {}
		MyModule(char moduleType, std::string moduleName, std::set<std::string> outputs) : moduleType(moduleType), outputs(outputs), moduleName(moduleName) {}
		MyModule(std::string moduleName, std::set<std::string> outputs) : outputs(outputs), moduleName(moduleName) {}
		char moduleType;
		std::string moduleName;
		std::set<moduleInput> inputs;
		std::set<std::string> outputs;
		moduleState state = moduleState::OFF;
	};
	using collectedState = std::vector<int>;
	collectedState collectState(std::map<std::string, MyModule>& moduleMap) {
		collectedState result;

		for (const auto& [moduleName, module] : moduleMap) {
			switch (module.moduleType) {
			case '%':
				result.emplace_back(static_cast<int>(module.state));
				break;
			case '&':
				int intermediateResult = 1;
				for (const auto& input : module.inputs) {
					intermediateResult = intermediateResult * (1 + static_cast<int>(input.signal));
				}
				result.emplace_back(intermediateResult);
				break;
			}

		}

		return result;
	}


	std::set<std::string> parseOutputs(std::string outputs) {
		std::set<std::string> result;
		auto curPos = 0;
		;
		std::string nextOutput;
		auto nextPos = outputs.find(',');
		for (; nextPos != std::string::npos; nextPos = outputs.find(',', curPos)) {
			result.emplace(outputs.substr(curPos, nextPos - curPos));
			curPos = nextPos +2;
		}
		result.emplace(outputs.substr(curPos, nextPos - curPos));
		return result;
	}

	std::pair<std::string, MyModule> parseLine(std::string line) {
		int nameStartIndex = 1;
		auto moduleType = line[0];
		if (moduleType == 'b') nameStartIndex = 0;
		auto moduleName = line.substr(nameStartIndex, line.find(' ')-nameStartIndex);
		auto outputs = line.substr(line.find('>') + 2, line.size());
		MyModule myModule = MyModule( moduleType, moduleName);
		myModule.outputs = parseOutputs(outputs);
		return { moduleName, myModule };
	}
	inline moduleState switchState(moduleState state) {
		return static_cast<moduleState>(1 - static_cast<int>(state));
	}
	void sendSignal(moduleSignal signal, std::string receivingModuleString, std::string sendingModuleString, std::map<std::string, MyModule>& moduleMap, std::queue<MyModule>& moduleQueue) {
		auto& module = moduleMap.find(receivingModuleString)->second;
		//if (signal == moduleSignal::HIGH && (sendingModuleString == "gt"/* || sendingModuleString == "nl"*/ || sendingModuleString == "lr" || sendingModuleString == "vr"))
		//	std::cout << sendingModuleString << " is sending " << (signal == moduleSignal::LOW ? "low" : "high") << " to " << receivingModuleString << "\n";
		switch (module.moduleType) {
		case '%':
			if (signal == moduleSignal::LOW) {
				module.state = switchState(module.state);
				moduleQueue.push(module);
			}
			break;
		case '&':
		{
			auto node = module.inputs.extract({ sendingModuleString, signal });
			node.value().signal = signal;
			module.inputs.insert(std::move(node));
			moduleQueue.push(module);
			break;
		}
		default:
			if (signal == moduleSignal::HIGH && (sendingModuleString == "gt" || sendingModuleString == "nl" || sendingModuleString == "lr" || sendingModuleString == "vr")) {
				std::cout << sendingModuleString << " is sending " << (signal==moduleSignal::LOW ? "low" : "high")<< " to " << receivingModuleString << "\n";
				gotcha = true;
			}
		}
	}

	std::pair<int, int> pushButton(std::map<std::string, MyModule>& moduleMap) {
		std::pair<int, int> result = {0,0};
		std::queue<MyModule> moduleQueue;
		moduleQueue.push(moduleMap.find("button")->second);
		while (!moduleQueue.empty()) {
			auto next = moduleQueue.front();
			moduleQueue.pop();
			switch (next.moduleType) {
			case '+':
			{
				++result.first;
				auto broadcaster = moduleMap.find("broadcaster")->second;
				for (const auto& output : broadcaster.outputs) {
					++result.first;
					sendSignal(moduleSignal::LOW, output, "broadcaster", moduleMap, moduleQueue);
				}
				break;
			}
			case '%':
				for (const auto& output : next.outputs) {
					if (next.state == moduleState::ON) {
						++result.second;
						sendSignal(moduleSignal::HIGH, output, next.moduleName, moduleMap, moduleQueue);
					}
					else {
						++result.first;
						sendSignal(moduleSignal::LOW, output, next.moduleName, moduleMap, moduleQueue);
					}
				}
				break;
			case '&':
			{
				bool allHigh = true;
				for (const auto& input : next.inputs) {
					if (input.signal == moduleSignal::LOW) {
						allHigh = false;
						break;
					}
				}
				if (allHigh) {
					for (const auto& output : next.outputs) {
						++result.first;
						sendSignal(moduleSignal::LOW, output, next.moduleName, moduleMap, moduleQueue);
					}
				}
				else {
					for (const auto& output : next.outputs) {
						++result.second;
						sendSignal(moduleSignal::HIGH, output, next.moduleName, moduleMap, moduleQueue);
					}
				}
				break;
			}
			default:
				std::cout << "holy shit how did I get here?!?!?\n";
			}
		}


		return result;
	}


	long long calc1(const char* filename) {
		std::ifstream infile(filename);
		std::string line;
		std::map<std::string, MyModule> moduleMap;
		moduleMap.emplace("button", MyModule{ '+', "button", {"broadcaster"}});
		while (std::getline(infile, line)) {
			auto pair = parseLine(line);
			auto moduleName = pair.first;
			auto& curModule = pair.second;
			for (const auto& output : curModule.outputs) {
				auto found = moduleMap.find(output);
				if (found == moduleMap.end()) {
					MyModule myModule = MyModule();
					myModule.inputs.emplace(moduleName, moduleSignal::LOW);
					myModule.moduleName = output;
					moduleMap.emplace(output, myModule);
				}
				else found->second.inputs.emplace(moduleName, moduleSignal::LOW);
			}
			auto found = moduleMap.find(moduleName);			
			if (found == moduleMap.end()) moduleMap.emplace(moduleName, curModule);
			else {
				found->second.moduleType = curModule.moduleType;
				found->second.outputs = curModule.outputs;
			}
		}
		long long result = 0;
		std::vector<collectedState> states = { collectState(moduleMap) };
		std::vector<std::pair<int, int>> pulses;
		for (int buttonPushed = 0; buttonPushed < 1000; ++buttonPushed) {
			auto pair = pushButton(moduleMap);
			auto curState = collectState(moduleMap);
			for (int cycleStart = 0; cycleStart < states.size(); ++cycleStart) {
				if (states[cycleStart] == curState) {
					++buttonPushed;
					int cycleLength = buttonPushed - cycleStart;
					int numberOfCycles = 1000 / cycleLength;
					int remainingPushes = 1000 % cycleLength;
					std::pair<int, int> intermediateResult = { 0,0 };
					for (int i = 0; i < cycleStart; ++i) {
						intermediateResult.first += pulses[i].first;
						intermediateResult.second += pulses[i].second;
					}
					std::pair<int, int> cyclePulses = { 0,0 };
					for (int i = cycleStart; i < cycleStart + cycleLength - 1; ++i) {
						if (i < remainingPushes) {
							intermediateResult.first += pulses[i].first;
							intermediateResult.second += pulses[i].second;
						}
						cyclePulses.first += pulses[i].first;
						cyclePulses.second += pulses[i].second;
					}
					cyclePulses.first += pair.first;
					cyclePulses.second += pair.second;
					cyclePulses.first *= numberOfCycles;
					cyclePulses.first *= numberOfCycles;
					intermediateResult.first += cyclePulses.first;
					intermediateResult.second += cyclePulses.second;
					return intermediateResult.first * intermediateResult.second;
				}
			}

			states.emplace_back(collectState(moduleMap));
			pulses.emplace_back(pair);
		}
		std::pair<int, int> intermediateResult = { 0,0 };
		for (const auto& pulse: pulses) {
			intermediateResult.first += pulse.first;
			intermediateResult.second += pulse.second;
		}
		return intermediateResult.first * intermediateResult.second;
	}


	long long calc2(const char* filename) {
		std::ifstream infile(filename);
		std::string line;
		std::map<std::string, MyModule> moduleMap;
		moduleMap.emplace("button", MyModule{ '+', "button", {"broadcaster"} });
		while (std::getline(infile, line)) {
			auto pair = parseLine(line);
			auto moduleName = pair.first;
			auto& curModule = pair.second;
			for (const auto& output : curModule.outputs) {
				auto found = moduleMap.find(output);
				if (found == moduleMap.end()) {
					MyModule myModule = MyModule();
					myModule.inputs.emplace(moduleName, moduleSignal::LOW);
					myModule.moduleName = output;
					moduleMap.emplace(output, myModule);
				}
				else found->second.inputs.emplace(moduleName, moduleSignal::LOW);
			}
			auto found = moduleMap.find(moduleName);
			if (found == moduleMap.end()) moduleMap.emplace(moduleName, curModule);
			else {
				found->second.moduleType = curModule.moduleType;
				found->second.outputs = curModule.outputs;
			}
		}
		int counter = 0;
		while (true) {
			++counter;
			pushButton(moduleMap); 
			if (gotcha) {
				std::cout << counter << "\n";
				gotcha = false;
			}
		}




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