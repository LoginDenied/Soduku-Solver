#include <iostream>
#include <string>

using namespace std;

class cell
{
public:
	int value;
	int possibleValues[9];

	cell()
	{
		value = 0;
		for (int i = 0; i < 9; i++)
		{
			possibleValues[i] = i + 1;
		}
	}
};

cell gameBoard[9][9];
bool changesMade = false;
bool furtherIterate = false;

void greetUser()
{
	cout << "Hello, welcome to the sudoku solver" << endl;
	cout << "Please enter the number of cells that are known eg. A0" << endl;
}

void displayBoard()
{
	cout << "  ABC   DEF   GHI" << endl; //To output the column indicator

	for (int i = 0; i < 9; i++)
	{
		cout << i << " "; //To output the row indicator

		for (int j = 0; j < 9; j++)
		{
			cout << gameBoard[j][i].value; //[Column][Row] due to eg.A11 notation for user

			if (j == 2 || j == 5) // To seperate the line into 000 | 000 | 000
			{
				cout << " | "; 
			}
		}
		cout << endl;
		if (i == 2 || i == 5)
		{
			cout << "  ---------------" << endl; //To seperate every three lines
		}
	}
}

void confirmBoard(); //Hack-ish way to let soreCells know of confirmBoard when compiling since they call each other

void storeCells()
{
	bool complete = false;
	
	while (complete == false)
	{
		char inputCellChar[2];
		cout << "Enter the alphabet of the column of the cell to key value in" << endl;
		cin >> inputCellChar[0];
		cout << endl;
		cout << "Enter the row number of the cell to key value in" << endl;
		cin >> inputCellChar[1];
		cout << endl;
		int inputCellInt[2];
		inputCellInt[0] = inputCellChar[0] - 65; //ASCII for A is 65
		inputCellInt[1] = inputCellChar[1] - 48; //ASCII for 0 is 48

		cout << "Enter the value of the cell" << endl;
		int inputValue;
		cin >> inputValue;
		cout << endl;

		//cout << inputCellChar[0] << "  " << inputCellChar[1] << endl; //Used to test if conversion of input was correct
		//cout << inputCellInt[0]  << "  " << inputCellInt[1] << endl; //Used to test if conversion of input was correct

		gameBoard[inputCellInt[0]][inputCellInt[1]].value = inputValue; //Store value into cell
		
		cout << "Do you have more cells to enter? (Y/N)" << endl;
		string moreCells;
		cin >> moreCells;
		cout << endl;
		if (moreCells == "N")
		{
			complete = true;
		}
	}

	cout << endl << endl;
	confirmBoard();
}

void confirmBoard()
{
	cout << "Please confirm if this board is correct (Y/N)" << endl;
	cout << endl;
	displayBoard();
	string input;
	cin >> input;
	if (input == "N")
	{
		cout << endl << endl;
		storeCells();
	}
}

void displayPossibleValue(int column, int row) //Mainly used to debug
{
	cout << "The possible values are: ";
	for (int i = 0; i < 9; i++)
	{
		if (gameBoard[column][row].possibleValues[i] != 0)
		{
			cout << gameBoard[column][row].possibleValues[i] << "  ";
		}
	}
	cout << endl;
}

int wrapAroundEight(int value) //Eight because cells are [0]-[8] due to being stored in a array
{
	int temp = value; //Minusing sets of 9 until it is between 0-8 (9 because 9 goes to 0 due to array starting from [0])
	while (temp > 8)
	{
		temp = temp - 9;
	}

	return temp; 
}

void removePossibleValue(int value, int column, int row)
{
	gameBoard[column][row].possibleValues[value - 1] = 0; //possibleValues is stored in an array, the location of the number is one lower then the value it stores, ie [8] = 9
}

void checkColumnPossibleValue(int column, int row)
{
	for (int i = 1; i < 9; i++) //<9 because the 9th cell will be itself
	{
		if (gameBoard[column][wrapAroundEight(row + i)].value != 0)
		{
			removePossibleValue(gameBoard[column][wrapAroundEight(row + i)].value, column, row);
		}
	}
}

void checkRowPossibleValue(int column, int row)
{
	for (int i = 1; i < 9; i++) //<9 because the 9th cell will be itself
	{
		if (gameBoard[wrapAroundEight(column + i)][row].value != 0)
		{
			removePossibleValue(gameBoard[wrapAroundEight(column + i)][row].value, column, row);
		}
	}
}

int wrapAroundTwo(int value) //Two because box cells are [0]-[2] due to being stored in a array
{
	int temp = value; //Minusing sets of 3 until it is between 0-2 (3 because 3 goes to 0 due to array starting from [0])
	while (temp > 2)
	{
		temp = temp - 3;
	}

	return temp;
}

