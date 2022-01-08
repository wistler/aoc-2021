#include "Day16.h"
#include <iomanip>

using namespace std;

typedef BigInt BIGINT;

const string DAY("Day16");

struct Packet {
	int ver;
	int typeID;

	int size = 0;
	virtual string str() = 0;
	virtual string disp() = 0;
};

struct Literal : Packet {
	BIGINT value;
	//Literal(int ver, int typeID): Packet(ver, typeID) {}

	string str() {
		stringstream ss;
		ss << "LP[ver: " << ver << ", type: " << typeID << "]{";
		ss << value;
		ss << "}";
		return ss.str();
	}

	string disp() {
		stringstream ss;
		ss << value;
		return ss.str();
	}
};

struct Operator : Packet {
	vector<Packet*> subpackets;
	//Operator(int ver, int typeID): Packet(ver, typeID) {}

	string str() {
		stringstream ss;
		ss << "OP[ver: " << ver << ", type: " << typeID << "]{";
		for each (Packet * sp in subpackets)
			ss << sp->str();
		ss << "}";
		return ss.str();
	}

	string disp() {
		stringstream ss;
		switch (typeID)
		{
		case 0: // sum
			ss << "(";
			for each (Packet * sp in subpackets) {
				ss << sp->disp() << (subpackets.back() == sp ? "" : " + ");
			}
			ss << ")";
			break;
		case 1: // prod
			ss << "(";
			for each (Packet * sp in subpackets) {
				ss << sp->disp() << (subpackets.back() == sp ? "" : " x ");
			}
			ss << ")";
			break;
		case 2: // min
			ss << "min(";
			for each (Packet * sp in subpackets) {
				ss << sp->disp() << (subpackets.back() == sp ? "" : ", ");
			}
			ss << ")";
			break;
		case 3: // max
			ss << "max(";
			for each (Packet * sp in subpackets) {
				ss << sp->disp() << (subpackets.back() == sp ? "" : ", ");
			}
			ss << ")";
			break;
		case 5: // >
			ss << "[" 
			   << subpackets[0]->disp() << " > " 
			   << subpackets[1]->disp() 
			   << "]";
			break;
		case 6: // <
			ss << "["
				<< subpackets[0]->disp() << " < "
				<< subpackets[1]->disp()
				<< "]";
			break;
		case 7: // ==
			ss << "["
				<< subpackets[0]->disp() << " == "
				<< subpackets[1]->disp()
				<< "]";
			break;
		}
		return ss.str();
	}
};

void readBits(std::istream& binary, const int n, std::stringstream& buff)
{
	for (int i = 0; i < n; i++)
	{
		char bit;
		binary >> bit;
		buff << bit;
	}
}

Packet* readPacket(istream& binary, int debug) {
	char ver[4] = { '\0' };
	char tID[4] = { '\0' };
	Packet* p;

	if (debug)
		cout << "Reading packet from pos: " << binary.tellg() << endl;

	binary >> ver[0] >> ver[1] >> ver[2];
	binary >> tID[0] >> tID[1] >> tID[2];

	if (debug)
		cout << "ver: " << ver << ", tID:" << tID << endl;

	int iver = bin2dec(ver);
	int typeID = bin2dec(tID);
	const int ENVELOP = 6; // envelope

	if (typeID == 4) {
		if (debug > 1)
			cout << "literal value" << endl;

		Literal* lp = new Literal(); // TODO: How to initialize Literal with Packet p ?
		lp->ver = bin2dec(ver);
		lp->typeID = bin2dec(tID);
		lp->size = ENVELOP;

		char bit;
		stringstream buff;

		while (true) {
			binary >> bit;
			readBits(binary, 4, buff);
			lp->size += 1 + 4;
			if (bit == '0') {
				break;
			}
		}
		lp->value = bin2dec(buff.str());

		p = lp;
	}
	else {
		if (debug > 1)
			cout << "operator" << endl;

		Operator* op = new Operator{};
		op->ver = bin2dec(ver);
		op->typeID = bin2dec(tID);
		op->size = ENVELOP;

		char bit;
		binary >> bit;
		op->size += 1;

		stringstream buff;

		if (bit == '0') {

			readBits(binary, 15, buff);
			op->size += 15;
			int subPacketLength = bin2dec(buff.str());
			if (debug)
				cout << "subpacket length LIMIT: " << subPacketLength << endl;

			while (subPacketLength > 0) {
				Packet* sp = readPacket(binary, debug);
				op->subpackets.push_back(sp);
				op->size += sp->size;
				subPacketLength -= sp->size;
			}

		}
		else { // bit == '1'

			readBits(binary, 11, buff);
			op->size += 11;
			int subPacketCount = bin2dec(buff.str());
			if (debug)
				cout << "subpacket count LIMIT: " << subPacketCount << endl;

			while (subPacketCount > 0) {
				Packet* sp = readPacket(binary, debug);
				op->subpackets.push_back(sp);
				op->size += sp->size;
				subPacketCount--;
			}
		}

		p = op;
	}

	if (debug) {
		cout << p->str()
			<< "(size " << p->size << ")" << endl;
	}
	return p;
}

int sumOfVers(Packet* p) {
	int sum = p->ver;
	if (p->typeID != 4) { // operator
		Operator* op = (Operator*)p;
		for each (Packet * sp in op->subpackets)
		{
			sum += sumOfVers(sp);
		}
	}
	return sum;
}

