#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crossfireOperations.h"

int InputPlayers()
{
	int PlayerNum, j;

	printf("Enter the number of players you want, between 2 and 6:"); //gives number of players
	scanf("%d", &PlayerNum);
	getchar();

	while(PlayerNum<2 || PlayerNum>6)	//if the correct number isn't chosen
	{
		printf("\nEnter a number between 2 and 6: "); //gives number of players
		scanf("%d", &PlayerNum);
		getchar();		//so it reads in the new line char
	}

	for(j=0; j<PlayerNum; j++)	//player name/type/stat
	{
		printf("\n\nEnter player name: ");
		fgets (Player[j].Name, 25, stdin);		//to read in second names
		type(&Player[j]);		//assign type
		stat(&Player[j]);		//assign stats
		printf("\nPlayer [%d]: %s"
				"Player Type: %s\n"
				"Strength: %d\n"
				"Magic: %d\n"
				"Dexterity: %d\n"
				"Luck: %d\n"
				"Smartness: %d\n",
				j+1, Player[j].Name,
				Player[j].Race,
				Player[j].Strength,
				Player[j].MagicSkills,
				Player[j].Dexterity,
				Player[j].Luck,
				Player[j].Smartness);
	}
	return PlayerNum;
}

void move(struct Players *Player)	//function to move players
 {
 	int right=0, left=0, down=0, up=0, e;
	
	//if statements to check if it can't move a certain way
 	if(board[Player->PlaceRow][Player->PlaceColumn].column==BOARDSIZE-1)
 	{
 		right=1;
		printf("You can't move right.\n");
 	}
 	if(board[Player->PlaceRow][Player->PlaceColumn].column==0)
 	{
 		left=1;
		printf("You can't move left.\n");
 	}
 	if(board[Player->PlaceRow][Player->PlaceColumn].row==BOARDSIZE-1)
 	{
 		down=1;
		printf("You can't move down.\n");
 	}
 	if(board[Player->PlaceRow][Player->PlaceColumn].up==0)
 	{
 		up=1;
		printf("You can't move up.\n");
 	}

	//input commands if they can move a certain way, designed on the number keypad
 	if(right==0)
 	{
 		printf("Enter 6 to move right\n");
 	}
 	if(left==0)
 	{
 		printf("Enter 4 to move left\n");
 	}
 	if(down==0)
 	{
 		printf("Enter 2 to move down\n");
 	}
 	if(up==0)
 	{
 		printf("Enter 8 to move up\n");
 	}

 	scanf("%d", &e);
 	getchar();
 	while(e!=2 && e!=4 && e!=6 && e!=8)		//if those numbers aren't entered
 	{
 		printf("Enter a valid number");
 		scanf("%d", &e);
 		getchar();
 	}
	//if statements to move players
 	if(e==4 && Player->PlaceColumn!=0)
 	{
 		Player->PlaceColumn = Player->PlaceColumn - 1;
 	}
 	if(e==6 && Player->PlaceColumn!=(BOARDSIZE-1))
 	{
 		Player->PlaceColumn = Player->PlaceColumn + 1;
 	}
 	if(e==2 && Player->PlaceRow != (BOARDSIZE-1))
 	{
 		Player->PlaceRow = Player->PlaceRow + 1;
 	}
 	if(e==8 && Player->PlaceRow != 0)
 	{
 		Player->PlaceRow = Player->PlaceRow - 1;
 	}
	
	//prints new place of the player
 	printf("\n(%d,%d) is now occupied by %s", Player->PlaceRow, Player->PlaceColumn, Player->Name);
 }


