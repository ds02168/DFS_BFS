#pragma warning(disable: 4996)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAX_ROW 30
#define MAX_COL 40
#define BUF_SIZE 255
#define MAX_STACK_SIZE 1200
#define EXIT_ROW 28
#define EXIT_COL 38
#define TRUE 1
#define FALSE 0


typedef struct {
	short int x;
	short int y;
}offsets;
offsets move[8] = { {-1, 0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1},{0,-1},{-1,-1} };



typedef struct {
	short int row;
	short int col;
	short int dir;
}element;
element stack[MAX_STACK_SIZE];


int** maze1;
int** mark1;

int** maze2;
int** mark2;

int** maze3;
int** mark3;

void setMarks();
void setMazes();
void releaseArrays();
void add(int*, element);
element delete(int*);
int isFull(int);
int isEmpty(int);
void path(int**, int**);
void printResult(int**, int**, int);

int main()
{
	setMarks();
	setMazes();

	printf("maze1(30x40)\n");
	path(mark1, maze1);

	printf("maze2(30x40)\n");
	path(mark2, maze2);
	printf("maze3(30x40)\n");
	path(mark3, maze3);

	releaseArrays();
	return 0;
}

void path(int** mark, int** maze)
{
	int i, row, col, next_row, next_col, dir;
	int found = FALSE;
	int top;
	element position;
	mark[1][1] = 1; top = 0;
	stack[0].row = 1;
	stack[0].col = 1;
	stack[0].dir = 0;

	while (top > -1 && !found) {
		position = delete(&top);
		row = position.row;
		col = position.col;
		dir = position.dir;
		while (dir < 8 && !found) {
			next_row = row + move[dir].x;
			next_col = col + move[dir].y;
			if (next_row == EXIT_ROW && next_col == EXIT_COL)
				found = TRUE;
			else if (!maze[next_row][next_col] && !mark[next_row][next_col]) {
				position.row = row;
				position.col = col;
				position.dir = ++dir;
				add(&top, position);
				row = next_row;
				col = next_col;
				mark[next_row][next_col] = 1;
				dir = 0;
			}
			else ++dir;
		}
	}


	if (found)
	{
		position.row = row;
		position.col = col;
		position.dir = ++dir;
		add(&top, position);

		printResult(mark, maze, top);
	}
	else {
		printf("미로에 출구가 존재하지 않습니다!\n");

	}
}

void printResult(int** mark, int** maze, int top)
{
	int row, col, mar, maz;
	char result[MAX_ROW][MAX_COL];
	int node_count = 0;
	element position;

	for (row = 0; row < MAX_ROW; row++)
	{
		for (col = 0; col < MAX_COL; col++)
		{
			mar = mark[row][col];
			maz = maze[row][col];
			if (mar == maz)
				result[row][col] = ' ';
			else if (mar != maz) {
				result[row][col] = 'X';
				node_count++;
			}
		}
	}

	for (int load = top; load > -1; load--)
	{
		position = stack[load];
		result[position.row][position.col] = 'O';
	}

	for (row = 0; row < MAX_ROW; row++)
	{
		for (col = 0; col < MAX_COL; col++)
		{
			printf("%c ", result[row][col]);
		}
		printf("\n");
	}

	printf("방문한 노드 수 = %d\n", node_count);
	printf("해답경로 길이 = %d\n", top);
	printf("\n\n\n\n");
}

void add(int* top, element position)
{
	if (*top >= (MAX_STACK_SIZE - 1))
	{
		printf("Stack is Full!!\n");
		exit(-1);
	}
	else
	{
		(*top)++;
		stack[*top].row = position.row;
		stack[*top].col = position.col;
		stack[*top].dir = position.dir;
	}
}


element delete(int* top)
{
	element temp;
	if (*top <= -1)
	{
		printf("Stack is Empty.\n");
		exit(-1);
	}
	else {
		temp = stack[*top];
		(*top)--;
	}
	return temp;
}

int isFull(int top)
{
	if (top >= (MAX_STACK_SIZE - 1))
		return TRUE;
	else
		return FALSE;
}

int isEmpty(int top)
{
	if (top <= -1)
		return TRUE;
	else
		return FALSE;
}


