#include <sstream>
#include <vector>

using namespace std;

const string WHITESPACE = " \n\r\t\f\v";

string ltrim(const string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == string::npos) ? "" : s.substr(start);
}

string rtrim(const string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string trim(const string& s) {
	return rtrim(ltrim(s));
}

string bin(long n, int w)
{
	string s = "0";
	for (long i = 1 << w; i > 0; i = i / 2)
	{
		s += "01"[(n & i) != 0];
	}
	return s;
}

int bin2dec(string word)
{
	int d = 0;
	for (int i = 0; i < word.size(); i++)
	{
		d = (d << 1) + (word[i] == '1');
	}
	return d;
}

template <typename V1>
string str(vector<V1>& vect, string sep) {
	stringstream ss;
	for each (V1 i in vect)
		ss << i << sep;
	return ss.str();
}

template <typename V1>
string str(vector<V1>& vect) { return str(vect, ","); }
