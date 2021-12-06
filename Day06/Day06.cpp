#include "Day06.h"
#include <tuple>
#include <map>

using namespace std;

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

struct CacheValue {
	int fishState;
	vector<int> offsprings;

	string str() {
		char buff[200];
		snprintf(buff, sizeof(buff), "value[%i,%s]", fishState, ::str(offsprings).c_str());
		return string{ buff };
	}
};

bool operator < (const CacheKey& one, const CacheKey& two) {
	return std::tie(one.fishState, one.steps) < std::tie(two.fishState, two.steps);
}

map<CacheKey, CacheValue> CACHE;

vector<int> simFish(int& fishState, int steps, bool debug) {
	
	
	CacheKey key{ fishState, steps };
	map<CacheKey, CacheValue>::iterator it;
	if ((it = CACHE.find(key)) != CACHE.end()) {
		CacheValue ans = it->second;
		// if (debug) cout << "simFish(): CACHED: " << key.str() << " -> " << it->second.str() << endl;
		fishState = ans.fishState;
		return ans.offsprings;
	}

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

		//int cycles = steps / 7;
		int remainder = steps % 7;
		fishState = (7 - remainder) % 7; // end-state for this fish.

		// for each cycle add one new offspring + its offsprings
		for (;steps > 0; steps -= 7)
		{
			int oFishState = 8;
			vector<int> itsOffsprings = simFish(oFishState, steps-1, debug);

			offsprings.push_back(oFishState);
			for each (int oFo in itsOffsprings)
				offsprings.push_back(oFo);
		}		
	}
	CacheValue val = CacheValue{ fishState, offsprings };
	if (debug) cout << "simFish(): -caching-: " << key.str() << endl; // << " -> " << val.str() << endl;
	CACHE[key] = val;
	return offsprings;
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
		//cout << "State " << 7 << ": " << str(simSchool(school, 7, true)) << endl;
		//cout << "State " << 8 << ": " << str(simSchool(school, 8, true)) << endl;
		cout << "State " << 18 << ": " << simSchool(school, 18, false).size() << endl;
	}
	cout << "State " << 80 << ": " << simSchool(school, 80, false).size() << endl;
	
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
