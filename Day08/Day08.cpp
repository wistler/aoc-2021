#include "Day08.h"
#include <set>
#include <algorithm>

using namespace std;

set<char> getWires(string word) {
	set<char> wires;
	for each (char ch in word)
		wires.insert(ch);
	return wires;
}

void process(istream& ss, int debug) {
	int count = 0;
	string line, word;
	vector<tuple<string, set<char>>> digits;
	set<tuple<string, set<char>>> six_segments; // 0 6 9
	set<tuple<string, set<char>>> five_segments; // 2 3 5
	
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
					cout << "Digit 1: " << get<0>(digits[1]) << endl;
					cout << "Digit 4: " << get<0>(digits[4]) << endl;
					cout << "Digit 7: " << get<0>(digits[7]) << endl;
					cout << "Digit 8: " << get<0>(digits[8]) << endl;
				}
				auto wires_seven = get<1>(digits[7]);
				auto wires_one = get<1>(digits[1]);
				set<char> c;
				set_difference(wires_seven.begin(), wires_seven.end(), wires_one.begin(), wires_one.end(), inserter(c, c.end()));
				if (c.size() != 1) {
					throw invalid_argument("Digits are not compatible");
				}
				const char wires_top = (*c.begin());
				if (debug > 1) {
					cout << "Wire TOP: " << wires_top << endl;
				}
				set<char> wires_right;
				set_intersection(wires_seven.begin(), wires_seven.end(), wires_one.begin(), wires_one.end(), inserter(wires_right, wires_right.end()));
				if (debug > 1) {
					cout << "Wires on RIGHT: ";
					for (char ch : wires_right) cout << ch << " "; cout << endl;
				}
				for each (auto var in six_segments)
				{
					set<char> tmp;
					set<char> wires_var = get<1>(var);
					set_intersection(wires_right.begin(), wires_right.end(), wires_var.begin(), wires_var.end(), inserter(tmp, tmp.end()));
					if (tmp.size() != wires_right.size()) {
						digits[6] = var;
						if (debug > 1) cout << "Digit 6: " << get<0>(digits[6]) << endl;
						break;
					}
				}
				six_segments.erase(six_segments.find(digits[6]));
				for each (auto var in five_segments)
				{
					set<char> tmp;
					set<char> wires_var = get<1>(var);
					set_intersection(
						wires_right.begin(), wires_right.end(), 
						wires_var.begin(), wires_var.end(), 
						inserter(tmp, tmp.end())
					);
					if (tmp.size() == wires_right.size()) {
						digits[3] = var;
						if (debug > 1) cout << "Digit 3: " << get<0>(digits[3]) << endl;
						break;
					}
				}
				five_segments.erase(five_segments.find(digits[3]));
				// 2 5 0 9 remaining
				set<char> wires_left;
				auto wires_six = get<1>(digits[6]);
				auto wires_three = get<1>(digits[3]);
				set_difference(wires_six.begin(), wires_six.end(), wires_three.begin(), wires_three.end(), inserter(wires_left, wires_left.end()));
				if (debug > 1) {
					cout << "Wires on LEFT: ";
					for (char ch : wires_left) cout << ch << " "; cout << endl;
				}
				for each (auto var in six_segments)
				{
					set<char> tmp;
					set<char> wires_var = get<1>(var);
					set_intersection(wires_left.begin(), wires_left.end(), wires_var.begin(), wires_var.end(), inserter(tmp, tmp.end()));
					if (tmp.size() != wires_left.size()) {
						digits[9] = var;
						if (debug > 1) cout << "Digit 9: " << get<0>(digits[9]) << endl;
					}
					if (tmp.size() == wires_left.size()) {
						digits[0] = var;
						if (debug > 1) cout << "Digit 0: " << get<0>(digits[0]) << endl;
					}
				}
				six_segments.clear();
				auto wires_nine = get<1>(digits[9]);
				for each (auto var in five_segments)
				{
					set<char> tmp;
					set<char> wires_var = get<1>(var);
					set_intersection(
						wires_nine.begin(), wires_nine.end(),
						wires_var.begin(), wires_var.end(),
						inserter(tmp, tmp.end())
					);
					if (tmp.size() == 5) {
						digits[5] = var;
						if (debug > 1) cout << "Digit 5: " << get<0>(digits[5]) << endl;
					} else {
						digits[2] = var;
						if (debug > 1) cout << "Digit 2: " << get<0>(digits[2]) << endl;
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
				{
					if (get<1>(digits[d]) == getWires(word)) {
						digit = d;
					}
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
