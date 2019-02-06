
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "crossfireOperations.h"


int main()
{
	srand(time(NULL));
	int Player_Num, i;
	setvbuf(stdout, 0, _IONBF, 0);
	Player_Num = InputPlayers(); //calls function to set up the players
	
	//pointer to slot (0,0)
	struct slot *upLeft;

	//pointer to slot (0,boardSize -1)
	struct slot *upRight;

	//pointer to slot (boardSize - 1, 0)
	struct slot *downLeft;

	//pointer to slot (boardSize - 1, boardSize -1)
	struct slot *downRight;


	//Creates the board
	createBoard(&upLeft, &upRight, &downLeft, &downRight);
	
	BoardType();//assign random types to board
	assignPlace(Player_Num);//places players on a slot

	for(i=0; i<Player_Num; i++)
	{
		boost(i);//changes stats of players depending on slot type
	}

	turns(Player_Num, upLeft, upRight, downLeft, downRight); //player turns
	
	for(i=0; i<Player_Num; i++)//cycles through players
	{
		if(Player[i].LifePoints>0)//prints the one alive player left
		{
			printf("\n\nCongratulations %sYou have won the game!!!", Player[i].Name);
		}
	}

	return 0;
}