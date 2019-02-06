#ifndef CROSSFIREOPERATIONS_H_
#define CROSSFIREOPERATIONS_H_
#define BOARDSIZE 7
#define MIN_DISTANCE 2		//min distance
#define MAX_DISTANCE 4		//max distance

#endif /* CROSSFIREOPERATIONS_H_ */


/*
 * Definition of boolean types
 * This avoids using <stdbool.h>
 */
typedef int bool;
enum { false, true };

enum dead {
	alive,
	dead,
	quit
	};

struct slot{
	//row number
	int row;
	//column number
	int column;
	//type of slot city, ground, hill
	char type;

	// adjacent left slot
	struct slot *left;
	// adjacent right slot
	struct slot *right;
	// adjacent up slot
	struct slot *up;
	// adjacent down slot
	struct slot *down;
}board[BOARDSIZE][BOARDSIZE];

struct Players	//struct for players
{
   char	Name[25];
   char	Race[7];
   int	Strength;
   int	MagicSkills;
   int	Dexterity;
   int	Luck;
   int	Smartness;
   int	LifePoints;
   int	PlaceRow;
   int	PlaceColumn;
}Player[6];

/*
 * FUNCTIONS PROTOTYPES
 */

/*
 * This function creates the board
 * Parameters:
 * 	boardSize: the size of the board
 * 	upLeft: pointer of pointer to slot at position (0, 0)
 * 	upRight: pointer of pointer to slot at position (0, size -1)
 * 	downLeft: pointer of pointer to slot at position (size -1, 0)
 * 	upLeft: pointer of pointer to slot at position (size - 1, size -1)
 */
void createBoard(struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight);


/*
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
void findSlots(int reqDist, int currDist,  struct slot * currSlot, struct slot * foundSlots, int * count,  bool explored[BOARDSIZE][BOARDSIZE]);



int InputPlayers();		//input for player, name, type calls type & stat function
void boost(int i);		//changing stats depending on slot type
void deboost(int i);	//removing stats when player moves off some slot types
void type(struct Players *Player);		//give player a type
void stat(struct Players *Player);		//give player stats
void move(struct Players *Player);		//function to move players
void assignPlace(const int Player_Num);	//place players on slots

void BoardType();		//assigns random type(ground/city/hill) onto slots
void magicattack(int attacker, int attacked);			//function for magic attack
void disattack(int attacker, int attacked);				//function for distance attack
void nearattack(int attacker, int attacked);			//near attack function

//function which cycles through turns, calls boost, deboost, move, magicattack, disattack, nearattack and findSlots functions
//depending on option slected
void turns(const int Player_Num, struct slot *upLeft, struct slot *upRight, struct slot *downLeft, struct slot *downRight);
