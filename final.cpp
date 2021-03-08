/* Kursinis darbas
ITF18/2 Airidas Sarkunas
Laivu musis
1 Aircraft Carrier, 5 spots
1 Battleship, 4 spots
1 Destroyer, 3 spots
1 Submarine, 3 spots
1 Patrol Boat, 2 spots
*/
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <cstdlib>
#include <time.h>
using namespace std;
const int TABLE = 10;
const int CARRIERS = 5;
void gotoxy(int x, int y);
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
class Coordinates
{
public:
	int x, y;
public:
};

class Player : public Coordinates
{
protected:
	Coordinates coordinates;
	int health;
	string **A;
	int laivai[5][4];
public:
	Player()
	{
		health = 17;
		A = new string*[TABLE];
		for (int i = 0; i < TABLE; i++)
		{
			A[i] = new string[TABLE];
		}
		for (int i = 0; i < TABLE; i++)
		{
			for (int j = 0; j < TABLE; j++)
				A[i][j] = "-";
		}

	}
	void PrintBoard() const
	{
		system("CLS");
		cout << "  0 1 2 3 4 5 6 7 8 9 | \n";
		for (int i = 0; i < TABLE; i++)
		{
			for (int j = 0; j < TABLE; j++)
			{
				if (j == 0)
				{
					cout << i << " " << A[i][j] << " ";
				}
				else if (j == TABLE - 1)
					cout << A[i][j] << " | ";
				else cout << A[i][j] << " ";
			}
			cout << endl;
		}
	}
	void Carriers()
	{
		int placed = 0;
		int direction;
		int laivas;
		while (placed != 5)
		{
			PrintBoard();
			cout << "KRYPTIS : \n";
			cout << "I apacia : 1 \n";
			cout << "I desine : 2 \n";
			cout << "Iveskite krypti bei tasko koordinates nuo kuriu pradesite statyti ";
			switch (placed)
			{
			case 0:
				cout << "5-vieti laiva :";
				laivas = 5;
				break;
			case 1:
				cout << "4-vieti laiva :";
				laivas = 4;
				break;
			case 2:
				cout << "3-vieti laiva :";
				laivas = 3;
				break;
			case 3:
				cout << "3-vieti laiva :";
				laivas = 3;
				break;
			case 4:
				cout << "2-vieti laiva :";
				laivas = 2;
				break;
			default:
				cout << "KLAIDA!";
			}
			cin >> direction >> coordinates.x >> coordinates.y;
			if (direction <= 2 && direction >= 1 && coordinates.x >= 0 && coordinates.x <= 9 && coordinates.y >= 0 && coordinates.y <= 9)
			{
				if (ValidPlacement(laivas, direction, coordinates))
				{
					if (direction == 1)
					{
						laivai[placed][0] = 1;
						laivai[placed][1] = coordinates.x;
						laivai[placed][2] = coordinates.y;
						laivai[placed][3] = coordinates.y + laivas - 1;
					}
					else if (direction == 2)
					{
						laivai[placed][0] = 2;
						laivai[placed][1] = coordinates.y;
						laivai[placed][2] = coordinates.x;
						laivai[placed][3] = coordinates.x + laivas - 1;
					}
					placed++;
					PlaceShip(laivas, coordinates, direction);
					PrintBoard();
				}
			}
		}
		gotoxy(0, 13);
	}

	void Recalculate(Coordinates & start, Coordinates & end)
	{
		if (start.x < 0)
			start.x = 0;
		if (start.y < 0)
			start.y = 0;
		if (start.x > 9)
			start.x = 9;
		if (start.y > 9)
			start.y = 9;
		if (end.x < 0)
			end.x = 0;
		if (end.x > 9)
			end.x = 9;
		if (end.y < 0)
			end.y = 0;
		if (end.y > 9)
			end.y = 9;
	}

