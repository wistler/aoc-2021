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
};

Coord operator + (const Coord& c1, const Coord& c2)
{
	return { c1.x + c2.x, c1.y + c2.y };
};

Coord operator * (const Coord& c1, int scale)
{
	return { c1.x * scale, c1.y * scale};
};


template <typename T1>
void assertEqual(T1 left, T1 right, string message) throw (exception) {
	if (left != right) {
		stringstream ss;
		ss << "ASSERTION FAILED: " << left << " != " << right << "\n" << message;
		throw exception(ss.str().c_str());
	}
}

auto getNeighbors(Coord point) {
	vector<Coord> neighbors{
		point + Coord{-1,0},
		point + Coord{+1,0},
		point + Coord{0,-1},
		point + Coord{0,+1},
	};
	return neighbors;
}

template <typename F1>
void processTotalRiskMap(map<Coord, BIGINT>& totalRiskMap, F1 riskLookupFn, Coord endPos, int debug)
{
	BIGINT steps{ 0 }, checks{ 0 }, flows{ 0 };

	set<Coord> now = { endPos };
	set<Coord> next;
	do {
		next.clear();
		steps++;

		for each (auto me in now)
		{
			if (debug > 1) cout << ">";
			flows++;

			auto myRisk = totalRiskMap[me];

			auto neighbors = getNeighbors(me);
			for each (auto nc in neighbors)
			{
				short nRisk = riskLookupFn(nc);
				if (nRisk == -1) continue; // not a valid neighbor

				auto totalRiskGettingToMe = nRisk + myRisk;
				checks++;

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
	cout << "Total Risk Map processed: " << endl 
		<< left << setw(10) << "Steps: " << right << setw(10) << steps << endl
		<< left << setw(10) << "Flows: " << right << setw(10) << flows << endl
		<< left << setw(10) << "Checks: " << right << setw(10) << checks << endl;
}

tuple<BIGINT, BIGINT> process(istream &ss, int debug)
{
	BIGINT count1 = -1, count2 = 0;
	map<Coord, short> riskMap;

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
			endPos = { x++,y };
			riskMap[endPos] = ch - '0';
		}
		y++; x = 0;
	}

	auto simpleLookup = [&](Coord c) -> short {
		auto it = riskMap.find(c);
		if (it == riskMap.end()) return -1;
		return it->second;
	};

	const Coord tileSize = { endPos.x + 1, endPos.y + 1 };
	auto tiledLookup = [&](Coord c) -> short {
		short xTile = c.x / tileSize.x;
		short yTile = c.y / tileSize.y;

		if (debug) {
			cout << "TL(" + c.str() + "): " << "xTile:" << xTile << ", yTile:" << yTile;
		}

		if (xTile >= 5 || yTile >= 5) {
			if (debug) cout << endl;
			return -1;
		}

		int tx = c.x % tileSize.x;
		int ty = c.y % tileSize.y;

		if (debug) {
			cout << ", tx:" << tx << ", ty:" << ty;
		}

		auto it = riskMap.find({tx,ty});
		if (it == riskMap.end()) {
			if (debug) cout << endl;
			return -1;
		}
		auto r = it->second + xTile + yTile;
		auto nr = r < 10 ? r : r - 9; // so that 10 -> 1, and so on..

		if (debug) {
			cout << ", r:" << r << ", nr:" << nr << endl;
		}

		return nr;
	};

	if (debug) {
		assertEqual(tiledLookup({ 0,0 }), simpleLookup({ 0,0 }), "0,0");
		assertEqual(tiledLookup(endPos), simpleLookup(endPos), "endPos: " + endPos.str());
		assertEqual(tiledLookup(endPos + Coord{ 1,0 }), (short)(simpleLookup({ 0, endPos.y }) + 1), "endPos + 1,0");
		assertEqual(tiledLookup(endPos + Coord{ 0,1 }), (short)(simpleLookup({ endPos.x ,0 }) + 1), "endPos + 0,1");
		assertEqual(tiledLookup(endPos + Coord{ 1,1 }), (short)(simpleLookup({ 0,0 }) + 2), "endPos + 1,1");
		assertEqual(tiledLookup(endPos + tileSize), (short)(simpleLookup(endPos) + 2), "endPos + endPos");
	}

	map<Coord, BIGINT> totalRiskMap;
	totalRiskMap[endPos] = riskMap[endPos]; // init
	processTotalRiskMap(totalRiskMap, simpleLookup, endPos, debug);
	count1 = totalRiskMap[startPos] - riskMap[startPos];

	cout << "Part 1 | Min Total Risk = " << count1 << endl;

	const Coord tiledEndPos = endPos + tileSize * 4;
	map<Coord, BIGINT> totalRiskMap2;
	totalRiskMap2[tiledEndPos] = tiledLookup(tiledEndPos);
	processTotalRiskMap(totalRiskMap2, tiledLookup, tiledEndPos, debug);
	count2 = totalRiskMap2[startPos] - riskMap[startPos];

	cout << "Part 2 | Min Total Risk = " << count2 << endl;

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
			315,
		},
	};

	for each (auto data in testData)
	{
		string input = get<0>(data);
		BIGINT part1exp = get<1>(data);
		BIGINT part2exp = get<2>(data);

		stringstream testDataStream(input);
		tuple<BIGINT, BIGINT> ans;
		bool failed = false;
		try {
			ans = process(testDataStream, false);
		} catch (...) {
			failed = true;
		}
		if (failed || (part1exp != -1 && get<0>(ans) != part1exp) || (part2exp != -1 && get<1>(ans) != part2exp))
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
