
/* * Players.c
 *
 *  Created on: 31 Mar 2017
 *      Author: Órla
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crossfireOperations.h"



void type(struct Players *Player) //player type function
{
	char cho='y';
	while(cho!='W')	//checks if the option is valid
	{
		printf("Enter W for Wizard, O for Ogre, E for Elf or H for Human: ");
		scanf("%c", &cho);
		getchar();
		if(cho =='W' || cho =='w' || cho =='H' || cho =='h' || cho =='E' || cho =='e' || cho =='O' || cho =='o')
			break;
	}

	if(cho=='W' || cho=='w')
	{
		strcpy(Player->Race, "Wizard");
	}
	if(cho=='H' || cho=='h')
	{
		strcpy(Player->Race, "Human");
	}
	if(cho=='E' || cho=='e')
	{
		strcpy(Player->Race, "Elf");
	}
	if(cho=='O' || cho=='o')
	{
		strcpy(Player->Race, "Ogre");
	}
	return;
}

void stat(struct Players *Player)		//player stat function
{
	Player->LifePoints = 100;	//all players lifepoints are set to 100

	if(strcmp(Player->Race, "Ogre")==0)		//min+rand()%(max+1-min)
	{
		Player->Strength= 80+rand()%21;				//[80,100]
		Player->MagicSkills= 0;						//0
		Player->Dexterity= 80+rand()%21;			//[80,100]
		Player->Luck= rand()%51;					//[0,50]
		Player->Smartness= rand()%21;				//[0,20]
		while(Player->Luck+Player->Smartness>=50)	//condition to check if luck + smartness is over 50
		{
			Player->Luck= rand()%51;
			Player->Smartness= rand()%21;
		}
	}

	if(strcmp(Player->Race, "Human")==0)	//all can be [1,100]
	{
		Player->Strength= 1+rand()%100;
		Player->MagicSkills= 1+rand()%100;
		Player->Dexterity= 1+rand()%100;
		Player->Luck= 1+rand()%100;
		Player->Smartness= 1+rand()%100;
		while(Player->Strength+Player->Dexterity+Player->MagicSkills+Player->Luck+Player->Smartness >=300) //check if sum total is over 300
		{
			Player->Strength= 1+rand()%100;
			Player->MagicSkills= 1+rand()%100;
			Player->Dexterity= 1+rand()%100;
			Player->Luck= 1+rand()%100;
			Player->Smartness= 1+rand()%100;
		}
	}

	if(strcmp(Player->Race, "Elf")==0)
	{
		Player->Strength= 1+rand()%50;		//[1,50]
		Player->MagicSkills=51+rand()%29;	//(50,80)
		Player->Dexterity= 1+rand()%100;	//[1,100]
		Player->Luck= 60+rand()%41;			//[60,100]
		Player->Smartness= 70+rand()%31;	//[70,100]
	}

	if(strcmp(Player->Race, "Wizard")==0)
	{
		Player->Strength= 1+rand()%20;		//[1,20]
		Player->MagicSkills= 80+rand()%21;	//[80,100]
		Player->Dexterity= 1+rand()%100;	//[1,100]
		Player->Luck= 50+rand()%51;			//[50,100]
		Player->Smartness= 90+rand()%11;	//[80,100]
	}

	return;
}

void assignPlace(const int Player_Num)
{
	srand(time(NULL));
	int i, row, column;

	for(i=0; i<Player_Num; i++)
	{
		row = rand()%BOARDSIZE; //places random row into variable
		column = rand()%BOARDSIZE;	//places random column into variable
		Player[i].PlaceRow = row;
		Player[i].PlaceColumn = column;
		printf("\n%s"
				"Is on row=%d, "
				"column=%d ,"
				"which is %s\n",
				Player[i].Name,
				Player[i].PlaceRow,
				Player[i].PlaceColumn,
				&board[Player[i].PlaceRow][Player[i].PlaceColumn].type);
	}
}

void boost(int i)
{
	int row = Player[i].PlaceRow;
	int column = Player[i].PlaceColumn;

	if(!strcmp(&board[row][column].type, "Hill"))
		{
			if(Player[i].Dexterity >= 60)		//If Dexterity >= 60, then the player gains 10 Strength points
			{
				Player[i].Strength = Player[i].Strength + 10;
			}
			if(Player[i].Dexterity < 50)		//If Dexterity < 50, then the player loses 10 Strength points
			{
				Player[i].Strength = Player[i].Strength - 10;
			}
		}
		if(!strcmp(&board[row][column].type, "City"))
		{
			if(Player[i].Smartness > 60)		//If Smartness > 60, then the player gains 10 Magic Skills points
			{
				Player[i].MagicSkills = Player[i].MagicSkills + 10;
			}
			if(Player[i].Smartness <= 50)		//If Smartness <=50, then the player loses 10 Dexterity points.
			{
				Player[i].Dexterity = Player[i].Dexterity - 10;
			}
		}
		if(Player[i].MagicSkills > 100)
		{
			Player[i].MagicSkills = 100;
		}
		if(Player[i].Strength > 100)
		{
			Player[i].Strength = 100;
		}
		if(Player[i].Dexterity > 100)
		{
			Player[i].Dexterity = 100;
		}
		if(Player[i].Dexterity < 0)
		{
			Player[i].Dexterity = 0;
		}
		if(Player[i].Strength < 0)
		{
			Player[i].Strength = 0;
		}
}

void deboost(int i)	//when moving off a slot takes away given stats and returns them to what was given at start
{
	int row = Player[i].PlaceRow;
	int column = Player[i].PlaceColumn;


	if(strcmp(&board[row][column].type, "Hill")==0)
	{
		if(Player[i].Dexterity >= 60)
		{
			Player[i].Strength = Player[i].Strength - 10;
		}
		if(Player[i].Dexterity < 50)
		{
			Player[i].Strength = Player[i].Strength + 10;
		}
	}
	if(strcmp(&board[row][column].type, "City")==0)
	{
		if(Player[i].Smartness > 60)
		{
			Player[i].MagicSkills = Player[i].MagicSkills - 10;
		}
		if(Player[i].Smartness <= 50)
		{
			Player[i].Dexterity = Player[i].Dexterity + 10;
		}
	}
	if(Player[i].MagicSkills > 100)
	{
		Player[i].MagicSkills = 100;
	}
	if(Player[i].Strength > 100)
	{
		Player[i].Strength = 100;
	}
	if(Player[i].Dexterity > 100)
	{
		Player[i].Dexterity = 100;
	}
	if(Player[i].Dexterity < 0)
	{
		Player[i].Dexterity = 0;
	}
	if(Player[i].Strength < 0)
	{
		Player[i].Strength = 0;
	}
}