BIGINT eval(Packet* p, int debug) {
	if (p->typeID == 4) { // operator
		return ((Literal*)p)->value;
	}

	Operator* op = (Operator*)p;

	switch (p->typeID)
	{
	case 0: // sum
	{
		BIGINT value = "0";
		stringstream dss;
		for each (Packet * sp in op->subpackets) {
			BIGINT v = eval(sp, debug);
			dss << v;
			if (value > value + v) {
				dss << "[OvF]";
			}
			dss << ", ";
			value += v;
		}
		if (debug) cout << "SUM (" << dss.str() << ") = " << value << endl;
		if (value < "0") exit(1);
		return value;
	}

	case 1: // prod
	{
		BIGINT value = 1;
		stringstream dss;
		for each (Packet * sp in op->subpackets) {
			BIGINT v = eval(sp, debug);
			dss << v;
			if (v != "0" && value > value * v) {
				dss << "[OvF]";
			}
			dss << ", ";
			value *= v;
		}
		if (debug) cout << "PROD (" << dss.str() << ") = " << value << endl;
		if (value < "0") exit(1);
		return value;
	}

	case 2: // min
	{
		BIGINT value = "0"; bool init = false;
		stringstream dss;
		for each (Packet * sp in op->subpackets) {
			BIGINT v = eval(sp, debug);
			dss << v;
			if (!init || value > v)
				value = v;
			dss << ", ";
			init = true;
		}
		if (debug) cout << "MIN (" << dss.str() << ") = " << value << endl;
		if (value < "0") exit(1);
		return value;
	}

	case 3: // max
	{
		BIGINT value = "0"; bool init = false;
		stringstream dss;
		for each (Packet * sp in op->subpackets) {
			BIGINT v = eval(sp, debug);
			dss << v;
			if (!init || value < v)
				value = v;
			dss << ", ";
			init = true;
		}
		if (debug) cout << "MAX (" << dss.str() << ") = " << value << endl;
		if (value < "0") exit(1);
		return value;
	}

	case 5: // >
	{
		auto v1 = eval(op->subpackets[0], debug);
		auto v2 = eval(op->subpackets[1], debug);
		auto ret = v1 > v2 ? 1 : 0;
		if (debug) cout << v1 << " > " << v2 << " = " << ret << endl;
		return ret;
	}
	case 6: // <
	{
		auto v1 = eval(op->subpackets[0], debug);
		auto v2 = eval(op->subpackets[1], debug);
		auto ret = v1 < v2 ? 1 : 0;
		if (debug) cout << v1 << " < " << v2 << " = " << ret << endl;
		return ret;
	}
	case 7: // ==
	{
		auto v1 = eval(op->subpackets[0], debug);
		auto v2 = eval(op->subpackets[1], debug);
		auto ret = v1 == v2 ? 1 : 0;
		if (debug) cout << v1 << " == " << v2 << " = " << ret << endl;
		return ret;
	}
	}

	exit(2);
}

tuple<BIGINT, BIGINT> process(istream& ss, int debug)
{
	BIGINT count1 = "0", count2 = "0";

	stringstream binary;

	for (char ch; ss >> ch;) {
		if (ch == '\n') continue;
		if (debug > 1) {
			cout << ch << " -> " << hexToBin(ch) << endl;
		}
		binary << hexToBin(ch);
	}

	if (debug) {
		cout << "binary: " << binary.str() << endl;
	}

	Packet* p = readPacket(binary, debug);

	cout << "FULL PACKET: " << p->disp() << endl;

	count1 = sumOfVers(p);

	cout << "Part 1 | Sum of versions = " << count1 << endl;

	count2 = eval(p, debug);

	cout << "Part 2 | Eval = " << count2 << endl;

	return { count1, count2 };
}

void main()
{
	cout << DAY << endl;

	vector<tuple<string, int, int>> testData{
		{ "D2FE28", 6, 2021 },
		{ "38006F45291200", 9, -1 },
		{ "8A004A801A8002F478", 16, -1 },
		{ "620080001611562C8802118E34", 12, -1 },
		{ "C0015000016115A2E0802F182340", 23, -1 },
		{ "A0016C880162017C3686B18A3D4780", 31, -1 },

		{ "C200B40A82", -1, 3 },
		{ "04005AC33890", -1, 54 },
		{ "880086C3E88112", -1, 7 },
		{ "CE00C43D881120", -1, 9 },
		{ "D8005AC2A8F0", -1, 1 },
		{ "F600BC2D8F", -1, 0 },
		{ "9C005AC2F8F0", -1, 0 },
		{ "9C0141080250320F1802104A08", -1, 1 },
	};

	for (auto it = testData.begin(); it != testData.end(); it++) {
		string input = get<0>(*it);
		BIGINT part1exp = get<1>(*it);
		BIGINT part2exp = get<2>(*it);

		stringstream testDataStream(input);
		tuple<BIGINT, BIGINT> ans;
		bool failed = false;
		try {
			ans = process(testDataStream, false);
		}
		catch (...) {
			failed = true;
		}
		if (failed || (part1exp != -1 && get<0>(ans) != part1exp) || (part2exp != -1 && get<1>(ans) != part2exp))
		{
			cout << "TEST DATA: " << input << endl;
			resetStream(testDataStream);
			process(testDataStream, 2);
			cout << "ERROR: Part1 ans: Expected " << part1exp << ", got " << get<0>(ans) << endl;
			cout << "ERROR: Part2 ans: Expected " << part2exp << ", got " << get<1>(ans) << endl;
			exit(1);
		}
	}

	exit(1);
	cout << endl
		<< "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	process(input, 1);
}