void turns(const int Player_Num, struct slot *upLeft, struct slot *upRight, struct slot *downLeft, struct slot *downRight)
{
	int turn, choice, attackchoice, abl[Player_Num], u=0;
	int k, j, i, count, distance, attack_player, counter=0;
	enum dead status[Player_Num];
	int death = Player_Num, anyplayers=0;
	struct slot* currSlot = NULL;
	struct slot *foundSlots;
	bool explored[BOARDSIZE][BOARDSIZE];

	foundSlots = malloc(BOARDSIZE * BOARDSIZE * sizeof(struct slot ));

	for(i=0; i<Player_Num; i++)
	{
		status[i]=alive;
	}

	while(death>1)// While more than one player is alive/ hasn't quit
	{
		for(turn=0; turn<Player_Num; turn++) //move, attack or quit for each player
		{
			//reset variables, anyplayers, abl[k], count, u after each turn
			for(k=0; k<Player_Num; k++)		//reset player attack array to 0's for next attack
			{
				abl[k]=0;
			}
			count=0;		//reset count for next distance attack
			u=0;
			anyplayers=0;
			
			if(death==1)//if theres 3 players left and two die/quit this will break the loop
			{
				break;
			}

			if(Player[turn].LifePoints<=0)	//if the player is dead or quit
			{
				printf("\nPlayer %d is out of the game", turn+1);
			}
			else if(status[turn] == alive)	//current players turn choices
			{
				//print stats after each player
				for(j=0; j<Player_Num; j++)
				{
					if(status[j]==dead)
					{
						printf("\nPlayer %d is dead\n", j+1);
					}
					else  if(status[j]==quit)
					{
						printf("\nPlayer %d has quit\n", j+1);
					}
				}//end for states if player is dead or quit
				
				printf("\nPlayer\tLife Points\tRace\tStrength\tMagic\tDexterity\tLuck\tWisdom\tName\n");
				for(j=0; j<Player_Num; j++)//prints players stats
				{
					if(status[j]==alive)
					{
						printf("%d\t%d\t\t%s\t%d\t\t%d\t%d\t\t%d\t%d\t%s",
								j+1,
								Player[j].LifePoints,
								Player[j].Race,
								Player[j].Strength,
								Player[j].MagicSkills,
								Player[j].Dexterity,
								Player[j].Luck,
								Player[j].Smartness,
								Player[j].Name);
					}
					else
					{
						printf("--------------------------------------------------------------------------------------------\n");
					}
				}
				
				printf("\nPlayer positions:\n");
				for(i=0; i<BOARDSIZE; i++)//cycles rows
				{
					for(j=0; j<BOARDSIZE; j++)//cycles columns
					{
						for(k=0; k<Player_Num; k++)//cycles players
						{
							if((board[i][j].row == Player[k].PlaceRow && board[i][j].column == Player[k].PlaceColumn) && (status[k]==alive))
							{	//prints players positions if they still have life points
								printf("P%d ", k+1);
							}
							else//so it will only print one blank space
							{
								counter++;
							}
							if(counter==Player_Num)	//Print a blank space so long as there's no players still in the game
							{
								printf("_");
							}
						}//end for loop for players
						counter=0;	//reset counter for next space
						printf("\t\t");//needs enough space for more than one player
					}//end for loop for columns
					printf("\n");//next row
				}//end for loop for rows

				printf("\nSlot Types:\n");//prints slot types
				for(i=0; i<BOARDSIZE; i++)//cycles rows
				{
					for(j=0; j<BOARDSIZE; j++)//cycles columns
					{	//if statements prints type
						if(!strcmp(&board[i][j].type, "Ground"))
						{
							printf("Ground");
						}
						if(!strcmp(&board[i][j].type, "City"))
						{
							printf("City");
						}
						if(!strcmp(&board[i][j].type, "Hill"))
						{
							printf("Hill");
						}
						printf("\t\t");
					}//end for loop column
					printf("\n");
				}//end for loop row

				//player[turn] choice - move or attack
				printf("\n%sDo you want to move or attack?\n"
						"Enter 1 to move or "
						"2 to attack or "
						"3 to quit the game\n", Player[turn].Name);
				scanf("%d", &choice);
				getchar();

				while(choice!=1 && choice!=2 && choice!=3)		//checks if choice is valid
				{
					printf("This is not an option!!\n"
							"Enter 1 to move or "
							"2 to attack or "
							"3 to quit the game\n");
					scanf("%d", &choice);
					getchar();
				}

				//if move -> move function
				if(choice==1)
				{
					deboost(turn);		//removes change of stats before moving
					move(&Player[turn]);	//moves player
					boost(turn);		//changes stats according to type of slot
				}

				//if attack -> attack function
				if(choice==2)
				{
					if(Player[turn].Smartness + Player[turn].MagicSkills>150)// Prints if the player is capable of a magic attack
					{
						printf("Enter 1 for a near attack, "
								"2 for a distant attack or "
								"3 for a magic attack: ");
						scanf("%d", &attackchoice);


						while(attackchoice!=1 && attackchoice!=2 && attackchoice!=3)//checks if attackchoice is valid
						{
							printf("That is not a valid input."
									"\nEnter 1 for a near attack, "
									"2 for a distant attack or "
									"3 for a magic attack: ");
							scanf("%d", &attackchoice);
						}//end while check valid
					}//end if magic attack
					else// Prints if player cannot use a magic attack
					{
						printf("Enter 1 for a near attack or "
								"2 for a distant attack:");
						scanf("%d", &attackchoice);

						while(attackchoice!=1 && attackchoice!=2)//checks if input valid
						{
							printf("That is not a valid input."
									"\nEnter 1 for a near attack or "
									"2 for a distant attack:");
							scanf("%d", &attackchoice);
						}//end while check valid
					}//end else no magic

					if(attackchoice==1)//near attack
					{
						k=0;
						while(k<Player_Num) //cycles through players, to find players to attack
						{
							if(k!=turn)//doesn't allow player to attack themselves
							{
								//if+else if Checks every player besides the player whos turn it is. Checks if they are close enough for a near attack to hit them
								if(Player[k].PlaceRow==Player[turn].PlaceRow && (Player[k].PlaceColumn==Player[turn].PlaceColumn || Player[k].PlaceColumn==Player[turn].PlaceColumn +1 || Player[k].PlaceColumn==Player[turn].PlaceColumn-1))
								{
									abl[k]=1;// If a player is near enough then their position in the array is set to 1 to indictae this
									u=1;// Signifies that at least one player is able to be attacked
								}
								else if(Player[k].PlaceColumn==Player[turn].PlaceColumn && (Player[k].PlaceRow==Player[turn].PlaceRow || Player[k].PlaceRow==Player[turn].PlaceRow+1 || Player[k].PlaceRow==Player[turn].PlaceRow-1))
								{
									abl[k]=1;
									u=1;
								}
							}
							k++;
						}//end while
						
						if(u>0)//if there are players to attack
						{
							printf("Your options are:\n");
							k=0;
							while(k<Player_Num)//cycles through players
							{	//if k players is near and it's not the current player and if they are alive
								if(abl[k]==1 && k!=turn && status[k]!= dead && status[k]!=quit)
								{
									printf("P%d %s", k+1, Player[k].Name);// Prints players you are able to attack
								}
								k++;
							}//end while cycle players

							printf("Choose which player to attack P?");
							scanf("%d", &attack_player);
							
							// If the player you input is yourself or a player you 
							//can't attack it will continue to ask for a player to attack
							while(abl[attack_player-1]==0 || status[attack_player-1]== dead || status[attack_player-1]==quit)							
							{
								printf("Choose which player to attack P?");
								scanf("%d", &attack_player);
							}
							
							nearattack(turn, attack_player-1);// 	Calls the near attack function
						}//end if(u>0)
						else//if there are no players to attack
						{
							printf("There are no players for you to attack\n");
							printf("Please choose another option\n");
							turn--;
						}
					}//end if(attackchoice==1)//near attack


					//distance attack
					if(attackchoice==2)
					{
						currSlot = &board[Player[turn].PlaceRow][Player[turn].PlaceColumn]; //input place int currSlot

						for(i=0; i<BOARDSIZE; i++)//cycles row
						{
							for(j=0; j<BOARDSIZE;j++)//cycles columns
							{
								explored[i][j] = false;//set explored board to false
							}
						}

						if(currSlot!= NULL)
						{
							//invokes function findSlots. The required distance is a constant
							//However you can reuse this function providing as input any arbitrary distance
							for(distance=MIN_DISTANCE; distance<=MAX_DISTANCE; distance++)
							{//cals recursive function in a loop between MIN_DISTANCE and MAX_DISTANCE
								findSlots(distance, 0, currSlot, foundSlots, &count, explored);
							}
						}
						
						printf("\nPlayers available to attack!!\n");//find players to attack
						for(int i=0; i<BOARDSIZE; i++)		//cycles rows
						{
							for(j=0; j<BOARDSIZE; j++)//cycles columns
							{
								if(explored[i][j]==true)//looks at the board which as been found in findSlots function
								{
									//printf("[%d][%d]\n", i, j);	//test
									for(k=0; k<Player_Num; k++)//cycles through players
									{
										if(k!=turn)//k can't be current players turn
										{	//if there's a player on the explored board who is alive then then they can be attacked
											if(i==Player[k].PlaceRow && j==Player[k].PlaceColumn && status[k] == alive)
											{
												abl[k]=1;//allows to keep track of players which can be attacked
												anyplayers++;//to know if there is a player to attack
											}//end if
										}//end if(k!=turn)
									}//end for cycle through players
								}//end if (explored[i][j]==true)
							}//end for cycle column
						}//end for cycle rows
						
						if(anyplayers==0)//if there are no players to attack
						{
							printf("There are no other players within range\n");
							printf("Please choose another option\n");
							turn--;
						}
						else//otherwise there are players to attack
						{
							for(k=0; k<Player_Num; k++)//cycles through players
							{
								if(abl[k]==1)//shows players within range
								{
									printf("You can attack Player %d, %s", k+1, Player[k].Name);
								}//end if(abl[k]==1)
							}//end for cycle through players
							
							do//input for options
							{
								printf("Select one of these players to attack Player ?\n");
								scanf("%d", &attack_player);
							}while(abl[attack_player-1]==0 || status[attack_player-1]== dead ||status[attack_player-1]== quit);

							disattack(turn, attack_player-1);//call function disattack
							
						}//end else otherwise there are players to attack			
					}////distance attack if(attackchoice==2)

					//magic attack
					if(attackchoice==3)
					{
						printf("Choose a player to attack:\n");
						for(int k=0; k<Player_Num; k++)//cycles through players
						{
							if(k!=turn && status[k]==alive)// Prints players available to be attacked
							{
								printf("P%d %s", k+1, Player[k].Name);
							}//end if
						}//end for cycles players
						scanf("%d", &attack_player);
						while(attack_player-1==turn || attack_player-1>Player_Num || attack_player<0 || status[attack_player-1]==dead || status[attack_player-1]==quit)
						{// If the input is the players whos turn it is,
							//greater than the amount of players in game,the inputted player
							//is dead or quit or the input is less than 1 the user will be asked to input again
							printf("Choose a player to attack:");
							scanf("%d", &attack_player);
						}//end while
						
						magicattack(turn, attack_player-1);
					}//end magic attack	
				}//end attacks

				//if choice is to quit
				// Changes players value to quit
				if(choice==3)
				{
					Player[turn].LifePoints=0;
					status[turn]=quit;
					death=death-1;
				}//end quit
				
				for(k=0; k<Player_Num; k++)//cycles through players
				{	//if players lifepoints is 0 or less and they're not dead or haven't quit
					if(Player[k].LifePoints<=0 && status[k]!=quit && status[k]!=dead)
					{
						status[k]=dead;
						death=death-1;// Amount of active players decreases
					}
				}
			}//else if(status[turn] == alive)	//current players turn choicess
		}//for(turn=0; turn<Player_Num; turn++) //move, attack or quit for each player
	}//while(death>1)// While more than one player is alive/ hasn't quit
	printf("\nPlayer\tLife Points\tRace\tStrength\tMagic\tDexterity\tLuck\tWisdom\tName\n");
	for(j=0; j<Player_Num; j++)//prints players stats
	{
		if(status[j]==alive)
		{
			printf("%d\t%d\t\t%s\t%d\t\t%d\t%d\t\t%d\t%d\t%s",
					j+1,
					Player[j].LifePoints,
					Player[j].Race,
					Player[j].Strength,
					Player[j].MagicSkills,
					Player[j].Dexterity,
					Player[j].Luck,
					Player[j].Smartness,
					Player[j].Name);
		}
		else
		{
			printf("--------------------------------------------------------------------------------------------\n");
		}
	}
}
