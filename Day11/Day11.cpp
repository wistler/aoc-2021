#include "Day11.h"
#include <iomanip>

using namespace std;

typedef unsigned long long BIGINT;


void printMap(map<Coord, short>& octoMap, int& minX, int& maxX, int& minY, int& maxY) {
	if (minX == -1) {
		for each(auto entry in octoMap) {
			if (entry.first.x < minX || minX < 0) minX = entry.first.x;
			if (entry.first.x > maxX || maxX < 0) maxX = entry.first.x;
			if (entry.first.y < minY || minY < 0) minY = entry.first.y;
			if (entry.first.y > maxY || maxY < 0) maxY = entry.first.y;
		}
		cout << "dbg    | range: "<< minX << ","<< maxX << ","<< minY <<","<< maxY << endl;
	}
	for (int y=minY; y<=maxY; y++) {
		for (int x=minX; x<=maxX; x++) {
			int v = octoMap[Coord{x,y}];
			if (v > 9)
				cout << "*";
			else
				cout << v;
		}
		cout << endl;
	}
}

void flash(map<Coord, short>& octoMap, set<Coord>& blown, Coord pt, int debug) {
	// octoMap[pt] += 1;
	if (octoMap[pt] > 9) {
		if (debug == 2) {
			cout << "dbg    | flash: " << pt.str() << " -> " << octoMap[pt] << endl;
		}
		blown.insert(pt); // track
		octoMap[pt] = 0;
		for each(Coord nc in getNeighbors(pt)) {
			if (blown.find(nc) != blown.end()) continue;
			if (octoMap.find(nc) == octoMap.end()) continue;
			octoMap[nc] += 1;
			flash(octoMap, blown, nc, debug);
		}
	}
}

BIGINT simSteps(map<Coord, short> octoMap, int stepCount, int debug) {
	BIGINT flashes = 0;
	int minX = -1, maxX = -1, minY = -1, maxY = -1;
	printMap(octoMap, minX, maxX, minY, maxY);

	set<Coord> blown;
	for (int i=1; i<=stepCount; i++) {
		blown.clear();

		for each(auto entry in octoMap) {
			octoMap[entry.first] += 1;
		}

		for each(auto entry in octoMap) {
			flash(octoMap, blown, entry.first, debug);
		}

		// at most flash once per step..
		int stepFlashes = (int) blown.size();
		flashes += stepFlashes;

		if (debug) {
			cout << "dbg    | Step " << setw(4) << i << ": Flashes: " << setw(4) << stepFlashes << endl;
		}

		if (debug == 1) {
			if (i <= 10 || i % 10 == 0) {
				printMap(octoMap, minX, maxX, minY, maxY);
			}
		} else if (debug == 2) {
			printMap(octoMap, minX, maxX, minY, maxY);
		}
	}
	return flashes;
}

int findSyncStep(map<Coord, short> octoMap, int debug) {
	int minX = -1, maxX = -1, minY = -1, maxY = -1;

	int step;
	set<Coord> blown;

	for (step=1; blown.size() != octoMap.size(); step++) {
		blown.clear();

		for each(auto entry in octoMap) {
			octoMap[entry.first] += 1;
		}

		for each(auto entry in octoMap) {
			flash(octoMap, blown, entry.first, debug);
		}
	}
	return step-1;
}

void process(istream& ss, int debug) {
	int syntaxErrorScore = 0;
	map<Coord, short> octoMap;
	
	string line;
	int x = 0, y = 0;
	while (getline(ss, line)) {
		line = trim(line);
		if (line.size() == 0) continue;
		stringstream ls(line);
		for (char ch; ls >> ch;) {
			if (ch == '\n') continue;
			auto c = Coord{ x++,y };
			octoMap[c] = ch - '0';
		}
		y++; x = 0;
	}

	int stepCount = 100;
	if (debug == 2)
		stepCount = 2;

	auto flashes = simSteps(octoMap, stepCount, debug);
	cout << "Part 1 | Flashes = " << flashes << endl;

	int syncStep = findSyncStep(octoMap, debug);
	cout << "Part 2 | SyncStep = " << syncStep << endl;
}

void main()
{
	string DAY("Day11");
	cout << DAY << endl;

	char testData1[] = R"(
		11111
		19991
		19191
		19991
		11111
	)";

	stringstream testDataStream1(testData1);
	cout << "With Test Data #1.." << endl;
	process(testDataStream1, 2);

	char testData2[] = R"(
		5483143223
		2745854711
		5264556173
		6141336146
		6357385478
		4167524645
		2176841721
		6882881134
		4846848554
		5283751526
	)";

	stringstream testDataStream2(testData2);
	cout << "With Test Data #2.." << endl;
	process(testDataStream2, 1);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, false);
}