void setMarks()
{
	char buffer[BUF_SIZE];
	char* buf;
	FILE* fp;
	int cur = 0;

	mark1 = (int**)malloc(sizeof(int*) * MAX_ROW);
	if (mark1 != NULL) {
		for (int i = 0; i < MAX_ROW; i++)
			mark1[i] = (int*)malloc(sizeof(int) * MAX_COL);
	}

	mark2 = (int**)malloc(sizeof(int*) * MAX_ROW);
	if (mark2 != NULL) {
		for (int i = 0; i < MAX_ROW; i++)
			mark2[i] = (int*)malloc(sizeof(int) * MAX_COL);
	}

	mark3 = (int**)malloc(sizeof(int*) * MAX_ROW);
	if (mark3 != NULL) {
		for (int i = 0; i < MAX_ROW; i++)
			mark3[i] = (int*)malloc(sizeof(int) * MAX_COL);
	}

	if ((fp = fopen("maze1.2.3.txt", "r")) == NULL)
	{
		printf("파일읽기 오류!\n");
		return;
	}

	buf = fgets(buffer, sizeof(buffer), fp);

	for (int i = 0; i < MAX_ROW; i++)
	{
		buf = fgets(buffer, sizeof(buffer), fp);
		for (int j = 0; j < MAX_COL; j++)
		{
			switch (buffer[j])
			{
			case '0': mark1[i][j] = 0; break;
			case '1': mark1[i][j] = 1; break;
			}
		}
	}


	buf = fgets(buffer, sizeof(buffer), fp);
	buf = fgets(buffer, sizeof(buffer), fp);
	buf = fgets(buffer, sizeof(buffer), fp);


	for (int i = 0; i < MAX_ROW; i++)
	{
		buf = fgets(buffer, sizeof(buffer), fp);
		for (int j = 0; j < MAX_COL; j++)
		{
			switch (buffer[j])
			{
			case '0': mark2[i][j] = 0; break;
			case '1': mark2[i][j] = 1; break;
			}
		}
	}

	buf = fgets(buffer, sizeof(buffer), fp);
	buf = fgets(buffer, sizeof(buffer), fp);
	buf = fgets(buffer, sizeof(buffer), fp);


	for (int i = 0; i < MAX_ROW; i++)
	{
		buf = fgets(buffer, sizeof(buffer), fp);
		for (int j = 0; j < MAX_COL; j++)
		{
			switch (buffer[j])
			{
			case '0': mark3[i][j] = 0; break;
			case '1': mark3[i][j] = 1; break;
			}
		}
	}


	if (fp != NULL)
		fclose(fp);
}


void setMazes()
{
	int i, j;
	maze1 = (int**)malloc(sizeof(int*) * MAX_ROW);
	if (maze1 != NULL) {
		for (int i = 0; i < MAX_ROW; i++)
			maze1[i] = (int*)malloc(sizeof(int) * MAX_COL);
	}

	maze2 = (int**)malloc(sizeof(int*) * MAX_ROW);
	if (maze2 != NULL) {
		for (int i = 0; i < MAX_ROW; i++)
			maze2[i] = (int*)malloc(sizeof(int) * MAX_COL);
	}

	maze3 = (int**)malloc(sizeof(int*) * MAX_ROW);
	if (maze3 != NULL) {
		for (int i = 0; i < MAX_ROW; i++)
			maze3[i] = (int*)malloc(sizeof(int) * MAX_COL);
	}


	for (i = 0; i < MAX_ROW; i++)
		for (j = 0; j < MAX_COL; j++)
			maze1[i][j] = mark1[i][j];

	for (i = 0; i < MAX_ROW; i++)
		for (j = 0; j < MAX_COL; j++)
			maze2[i][j] = mark2[i][j];

	for (i = 0; i < MAX_ROW; i++)
		for (j = 0; j < MAX_COL; j++)
			maze3[i][j] = mark3[i][j];

}

void releaseArrays()
{
	for (int i = 0; i < MAX_ROW; i++)
		free(mark1[i]);
	free(mark1);

	for (int i = 0; i < MAX_ROW; i++)
		free(mark2[i]);
	free(mark2);

	for (int i = 0; i < MAX_ROW; i++)
		free(mark3[i]);
	free(mark3);

	for (int i = 0; i < MAX_ROW; i++)
		free(maze1[i]);
	free(maze1);

	for (int i = 0; i < MAX_ROW; i++)
		free(maze2[i]);
	free(maze2);

	for (int i = 0; i < MAX_ROW; i++)
		free(maze3[i]);
	free(maze3);
}