	bool ValidPlacement(int laivas, int direction, Coordinates & c)
	{
		if (A[c.y][c.x] == "O")
			return false;
		Coordinates start;
		Coordinates end;
		if (direction == 1)
		{
			if (c.y + laivas > 10)
				return false;
			start.x = c.x - 1;
			start.y = c.y - 1;
			end.x = c.x + 1;
			end.y = c.y + laivas;
			Recalculate(start, end);
			for (int i = start.y; i <= end.y; i++)
			{
				for (int j = start.x; j <= end.x; j++)
				{
					if (A[i][j] == "O")
						return false;
				}
			}
		}
		else if (direction == 2)
		{
			if (c.x + laivas > 10)
				return false;
			start.x = c.x - 1;
			start.y = c.y - 1;
			end.x = c.x + laivas;
			end.y = c.y + 1;
			Recalculate(start, end);
			for (int i = start.y; i <= end.y; i++)
			{
				for (int j = start.x; j <= end.x; j++)
				{
					if (A[i][j] == "O")
						return false;
				}
			}
		}
		return true;
	}
	void PlaceShip(int laivas, Coordinates & c, int direction)
	{
		if (direction == 1)
		{
			for (int i = c.y; i < c.y + laivas; i++)
				A[i][c.x] = "O";
		}
		else if (direction == 2)
		{
			for (int i = c.x; i < c.x + laivas; i++)
				A[c.y][i] = "O";
		}
	}
	void isSunk()
	{
		for (int i = 0; i < 5; i++)
		{
			int count = 0;
			if (laivai[i][0] == 1)
			{
				for (int j = laivai[i][2]; j <= laivai[i][3]; j++)
				{
					if (A[j][laivai[i][1]] == "X")
						count++;
				}
				if (count == laivai[i][3] - laivai[i][2] + 1)
				{
					Sink(i);
				}
			}
			else if (laivai[i][0] == 2)
			{
				for (int j = laivai[i][2]; j <= laivai[i][3]; j++)
				{
					if (A[laivai[i][1]][j] == "X")
						count++;
				}
				if (count == laivai[i][3] - laivai[i][2] + 1)
				{
					Sink(i);
				}
			}
		}
	}
	void Sink(int i)
	{
		if (laivai[i][0] == 1)
		{

			for (int j = laivai[i][2]; j <= laivai[i][3]; j++)
			{
				if (j == laivai[i][2] && j - 1 >= 0)
					A[j - 1][laivai[i][1]] = ".";
				else if (j == laivai[i][3] && j + 1 <= 9)
					A[j + 1][laivai[i][1]] = ".";
				if (laivai[i][1] + 1 <= 9)
					A[j][laivai[i][1] + 1] = ".";
				if (laivai[i][1] - 1 >= 0)
					A[j][laivai[i][1] - 1] = ".";
				A[j][laivai[i][1]] = "s";
			}
		}
		else if (laivai[i][0] == 2)
		{
			for (int j = laivai[i][2]; j <= laivai[i][3]; j++)
			{
				if (j == laivai[i][2] && j - 1 >= 0)
					A[laivai[i][1]][j - 1] = ".";
				else if (j == laivai[i][3] && j + 1 <= 9)
					A[laivai[i][1]][j + 1] = ".";
				if (laivai[i][1] + 1 <= 9)
					A[laivai[i][1] + 1][j] = ".";
				if (laivai[i][1] - 1 >= 0)
					A[laivai[i][1] - 1][j] = ".";
				A[laivai[i][1]][j] = "s";
			}
		}
	}
	bool Hit(pair<int, int> hit)
	{
		if (A[hit.second][hit.first] == "O")
		{
			A[hit.second][hit.first] = "X";
			{
				gotoxy(0, 16);
				SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY));
				cout << "I JUS PATAIKE!\n";
				SetConsoleTextAttribute(hConsole, (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE));
				Sleep(1500);
				return true;
			}
		}
		else
		{
			A[hit.second][hit.first] = "m";
			return false;
		}
	}
	int hp()
	{
		return health;
	}
	bool sunk(pair<int, int> a)
	{
		if (A[a.second][a.first] == "s")
			return true;
		else return false;
	}
	bool good(int x, int y)
	{
		if (A[y][x] == "s" || A[y][x] == "m" || A[y][x] == "." || A[y][x] == "X" || x > 9 || x < 0 || y > 9 || y < 0)
			return false;
		else return true;
	}

	void operator--()
	{
		health--;
	}

	~Player()
	{
		for (int i = 0; i < TABLE; i++)
			delete[] A[i];
		delete[] A;
	}
};

