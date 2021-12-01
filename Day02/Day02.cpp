#include "Day02.h"

using namespace std;

void runPart1(istream &ss) {
	// TODO implement
	int num, sum = 0;	
	while (ss >> num) {
		sum += num;
	}

	cout << "Part 1 | Sum = " << sum << endl;
}

void runPart2(istream& ss) {
	// TODO implement
	int num, sum = 0;
	while (ss >> num) {
		sum += num;
	}

	cout << "Part 2 | Sum = " << sum << endl;
}

void main()
{
	string DAY("Day02");
	cout << DAY << endl;

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
	ifstream input = get_input_file(DAY + "/input.txt");
	runPart1(input);
	resetStream(input);
	runPart2(input);
}
