#include "Day03.h"

using namespace std;

void calculatePowerConsumption(istream &ss) {
	string word;
	
	vector<int> sums;
	int wordCount = 0;

	while (ss >> word) {
		if (sums.size() != word.size()) {
			sums.resize(word.size());
		}
		for (short i = 0; i < word.size(); i++)
		{
			sums[i] += word[i] == '1';
		}
		wordCount++;
	}

	int gamma = 0, epsilon = 0;
	for (short i = 0; i < sums.size(); i++)
	{
		gamma <<= 1;
		epsilon <<= 1;

		gamma += sums[i] >= wordCount / 2;
		epsilon += sums[i] < wordCount / 2;
	}
	
	cout << "Part 1 | Gamma Rt.   = " << gamma << " \t| 0b" << bin(gamma, sums.size()) << endl
		 << "       | Epsilon Rt. = " << epsilon << " \t| 0b" << bin(epsilon, sums.size()) << endl
		 << "       | Power Cons. = " << gamma * epsilon << endl;
}

void calculateLifeSupportRate(istream& ss) {
	string word;
	string mcbFilter = "";
	string lcbFilter = "";

	const int resetPosition = ss.tellg();
	while (true) {
		ss.clear();
		ss.seekg(resetPosition);

		int sum1 = 0, sum2 = 0;
		int count1 = 0, count2 = 0;
		int bit = mcbFilter.size();
		// cout << "Bit: " << bit << endl;
		while (ss >> word) {
			if (word.rfind(mcbFilter, 0) == 0) {
				sum1 += word[bit] == '1';
				count1++;
			}
			if (word.rfind(lcbFilter, 0) == 0) {
				sum2 += word[bit] == '1';
				count2++;
			}
		}
		// cout << "MCB: " << sum1 << " of " << count1 << " -> " << "01"[sum1 >= count1 / 2.0] << endl;
		// cout << "LCB: " << sum2 << " of " << count2 << " -> " << "01"[sum2 < count2 / 2.0] << endl;
		mcbFilter += "01"[count1 == 1 ? sum1 : sum1 >= count1 / 2.0];
		lcbFilter += "01"[count2 == 1 ? sum2 : sum2 < count2 / 2.0];

		if (mcbFilter.size() == word.size())
			break;
	}

	int o2 = bin2dec(mcbFilter);
	int co2 = bin2dec(lcbFilter);
	cout << "Part 2 | O2 Gen Rate    = " << o2<< " \t| 0b" << mcbFilter << endl
		 << "       | CO2 Scrub Rate = " << co2 << " \t| 0b" << lcbFilter << endl
		 << "       | Life Sup. Rate = " << o2 * co2 << endl;
}

void main()
{
	string DAY("Day03");
	cout << DAY << endl;

	char testData[] = R"(
		00100
		11110
		10110
		10111
		10101
		01111
		00111
		11100
		10000
		11001
		00010
		01010
	)";

	stringstream testDataStream(testData);
	cout << "With Test Data .." << endl;
	calculatePowerConsumption(testDataStream);
	resetStream(testDataStream);
	calculateLifeSupportRate(testDataStream);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	calculatePowerConsumption(input);
	resetStream(input);
	calculateLifeSupportRate(input);
}