class CPU : public Player
{
protected:
	string **Hidden;
	bool sequence;
	int confirmeddir;
	Coordinates firsthit;
public:
	CPU()
	{
		Player();
		sequence = false;
		confirmeddir = 0;
		Hidden = new string*[TABLE];
		for (int i = 0; i < TABLE; i++)
		{
			Hidden[i] = new string[TABLE];
		}
		for (int i = 0; i < TABLE; i++)
		{
			for (int j = 0; j < TABLE; j++)
				Hidden[i][j] = "-";
		}
	}
	void RandomizeFleet()
	{
		int placed = 0;
		int direction;
		int laivas;
		while (placed != 5)
		{
			coordinates.x = rand() % 10;
			direction = rand() % 2 + 1;
			coordinates.y = rand() % 10;
			switch (placed)
			{
			case 0:
				laivas = 5;
				break;
			case 1:
				laivas = 4;
				break;
			case 2:
				laivas = 3;
				break;
			case 3:
				laivas = 3;
				break;
			case 4:
				laivas = 2;
				break;
			default:
				cout << "KLAIDA\n";
			}
			if (direction <= 2 && direction >= 1 && coordinates.x >= 0 && coordinates.x <= 9 && coordinates.y >= 0 && coordinates.y <= 9)
			{
				if (ValidPlacement(laivas, direction, coordinates))
				{
					if (direction == 1)
					{
						laivai[placed][0] = 1;
						laivai[placed][1] = coordinates.x;
						laivai[placed][2] = coordinates.y;
						laivai[placed][3] = coordinates.y + laivas - 1;
					}
					else if (direction == 2)
					{
						laivai[placed][0] = 2;
						laivai[placed][1] = coordinates.y;
						laivai[placed][2] = coordinates.x;
						laivai[placed][3] = coordinates.x + laivas - 1;
					}
					placed++;
					PlaceShip(laivas, coordinates, direction);
					showBoard();
				}
			}
		}
	}
	void showBoard()
	{
		gotoxy(24, 0);
		cout << "  0 1 2 3 4 5 6 7 8 9  \n";
		for (int i = 0; i < TABLE; i++)
		{
			gotoxy(24, i + 1);
			for (int j = 0; j < TABLE; j++)
			{
				if (j == 0)
				{
					cout << i << " " << Hidden[i][j] << " ";
				}
				else cout << Hidden[i][j] << " ";
			}
		}
		gotoxy(0, 15);
	}
	pair<int, int> Shoot()
	{
		return make_pair(rand() % 10, rand() % 10);
	}
	bool Hit1(pair<int, int> hit)
	{
		if (A[hit.second][hit.first] == "O")
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN));
			gotoxy(0, 16);
			cout << "PATAIKEI\n";
			Sleep(1500);
			SetConsoleTextAttribute(hConsole, (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE));
			A[hit.second][hit.first] = "X";
			Hidden[hit.second][hit.first] = "X";
			return true;
		}
		else
		{
			A[hit.second][hit.first] = "m";
			Hidden[hit.second][hit.first] = "m";
			return false;
		}
	}
	void isCPUSunk()
	{
		for (int i = 0; i < 5; i++)
		{
			int count = 0;
			if (laivai[i][0] == 1)
			{
				for (int j = laivai[i][2]; j <= laivai[i][3]; j++)
				{
					if (A[j][laivai[i][1]] == "X")
						count++;
				}
				if (count == laivai[i][3] - laivai[i][2] + 1)
					SinkCPU(i);
			}
			else if (laivai[i][0] == 2)
			{
				for (int j = laivai[i][2]; j <= laivai[i][3]; j++)
				{
					if (A[laivai[i][1]][j] == "X")
						count++;
				}
				if (count == laivai[i][3] - laivai[i][2] + 1)
					SinkCPU(i);
			}
		}
	}
	void SinkCPU(int i)
	{
		if (laivai[i][0] == 1)
		{
			for (int j = laivai[i][2]; j <= laivai[i][3]; j++)
			{
				if (j == laivai[i][2] && j - 1 >= 0)
					Hidden[j - 1][laivai[i][1]] = ".";
				else if (j == laivai[i][3] && j + 1 <= 9)
					Hidden[j + 1][laivai[i][1]] = ".";
				if (laivai[i][1] + 1 <= 9)
					Hidden[j][laivai[i][1] + 1] = ".";
				if (laivai[i][1] - 1 >= 0)
					Hidden[j][laivai[i][1] - 1] = ".";
				Hidden[j][laivai[i][1]] = "s";
			}
		}
		else if (laivai[i][0] == 2)
		{
			for (int j = laivai[i][2]; j <= laivai[i][3]; j++)
			{
				if (j == laivai[i][2] && j - 1 >= 0)
					Hidden[laivai[i][1]][j - 1] = ".";
				else if (j == laivai[i][3] && j + 1 <= 9)
					Hidden[laivai[i][1]][j + 1] = ".";
				if (laivai[i][1] + 1 <= 9)
					Hidden[laivai[i][1] + 1][j] = ".";
				if (laivai[i][1] - 1 >= 0)
					Hidden[laivai[i][1] - 1][j] = ".";
				Hidden[laivai[i][1]][j] = "s";
			}
		}
	}
	void setSequence(bool x) { sequence = x; }
	bool getsequence() { return sequence; }
	int dir() { return confirmeddir; }
	void setdir(int x) { confirmeddir = x; }
	~CPU()
	{
		for (int i = 0; i < TABLE; i++)
			delete[] Hidden[i];
		delete[] Hidden;
	}
};

