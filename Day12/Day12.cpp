#include "Day12.h"
#include <iomanip>

using namespace std;

struct GraphNode
{
	string name;

	GraphNode *prev;
	vector<GraphNode *> next;
};

bool isBig(string cave)
{
	return isupper(cave[0]);
}

string pathTo(GraphNode *fromCave)
{
	string path = fromCave->name;
	while (fromCave->prev)
	{
		fromCave = fromCave->prev;
		path = fromCave->name + "," + path;
	}
	return path;
}

bool canVisitPart1(GraphNode *fromCave, string next, int debug)
{
	if (isBig(next))
		return true;

	// look back
	while (fromCave->prev)
	{
		fromCave = fromCave->prev;
		if (fromCave->name == next)
			return false;
	}
	return true;
}

bool canVisitPart2(GraphNode *fromCave, string next, int debug)
{
	if (isBig(next))
		return true;

	if (next == "start")
		return false;

	if (next == "end")
		return true;

	set<string> history;
	bool smallCaveRevisitRestricted = false;

	do
	{
		if (isBig(fromCave->name))
			continue;

		if (history.find(fromCave->name) != history.end())
			smallCaveRevisitRestricted = true;

		history.insert(fromCave->name);

	} while (fromCave = fromCave->prev);

	if (history.find(next) != history.end())
		return !smallCaveRevisitRestricted;

	return true;
}

template <typename F1>
int traverse(map<string, set<string>> &caveSystem, GraphNode *fromCave, F1 visitor, int debug)
{
	int pathsToEnd = 0;
	auto connections = caveSystem[fromCave->name];
	for each (auto conn in connections)
	{
		if (visitor(fromCave, conn, debug))
		{
			GraphNode *n = new GraphNode{conn, fromCave};
			fromCave->next.push_back(n);
			if (debug > 1)
			{
				cout << fromCave->name << " -> " << conn << endl;
			}
			if (conn != "end")
			{
				pathsToEnd += traverse(caveSystem, n, visitor, debug);
			}
			else
			{
				pathsToEnd++;
				if (debug)
				{
					cout << "FOUND: " << pathTo(n) << endl;
				}
			}
		}
	}
	return pathsToEnd;
}

int release(GraphNode* node) {
	int nodeCount = 1;
	for (GraphNode* child : node->next) {
		nodeCount += release(child);
	}
	delete node;
	return nodeCount;
}

void process(istream &ss, int debug)
{
	map<string, set<string>> caveSystem;

	string line;
	int x = 0, y = 0;
	while (getline(ss, line))
	{
		line = trim(line);
		if (line.size() == 0)
			continue;

		string a = line.substr(0, line.find('-'));
		string b = line.substr(line.find('-') + 1);

		caveSystem[a].insert(b);
		caveSystem[b].insert(a);

		if (debug > 1)
			cout << "[" << a << (isBig(a) ? "*" : "") << "] <-> [" << b << (isBig(b) ? "*" : "") << "]" << endl;
	}

	if (debug > 1)
		for each (auto it in caveSystem)
		{
			cout << "Cave " << it.first << ": ";
			for each (auto ita in it.second)
				cout << ita << ",";
			cout << endl;
		}

	GraphNode* root = new GraphNode{"start", NULL};
	int paths1 = traverse(caveSystem, root, canVisitPart1, debug);
	cout << "Part 1 | Count = " << paths1 << endl;
	cout << "dbg    | Released nodes: " << release(root) << endl;

	GraphNode* root2 = new GraphNode{"start", NULL};
	int paths2 = traverse(caveSystem, root2, canVisitPart2, debug);
	cout << "Part 2 | Count = " << paths2 << endl;
	cout << "dbg    | Released nodes: " << release(root2) << endl;
}

void main()
{
	string DAY("Day12");
	cout << DAY << endl;

	char testData1[] = R"(
		start-A
		start-b
		A-c
		A-b
		b-d
		A-end
		b-end
	)";

	stringstream testDataStream1(testData1);
	cout << "With Test Data #1.." << endl;
	process(testDataStream1, 2);

	char testData2[] = R"(
		dc-end
		HN-start
		start-kj
		dc-start
		dc-HN
		LN-dc
		HN-end
		kj-sa
		kj-HN
		kj-dc
	)";

	stringstream testDataStream2(testData2);
	cout << "With Test Data #2.." << endl;
	process(testDataStream2, 1);

	char testData3[] = R"(
		fs-end
		he-DX
		fs-he
		start-DX
		pj-DX
		end-zg
		zg-sl
		zg-pj
		pj-he
		RW-he
		fs-DX
		pj-RW
		zg-RW
		start-pj
		he-WI
		zg-he
		pj-fs
		start-RW
	)";

	stringstream testDataStream3(testData3);
	cout << "With Test Data #3.." << endl;
	process(testDataStream3, false);

	cout << endl
		 << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, false);
}
