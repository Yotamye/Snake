
//Snake Project, by Yotam Yehiely, id 315577437

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define COLS 75
#define ROWS 25
#define WALL '#'
#define BLANK ' '

typedef struct {
	char sign;
	int x;
	int y;
}Snake;

void gotoxy(int y, int x);
int getKey();
void print_Menu();
void print_gameover();
void printBoard(char board[][COLS]);
void createTable(char board[][COLS]);
void buildFirstSnake(Snake snake[]);
void moveSnake(Snake snake[],char board[][COLS], char key, int Logsize);
bool selfEating(Snake head, char board[][COLS]);
void put_Dollar(char board[][COLS]);
bool if_Eat(Snake head, char board[][COLS]);
void snake_Grow(Snake snake[], char board[][COLS], int Logsize);
bool if_dead(Snake snake, char board[][COLS]);
Snake Calc_nextmove(Snake head, char c);
void print_winner();
int if_valid(int menu_choice);

void main() {

	char key;
	int x = 40, y = 12, dollar_Count;  // x for coulmn and y for line 
	bool alive = true;
	int menuChoice, snakeSize;
	Snake next_head;
	Snake snake[20];

	print_Menu();
	scanf("%d", &menuChoice);
	menuChoice = if_valid(menuChoice);

	
	while (menuChoice != 0) 
	{
		if (menuChoice == 1)
		{
			char board[ROWS][COLS];
			createTable(board);
			system("cls");
			printBoard(board);
			put_Dollar(board);
			dollar_Count = 0;
			snakeSize = 10;
			buildFirstSnake(snake);
			board[y][x] = '@';
			gotoxy(x, y);	// start from the middle of the screen
			key = RIGHT;		// the direction is Right untill an arrow hit
			while (!_kbhit()); // wait untill a key is pressed
			alive = true;

			while (key != ESC)
			{

				Sleep(100);	// sleeps for less than half a second before continuing to the next command
				if (_kbhit()) // if any key was hit
					key = getKey();  // change direction

				gotoxy(snake[snakeSize - 1].x, snake[snakeSize - 1].y); // go to tail and "delete" it
				printf("%c", BLANK);

				next_head = Calc_nextmove(snake[0], key);
				if (if_dead(next_head, board) == true) // checks all the situations that you lose the game
					alive = false;

				if (if_Eat(next_head, board))	// checks if the snake eat a dollar
				{
					put_Dollar(board);
					dollar_Count++;
					snakeSize++;
					snake_Grow(snake, board, snakeSize);

				}

				moveSnake(snake, board, key, snakeSize); // make a move in current direction if no change

				for (int i = 0; i < snakeSize; i++) { // print the snake in the board
					gotoxy(snake[i].x, snake[i].y);
					printf("%c", snake[i].sign);
				}

				gotoxy(26, 81);
				printf("Dollars eat: %d    ", dollar_Count);
				printf("Snake Size: %d", snakeSize);


				if (alive == false)
				{
					gotoxy(26, 81);
					system("cls\n");
					print_gameover();
					break;

				}
				else if (dollar_Count == 10)
				{
					gotoxy(26, 81);
					system("cls\n");
					print_winner();
					break;
				}
			}
			print_Menu();
			scanf("%d", &menuChoice);
			if_valid(menuChoice);
		}

		else
			exit(1);
	}
}

