#include <direct.h> // _getcwd
#include <fstream>

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
	if (!input) {
		cout << "ERROR: Could not open input: " << f << endl
			 << "PWD:" << get_working_path();
		exit(1);
	}
	return input;
}