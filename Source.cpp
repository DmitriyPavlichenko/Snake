#include <iostream>
#include <ctime>
#include <conio.h>
#include <Windows.h>


using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

bool GameUnstopped = true;
short PressedKey;
short ArrNumb;
#define Height 13
#define Width 37
struct Data
{
	//Player
	short PlayerX = Width / 2;
	short PlayerY = Height / 2;
	int TailXY[2][Width * Height - 2 * (Width + Height - 2)]{ {0} };
	int PlayerLength = 0;

	//Fruit
	short FruitX = 0;
	short FruitY = 0;
} Coordinates;

void PressingCatch()
{
	Sleep(50);
	//input
	if (_kbhit())
	{
		PressedKey = _getch();
		if (!PressedKey || PressedKey == 224)
		{
			PressedKey = _getch();
		}
	}

	//head shift
	switch (PressedKey)
	{
	case 75:	// <
	{
		Coordinates.PlayerX--;
		if (Coordinates.PlayerX < 1)
		{
			Coordinates.PlayerX = Width - 2;
		}
	}
	break;
	case 72:	// ^
	{
		Coordinates.PlayerY--;
		if (Coordinates.PlayerY < 1)
		{
			Coordinates.PlayerY = Height - 2;
		}
	}
	break;
	case 77:	// >
	{
		Coordinates.PlayerX++;
		if (Coordinates.PlayerX >= Width - 1)
		{
			Coordinates.PlayerX = 1;
		}
	}
	break;
	case 80:	// v
	{
		Coordinates.PlayerY++;
		if (Coordinates.PlayerY > Height - 2)
		{
			Coordinates.PlayerY = 1;
		}
	}
	break;
	case 27:	// to exit
	{
		GameUnstopped = false;
	}
	break;
	}
}
void FruitSpawn()
{
	Coordinates.FruitX = rand() % (Width - 2) + 1;
	Coordinates.FruitY = rand() % (Height - 2) + 1;
	/*for (size_t i = 0; i < Width * Height - 2 * (Width + Height - 2); i++)
	{
		if (Coordinates.FruitX == Coordinates.TailXY[0][i] && Coordinates.FruitY == Coordinates.TailXY[1][i])
		{
			Coordinates.FruitX = rand() % (Width - 2) + 1;
			Coordinates.FruitY = rand() % (Width - 2) + 1;
			i = 0;
		}
	}*/
}
void Logic()
{
	//tail saving
	if (ArrNumb < Coordinates.PlayerLength)
	{
		Coordinates.TailXY[0][ArrNumb] = Coordinates.PlayerX;
		Coordinates.TailXY[1][ArrNumb] = Coordinates.PlayerY;
		ArrNumb++;
	}
	if (!(ArrNumb < Coordinates.PlayerLength))
	{
		ArrNumb = 0;
	}

	//head shifting
	PressingCatch();

	//fruit spawn
	if (Coordinates.PlayerX == Coordinates.FruitX && Coordinates.PlayerY == Coordinates.FruitY)
	{
		Coordinates.PlayerLength++;
		FruitSpawn();
	}

	//gameover by tail
	for (size_t i = 0; i < Coordinates.PlayerLength; i++)
	{
		if (Coordinates.TailXY[0][i] == Coordinates.PlayerX
			&& Coordinates.TailXY[1][i] == Coordinates.PlayerY)
		{
			GameUnstopped = false;
		}
	}

}
void setcur(int x, int y) //consol cleaning
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void Drawing()
{
	Sleep(50);
	bool compl_check = false;
	setcur(0, 0);
	for (size_t i = 0; i < Width; i++)
	{
		cout << "#";
	}

	for (size_t j = 1; j < Height - 1; j++)
	{
		cout << "\n#";
		for (size_t k = 1; k < Width - 1; k++)
		{
			if (k == Coordinates.PlayerX && j == Coordinates.PlayerY)
			{
				cout << "@";
			}
			else {
				for (size_t tmp = 0; tmp < Coordinates.PlayerLength; tmp++)
				{
					if (k == Coordinates.TailXY[0][tmp] && j == Coordinates.TailXY[1][tmp])
					{
						cout << "*";
						tmp = Coordinates.PlayerLength;
						compl_check = true;
					}
				}
				if (!compl_check)
				{
					if (k == Coordinates.FruitX && j == Coordinates.FruitY)
					{
						SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
						cout << "*";
						SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
					}
					else
					{
						cout << " ";
					}
				}
				compl_check = false;
			}
		}
		cout << "#";
	}

	cout << endl;
	for (size_t i = 0; i < Width; i++)
	{
		cout << "#";
	}
	cout << "\nYour score: " << Coordinates.PlayerLength * 10;
}
int main()
{
	srand(time(0));

	FruitSpawn();
	while (GameUnstopped)
	{
		Drawing();
		Logic();
	}

	return 0;
}