#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <queue>
#include <set>
namespace day19 {
	struct Workflow {
		Workflow() {};
		Workflow(std::string key, std::string opString, int val, std::string next): key(key), val(val),next(next), opString(opString){}
		std::string key;
		int val;
		std::string opString;
		std::string next;
	};


	typedef std::function<std::string(const std::map<std::string, int>)> Test;

	Test parseString(std::string functionString) {
		auto key = functionString.substr(0, 1);
		auto opString = functionString.substr(1, 1);
		auto foundColon = functionString.find(':');
		auto val = std::stoi(functionString.substr(2, foundColon - 2));
		std::string next = functionString.substr(foundColon + 1);
		std::function<bool(int)> op;
		if (opString == "<") {
			op = [=](int i) {return i < val; };
		}
		else if (opString == ">") {
			op = [=](int i) {return i > val; };
		}else op = [](int i) {return true; };

		return [key, next, op](const std::map<std::string, int>& values) {
			auto it = values.find(key);
			if (it == values.end() || !op(it->second)) return "";
			return next.c_str(); 
			};
	}


	long long sumPart(const std::map<std::string, int>& part) {
		return part.find("x")->second + part.find("a")->second + part.find("m")->second + part.find("s")->second;
	}

	long long calc1(const char* filename) {
		std::map<std::string, std::vector<Test>> workflows;
		std::ifstream infile(filename);
		std::string line;
		while (std::getline(infile, line)) {
			if (line == "") break;
			auto key = line.substr(0, line.find('{'));
			std::vector<Test> tests;
			auto commaFound = line.find(',');
			auto functionString = line.substr(line.find('{') + 1, commaFound - line.find('{') - 1);
			tests.push_back(parseString(functionString));
			auto lastCommaFound = commaFound;
			while ((commaFound = line.find(',', commaFound + 1)) != std::string::npos) {
				functionString = line.substr(lastCommaFound + 1, commaFound - lastCommaFound - 1);
				tests.push_back(parseString(functionString));
				lastCommaFound = commaFound;
			}
			tests.push_back([=](const std::map<std::string, int>& values) {return line.substr(lastCommaFound + 1, line.rfind('}') - 1 - lastCommaFound); });
			workflows.emplace(key, tests);
		}
		std::vector<std::map<std::string, int>> metalparts;
		while (std::getline(infile, line)) {
			auto equalsFound = line.find('=');
			auto commaFound = line.find(',');
			auto x = std::stoi(line.substr(equalsFound +1, commaFound - equalsFound - 1));
			auto lastEqualsFound = equalsFound;
			auto lastCommaFound = commaFound;
			equalsFound = line.find('=', lastEqualsFound + 1);
			commaFound = line.find('=', lastCommaFound + 1);
			lastEqualsFound = equalsFound;
			lastCommaFound = commaFound;
			auto m = std::stoi(line.substr(equalsFound + 1, commaFound - equalsFound - 1));
			equalsFound = line.find('=', lastEqualsFound + 1);
			commaFound = line.find('=', lastCommaFound + 1);
			lastEqualsFound = equalsFound;
			lastCommaFound = commaFound;
			auto a = std::stoi(line.substr(equalsFound + 1, commaFound - equalsFound - 1));
			equalsFound = line.find('=', lastEqualsFound + 1);
			commaFound = line.find('=', lastCommaFound + 1);
			lastEqualsFound = equalsFound;
			lastCommaFound = commaFound;
			auto s = std::stoi(line.substr(equalsFound + 1, commaFound - equalsFound - 1));
			metalparts.push_back({ { "x", x }, { "m", m }, { "a", a }, { "s", s } });
		}
		infile.close();
		long long result = 0;
		for (const auto& part : metalparts) {
			std::string state = "in";
			while (state != "A" && state != "R") {
				auto workflow = workflows.find(state)->second;
				for (const auto& test : workflow) {
					auto nextState = test(part);
					if (nextState != "") {
						state = nextState;
						break;
					}
				}
				if (state == "A") result += sumPart(part);
			}
		}
		return result;
	}

	Workflow parseStringForWorkflow(std::string functionString) {
		auto key = functionString.substr(0, 1);
		auto opString = functionString.substr(1, 1);
		auto foundColon = functionString.find(':');
		auto val = std::stoi(functionString.substr(2, foundColon - 2));
		std::string next = functionString.substr(foundColon + 1);
		return Workflow(key, opString, val, next);
	}



	typedef std::map<std::string, std::pair<int, int>> partMap;
	long long calculateIntermediateResult(partMap parts) {
		long long intermediateResult = 1;
		for (const auto& partRange : parts) {
			intermediateResult *= (partRange.second.second - partRange.second.first + 1);
		}
		return intermediateResult;
	}

	long long calculatePossibilities(std::string state, const std::map<std::string, std::vector<Workflow>>& workflows, partMap parts) {
		auto curWorkflows = workflows.find(state)->second;
		long long result = 0;
		for (const auto& workflow : curWorkflows) {
			if (auto key = workflow.key != "") {
				auto op = workflow.opString;
				auto val = workflow.val;
				partMap copiedParts = parts;
				if (op == "<") {
					copiedParts[workflow.key].second = std::min(copiedParts[workflow.key].second, val - 1);
					parts[workflow.key].first = std::max(parts[workflow.key].first, val);
				}
				else {
					copiedParts[workflow.key].first = std::max(copiedParts[workflow.key].first, val + 1);
					parts[workflow.key].second = std::min(parts[workflow.key].second, val);
				}
				if (workflow.next == "A") {
					result += calculateIntermediateResult(copiedParts);
				}
				else if(workflow.next != "R") {
					result += calculatePossibilities(workflow.next, workflows, copiedParts);
				}
			}
			else {
				if (workflow.next == "A") {
					result += calculateIntermediateResult(parts);
				}
				else if(workflow.next != "R") {
					result += calculatePossibilities(workflow.next, workflows, parts);
				}
				
			}
		}
		return result;
	}

	long long calc2(const char* filename) {
		std::map<std::string, std::vector<Workflow>> workflows;
		std::ifstream infile(filename);
		std::string line;
		while (std::getline(infile, line)) {
			if (line == "") break;
			auto key = line.substr(0, line.find('{'));
			std::vector<Workflow> tests;
			auto commaFound = line.find(',');
			auto functionString = line.substr(line.find('{') + 1, commaFound - line.find('{') - 1);
			tests.push_back(parseStringForWorkflow(functionString));
			auto lastCommaFound = commaFound;
			while ((commaFound = line.find(',', commaFound + 1)) != std::string::npos) {
				functionString = line.substr(lastCommaFound + 1, commaFound - lastCommaFound - 1);
				tests.push_back(parseStringForWorkflow(functionString));
				lastCommaFound = commaFound;
			}
			tests.push_back(Workflow("", "", 0, line.substr(lastCommaFound + 1, line.rfind('}') - 1 - lastCommaFound)));
			workflows.emplace(key, tests);
		}
		infile.close();
		return calculatePossibilities("in", workflows, { {"x", {1, 4000}},{"m", {1, 4000}},{"a", {1, 4000}},{"s", {1, 4000}} });
	}
	long long calc3(const char* filename) {
		std::ifstream infile(filename);
		std::string line;
		while (std::getline(infile, line)) {

		}
		infile.close();
		return -1;
	}
	long long calc4(const char* filename) {
		std::ifstream infile(filename);
		std::string line;
		while (std::getline(infile, line)) {

		}
		infile.close();
		return -1;
	}
}