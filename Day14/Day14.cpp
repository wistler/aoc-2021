#include "Day14.h"
#include <iomanip>

using namespace std;

typedef unsigned long long BIGINT;

const string DAY("Day14");


BIGINT diffOfMostAndLeastCommon(map<string, BIGINT> linkCounts, string startingChain, int debug) {
	map<char, BIGINT> h;
	h.clear();
	for each (auto it in linkCounts) {
		if (debug) {
			cout << "dbg    | H1: " << it.first << " -> " << it.second << "  "
				 << it.first[0] << "|" << it.first[1] << endl;
		}
		h[it.first[0]] += it.second;
		h[it.first[1]] += it.second;
	}

	h[startingChain.front()] += 1;
	h[startingChain.back()] += 1;
	for each (auto it in h) {
		h[it.first] /= 2;
		if (debug) {
			cout << "dbg    | H2: " << (char) it.first << " -> " << it.second << endl;
		}
	}

	vector<BIGINT> counts;
	for each (auto it in h)
		counts.push_back(it.second);
	
	sort(counts.begin(), counts.end(), greater<BIGINT>());
	if (debug) 
		cout << "dbg    | Hist: " << str(counts) << endl;
	
	return counts.front() - counts.back();
}

tuple<BIGINT, BIGINT> process(istream &ss, int debug)
{
	BIGINT count1 = -1, count2 = 0;
	string line;
	string startingChain;
	
	map<string, vector<string>> insertionRules;

	map<string, BIGINT> linkCounts;


	while (getline(ss, line))
	{
		line = trim(line);
		if (line.size() == 0)
		{
			continue;
		}
		if (startingChain == "") {
			startingChain = line;
			continue;
		}
		char a, b, c;
		a = line[0];
		b = line[1];
		c = line[6];
		insertionRules[string{ a,b }] = { string{ a,c }, string{ c,b } };
		linkCounts[string{ a,b }] = 0;
	}

	if (debug) {
		for each (auto rule in insertionRules)
		{
			cout << "dbg    | " << rule.first << " -> " << str(rule.second) << endl;
		}
	}

	if (debug > 1) {
		cout << left << setw(14) << "Template: " << startingChain << endl;
	}

	for (int i = 0; i < startingChain.size() - 1; i++)
	{
		string link = startingChain.substr(i, 2);
		linkCounts[link] ++;
	}
	if (debug) {
		for each (auto it in linkCounts) {
			cout << "dbg    | 0: " << it.first << " -> " << it.second << endl;
		}
	}
	for (int i = 0; i < 10; i++) {
		map<string, BIGINT> newLinkCounts; // = linkCounts;
		for each (auto li in linkCounts) {
			if (li.second > 0) {
				if (insertionRules.find(li.first) != insertionRules.end()) {
					for each (auto insert in insertionRules[li.first]) {
						newLinkCounts[insert] += li.second;
					}
				}
				else {
					newLinkCounts[li.first] += li.second;
				}
			}
		}
		linkCounts = newLinkCounts;
		if (debug && i < 5) {
			for each (auto it in linkCounts) {
				cout << "dbg    | " << i+1 << ": " << it.first << " -> " << it.second << endl;
			}
		}
	}
	
	count1 = diffOfMostAndLeastCommon(linkCounts, startingChain, debug);

	cout << "Part 1 | Difference = " << count1 << endl;

	for (int i = 0; i < 30; i++) {
		map<string, BIGINT> newLinkCounts; // = linkCounts;
		for each (auto li in linkCounts) {
			if (li.second > 0) {
				if (insertionRules.find(li.first) != insertionRules.end()) {
					for each (auto insert in insertionRules[li.first]) {
						newLinkCounts[insert] += li.second;
					}
				} else {
					newLinkCounts[li.first] += li.second;
				}
			}
		}
		linkCounts = newLinkCounts;
	}

	count2 = diffOfMostAndLeastCommon(linkCounts, startingChain, debug);


	cout << "Part 2 | Difference = " << count2 << endl;

	return {count1, count2};
}

void main()
{
	cout << DAY << endl;

	vector<tuple<string, BIGINT, BIGINT>> testData{
		{
			R"(
				NNCB

				CH -> B
				HH -> N
				CB -> H
				NH -> C
				HB -> C
				HC -> B
				HN -> C
				NN -> C
				BH -> H
				NC -> B
				NB -> B
				BN -> B
				BB -> N
				BC -> B
				CC -> N
				CN -> C
			)",
			1588,
			2188189693529,
		},
	};

	for each (auto data in testData)
	{
		string input = get<0>(data);
		BIGINT part1exp = get<1>(data);
		BIGINT part2exp = get<2>(data);

		stringstream testDataStream(input);
		auto ans = process(testDataStream, false);
		if ((part1exp != -1 && get<0>(ans) != part1exp) || (part2exp != -1 && get<1>(ans) != part2exp))
		{
			cout << "TEST DATA: " << input << endl;
			resetStream(testDataStream);
			process(testDataStream, 2);
			cout << "ERROR: Part1 ans: Expected " << part1exp << ", got " << get<0>(ans) << endl;
			cout << "ERROR: Part2 ans: Expected " << part2exp << ", got " << get<1>(ans) << endl;
			exit(1);
		}
	}

	cout << endl
		 << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, false);
}
