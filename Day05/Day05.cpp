#include "Day05.h"

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

struct Line {
	int x1, y1, x2, y2;
	string str() {
		char buff[100];
		snprintf(buff, sizeof(buff), "%i,%i -> %i,%i", x1, y1, x2, y2);
		return string{ buff };
	}
};

vector<Line> getLines(istream &ss) {
	vector<Line> lines;
	string line;
	while (ss.good()) {
		getline(ss, line);
		line = trim(line);
		// cout << "Line: " << line << endl;
		Line li{};
		if (sscanf_s(line.c_str(), "%i,%i -> %i,%i", &li.x1, &li.y1, &li.x2, &li.y2) != 4) {
			cout << "ERROR Parsing line: " << line << endl;
		}
		else {
			lines.push_back(li);
		}
	}
	return lines;
}

inline void plotCoord(map<Coord, int>& m, Coord c) {
	m[c]++;
}

inline int getStep(int x1, int x2) {
	return x2 - x1 == 0 ? 0 : x2 - x1 > 0 ? +1 : -1;
}

void plotLine(map<Coord, int>& m, Line& li, const bool includeDiag) {
	const bool isDiag = li.x1 != li.x2 && li.y1 != li.y2;

	if (!isDiag || includeDiag) {
		const int dx = getStep(li.x1, li.x2);
		const int dy = getStep(li.y1, li.y2);
		for (int x = li.x1, y = li.y1; !(x == li.x2 && y == li.y2); x += dx, y += dy) {
			plotCoord(m, Coord{ x, y });
		}
		plotCoord(m, Coord{ li.x2, li.y2 });
	}
}

map<Coord, int> plot(vector<Line>& lines, const bool includeDiag) {
	map<Coord, int> m;
	for each (Line li in lines) {
		plotLine(m, li, includeDiag);
	}
	return m;
}

void drawMap(map<Coord, int>& map) {
	int minX{0}, maxX{0}, minY{0}, maxY{0};
	for (auto const& kv : map) {
		Coord c = kv.first;
		if (minX > c.x) minX = c.x;
		if (maxX < c.x) maxX = c.x;
		if (minY > c.y) minY = c.y;
		if (maxY < c.y) maxY = c.y;
	}
	for (int y = minY; y <= maxY; y++)
		for (int x = minX; x <= maxX; x++)
			cout << (map[Coord{ x, y }] ? to_string(map[Coord{ x, y }]) : ".") 
				 << (x == maxX ? "\n" : "");
}

void parseInput(istream& ss, const bool debug) {
	vector<Line> lines = getLines(ss);

	map<Coord, int> map = plot(lines, false);	
	int dangerous = 0;
	for (auto const& x : map) {
		if (x.second > 1) dangerous++;
	}
	cout << "Part 1 | Dangerous Spots = " << dangerous << endl;
	if (debug) drawMap(map);

	map = plot(lines, true);
	int allDangerous = 0;
	for (auto const& x : map) {
		if (x.second > 1) allDangerous++;
	}
	cout << "Part 2 | Dangerous Spots = " << allDangerous << endl;
	if (debug) drawMap(map);
}

void main()
{
	string DAY("Day05");
	cout << DAY << endl;

	char testData[] = R"(
		0,9 -> 5,9
		8,0 -> 0,8
		9,4 -> 3,4
		2,2 -> 2,1
		7,0 -> 7,4
		6,4 -> 2,0
		0,9 -> 2,9
		3,4 -> 1,4
		0,0 -> 8,8
		5,5 -> 8,2
	)";

	stringstream testDataStream(testData);
	cout << "With Test Data .." << endl;
	parseInput(testDataStream, true);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	parseInput(input, false);
	
}
