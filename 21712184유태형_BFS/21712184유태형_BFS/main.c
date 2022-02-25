#pragma warning(disable: 4996)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAX_ROW 30
#define MAX_COL 40
#define BUF_SIZE 255
#define MAX_QUEUE_SIZE 1200
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
	short int parent_row;
	short int parent_col;
}element;
element queue[MAX_QUEUE_SIZE];


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
element delete(int*, int*);
int isFull(int);
int isEmpty(int, int);
void path(int**, int**);
void printResult(int**, int**, int, int);

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
	int front, rear;
	int i, row, col, next_row, next_col, dir, parent_row, parent_col;
	int found = FALSE;
	element position;

	mark[1][1] = 1; front = -1; rear = 0;
	queue[0].row = 1; queue[0].col = 1; queue[0].dir = 0; queue[0].parent_row = 0; queue[0].parent_col = 0;
	while ((front != rear) && !found) {
		position = delete(&front, &rear);
		row = position.row;
		col = position.col;
		dir = position.dir;
		parent_row = position.parent_row;
		parent_col = position.parent_col;

		while (dir < 8 && !found) {
			next_row = row + move[dir].x;
			next_col = col + move[dir].y;
			if (next_row == EXIT_ROW && next_col == EXIT_COL)
				found = TRUE;
			else if (!maze[next_row][next_col] && !mark[next_row][next_col]) {
				position.row = next_row;
				position.col = next_col;
				position.parent_row = row;
				position.parent_col = col;
				position.dir = 0;
				add(&rear, position);
				mark[next_row][next_col] = 1;
				++dir;
			}
			else
				++dir;

		}
	}

	if (found)
	{
		position.row = next_row;
		position.col = next_col;
		position.parent_row = row;
		position.parent_col = col;
		position.dir = 0;
		add(&rear, position);

		printResult(mark, maze, front, rear);
	}
	else
	{
		printf("출구를 찾지 못했습니다!!\n");
	}
}

void printResult(int** mark, int** maze, int front, int rear)
{
	int row, col, mar, maz;
	char result[MAX_ROW][MAX_COL];
	int node_count = 0;
	int load_count = 0;
	element trace_node;

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

	trace_node = queue[rear];
	result[trace_node.row][trace_node.col] = 'O';
	load_count++;

	while (trace_node.parent_row != 0 && trace_node.parent_col != 0) {
		for (int i = 0; i <= rear; i++) {
			if (trace_node.parent_row == queue[i].row && trace_node.parent_col == queue[i].col) {
				trace_node = queue[i];
				result[trace_node.row][trace_node.col] = 'O';
				load_count++;
			}
		}
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
	printf("해답경로 길이 = %d\n", load_count);
	printf("\n\n\n\n");
}

void add(int* rear, element position)
{
	if (isFull(*rear))
	{
		printf("Queue is Full!!\n");
		exit(-1);
	}
	else
	{
		(*rear)++;
		queue[*rear].row = position.row;
		queue[*rear].col = position.col;
		queue[*rear].dir = position.dir;
		queue[*rear].parent_row = position.parent_row;
		queue[*rear].parent_col = position.parent_col;
	}
}


element delete(int* front, int* rear)
{
	element temp;
	if (isEmpty(*front, *rear))
	{
		printf("Queue is Empty!!\n");
		exit(-1);
	}
	else
	{
		(*front)++;
		temp.row = queue[*front].row;
		temp.col = queue[*front].col;
		temp.dir = queue[*front].dir;
		temp.parent_row = queue[*front].parent_row;
		temp.parent_col = queue[*front].parent_col;
	}
	return temp;
}

int isFull(int rear)
{
	if (rear >= (MAX_QUEUE_SIZE - 1))
		return TRUE;
	else
		return FALSE;
}

int isEmpty(int front, int rear)
{
	if (front == rear)
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

