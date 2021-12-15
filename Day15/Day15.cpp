#include "Day15.h"
#include <iomanip>

using namespace std;

typedef unsigned long long BIGINT;

const string DAY("Day15");


struct Coord {
	int x, y;
	string str() {
		char buff[40];
		snprintf(buff, sizeof(buff), "%i,%i", x, y);
		return string{ buff };
	}
};

bool operator < (const Coord& c1, const Coord& c2)
{
	return std::tie(c1.x, c1.y) < std::tie(c2.x, c2.y);
}

auto getNeighbors(Coord point) {
	vector<Coord> neighbors{
		Coord{ point.x - 1, point.y },
		Coord{ point.x + 1, point.y },
		Coord{ point.x, point.y - 1 },
		Coord{ point.x, point.y + 1 }
	};
	return neighbors;
}

void processTotalRiskMap(map<Coord, BIGINT>& totalRiskMap, const map<Coord, short>& riskMap, Coord endPos, int debug)
{
	set<Coord> now = { endPos };
	set<Coord> next;
	do {
		if (debug > 1) cout << ">";
		next.clear();

		for each (auto me in now)
		{
			auto myRisk = totalRiskMap[me];

			auto neighbors = getNeighbors(me);
			for each (auto nc in neighbors)
			{
				auto n = riskMap.find(nc);
				if (n == riskMap.end()) continue; // not a valid neighbor

				auto totalRiskGettingToMe = n->second + myRisk;
				if (totalRiskMap[nc] == 0 || totalRiskMap[nc] > totalRiskGettingToMe) {
					// if there were other paths from n that were lower risk,
					// keep that.
					totalRiskMap[nc] = totalRiskGettingToMe;
					next.insert(nc);
					if (debug > 1) cout << "^";
				}
			}
		}
		now = next; // copy
	} while (now.size() > 0);

	if (debug) cout << endl;
}

tuple<BIGINT, BIGINT> process(istream &ss, int debug)
{
	BIGINT count1 = -1, count2 = 0;
	map<Coord, short> riskMap;
	map<Coord, BIGINT> totalRiskMap;

	Coord startPos{ 0,0 };
	Coord endPos{ 0,0 };

	string line;
	int x = 0, y = 0;
	while (getline(ss, line)) {
		line = trim(line);
		if (line.size() == 0)
			continue;
		stringstream ls(line);
		for (char ch; ls >> ch;) {
			if (ch == '\n') continue;
			riskMap[Coord{ x++,y }] = ch - '0';
		}
		endPos = { x,y };
		y++; x = 0;
	}

	totalRiskMap[endPos] = riskMap[endPos]; // init
	processTotalRiskMap(totalRiskMap, riskMap, endPos, debug);
	count1 = totalRiskMap[startPos] - riskMap[startPos];

	cout << "Part 1 | Min Total Risk = " << count1 << endl;
	cout << "Part 2 | Difference = " << count2 << endl;

	return {count1, count2};
}

void main()
{
	cout << DAY << endl;

	vector<tuple<string, BIGINT, BIGINT>> testData{
		{
			R"(
				1163751742
				1381373672
				2136511328
				3694931569
				7463417111
				1319128137
				1359912421
				3125421639
				1293138521
				2311944581
			)",
			40,
			-1,
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
