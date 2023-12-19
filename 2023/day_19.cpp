#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>
#include <vector>

#include "aoc.hpp"

enum class PartCategory {
	x,
	m,
	a,
	s,

	count,
};

static PartCategory CharToPartCat(char c) {
	if (c == 'x')
		return PartCategory::x;
	if (c == 'm')
		return PartCategory::m;
	if (c == 'a')
		return PartCategory::a;
	if (c == 's')
		return PartCategory::s;

	assert(false);
	return PartCategory::count;
}

struct Statement {
	std::string workflow_id;

	PartCategory c;
	char op;
	int n;
};

struct WorkFlow {
	std::vector<Statement> rule;
};

static Statement ParseRule(const std::string& str) {
	Statement rule = {};

	std::regex re(R"((\w)([<>])(\d+):(\w+))");
	std::smatch m;
	std::regex_search(str, m, re);
	if (m.size()) {
		rule.c = CharToPartCat(m[1].str()[0]);
		rule.op = m[2].str()[0];
		rule.n = atoi(m[3].str().c_str());
		rule.workflow_id = m[4].str();
	}
	else {
		rule.workflow_id = str;
	}

	return rule;
}

struct Part {
	int c[(int)PartCategory::count];
};

static std::string EvaluateStatement(const Statement& rule, const Part& part) {
	if (rule.op == 0) {
		return rule.workflow_id;
	}

	if (rule.op == '>') {
		if (part.c[(int)rule.c] > rule.n) {
			return rule.workflow_id;
		}
		return "";
	}
	else if (rule.op == '<') {
		if (part.c[(int)rule.c] < rule.n) {
			return rule.workflow_id;
		}
		return "";
	}
	else {
		assert(false);
	}

	assert(false);
	return "?";
}

static bool IsPartAccapted(const std::unordered_map<std::string, WorkFlow>& workflows, const Part& part) {
	std::string id = "in";

evaluate:
	const auto& w = workflows.at(id);

	for (const auto& s : w.rule) {
		std::string result = EvaluateStatement(s, part);
		if (result == "A")
			return true;
		if (result == "R")
			return false;
		if (result != "") {
			id = result;
			goto evaluate;
		}
	}

	assert(false);
	return false;
}

int main() {

	std::unordered_map<std::string, WorkFlow> workflows;
	std::regex re(R"((\w+)\{(.+)\})");
	for (std::string line; std::getline(std::cin, line);) {
		if (line == "")
			break;

		std::smatch m;
		std::regex_search(line, m, re);

		auto& w = workflows[m[1].str()];

		for (const auto& rule_str : split(m[2].str(), ",")) {
			w.rule.push_back(ParseRule(rule_str));
		}
	}

	std::vector<Part> parts;

	re = R"(x=(\d+),m=(\d+),a=(\d+),s=(\d+))";
	for (std::string line; std::getline(std::cin, line);) {
		std::smatch m;
		std::regex_search(line, m, re);

		parts.push_back({
			atoi(m[1].str().c_str()),
			atoi(m[2].str().c_str()),
			atoi(m[3].str().c_str()),
			atoi(m[4].str().c_str()),
			});
	}

	int result1 = 0;
	for (const auto& part : parts) {
		if (IsPartAccapted(workflows, part)) {
			result1 += part.c[0] + part.c[1] + part.c[2] + part.c[3];
			printf("A\n");
		}
		else {
			printf("R\n");
		}
	}

	return 0;
}
