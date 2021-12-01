// AoC2021.cpp : Defines the entry point for the application.
//

#include "Day01.h"

using namespace std;

void runPart1(istream &ss) {
	int depth, lastDepth = 0;
	int increments = 0;
	while (ss >> depth) {
		if (lastDepth && depth && depth > lastDepth) {
			increments++;
		}
		lastDepth = depth;
	}

	cout << "Part 1 | Increments = " << increments << endl;
}

void runPart2(istream& ss) {
	int depth;
	int lastDepths[3] = { 0, 0, 0};
	int increments = 0;
	while (ss >> depth) {
		int sumLastDepths = 0;
		int sumCurrentDepths = 0;
		if (lastDepths[0] && lastDepths[1] && lastDepths[2]) {
			sumLastDepths = lastDepths[0] + lastDepths[1] + lastDepths[2];
		}
		if (lastDepths[0] && lastDepths[1]) {
			sumCurrentDepths = depth + lastDepths[0] + lastDepths[1];
		}
		if (sumLastDepths && sumCurrentDepths && sumCurrentDepths > sumLastDepths) {
			increments++;
		}

		lastDepths[2] = lastDepths[1];
		lastDepths[1] = lastDepths[0];
		lastDepths[0] = depth;
	}

	cout << "Part 2 | Increments = " << increments << endl;
}

void main()
{
	char testData[] = R"(
		199
		200
		208
		210
		200
		207
		240
		269
		260
		263
	)";

	stringstream testDataStream(testData);
	cout << "With Test Data .." << endl;
	runPart1(testDataStream);
	resetStream(testDataStream);
	runPart2(testDataStream);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file("Day01/input.txt");
	runPart1(input);
	resetStream(input);
	runPart2(input);
}
