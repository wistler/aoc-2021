#include "Day08.h"

using namespace std;

set<char> getWires(string word) {
	set<char> wires;
	for each (char ch in word)
		wires.insert(ch);
	return wires;
}

template <typename S1>
set<S1> operator - (const set<S1>& one, const set<S1>& two) {
	set<S1> c;
	set_difference(one.begin(), one.end(), two.begin(), two.end(), inserter(c, c.end()));
	return c;
}

template <typename S1>
set<S1> operator | (const set<S1>& one, const set<S1>& two) {
	set<S1> c;
	set_intersection(one.begin(), one.end(), two.begin(), two.end(), inserter(c, c.end()));
	return c;
}

template <typename S1>
bool operator < (const set<S1>& subset, const set<S1>& super) {
	return ((super | subset) == subset);
}

void process(istream& ss, int debug) {
	int count = 0;
	string line, word;
	vector<tuple<string, set<char>>> digits;
	set<tuple<string, set<char>>> six_segments; // 0 6 9
	set<tuple<string, set<char>>> five_segments; // 2 3 5
	
	const auto wordOf = [&](tuple<string, set<char>> tup) { return get<0>(tup); };
	const auto wireOf = [&](tuple<string, set<char>> tup) { return get<1>(tup); };
	auto wireOfDigit = [&](int d) { return wireOf(digits[d]); };

	int sum = 0;
	while (getline(ss, line)) {
		digits.clear();
		digits.resize(10);

		six_segments.clear();
		five_segments.clear();

		stringstream ls(line);
		bool beforeBreak = true;
		int output = 0;

		if (debug == 1 && trim(line) != "") {
			cout << trim(line) << endl;
		}

		while (ls >> word) {
			if (word == "|") {
				beforeBreak = false;
				
				if (debug > 1) {
					cout << "Digit 1: " << wordOf(digits[1]) << endl;
					cout << "Digit 4: " << wordOf(digits[4]) << endl;
					cout << "Digit 7: " << wordOf(digits[7]) << endl;
					cout << "Digit 8: " << wordOf(digits[8]) << endl;
				}
				if ((wireOfDigit(7) - wireOfDigit(1)).size() != 1) {
					throw invalid_argument("Digits are not compatible: 1 & 7");
				}
				set<char> wires_right = wireOfDigit(7) | wireOfDigit(1);
				
				if (debug > 1) {
					cout << "Wires on RIGHT: ";
					for (char ch : wires_right) cout << ch << " "; cout << endl;
				}
				for each (auto var in six_segments)
				{
					if ((wires_right | wireOf(var)) != wires_right) {
						digits[6] = var;
						if (debug > 1) cout << "Digit 6: " << wordOf(digits[6]) << endl;
						break;
					}
				}
				six_segments.erase(six_segments.find(digits[6]));
				for each (auto var in five_segments)
				{
					if ((wires_right | wireOf(var)) == wires_right) {
						digits[3] = var;
						if (debug > 1) cout << "Digit 3: " << wordOf(digits[3]) << endl;
						break;
					}
				}
				five_segments.erase(five_segments.find(digits[3]));
				// 2 5 0 9 remaining
				
				auto wires_left = wireOfDigit(6) - wireOfDigit(3);
				if (debug > 1) {
					cout << "Wires on LEFT: ";
					for (char ch : wires_left) cout << ch << " "; cout << endl;
				}
				for each (auto var in six_segments)
				{
					auto tmp = wires_left | wireOf(var);
					if (wires_left < wireOf(var)) {
						digits[0] = var;
						if (debug > 1) cout << "Digit 0: " << wordOf(digits[0]) << endl;
					} else {
						digits[9] = var;
						if (debug > 1) cout << "Digit 9: " << wordOf(digits[9]) << endl;
					}
				}
				six_segments.clear();
				for each (auto var in five_segments)
				{
					if ((wireOfDigit(9) | wireOf(var)).size() == 5) {
						digits[5] = var;
						if (debug > 1) cout << "Digit 5: " << wordOf(digits[5]) << endl;
					} else {
						digits[2] = var;
						if (debug > 1) cout << "Digit 2: " << wordOf(digits[2]) << endl;
					}
				}
				five_segments.clear();

				continue;
			}
			const int size = word.size();
			if (beforeBreak) {
				auto tup = make_tuple(word, getWires(word));
				if (size == 2)
					digits[1] = tup;
				else if (size == 3)
					digits[7] = tup;
				else if (size == 7)
					digits[8] = tup;
				else if (size == 4)
					digits[4] = tup;
				else if (size == 6)
					six_segments.insert(tup);
				else if (size == 5)
					five_segments.insert(tup);
			}
			else {

				if (size == 2 || size == 3 || size == 4 || size == 7) {
					count++;
				}

				int digit = -1;
				for (int d = 0; d <= 9; d++)
					if (wireOfDigit(d) == getWires(word)) {
						digit = d;
						break;
					}
				if (digit < 0) throw invalid_argument("Digit not found for: " + word);
				if (debug) {
					cout << word << " -> " << digit << endl;
				}
				output = output * 10 + digit;
			}
		}
		sum += output;
	}
	cout << "Part 1 | Count = " << count << endl;
	cout << "Part 2 | Output Sum = " << sum << endl;
}

void main()
{
	string DAY("Day08");
	cout << DAY << endl;

	char testData1[] = R"(
		acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf
	)";

	stringstream testDataStream1(testData1);
	cout << "With Test Data #1.." << endl;
	process(testDataStream1, 2);

	char testData2[] = R"(
		be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
		edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
		fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
		fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
		aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
		fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
		dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
		bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
		egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
		gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce
	)";

	stringstream testDataStream2(testData2);
	cout << endl << "With Test Data #2.." << endl;
	process(testDataStream2, 1);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, 0);
}
