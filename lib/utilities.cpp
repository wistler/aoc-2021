#include <direct.h> // _getcwd
#include <fstream>
#include <tuple>

using namespace std;

string get_working_path()
{
	char temp[256];
	return (_getcwd(temp, sizeof(temp)) ? string(temp) : string(""));
}

void resetStream(istream& s) {
	s.clear();
	s.seekg(0);
}

ifstream get_input_file(const string& f) {
	
	string PROJECT_DIR = "../../../../";

	ifstream input(PROJECT_DIR + f);
	ifstream input_alt(PROJECT_DIR + "../" + f);
	if (!input && !input_alt) {
		cout << "ERROR: Could not open input: " << f << endl
			 << "PWD:" << get_working_path();
		exit(1);
	}
	if (input) return input;
	return input_alt;
}

struct Coord {
	int x, y;
	string str() {
		char buff[40];
		snprintf(buff, sizeof(buff), "%i,%i", x, y);
		return string{ buff };
	}
};

bool operator < (const Coord& c1, const Coord& c2)
{
	return std::tie(c1.x, c1.y) < std::tie(c2.x, c2.y);
};

Coord operator + (const Coord& c1, const Coord& c2)
{
	return { c1.x + c2.x, c1.y + c2.y };
};

Coord operator * (const Coord& c1, int scale)
{
	return { c1.x * scale, c1.y * scale };
};

auto getNeighborsNoDiags(Coord point) {
	vector<Coord> neighbors{
		point + Coord{-1,0},
		point + Coord{+1,0},
		point + Coord{0,-1},
		point + Coord{0,+1},
	};
	return neighbors;
}

auto getNeighbors(Coord point) {
	vector<Coord> neighbors{
		point + Coord{-1,-1},
		point + Coord{ 0,-1},
		point + Coord{+1,-1},

		point + Coord{-1, 0},
		// point + Coord{ 0, 0},
		point + Coord{+1, 0},

		point + Coord{-1,+1},
		point + Coord{ 0,+1},
		point + Coord{+1,+1},
	};
	return neighbors;
}


template <typename T1>
void assertEqual(T1 left, T1 right, string message) {
	if (left != right) {
		stringstream ss;
		ss << "ASSERTION FAILED: " << left << " != " << right << "\n" << message;
		throw exception(ss.str().c_str());
	}
}