void checkBoxPossibleValue(int column, int row) //To figure out how to code this portion
{
	int boxCoord[2]; //Finding out which location in the box it is in
	boxCoord[0] = wrapAroundTwo(column);
	boxCoord[1] = wrapAroundTwo(row);

	if (boxCoord[0] == 0 && boxCoord[1] == 0) //First row left
	{
		for (int columnAddiion = 0; columnAddiion <= 2; columnAddiion++)
		{
			for (int rowAddition = 0; rowAddition <= 2; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}

	if (boxCoord[0] == 1 && boxCoord[1] == 0) //First row, middle
	{
		for (int columnAddiion = -1; columnAddiion <= 1; columnAddiion++)
		{
			for (int rowAddition = 0; rowAddition <= 2; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}

	if (boxCoord[0] == 2 && boxCoord[1] == 0) //First row, right
	{
		for (int columnAddiion = -2; columnAddiion <= 0; columnAddiion++)
		{
			for (int rowAddition = 0; rowAddition <= 2; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}

	if (boxCoord[0] == 0 && boxCoord[1] == 1) //Second row, left
	{
		for (int columnAddiion = 0; columnAddiion <= 2; columnAddiion++)
		{
			for (int rowAddition = -1; rowAddition <= 1; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}

	if (boxCoord[0] == 1 && boxCoord[1] == 1) //Second row, middle
	{
		for (int columnAddiion = -1; columnAddiion <= 1; columnAddiion++)
		{
			for (int rowAddition = -1; rowAddition <= 1; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}

	if (boxCoord[0] == 2 && boxCoord[1] == 1) //Second row, right
	{
		for (int columnAddiion = -2; columnAddiion <= 0; columnAddiion++)
		{
			for (int rowAddition = -1; rowAddition <= 1; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}

	if (boxCoord[0] == 0 && boxCoord[1] == 2) //Third row left
	{
		for (int columnAddiion = 0; columnAddiion <= 2; columnAddiion++)
		{
			for (int rowAddition = -2; rowAddition <= 0; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}

	if (boxCoord[0] == 1 && boxCoord[1] == 2) //Third row, middle
	{
		for (int columnAddiion = -1; columnAddiion <= 1; columnAddiion++)
		{
			for (int rowAddition = -2; rowAddition <= 0; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}

	if (boxCoord[0] == 2 && boxCoord[1] == 2) //Third row, right
	{
		for (int columnAddiion = -2; columnAddiion <= 0; columnAddiion++)
		{
			for (int rowAddition = -2; rowAddition <= 0; rowAddition++)
			{
				if (gameBoard[column + columnAddiion][row + rowAddition].value != 0)
				{
					removePossibleValue(gameBoard[column + columnAddiion][row + rowAddition].value, column, row);
				}
			}
		}
	}
}

void checkPossibleValueAll()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			checkRowPossibleValue(i, j);
			checkColumnPossibleValue(i, j);
			checkBoxPossibleValue(i, j);
		}
	}
}

bool checkSinglePossible(int column, int row)
{
	int counter = 9;
	for (int i = 0; i < 9; i++)
	{
		if (gameBoard[column][row].possibleValues[i] == 0)
		{
			counter--;
		}
	}

	if (counter == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void setSinglePossibleAsValue(int column, int row) //To code
{
	for (int i = 0; i < 9; i++)
	{
		if (gameBoard[column][row].possibleValues[i] != 0)
		{
			gameBoard[column][row].value = gameBoard[column][row].possibleValues[i];
		}
	}
}

void updateAllValues()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (checkSinglePossible(i, j) == true && gameBoard[i][j].value == 0) //Checking if there is already a solved value for the location
			{
				setSinglePossibleAsValue(i, j);
				changesMade = true;
			}
		}
	}
}

void recursiveSolveUsingHints()
{
	do
	{
		checkPossibleValueAll();
		updateAllValues();

		if (changesMade == true)
		{
			furtherIterate = true;
			changesMade = false;
		}
		else
		{
			furtherIterate = false;
		}
	} while (furtherIterate == true);
}

void setTestCaseOne() //Mainly used to test
{
	gameBoard[0][0].value = 2;
	gameBoard[0][2].value = 9;
	gameBoard[0][4].value = 1;
	gameBoard[0][5].value = 4;
	gameBoard[0][7].value = 8;
	gameBoard[0][8].value = 5;
	gameBoard[1][0].value = 6;
	gameBoard[1][2].value = 8;
	gameBoard[1][3].value = 7;
	gameBoard[1][4].value = 2;
	gameBoard[1][6].value = 3;
	gameBoard[1][7].value = 9;
	gameBoard[2][0].value = 3;
	gameBoard[2][3].value = 8;
	gameBoard[2][5].value = 6;
	gameBoard[2][6].value = 2;
	gameBoard[2][7].value = 1;
	gameBoard[2][8].value = 7;
	gameBoard[3][0].value = 1;
	gameBoard[3][2].value = 6;
	gameBoard[3][3].value = 9;
	gameBoard[3][4].value = 5;
	gameBoard[3][5].value = 3;
	gameBoard[3][6].value = 4;
	gameBoard[3][7].value = 7;
	gameBoard[3][8].value = 8;
	gameBoard[4][0].value = 4;
	gameBoard[4][1].value = 8;
	gameBoard[4][3].value = 1;
	gameBoard[4][4].value = 6;
	gameBoard[4][5].value = 7;
	gameBoard[4][8].value = 9;
	gameBoard[5][1].value = 5;
	gameBoard[5][2].value = 7;
	gameBoard[5][4].value = 4;
	gameBoard[5][6].value = 1;
	gameBoard[5][8].value = 3;
	gameBoard[6][0].value = 7;
	gameBoard[6][1].value = 3;
	gameBoard[6][2].value = 1;
	gameBoard[6][3].value = 5;
	gameBoard[6][6].value = 9;
	gameBoard[6][8].value = 6;
	gameBoard[7][0].value = 5;
	gameBoard[7][2].value = 2;
	gameBoard[7][3].value = 4;
	gameBoard[7][4].value = 7;
	gameBoard[7][5].value = 9;
	gameBoard[7][6].value = 8;
	gameBoard[7][7].value = 3;
	gameBoard[7][8].value = 1;
	gameBoard[8][0].value = 8;
	gameBoard[8][1].value = 9;
	gameBoard[8][4].value = 3;
	gameBoard[8][5].value = 1;
	gameBoard[8][6].value = 7;
	gameBoard[8][8].value = 2;
}

void testResultTestCaseOne() //Mainly used to test
{
	int generatedValues[9][9]; //Could probably use a pointer array instead of a duplicate array to save memory
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			generatedValues[i][j] = gameBoard[i][j].value;
		}
	}

	int correctAnswer[9][9] = { {2,7,9,3,1,4,6,8,5}, {6,1,8,7,2,5,3,9,4}, {3,4,5,8,9,6,2,1,7},{1,2,6,9,5,3,4,7,8}, {4,8,3,1,6,7,5,2,9}, {9,5,7,2,4,8,1,6,4}, {7,3,1,5,8,2,9,4,6}, {5,6,2,4,7,9,8,3,1}, {8,9,4,6,3,1,7,5,2} };

	bool correct = true;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (generatedValues[i][j] != correctAnswer[i][i])
			{
				correct = false;
			}
		}
	}

	if (correct = false)
	{
		cout << "There is an error in the solution" << endl;
	}
	else
	{
		cout << "The solution is correct" << endl;
	}
}

void setTestCaseTwo() //Mainly used to test
{
	gameBoard[0][3].value = 1;
	gameBoard[0][6].value = 5;
	gameBoard[0][7].value = 8;
	gameBoard[1][1].value = 6;
	gameBoard[1][7].value = 4;
	gameBoard[2][2].value = 4;
	gameBoard[2][6].value = 7;
	gameBoard[2][8].value = 9;
	gameBoard[3][1].value = 9;
	gameBoard[3][3].value = 5;
	gameBoard[3][4].value = 3;
	gameBoard[4][0].value = 8;
	gameBoard[4][2].value = 3;
	gameBoard[5][3].value = 4;
	gameBoard[5][4].value = 7;
	gameBoard[5][7].value = 3;
	gameBoard[6][1].value = 1;
	gameBoard[6][4].value = 5;
	gameBoard[6][8].value = 7;
	gameBoard[7][1].value = 8;
	gameBoard[7][4].value = 4;
	gameBoard[7][6].value = 1;
	gameBoard[8][0].value = 9;
	gameBoard[8][3].value = 6;
	gameBoard[8][8].value = 2;
}

int main()
{
	greetUser();
	cout << endl << endl;

	displayBoard();
	cout << endl << endl;

	storeCells();
	//Call function for setting test case here if testing program
	cout << endl << endl;

	displayBoard();
	cout << endl << endl;

	cout << "The program will now attempt to solve using an elimination method" << endl;
	cout << endl << endl;

	recursiveSolveUsingHints();

	cout << "This is the board after solving using elimination" << endl;
	displayBoard();
	cout << endl << endl;

  return 0;
}