void print_Menu() {

	printf("***************************************************\n\n");
	printf("                  WELCOME TO SANKE\n\n");
	printf("                PLEASE CHOOSE OPTION : \n\n");
	printf("                  0. Exit\n");
	printf("                  1. Basic lvl \n\n");
	//printf("                  2. Intermidiate lvl \n");
	//printf("                  3. Advanced lvl \n\n");
	printf("***************************************************\n");
}
void gotoxy(int y, int x) {
	// // This function moves the cursor to the line y and column x.
	// (0,0) is the upper left corner of the screen.
	printf("\x1b[%d;%df", x + 1, y + 1);
}
int getKey(){
	// This function gets keyCodes for special keys such as arrows , escape and others
	// It returns the Key code such as 72 for arrow up, 27 for Escape... 
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}
void createTable(char board[][COLS]) {
	// gets the board and initializing it with walls and spaces
	for (int i = 0; i < 75; i++)	//first row of walls.
		board[0][i] = WALL;
		printf("\n");
	for (int row = 1; row < ROWS-1; row++)	// for every row
	{
		board[row][0] = WALL;

			for (int spaces = 1; spaces < COLS-1; spaces++)
				board[row][spaces] = BLANK;

board[row][COLS - 1] = WALL;
printf("\n");
	}
	for (int k = 0; k < 75; k++)	//last row of walls.
		board[ROWS - 1][k] = WALL;
	printf("\n");
}
void printBoard(char board[][COLS]) {
	// gets the board and printing it on the screen

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
}
void buildFirstSnake(Snake snake[]) {
	// gets the snake and initializing the snake to first position 
	for (int i = 0; i < 10; i++)
	{
		snake[i].sign = '@';
		snake[i].x = 40;
		snake[i].y = 12;
	}
}
void moveSnake(Snake snake[], char board[][COLS], char key, int Logsize) {
	// gets the snake, the board, a key, and the true size of the snake, and making the snake move acrross the board and update the real data on the board

	board[snake[Logsize - 1].y][snake[Logsize - 1].x] = BLANK; //update tail
	for (int i = Logsize - 1; i > 0; i--)
	{
		snake[i] = snake[i - 1];			// moving evry box forward in the snake
		board[snake[i].y][snake[i].x] = snake[i].sign; // change the board to the exact sign

	}
	
	snake[0] = Calc_nextmove(snake[0], key);
	board[snake[0].y][snake[0].x] = snake[0].sign;
	
}
bool selfEating(Snake head, char board[][COLS]) {
	// gets the head of the snake and the board and check if the snake hit itself
	if (board[head.y][head.x] == '@')
		return true;
	return false;
}
void put_Dollar(char board[][COLS]) {

	// gets the board and initialize it with DOLLAR, randomly

	bool empty = false;
	int num1, num2;
	srand(time(NULL));

	while (empty == false)
	{
		num1 = 1 + rand() % ROWS - 2;
		num2 = 1 + rand() % COLS - 2;
		if (board[num1][num2] == BLANK)
		{
			empty = true;
		}
	}

	board[num1][num2] = '$';
	gotoxy(num2, num1);
	printf("$");
}
bool if_Eat(Snake head, char board[][COLS]) {
	// gets the head of the snake and the board and check if the snake eat a DOLLAR
	if (board[head.y][head.x] == '$') {		
		return true;
	}
	return false;
}
void snake_Grow(Snake snake[], char board[][COLS], int Logsize) {
	// gets te snake and the board and copy the tail from the previes box
	snake[Logsize - 1] = snake[Logsize - 2]; 
}
bool if_dead(Snake snake, char board[][COLS]){
	// gets the snake and he board and retuen true if the snake died in some way (self eating or hiting a wall) or flase if none of them happend
	if (selfEating(snake, board)) // if eat itself
		return true;
	
	else if (snake.x == 0 || snake.x == COLS - 1 || snake.y == 0 || snake.y == ROWS - 1) // if touched a wall
	
		return true;

	else				// the snake is alive
		return false;
}
Snake Calc_nextmove(Snake head, char c) {
	// gets the head of the sanke and the board and calculate the next direction of the snake
	switch (c)
	{
	case RIGHT:
		head.x++;
		break;
	case DOWN:
		head.y++;
		break;
	case LEFT:
		head.x--;
		break;
	case UP:
		head.y--;
		break;
	}
	return head;
}
void print_gameover() {
// prints loser screen
	printf("***************************************************\n");
	printf("                  \n");
	printf("                  \n");
	printf("                  GAME OVER !!!!\n");
	printf("                  \n");
	printf("                  \n");
	printf("***************************************************\n");
}
void print_winner() {
	// prints winenrs screen
	printf("***************************************************\n");
	printf("                  \n");
	printf("                  \n");
	printf("                  WINNER !!!!\n");
	printf("                  \n");
	printf("                  \n");
	printf("***************************************************\n");
}
int if_valid(int menu_choice) {
	// gets the munu choice, and make sure to enter the game after the menu choise is valid

	while (menu_choice != 0 && menu_choice != 1)
	{
			//gotoxy(26, 81);
			printf("please enter 1 or 0 : ");
			scanf("%d", &menu_choice);
	}
	return menu_choice;
}