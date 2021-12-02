#include "Day02.h"

using namespace std;

void runPart1(istream &ss) {
	string dir;
	int dist;
	int horTotal = 0, depth = 0;
	while (ss >> dir >> dist) {
		if (dir == "forward") {
			horTotal += dist;
		} else if (dir == "down") {
			depth += dist;
		} else if (dir == "up") {
			depth -= dist;
		}
	}

	cout << "Part 1 | Horizontal Distance = " << horTotal << endl
		 << "       | Depth = " << depth << endl
		 << "       | Product = " << horTotal * depth << endl;
}

void runPart2(istream& ss) {
	string dir;
	int dist;
	int horTotal = 0, depth = 0, aim = 0;
	while (ss >> dir >> dist) {
		if (dir == "forward") {
			horTotal += dist;
			depth += aim * dist;
		}
		else if (dir == "down") {
			aim += dist;
		}
		else if (dir == "up") {
			aim -= dist;
		}
	}

	cout << "Part 2 | Horizontal Distance = " << horTotal << endl
		 << "       | Depth = " << depth << endl
		 << "       | Product = " << horTotal * depth << endl;
}

void main()
{
	string DAY("Day02");
	cout << DAY << endl;

	char testData[] = R"(
		forward 5
		down 5
		forward 8
		up 3
		down 8
		forward 2
	)";

	stringstream testDataStream(testData);
	cout << "With Test Data .." << endl;
	runPart1(testDataStream);
	resetStream(testDataStream);
	runPart2(testDataStream);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	runPart1(input);
	resetStream(input);
	runPart2(input);
}
