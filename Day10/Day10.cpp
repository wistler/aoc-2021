#include "Day10.h"
#include <iomanip>

using namespace std;

typedef unsigned long long BIGINT;

map<char, int> SYNTAX_ERROR_SCORE = {
	{')', 3},
	{']', 57},
	{'}', 1197},
	{'>', 25137},
};

map<char, int> COMPLETION_SCORE = {
	{'(', 1},
	{'[', 2},
	{'{', 3},
	{'<', 4},
};

map<char, char> PAIRS = {
	{'(', ')'},
	{'[', ']'},
	{'{', '}'},
	{'<', '>'},
};

inline bool isOpen(char ch) {
	return ch == '(' || ch == '[' || ch == '{' || ch == '<';
}


void process(istream& ss, int debug) {
	int syntaxErrorScore = 0;
	vector<BIGINT> autoCompleteScores;

	stringstream part1, part2;

	string line;
	while (getline(ss, line)) {
		line = trim(line);
		if (line.size() == 0) continue;

		stringstream ls(line);
		vector<char> stack;
		int pos = 0;
		bool badLine = false;
		for (char ch; !badLine && ls >> ch; pos++) {
			if (isOpen(ch)) {
				stack.push_back(ch);
			} else {
				auto it = PAIRS.find(stack.back());
				char expected = it->second;
				if (expected != ch) {
					if (debug) {
						part1 << std::left << setw(30) << line << " | Pos " << pos << ": Expected " << expected << " but found " << ch << " instead." << endl;
					}
					syntaxErrorScore += SYNTAX_ERROR_SCORE.find(ch)->second;
					badLine = true;
				} else {
					stack.pop_back();
				}
			}
		}
		if (!badLine) {
			BIGINT completionScore = 0;
			
			for (auto it = stack.rbegin(); it != stack.rend(); it++)
				completionScore = completionScore * 5 + COMPLETION_SCORE.find(*it)->second;

			if (debug) {
				part2 << std::left << setw(30) << line << " | Stack: " << setw(15) << str(stack, "") << " Score: " << completionScore << endl;
			}

			autoCompleteScores.push_back(completionScore);
		}
	}

	cout << part1.str();
	cout << "Part 1 | Syntax Error Score = " << syntaxErrorScore << endl;

	sort(autoCompleteScores.begin(), autoCompleteScores.end());
	if (debug) {
		part2 << "dbg    | Sorted Scores: " << str(autoCompleteScores) << endl;
	}
	cout << part2.str();
	size_t mid = (autoCompleteScores.size() + 1) / 2 - 1;
	cout << "Part 2 | Autocomplete Score count: " << autoCompleteScores.size() << ", middle: " << autoCompleteScores[mid] << endl;
}

void main()
{
	string DAY("Day10");
	cout << DAY << endl;

	char testData1[] = R"(
		[({(<(())[]>[[{[]{<()<>>
		[(()[<>])]({[<{<<[]>>(
		{([(<{}[<>[]}>{[]{[(<()>
		(((({<>}<{<{<>}{[]{[]{}
		[[<[([]))<([[{}[[()]]]
		[{[{({}]{}}([{[{{{}}([]
		{<[[]]>}<{[{[{[]{()[[[]
		[<(<(<(<{}))><([]([]()
		<{([([[(<>()){}]>(<<{{
		<{([{{}}[<[[[<>{}]]]>[]]
	)";

	stringstream testDataStream1(testData1);
	cout << "With Test Data #1.." << endl;
	process(testDataStream1, 2);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, false);
}
