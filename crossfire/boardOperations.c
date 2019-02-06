
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "crossfireOperations.h"


void createBoard(struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight)
{
	int i, j;


	for(i =0; i< BOARDSIZE; i++){
		//This allocates in memory the space for the slots in each row of the board
//		board[i] = malloc(BOARDSIZE * sizeof(struct slot));

		//For each slot it sets up the row and column number
		for(j =0;j < BOARDSIZE; j++){
			board[i][j].row = i;
			board[i][j].column = j;
		}
	}

	//It sets up the adjacent slots for the slots that are
	//not at the borders. These slots have 4 adjacent elements
	for(i =1; i< BOARDSIZE-1; i++){
		for(j =1;j < BOARDSIZE-1; j++){
			board[i][j].up = &board[i-1][j];
			board[i][j].right = &board[i][j+1];
			board[i][j].down = &board[i+1][j];
			board[i][j].left = &board[i][j-1];
		}
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last row, except the slots at the edges.
	//
	for(j = 1; j < BOARDSIZE -1; j++){
		//It sets up the adjacent slots for the slots that are in the first row.
		//These slots don't have an adjacent element on top of them
		// because they are on the first row of the board
		board[0][j].right = &board[0][j+1];
		board[0][j].left = &board[0][j-1];
		board[0][j].down = &board[1][j];

		//It sets up the adjacent slots for the slots that are in the last row.
		//These slots don't have an adjacent element on down them
		// because they are on the last row of the board
		board[BOARDSIZE - 1][j].right = &board[BOARDSIZE - 1][j+1];
		board[BOARDSIZE -1][j].left = &board[BOARDSIZE - 1][j-1];
		board[BOARDSIZE - 1][j].up = &board[BOARDSIZE - 2][j];
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last column, except the slots at the edges.
	//
	for(i = 1; i < BOARDSIZE -1; i++){
		//It sets up the adjacent slots for the slots that are in the first column.
		//These slots don't have an adjacent element on the left
		// because they are on the first column of the board
		board[i][0].right = &board[i][1];
		board[i][0].up = &board[i-1][0];
		board[i][0].down = &board[i+1][0];

		//It sets up the adjacent slots for the slots that are in the last column.
		//These slots don't have an adjacent element on the right
		// because they are on the last column of the board
		board[i][BOARDSIZE-1].left = &board[i][BOARDSIZE-2];
		board[i][BOARDSIZE -1].up = &board[i -1][BOARDSIZE-1];
		board[i][BOARDSIZE -1].down = &board[i+1][BOARDSIZE -1];
	}


	//It sets up the adjacent slots for the slot at position (0,0).
	//This only has only 2 adjacent slots: right and down
	board[0][0].right = &board[0][1];
	board[0][0].down = &board[1][0];

	//It sets up the adjacent slots for the slot at position (0,BOARDSIZE -1).
	//This only has only 2 adjacent slots: left and down
	board[0][BOARDSIZE-1].left = &board[0][BOARDSIZE-2];
	board[0][BOARDSIZE -1].down = &board[1][BOARDSIZE -1];

	//It sets up the adjacent slots for the slot at position (boarSize -1 ,0).
	//This only has only 2 adjacent slots: up and right
	board[BOARDSIZE -1][0].right = &board[BOARDSIZE -1][1];
	board[BOARDSIZE -1][0].up = &board[BOARDSIZE -2][0];

	//It sets up the adjacent slots for the slot at position (boarSize -1 ,BOARDSIZE-1).
	//This only has only 2 adjacent slots: up and left
	board[BOARDSIZE - 1][BOARDSIZE-1].up = &board[BOARDSIZE-2][BOARDSIZE-1];
	board[BOARDSIZE - 1][BOARDSIZE -1].left = &board[BOARDSIZE -1][BOARDSIZE -2];





	//assigns a pointer to slot at position (0, 0)
	*upLeft = &board[0][0];
	//assigns pointer of pointer to slot at position (0, BOARDSIZE -1)
	*upRight = &board[0][BOARDSIZE -1];
	//assigns pointer of pointer to slot at position ( BOARDSIZE -1,)
	*downLeft = &board[BOARDSIZE -1][0];
	//assigns pointer of pointer to slot at position (BOARDSIZE -1, BOARDSIZE -1)
	*downRight = &board[BOARDSIZE -1][BOARDSIZE -1];

}

void BoardType()
{
	int i, j;

	printf("\n\nBoard is:\n");

	for(i=0; i<BOARDSIZE; i++)
		{
			for(j=0; j<BOARDSIZE; j++)
			{
				int random;
				random = (1+rand()%3);

				if(random==1)			//if random is 1 then slot type is ground
				{
					strcpy(&board[i][j].type, "Ground");
				}
				else if(random==2)		//if it's 2 then slot type is city
				{
					strcpy(&board[i][j].type, "City");
				}
				else					//if it's 3 then slot type is hill
				{
					strcpy(&board[i][j].type, "Hill");
				}
				printf("(%d,%d): %s\t", i, j, &board[i][j].type);
			}
			printf("\n");
		}

}

/* recursive bit
 *
 * The recursive function that traverses the board to find the slots at a predefined
 * distance from the current slot and place them in foundSlots.
 * Parameters:
 * 	reqDist: the required distance from the starting slot
 * 	currDist: the distance of the current slot from the starting slot
 * 	currSlot: a pointer to the current slot that is traversed
 * 	foundSlots: the array of slots that are at a required distance from the starting slot
 * 	count: pointer to an integer representing the number of slots that are found to be at a required distance from the starting slot
 * 	explored: matrix indicating for each slot at row x and column y has been traversed (true) or not (false)
*/
void findSlots(int reqDist, int currDist,  struct slot * currSlot, struct slot * foundSlots, int * count,  bool explored[BOARDSIZE][BOARDSIZE])
{
//	printf("\ncount is %d", *count);	test
//	printf("currSlot[%d][%d]\n", currSlot->row, currSlot->column);		test
	//The base case: the current slot is at the required distance from the starting slot
	if(currDist == reqDist)
	{
//		printf("findslots test 1\n");
		//the current slot was not explored
		if(explored[currSlot->row][currSlot->column] == false)
		{
//			printf("findslots test 2\n");
			//The next available position (indicated by count) at foundSlots points to the current slot
			*(foundSlots + *count) = *currSlot;
//			printf("findslots test 2a\n");
			//the counter is incremented
			(*count)++;
//			printf("findslots test 2b\n");
			//the matrix of the explored slots set to true the element at the row and column of the current slot
			explored[currSlot->row][currSlot->column] = true;
		}
	}
	//The recursive call: the current slot is at a distance that is less than the required distance (more slots still have to be traversed)
	else
	{
//		printf("findslots test 3\n");
		//if the current slot has the up slot != NULL (i.e. the slot is not in the first row)
		if(currSlot->up != NULL)
		{
//			printf("UP\n");		test
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the up slot
			findSlots(reqDist, currDist +1,  currSlot->up, foundSlots, count, explored);
		}
		//if the current slot has the right slot != NULL (i.e. the slot is not in the last column)
		if(currSlot->right != NULL)
		{
//			printf("RIGHT\n");	test
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the right slot
			findSlots(reqDist, currDist +1,  currSlot->right, foundSlots, count, explored);
		}
		//if the current slot has the down slot != NULL (i.e. the slot is not in the last row)
		if(currSlot->down != NULL)
		{
//			printf("DOWN\n");	test
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the down slot
			findSlots(reqDist, currDist +1,  currSlot->down, foundSlots, count, explored);
		}
		//if the current slot has the left slot != NULL (i.e. the slot is not in the first column)
//		if(currSlot->left != NULL)			//This bit doesn't work
//		{
//			printf("LEFT\n");
//			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the left slot
//			findSlots(reqDist, currDist +1,  currSlot->left, foundSlots, count, explored);
//		}
		if (currSlot->column - 1 >= 0 && currSlot->column -1 < BOARDSIZE) {
			findSlots(reqDist, currDist +1,  &board[currSlot->row][currSlot->column -1], foundSlots, count, explored);
		}
	}
}