int main()
{
	SetConsoleTextAttribute(hConsole, (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED));
	int turn;
	pair<int, int> hit;
	Player p1;
	p1.Carriers();
	cout << "Laukite, kol zaidejas sudelios laivus...\n";
	Sleep(5000);
	CPU c1;
	srand(time(NULL));
	c1.RandomizeFleet();
	turn = 1;
	bool stepup = true, stepdown = true, stepleft = true, stepright = true;
	int steps = 0;
	int random_dir;
	bool hit1 = false;
	pair<int, int> nexthit;
	pair<int, int> firsthit;
	while (true)
	{
		p1.PrintBoard();
		c1.showBoard();
		if (turn == 1)
		{
			gotoxy(0, 13);
			cout << "Iveskite suvio koordinates (x, y): \n";
			while (true)
			{
				cin >> hit.first >> hit.second;
				if (hit.first >= 0 && hit.first <= 9 && hit.second <= 9 && hit.second >= 0)
					break;
			}
			if (c1.Hit1(hit))
			{
				c1.isCPUSunk();
				--c1;
			}
			turn = 2;
		}
		else
		{
			if (c1.getsequence())
			{
				if (c1.dir() == 0)
				{
					nexthit = firsthit;
					while (true)
					{
						while (true)
						{
							random_dir = rand() % 4 + 1;
							if (random_dir == 1)
							{
								if (p1.good(nexthit.first, nexthit.second - 1))
									break;
							}
							else if (random_dir == 2)
							{
								if (p1.good(nexthit.first, nexthit.second + 1))
									break;
							}
							else if (random_dir == 3)
							{
								if (p1.good(nexthit.first - 1, nexthit.second))
									break;
							}
							else if (random_dir == 4)
							{
								if (p1.good(nexthit.first + 1, nexthit.second))
									break;
							}
						}
						if (random_dir == 1 && stepup == true && nexthit.second - 1 >= 0)
						{
							nexthit.second--;
							steps++;
							break;
						}
						else if (random_dir == 2 && stepdown == true && nexthit.second + 1 <= 9)
						{
							nexthit.second++;
							steps++;
							break;
						}
						else if (random_dir == 3 && stepleft == true && nexthit.first - 1 >= 0)
						{
							nexthit.first--;
							steps++;
							break;
						}
						else if (random_dir == 4 && stepright == true && nexthit.first + 1 <= 9)
						{
							nexthit.first++;
							steps++;
							break;
						}
					}
				}
				else
				{
					if (c1.dir() == 1)
						nexthit.second--;
					else if (c1.dir() == 2)
						nexthit.second++;
					else if (c1.dir() == 3)
						nexthit.first--;
					else if (c1.dir() == 4)
						nexthit.first++;

					if (!(p1.good(nexthit.first, nexthit.second)) || hit1 == false)
					{
						if (c1.dir() == 1)
						{
							nexthit = firsthit;
							nexthit.second--;
							c1.setdir(2);
						}
						else if (c1.dir() == 2)
						{
							nexthit = firsthit;
							nexthit.second--;
							c1.setdir(1);
						}
						else if (c1.dir() == 3)
						{
							nexthit = firsthit;
							nexthit.first++;
							c1.setdir(4);
						}
						else if (c1.dir() == 4)
						{
							nexthit = firsthit;
							nexthit.first--;
							c1.setdir(3);
						}
					}
				}
			}
			else if (!c1.getsequence())
			{
				while (true)
				{
					nexthit = c1.Shoot();
					if (p1.good(nexthit.first, nexthit.second))
						break;
				}
			}
			if (p1.Hit(nexthit))
			{
				--p1;
				hit1 = true;
				if (steps == 1)
				{
					if (random_dir == 1)
						c1.setdir(1);
					else if (random_dir == 2)
						c1.setdir(2);
					else if (random_dir == 3)
						c1.setdir(3);
					else if (random_dir == 4)
						c1.setdir(4);
					steps++;
				}
				if (c1.getsequence() == false)
				{
					c1.setSequence(true);
					firsthit = nexthit;
				}
				p1.isSunk();
				if (p1.sunk(firsthit))
				{
					c1.setSequence(false);
					c1.setdir(0);
					steps = 0;
					stepup = true, stepdown = true, stepleft = true, stepright = true;;
				}
			}
			else
			{
				hit1 = false;
				if (steps == 1)
				{
					if (c1.dir() == 1)
					{
						stepup = false;
						c1.setdir(0);
					}
					else if (c1.dir() == 2)
					{
						stepdown = false;
						c1.setdir(0);
					}
					else if (c1.dir() == 3)
					{
						stepleft = false;
						c1.setdir(0);
					}
					else if (c1.dir() == 4)
					{
						stepright = false;
						c1.setdir(0);
					}
					steps = 0;
				}
			}
			turn = 1;
		}
		if (p1.hp() == 0)
		{
			cout << "PRALAIMEJOTE!\n";
			break;
		}
		else if (c1.hp() == 0)
		{
			cout << "LAIMEJOTE!\n";
			break;
		}
	}
	cout << "ZAIDIMAS BAIGIASI\n";
	return 0;
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
