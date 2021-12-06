#include "Day06.h"
#include <tuple>
#include <map>

using namespace std;

typedef unsigned long long BIGINT;

bool contains(const vector<int>& vect, int num) {
	for (int i = 0; i < vect.size(); i++)
		if (vect[i] == num) return true;
	return false;
}

string str(vector<int>& vect) {
	stringstream ss;
	for each (int i in vect)
		ss << i << ",";
	return ss.str();
}

struct CacheKey {
	int fishState;
	int steps;

	string str() {
		char buff[40];
		snprintf(buff, sizeof(buff), "key[%i,%i]", fishState, steps);
		return string{ buff };
	}
};

bool operator < (const CacheKey& one, const CacheKey& two) {
	return std::tie(one.fishState, one.steps) < std::tie(two.fishState, two.steps);
}

map<CacheKey, BIGINT> CACHE;

vector<int> simFish(int& fishState, int steps, bool debug) {
	vector<int> offsprings;
	if (fishState >= steps) {
		// state 3, step 2 -> state 1, step 0
		// state 3, step 3 -> state 0, step 0
		fishState -= steps;
		steps = 0;
	}
	else {
		// state 3, steps 4 -> state 6, step 0, offsprings = [8]
		// state 3, steps 5 -> state 5, step 0, offsprings = [7]

		steps = steps - fishState;
		fishState = 0;

		int remainder = steps % 7;
		fishState = (7 - remainder) % 7; // end-state for this fish.

		// for each cycle add one new offspring + its offsprings
		for (;steps > 0; steps -= 7) // NEEDS OPTIMIZATION
		{
			int oFishState = 7 + 2;
			vector<int> itsOffsprings = simFish(oFishState, steps, debug);

			offsprings.push_back(oFishState);
			for each (int oFo in itsOffsprings)
				offsprings.push_back(oFo);
		}
	}
	return offsprings;
}

BIGINT simFishCount(int fishState, int steps, bool debug) {
	if (fishState >= steps) {
		return 1; // no offsprings, just self
	}

	CacheKey key{ fishState, steps };
	auto it = CACHE.find(key);
	if (it != CACHE.end()) {
		return it->second;
	}

	BIGINT offsprings = 0;
	steps = steps - fishState;
	// for each cycle add one new offspring + its offsprings
	for (; steps > 0; steps -= 7)
		offsprings += simFishCount(8, steps-1, debug);

	BIGINT val = 1 + offsprings;
	CACHE[key] = val;
	return val;
}

vector<int> simSchool(vector<int> school, int steps, bool debug) {
	vector<int> newSchool;
	
	vector<vector<int>> allOffsprings;
	for each (int fish in school)
	{
		vector<int> offsprings = simFish(fish, steps, debug);
		newSchool.push_back(fish);
		allOffsprings.push_back(offsprings);
	}
	for each (vector<int> vect in allOffsprings)
		for each (int offspring in vect)
			newSchool.push_back(offspring);
	return newSchool;
}

BIGINT simSchoolCount(vector<int> school, int steps, bool debug) {
	BIGINT newSchoolCount = 0;
	for each (int fish in school)
	{
		newSchoolCount += simFishCount(fish, steps, debug);
	}
	return newSchoolCount;
}

void process(istream& ss, bool debug) {
	vector<int> school;
	
	for (int i; ss >> i;) {
		school.push_back(i);
		if (ss.peek() == ',')
			ss.ignore();
	}

	if (debug) {
		cout << "State 0: " << str(school) << endl;
		for (int steps = 1; steps <= 18; steps++)
		{
			cout << "State " << steps << ": " << str(simSchool(school, steps, false)) << endl;
		}
		cout << "State " << 18 << ": " << simSchool(school, 18, false).size() << endl;
	}
	cout << "State " << 80 << ": " << simSchool(school, 80, false).size() << endl;
	cout << "State " << 256 << ": " << simSchoolCount(school, 256, false) << endl;
	
}

void main()
{
	string DAY("Day06");
	cout << DAY << endl;

	char testData[] = "3,4,3,1,2";

	stringstream testDataStream(testData);
	cout << "With Test Data .." << endl;
	process(testDataStream, true);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, false);
}
