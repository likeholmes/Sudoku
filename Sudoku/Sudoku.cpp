// Sudoku.cpp: 定义应用程序的入口点。
//

#include "Sudoku.h"
#include <array>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
using namespace std;

// 目标1：生成9*9标准数独
//		方法1：按行放置每一个数字，对于任意位置p(i,j)，待放置数字放在数列a，每一行一开始数列a中包含1-9。在排除当前数列不满足条件的数字后，
// 随机从a中取出数字放到位置p，直到a为空。
// 需要满足的条件时，v[i,j] != v[i-x,j]；i/3,j/3所在的3*3区域v[(i/3)+x, (j/3)+y] != v[i,j]。
// 数字选取：数组a首先按顺序存储1-9，随机选中1-j的位置，将a[x]选中，然后a[x]替换为a[10-j]。
//		结论：不可靠，通过随机数随意生成，可能会出现无法满足所有条件的位置和值
// 
//		方法2：回溯法，首先用随机数生成第一行，在那之后递归生成每一个数，如果生成的数独还有空，则回溯上一步
// 
// 目标2：生成n*n数独（n大小有限制）
// 目标3：增加限制生成数独
// 目标4：生成不规则数独

void Print(const array<array<int, 10>, 10>& sudoku)
{
	for (size_t i = 1; i < 10; i++)
	{
		for (size_t j = 1; j < 10; j++)
		{
			cout << sudoku[i][j] << "   ";
		}
		cout << endl << endl;
	}
}

void Move(array<int, 10>& sub, array<int, 10>& index, int val, int tailPtr)
{
	int oldIndex = index[val];
	int tmp = sub[tailPtr];
	sub[tailPtr] = sub[oldIndex];
	sub[oldIndex] = tmp;
	index[val] = tailPtr;
	index[tmp] = oldIndex;
}

void Select(array<int, 10> &sub, array<int, 10> &index, array<array<int, 10>, 10>& sudoku, int i, int j, int tail)
{
	int tailPtr = tail;
	// 将同一列的值移到数列的末尾
	for (int x = 1; x < i; x++)
	{
		int val = sudoku[x][j];
		if(index[val] <= tailPtr)
		{
			Move(sub, index, val, tailPtr);
			tailPtr--;
		}
	}
	// 将9宫格中出现过的值移到数列的末尾
	int w = (i-1) / 3 * 3 + 1;
	int h = (j-1) / 3 * 3 + 1;
	for (size_t x = w; x < i; x++)
	{
		for (size_t y = h; y < h+3; y++)
		{
			int val = sudoku[x][y];
			if (index[val] <= tailPtr)
			{
				Move(sub, index, val, tailPtr);
				tailPtr--;
			}
		}
	}
	for (size_t y = h; y < j; y++)
	{
		int val = sudoku[i][y];
		if (index[val] <= tailPtr)
		{
			Move(sub, index, val, tailPtr);
			tailPtr--;
		}
	}
	
	// 随机选择一个值到目标位置
	int val = sub[rand() % tailPtr + 1];
	sudoku[i][j] = val;
	// 将选中的值移到数列末尾
	Move(sub, index, val, tail);
}

void Init(array<int, 10>& arr)
{
	for (size_t i = 0; i < arr.size(); i++)
	{
		arr[i] = i;
	}
}

bool IsValid(const array<array<int, 10>, 10>& sudoku, int row, int col, int num)
{
	for (size_t i = 1; i < row; i++)
	{
		if (sudoku[i][col] == num)
			return false;
	}

	for (size_t i = 0; i < col; i++)
	{
		if (sudoku[row][i] == num)
			return false;
	}

	int w = (row - 1) / 3 * 3 + 1;
	int h = (col - 1) / 3 * 3 + 1;
	for (size_t x = w; x < w + 3; x++)
	{
		for (size_t y = h; y < h + 3; y++)
		{
			if (sudoku[x][y] == num)
				return false;
		}
	}

	return true;
}

bool RecurseGen(array<array<int, 10>, 10>& sudoku, int row, int col)
{
	if (row > 9 || col > 9)
		return true;
	if (sudoku[9][9])
		return true;
	// 这种生成的很高概率，第二行前面几列数字非常小
	for (size_t i = 1; i < 10; i++)
	{
		if (IsValid(sudoku, row, col, i))
		{
			sudoku[row][col] = i;
			int nrow = row;
			int ncol = col + 1;
			if (col == 9)
			{
				ncol = 1;
				nrow += 1;
			}
			if (RecurseGen(sudoku, nrow, ncol))
				return true;
			sudoku[row][col] = 0;
		}
	}
	return false;
}

int main()
{
	srand(time(0));

	array<array<int, 10>, 10> sudoku{ 0 };	//	待填充数独
	array<int, 10> sub;						// 候选数列
	array<int, 10> index;

	{
		size_t i = 1;
		Init(sub);
		Init(index);
		for (size_t j = 1; j < 10; j++)
		{
			int tail = 10 - j;
			Select(sub, index, sudoku, i, j, tail);
		}
	}

	RecurseGen(sudoku, 2, 1);

	Print(sudoku);
	return 0;
}
