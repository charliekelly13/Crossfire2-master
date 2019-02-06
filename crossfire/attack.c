/*
 * attack.c
 *
 *  Created on: 10 Apr 2017
 *      Author: Órla
 */

#include <stdio.h>
#include "crossfireOperations.h"



void disattack(int attacker, int attacked)		//distant attack function
{
	if(Player[attacker].Dexterity>Player[attacked].Dexterity)	//If the dexterity points of the attacked player are greater than the attacked players dexterity
	{		//the attacker life points = life points - 0.3 * (attacker player’s strength points).
		Player[attacked].LifePoints = Player[attacked].LifePoints - (0.3*Player[attacker].Strength);
	}
}

void magicattack(int attacker, int attacked)		//magic attack function
{
	Player[attacked].LifePoints = Player[attacked].LifePoints - ((0.5* Player[attacker].MagicSkills)+(0.2 * Player[attacker].Smartness));
}

void nearattack(int attacker, int attacked)		//attack function
{
	if(Player[attacked].Strength>70)//If the Strength points of the attacked player are <=70,
	{						//then attacked player life points = life points - 0.5 * his/her Strength points.
		Player[attacker].LifePoints=Player[attacker].LifePoints - (0.3* Player[attacked].Strength);
	}
	else if(Player[attacked].Strength<=70)	//If the strength points of the attacked player are > 70,
	{								//the attacker life points = life points - 0.3 * (attacked player’s strength points).
		Player[attacked].LifePoints=Player[attacked].LifePoints - (0.5* Player[attacker].Strength);
	}
}