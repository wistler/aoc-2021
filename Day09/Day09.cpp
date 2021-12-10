#include "Day09.h"

using namespace std;

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

void genBasin(map<Coord, short>& heightMap, Coord pt, set<Coord>& basin, int debug) {
	basin.insert(pt);
	for each (auto nc in getNeighbors(pt)) {
		auto it = heightMap.find(nc);
		if (it != heightMap.end() && it->second != 9 && basin.find(nc) == basin.end()) {
			genBasin(heightMap, nc, basin, debug);
		}
	}
}

int getBasinSize(map<Coord, short>& heightMap, Coord pt, int debug) {
	set<Coord> basin;
	genBasin(heightMap, pt, basin, debug);
	return (int) basin.size();
}

void process(istream& ss, int debug) {
	int sum = 0;
	map<Coord, short> heightMap;
	
	string line;
	int x = 0, y = 0;
	while (getline(ss, line)) {
		stringstream ls(line);
		for (char ch; ls >> ch;) {
			if (ch == '\n') continue;
			heightMap[Coord{ x++,y }] = ch - '0';
		}
		y++; x = 0;
	}

	vector<Coord> lowPoints;
	for each (auto entry in heightMap) {
		bool lowest = true;
		for each (Coord nc in getNeighbors(entry.first)) {
			auto it = heightMap.find(nc);
			if (it != heightMap.end() && it->second <= entry.second) {
				lowest = false;
				break;
			}
		}
		if (lowest) {
			lowPoints.push_back(entry.first);
			sum += entry.second + 1;
			if (debug) cout << "dbg    | Minima @ " << entry.first.x << "," << entry.first.y << " -> Risk " << entry.second + 1 << endl;
		}
	}

	if (debug) cout << "dbg    | Map Size = " << heightMap.size()  << endl;
	cout << "Part 1 | Risk Sum = " << sum << endl;

	vector<int> basinSizes;
	for each (auto pt in lowPoints)
		basinSizes.push_back(getBasinSize(heightMap, pt, debug));
	sort(basinSizes.begin(), basinSizes.end(), greater<int>());

	if (debug) cout << "dbg    | Basin sizes = " << str(basinSizes) << endl;
	cout << "Part 2 | Largest Basins Product = " << basinSizes[0] * basinSizes[1] * basinSizes[2] << endl;
}

void main()
{
	string DAY("Day09");
	cout << DAY << endl;

	char testData1[] = R"(
		2199943210
		3987894921
		9856789892
		8767896789
		9899965678
	)";

	stringstream testDataStream1(testData1);
	cout << "With Test Data #1.." << endl;
	process(testDataStream1, 2);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, 0);
}
