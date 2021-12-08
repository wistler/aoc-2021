#include "Day07.h"

using namespace std;

typedef unsigned long BIGINT;
typedef map<int, BIGINT> CacheType;
CacheType CACHE;

BIGINT sumOf(int n) {
	if (n <= 1) return n;
	
	CacheType::iterator it = CACHE.find(n);
	if (it != CACHE.end()) {
		return it->second;
	}

	BIGINT sum = n + sumOf(n - 1);
	CACHE[n] = sum;
	return sum;
}

template <typename F1>
tuple<int, int> minimize(F1 costFn, tuple<int, int> range, vector<int>& points, bool debug) {
	int leastCost = -1;
	int leastCostPos = -1;
	for (int p = get<0>(range); p <= get<1>(range); p++)
	{
		int cost = 0;
		stringstream dbg;
		dbg << " // ";
		for each (int pt in points) {
			auto c = costFn(pt, p);
			cost += c;
			if (debug) dbg << c << ",";
		}

		if (debug) cout << "dbg    | pos, cost   : " << p << "," << cost << dbg.str() << endl;
		if (leastCost == -1 || leastCost > cost) {
			leastCost = cost;
			leastCostPos = p;
		}
	}
	return {leastCost, leastCostPos};
}

void process(istream& ss, bool debug) {
	vector<int> crabs;
	for (int i; ss >> i;) {
		crabs.push_back(i);
		if (ss.peek() == ',')
			ss.ignore();
	}

	int minPos = -1, maxPos = -1;
	for each (int i in crabs) {
		if (minPos == -1 || minPos > i) minPos = i;
		if (maxPos == -1 || maxPos < i) maxPos = i;
	}
	tuple<int, int> range{ minPos, maxPos };
	if (debug) cout << "dbg    | minP, maxP  : " << minPos << "," << maxPos << endl;

	auto linearCostFn = [](int a, int b) { return abs(a - b); };
	auto crabCostFn = [](int a, int b) { return sumOf(abs(a - b)); };

	tuple<int,int> minCost = minimize(linearCostFn, range, crabs, debug);
	cout << "Part 1 | Cheapest LINEAR Cost, Pos: " << get<0>(minCost) << "," << get<1>(minCost) << endl << endl;
	minCost = minimize(crabCostFn, range, crabs, debug);
	cout << "Part 2 | Cheapest  CRAB  Cost, Pos: " << get<0>(minCost) << "," << get<1>(minCost) << endl;
}

void main()
{
	string DAY("Day07");
	cout << DAY << endl;

	char testData[] = "16,1,2,0,4,2,7,1,2,14";

	stringstream testDataStream(testData);
	cout << "With Test Data .." << endl;
	process(testDataStream, true);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, false);
}
