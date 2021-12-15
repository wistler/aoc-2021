#include "Day13.h"
#include <iomanip>

using namespace std;

typedef unsigned long long BIGINT;

const string DAY("Day13");


void printMap(set<Coord> &points)
{
	int minX = -1, maxX = -1, minY = -1, maxY = -1;
	if (minX == -1)
	{
		for each (auto pt in points)
		{
			if (pt.x < minX || minX < 0)
				minX = pt.x;
			if (pt.x > maxX || maxX < 0)
				maxX = pt.x;
			if (pt.y < minY || minY < 0)
				minY = pt.y;
			if (pt.y > maxY || maxY < 0)
				maxY = pt.y;
		}
	}
	for (int y = minY; y <= maxY; y++)
		for (int x = minX; x <= maxX; x++)
			cout << (points.find({x, y}) != points.end() ? "#" : " ")
				 << (x == maxX ? "\n" : "");
}

void fold(set<Coord> &points, char xy, int v, int debug)
{
	for (auto it = points.begin(); it != points.end();)
	{
		if (xy == 'y' && it->y > v)
		{
			points.insert({it->x, v - (it->y - v)});
			points.erase(it++); // move to next before erase current
		}
		else if (xy == 'x' && it->x > v)
		{
			points.insert({v - (it->x - v), it->y});
			points.erase(it++); // move to next before erase current
		}
		else
		{
			it++;
		}
	}
}

tuple<BIGINT, BIGINT> process(istream &ss, int debug)
{
	int count1 = -1, count2 = 0;
	string line;
	set<Coord> points;
	bool foldInstructions = false;

	while (getline(ss, line))
	{
		line = trim(line);
		if (line.size() == 0)
		{
			if (points.size() > 0)
				foldInstructions = true;
			continue;
		}
		if (!foldInstructions)
		{
			int x, y;
			if (sscanf_s(line.c_str(), "%d,%d", &x, &y) != 2)
			{
				cout << "ERROR: Unexpected line format [1]: " << line << endl;
				exit(2);
			}
			if (debug)
				cout << "pt = " << Coord{x, y}.str() << endl;
			points.insert({x, y});
		}
		else
		{
			size_t idx = line.find("=");
			if (idx == string::npos)
			{
				cout << "ERROR: Unexpected line format [2]: " << line << endl;
				exit(2);
			}

			char xy = line[idx - 1];
			int v = stoi(line.substr(idx + 1));

			if (debug)
				cout << "fold inst = " << xy << ", " << v << endl;
			fold(points, xy, v, debug);
			if (count1 < 0)
			{
				count1 = (int) points.size();
			}
		}
	}

	cout << "Part 1 | Count = " << count1 << endl;
	cout << "Part 2 | Printout: " << endl << endl;

	printMap(points);

	return {count1, count2};
}

void main()
{
	cout << DAY << endl;

	vector<tuple<string, BIGINT, BIGINT>> testData{
		{
			R"(
				6,10
				0,14
				9,10
				0,3
				10,4
				4,11
				6,0
				6,12
				4,1
				0,13
				10,12
				3,4
				3,0
				8,4
				1,10
				2,14
				8,10
				9,0

				fold along y=7
				fold along x=5
			)",
			17,
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
