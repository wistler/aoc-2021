#include "Day04.h"

using namespace std;

bool findInBoard(const vector<int>& board, int number, int& r, int& c) {
	for (r = 0; r < 5; r++) {
		for (c = 0; c < 5; c++) {
			int i = r * 5 + c;
			if (board[i] == number) {
				return true;
			}
		}
	}
	return false;
}

bool contains(const vector<int>& vect, int num) {
	for (int i = 0; i < vect.size(); i++)
		if (vect[i] == num) return true;
	return false;
}

int getBingoScore(const vector<int> &drawOrder, const vector<int> &board, int &turns) {
	vector<int> markedCols = { 0,0,0,0,0 };
	vector<int> markedRows = { 0,0,0,0,0 };
	
	for (turns = 0; turns < drawOrder.size(); turns++)
	{
		int row, col;
		const int drawnNumber = drawOrder[turns];
		if (findInBoard(board, drawnNumber, row, col)) {
			markedRows[row]++;
			markedCols[col]++;

			if (markedCols[col] == 5 || markedRows[row] == 5) {
				// calculate score
				vector<int> drawn{ drawOrder };
				drawn.resize(turns+1);

				int sum = 0;
				for each (int number in board)
				{
					if (!contains(drawn, number)) {
						sum += number;
					}
				}
				// cout << "Sum: " << sum << ", drawnNumber: " << drawnNumber << endl;
				return sum * drawnNumber;
			}
		}
	}
	return 0;
}

void parseInput(istream& ss) {
	string drawOrderLine = "";
	while (trim(drawOrderLine).size() == 0) {
		getline(ss, drawOrderLine);
	}

	vector<int> drawOrder;
	stringstream drawOrderSS(drawOrderLine);
	for (int i; drawOrderSS >> i;) {
		drawOrder.push_back(i);
		if (drawOrderSS.peek() == ',')
			drawOrderSS.ignore();
	}

	vector<vector<int>> boards;
	while(true)
	{
		vector<int> board;
		bool badBoard = false;
		board.resize(25);
		for (int i = 0; i < 25; i++)
		{
			ss >> board[i];
			if (i > 0 && board[i - 1] == 0 && board[i] == 0) {
				badBoard = true;
				break;
			}
		}
		if (badBoard) break;
		boards.push_back(board);
	}

	cout << "Number of boards: " << boards.size() << endl;
	int minTurns{-1}, firstWinningBoard{0}, firstWinningScore{0};
	int maxTurns{-1}, lastWinningBoard{0}, lastWinningScore{0};
	for (int i = 0; i < boards.size(); i++)
	{
		int turns = 0;
		int score = getBingoScore(drawOrder, boards[i], turns);
		cout << "Board " << i + 1 << " scores " << score << " in " << turns + 1 << " turns." << endl;
		if (minTurns == -1 || turns < minTurns) {
			minTurns = turns;
			firstWinningBoard = i + 1;
			firstWinningScore = score;
		}
		if (maxTurns == -1 || turns > maxTurns) {
			maxTurns = turns;
			lastWinningBoard = i + 1;
			lastWinningScore = score;
		}
	}
	cout << "FIRST Winning board is: " << firstWinningBoard << ", with a score of " << firstWinningScore << endl;
	cout << "LAST  Winning board is: " << lastWinningBoard << ", with a score of " << lastWinningScore << endl;
}

void main()
{
	string DAY("Day04");
	cout << DAY << endl;

	char testData[] = R"(
		7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

		22 13 17 11  0
		 8  2 23  4 24
		21  9 14 16  7
		 6 10  3 18  5
		 1 12 20 15 19

		 3 15  0  2 22
		 9 18 13 17  5
		19  8  7 25 23
		20 11 10 24  4
		14 21 16 12  6

		14 21 17 24  4
		10 16 15  9 19
		18  8 23 26 20
		22 11 13  6  5
		 2  0 12  3  7
	)";

	stringstream testDataStream(testData);
	cout << "With Test Data .." << endl;
	parseInput(testDataStream);

	cout << endl << "With Input Data .." << endl;
	ifstream input = get_input_file(DAY + "/input.txt");
	parseInput(input);
}
