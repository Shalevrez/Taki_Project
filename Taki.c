// Code was written by ID 208276956
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#pragma region Defines
#define NAME_LEN 20
#define MAX_P_DECK 50 // Maximum cards in one's deck
#define FIRST_DRAW 4 // First draw of cards to the players
#define MINIMAL_DECK 4
#define CARDS 14
#define STAT_ARRAY_COLS 2
#define STRING_END '\0'
#define SPACE ' '
#define COLORS 4
#define TURN_TYPE_REGULAR "take a card from the deck" // Message for regular turn
#define TURN_TYPE_TAKI "finish your turn" // Message for open taki
/*Card Dimensions*/
#define CARD_HEIGTH 6
#define CARD_WIDTH 9
#define CARD_TYPE_LINE 2
#define CARD_COLOR_LINE 3
#define CARD_MID_CHAR 4
/*Card types*/
#define FIRST_SPECIAL 10
#define PLUS 10
#define PLUS_T "+"
#define STOP 11
#define STOP_T "STOP"
#define COLOR_CHANGE 12
#define COLOR_CHANGE_T "COLOR"
#define TAKI 13
#define TAKI_T "TAKI"
#define DIRECTION_CHANGE 14
#define DIRECTION_CHANGE_T "<->"
/*Card colors*/
#define YELLOW_I 1
#define YELLOW_C 'Y'
#define RED_I 2
#define RED_C 'R'
#define BLUE_I 3
#define BLUE_C 'B'
#define GREEN_I 4
#define GREEN_C 'G'
#define PLAY_REGULAR 1
#define PLAY_REVERSE -1
#define ZERO 0
#define ONE 1
#define NOT_FOUND -1
#define LINE_TYPE 7
#define LINE_COUNT 4
#pragma endregion

#pragma region Structs
typedef struct card {
	char color;
	int type;
}Card;
typedef struct player {
	int index;
	char first_name[NAME_LEN];
	Card* deck;
	int cards;
}Player;
typedef struct statistic {
	int card_type;
	int count;
}Statistics;
typedef struct game {
	Card last_card;
	int players;
	Player* players_array;
	int direction;
	int last_player;
	Statistics statistics[CARDS];
	char winner_name[NAME_LEN];
	bool winner;
}Game;
#pragma endregion

#pragma region signatures By assignment
int Get_Integer();
void Initialize_Game(Game* game);
void Initialize_Statistics(Statistics* arr, int index);
void Check_Game_Direction(Game* game);
void Free_Game(Game game);
// 1
void Welcome();
// 2
void Print_Get_Players();
int Get_Players();
// 3
void Print_Get_Player_Name(int index);
Player* Players_Array(int players);
// 4
Card Draw_Card();
char Color_Identifier(int num);
void First_Draw(Player* arr, int players);
bool Special_Card(Card card);
void Reset_Decks(Player* arr, int players);
// 5
bool Winner(Player* player, Game* game);
// 5.1
void Print_Card(Card card);
void Print_Char_X_Times(char c, int length);
void Print_5_Chars_Card(Card card);
void Print_4_Chars_Card(Card card);
void Print_3_Chars_Card(Card card);
void Print_1_Chars_Card(Card card);
void Print_Color_Line(char color);
// 5.2
void Print_Upper_Card(Card card);
void Print_Player_Turn(Player player);
// 5.3
void Print_Put_Card(int cards, char* turn_type);
void Add_Card_To_Deck(Player* player);
void Remove_Card_From_Deck(Player* player, int index);
bool Card_In_Range(int cards, int index);
void Player_Turn(Player* player, Game* game, char* turn_type);
void Print_Invalid_Card();
// 5.4
bool In_Range(int input, int range);
void Print_Choose_Color();
void Special_Change_Color(Game* game);
void Special_Change_Direction(Game* game);
int Special_Open_Taki(Player* player, Game* game, char* turn_type);
void Special_Stop_Next(Player* player, Game* game);
// 6
void Print_Winner(char* winner_name);
// 7
void Add_To_Stats(int type, Statistics statistics[CARDS]);
void Print_Stats(Game* game);
void Merge_Sort(Statistics arr[], int size);
void Merge(Statistics arr1[], int size1, Statistics arr2[], int size2, Statistics res[]);
void Copy_Array(Statistics dest[], Statistics src[], int size);
#pragma endregion

void main()
{
	srand(time(NULL)); // Random
	Game game; // Contains the information about the game - Not initialized
	Initialize_Game(&game); // Initialize the game - Reset all the properties of the game

	Welcome(); // 1 - Welcome message
	game.players = Get_Players(); // 2 - Get number of players
	game.players_array = Players_Array(game.players); // 3 - Get an array of players
	Reset_Decks(game.players_array, game.players);

	game.last_card = Draw_Card(); // 4 - Draw the first card of the deck
	while (Special_Card(game.last_card)) // Check if the card is special card (10-14). If so - draw again. Stop when it's not a special card
	{
		game.last_card = Draw_Card();
	}
	Add_To_Stats(game.last_card.type, &game.statistics); // After the first card was declared and set - add this card to the statistics.
	First_Draw(game.players_array, game.players); // The first draw of the game - 4 cards for each player

	// 5 - While there's no winner in the game
	while (!game.winner)
	{
		for (game.last_player = 0; game.last_player < game.players && game.last_player >= 0;) // Run on the players's array, and play the turn for each player.
		{
			Player_Turn(&game.players_array[game.last_player], &game, TURN_TYPE_REGULAR); // Play

			if (Winner(&game.players_array[game.last_player], &game)) // If this player won - "Break" the loop by assignning the last player as a bigger number than allowed in the condition of the loop.
			{
				game.last_player = game.players;
			}
			
			Check_Game_Direction(&game); // Check the game direction and assign the last player's index to the last_player property.
		}

		if (!game.winner) // If there's no winner at the end of the loop, reset the last player to 0.
		{
			game.last_player = 0;
		}
	}

	Print_Winner(&game.winner_name); // Prints the message with the winner's name.
	Print_Stats(&game); // Prints the game statistics.

	
	Free_Game(game); // Free the players's array and deck since theyv'e been dynamically allocated. 
}

void Free_Game(Game game) // Free the game struct's dynamic properties (players_array, decks, cards...)
{
	int i, j;

	for (i = 0; i < game.players; i++)
	{
		for (j = 0; j < game.players_array[i].cards; j++)
		{
			free(game.players_array[i].deck[i].color);
			free(game.players_array[i].deck[i].type);
		}

		free(game.players_array[i].deck);
		free(game.players_array[i].cards);
		free(game.players_array[i].first_name);
		free(game.players_array[i].index);
	}
}
#pragma region Get methods
int Get_Integer() // Get integer input method
{
	int output;
	scanf("%d", &output);

	return output;
}
#pragma endregion
void Initialize_Game(Game* game) // Reset the game properties
{
	int i, j;

	game->last_card.color = NULL;
	game->last_card.type = NULL;
	game->last_player = ZERO;
	game->direction = PLAY_REGULAR;

	for (i = 0; i < CARDS; i++) // Reset the statistics array
	{
		Initialize_Statistics(game->statistics + i, i);
	}


	for (j = 0; j < NAME_LEN; j++) // Reset the winner name - to a "null" name
	{
		game->winner_name[j] = STRING_END;
	}

	game->winner = false;
}
void Initialize_Statistics(Statistics* arr, int index) // Reset the cell of the statistic
{
	arr->card_type = index + 1;
	arr->count = 0;
}
void Check_Game_Direction(Game* game) // Checking the game play-direction, and assaign the right value to the last_player property.
{
	if (game->direction == PLAY_REVERSE) // Check the game direction. If it's reversed than start the play loop from the last player - (players-1). Otherwise, add the direction (-1) to the next player, so the next player will be [INDEX-1]
	{
		if (game->last_player == ZERO)
		{
			game->last_player = game->players - 1;
		}

		else
		{
			game->last_player += game->direction;
		}
	}
	else // If the game direction isn't reversed, than just add +1 to the next player, so the next player will be [INDEX+1]
	{
		game->last_player += game->direction;
	}
}
// 1 - Opening message
void Welcome() // Welcome message
{
	printf("************  Welcome to TAKI game !!! ***********\n");
}
// 2 - Ask and get the players amount
void Print_Get_Players() // Ask the user to enter the number of players playing the game
{
	printf("Please enter the number of players:\n");
}
int Get_Players() // Get the players amount
{
	Print_Get_Players();

	int players = Get_Integer();

	return players;
}
// 3 - Ask for the players's first names
void Print_Get_Player_Name(int index) // Ask each player to enter it's first name
{
	printf("Please enter the first name of player #%d:\n", index);
}
Player* Players_Array(int players) // Get the names from each player, and assign the names to the players's array, in each index
{
	int i;
	Player* arr = (Player*)malloc(sizeof(Player) * players);
	char name[NAME_LEN];

	for (i = 0; i < players; i++)
	{
		Print_Get_Player_Name(i + 1);;
		scanf("%s", arr[i].first_name);
		arr[i].index = i + 1;
	}

	return arr;
}
// 4 - Draw 4 cards into the players's decks
Card Draw_Card() // A method to draw one card
{
	Card card;
	int type = 1 + rand() % 14;
	int color;

	switch (type)
	{
	case COLOR_CHANGE:
		color = SPACE;
		break;
	default:
		color = 1 + rand() % 4;
		break;
	}

	card.type = type;
	card.color = Color_Identifier(color);

	return card;
}
char Color_Identifier(int num) // "Translate" the random number from the rand() method into a character.
{
	char output;

	switch (num)
	{
	case RED_I:
		output = RED_C;
		break;
	case GREEN_I:
		output = GREEN_C;
		break;
	case BLUE_I:
		output = BLUE_C;
		break;
	case YELLOW_I:
		output = YELLOW_C;
		break;
	default:
		output = SPACE;
		break;
	}

	return output;
}
void First_Draw(Player* arr, int players) // Give each player 4 cards to it's own deck
{
	int i, j;
	for (i = 0; i < players; i++) // 4 - Give every player 4 cards at the beginning of the game
	{
		for (j = 0; j < FIRST_DRAW; j++)
		{
			(arr + i)->deck[j] = Draw_Card();
		}

		arr[i].cards = FIRST_DRAW;
	}
}
bool Special_Card(Card card) // Check if the drawed card is special card (taki, plus, stop...)
{
	bool result = false;

	if (card.type >= FIRST_SPECIAL)
	{
		result = true;
	}

	return result;
}
void Reset_Decks(Player* arr, int players) // Reset each player's deck to size of 4 cards
{
	int i;
	Card* tmp;

	for (i = 0; i < players; i++)
	{
		tmp = (Card*)malloc(MINIMAL_DECK * sizeof(Card));


		arr[i].deck = tmp;
	}
}
// 5 - While no winner
bool Winner(Player* player, Game* game) // Check if the user doesn't have any cards left in it's deck. If so - he's the winner
{
	bool result = false;

	if (player->cards == ZERO)
	{
		result = true;
		game->winner = result;
		strcpy(game->winner_name, player->first_name);
	}

	return result;
}
// 5.1 - Print the current top card
void Print_Card(Card card) // Prints a card
{
	// line 1
	Print_Char_X_Times('*', CARD_WIDTH);
	printf("\n");
	// line 2
	Print_Char_X_Times('*', 1);
	Print_Char_X_Times(' ', CARD_WIDTH - 2);
	Print_Char_X_Times('*', 1);
	printf("\n");
	// line 3 - type (switch on type) - also prints the color if needed
	switch (card.type)
	{
	case COLOR_CHANGE:
		Print_5_Chars_Card(card);
		break;
	case TAKI:
		Print_4_Chars_Card(card);
		break;
	case STOP:
		Print_4_Chars_Card(card);
		break;
	case DIRECTION_CHANGE:
		Print_3_Chars_Card(card);
		break;
	default:
		Print_1_Chars_Card(card);
		break;
	}
	// line 5
	Print_Char_X_Times('*', 1);
	Print_Char_X_Times(' ', CARD_WIDTH - 2);
	Print_Char_X_Times('*', 1);
	printf("\n");
	// line 6
	Print_Char_X_Times('*', CARD_WIDTH);
}
void Print_5_Chars_Card(Card card) // Print 5 characters's card type line with it's color
{
	switch (card.type)
	{
	case COLOR_CHANGE:
		printf("* COLOR *\n");
		Print_Color_Line(card.color);
		break;
	default: // Ready for an implementation of another 5 characters type card
		break;
	}
}
void Print_4_Chars_Card(Card card) // Print 4 characters's card type line with it's color
{
	switch (card.type)
	{
	case TAKI:
		printf("*  TAKI *\n");
		Print_Color_Line(card.color);
		break;
	default:
		printf("*  STOP *\n");
		Print_Color_Line(card.color);
		break;
	}
}
void Print_3_Chars_Card(Card card) // Print 3 characters's card type line with it's color
{
	switch (card.type)
	{
	case DIRECTION_CHANGE:
		printf("*  <->  *\n");
		Print_Color_Line(card.color);
		break;
	default: // Ready for an implementation of another 3 characters type card
		break;
	}
}
void Print_1_Chars_Card(Card card) // Print 1 character's card type line with it's color
{
	switch (card.type)
	{
	case PLUS:
		printf("*   +   *\n");
		Print_Color_Line(card.color);
		break;
	default:
		printf("*   %d   *\n", card.type);
		Print_Color_Line(card.color);
		break;
	}
}
void Print_Color_Line(char color) // Prints the color line of the card
{
	int i;
	for (i = 0; i < CARD_WIDTH; i++)
	{
		switch (i)
		{
		case 0:
			Print_Char_X_Times('*', 1);
			break;
		case CARD_WIDTH - 1:
			Print_Char_X_Times('*', 1);
			break;
		case CARD_MID_CHAR:
			Print_Char_X_Times(color, 1);
			break;
		default:
			Print_Char_X_Times(' ', 1);
			break;
		}
	}
	printf("\n");
}
void Print_Char_X_Times(char c, int length) // Prints a specified character in a specified amount
{
	int i;
	for (i = 0; i < length; i++)
	{
		printf("%c", c);
	}
}
// 5.2 Print the current player turn (p1/p2...)
void Print_Upper_Card(Card card) // Prints the upper card - the top card
{
	printf("Upper card:\n");
	Print_Card(card);
}
void Print_Player_Turn(Player player) // Prints the player's turn, with it's deck
{
	int i;
	printf("%s's turn:\n\n", player.first_name);

	for (i = 0; i < player.cards; i++)
	{
		printf("Card #%d:\n", (i + 1));
		Print_Card(player.deck[i]);
		printf("\n");
	}
}
// 5.3 Ask the player to choose the card that he wants to put down.
void Print_Put_Card(int cards, char* turn_type) // Ask the player to choose which card does it want to put on the table
{
	printf("Please enter %d if you want to %s or %d-%d if you want to put one of your cards in the middle:\n", ZERO, turn_type, ONE, cards);
}
void Add_Card_To_Deck(Player* player) // If the player didn't put a card on the table - this method gives it another card to it's deck and "realloc" without realloc method it's deck
{
	int i, j;

	int old_count = player->cards;
	player->cards++;
	Card* tmp_deck = (Card*)malloc(player->cards * sizeof(Card));

	for (i = 0; i < old_count; i++)
	{
		tmp_deck[i] = player->deck[i];
	}

	for (j = 0; j < old_count; j++)
	{
		player->deck[i] = tmp_deck[i];
	}

	player->deck[old_count] = Draw_Card();
	free(tmp_deck);
}
void Remove_Card_From_Deck(Player* player, int index) // Removes the card that the player chose to put on the table. If it's cards amount is 4 or less, the method won't affect the deck's size
{
	int i, j, k;
	Card* tmp_deck = (Card*)malloc(player->cards * sizeof(Card));

	int cards = player->cards;

	for (i = 0; i < index; i++) // Copy the deck to a temporary variable - until the index of the chosen card
	{
		tmp_deck[i].color = player->deck[i].color;
		tmp_deck[i].type = player->deck[i].type;
	}

	for (j = index + 1; j < cards; j++) // Copy the deck to a temporary variable - after the index of the chosen card to the end
	{
		tmp_deck[j - 1].color = player->deck[j].color;
		tmp_deck[j - 1].type = player->deck[j].type;
	}

	player->cards--; // Remove a card from the counting, and see if the cards amount is less than 4 (minimal deck count for now)
	if (player->cards < MINIMAL_DECK) // If so - the deck will get size of 4
	{
		tmp_deck[cards - 1].color = STRING_END;
		tmp_deck[cards - 1].type = ZERO;

		player->deck = (Card*)malloc(MINIMAL_DECK * sizeof(Card));
	}
	else // Otherwise, the deck will get one size below
	{
		player->deck = (Card*)malloc((player->cards) * sizeof(Card));
	}

	for (k = 0; k < player->cards; k++) // Copy back the cards to the player's deck. Assign the last variable (cards+1) as an empty card
	{
		player->deck[k].color = tmp_deck[k].color;
		player->deck[k + 1].color = NOT_FOUND;
		player->deck[k].type = tmp_deck[k].type;
		player->deck[k + 1].type = SPACE;
	}
	// Free the variable
	free(tmp_deck);
}
bool Card_In_Range(int cards, int index) // Checks if the card that the user chose is in the range of cards from it's deck
{
	bool result = true;

	if (index > cards || index <= NOT_FOUND)
	{
		result = false;
	}

	return result;
}
void Player_Turn(Player* player, Game* game, char* turn_type) // The Player's turn
{
	int input;

	Print_Upper_Card(game->last_card); // Prints: upper card, player's deck, ask the player to put a card
	printf("\n\n\n");
	Print_Player_Turn(*player);
	Print_Put_Card(player->cards, turn_type);
	input = Get_Integer();
	Card selected;

	switch (input) // Check the input
	{
	case ZERO: // If the input is 0, draw a card.
		Add_Card_To_Deck(player);
		break;
	default: // Otherwise, check if the input is in the range of the player's cards
		if (Card_In_Range(player->cards, input)) // If so - check if it's valid input
		{
			selected.color = player->deck[input - 1].color;
			selected.type = player->deck[input - 1].type;
			
			if (selected.color == game->last_card.color || selected.type == game->last_card.type || selected.type == COLOR_CHANGE) // If the card is valid (by color, type or it's a color change card), add it to the statistics and assign it as the last card
			{
				game->last_card.color = player->deck[input - 1].color;
				game->last_card.type = player->deck[input - 1].type;
				Add_To_Stats(selected.type, game->statistics);
				Remove_Card_From_Deck(player, input - 1);
				switch (game->last_card.type) // Check the card's type.
				{
				case COLOR_CHANGE: // If it's a color change - call a color change method
					Special_Change_Color(game);
					break;
				case DIRECTION_CHANGE: // If it's a direction change - call a direction change method
					Special_Change_Direction(game);
					break;
				case PLUS: // If it's a plus card, ask the user to add another card
					Player_Turn(player, game, turn_type);
					break;
				case TAKI: // If it's a taki card, "open" taki and call the taki method
					Special_Open_Taki(player, game, TURN_TYPE_TAKI);
					break;
				case STOP: // If it's a stop card, stop the next player's turn
					Special_Stop_Next(player, game);
				default:
					break;
				}
			}
			else // Otherwise, the card is invalid by color/type, and the user needs to put a card again
			{
				Print_Invalid_Card();
				Player_Turn(player, game, turn_type);
			}
		}
		else // Otherwise, the card isn't in the range of cards of the user, and the user needs to put a card again
		{
			Print_Invalid_Card();
			Player_Turn(player, game, turn_type);
		}
		break;
	}
}
void Print_Invalid_Card() // Prints an invalid card message
{
	printf("Invalid card! Try again.\n");
}
// 5.4 - Do the move, check which card was put - after seeing it's valid!
bool In_Range(int input, int range) // Check if the card is in the range of the cards in the player's deck
{
	bool result = true;

	if (input > range || input < 1)
	{
		result = false;
	}

	return result;
}
// Color change
void Print_Choose_Color() // Asks the user to choose a color
{
	printf("Please enter your color choice:\n");
	printf("%d - Yellow\n", YELLOW_I);
	printf("%d - Red\n", RED_I);
	printf("%d - Blue\n", BLUE_I);
	printf("%d - Green\n", GREEN_I);
}
void Special_Change_Color(Game* game) // Gets the user's color after change, and changes it for now.
{
	int input;
	Print_Choose_Color();
	input = Get_Integer();

	while (!In_Range(input, COLORS))
	{
		Print_Invalid_Card();
		Print_Choose_Color;
		input = Get_Integer();
	}

	game->last_card.color = Color_Identifier(input);
	game->last_card.type = COLOR_CHANGE;
}
// Direction change
void Special_Change_Direction(Game* game) // Change the game direction - multiply by (-1) every time the card is being chose by a player.
{
	game->direction *= PLAY_REVERSE;
}
// Taki
int Special_Open_Taki(Player* player, Game* game, char* turn_type) // A taki fall turn - won't stop until the player enters 0
{
	int input;
	Card selected;
	int last;
	
	// Prints like a regular turn, but with a Taki message being assigned to the text instead
	Print_Upper_Card(game->last_card);
	printf("\n\n\n");
	Print_Player_Turn(*player);
	Print_Put_Card(player->cards, turn_type);
	input = Get_Integer();

	if (input == ZERO) // If the user entered 0, return 0 so the method will check the last card's type
	{
		return ZERO;
	}

	else if (!Card_In_Range(player->cards, input)) // If the card chosen is not in the range of the deck of the user - print error message and ask the user to enter new card 
	{
		Print_Invalid_Card();
		Special_Open_Taki(player, game, TURN_TYPE_TAKI);
	}

	else // The card that was chosen is valid
	{
		selected.color = player->deck[input - 1].color;
		selected.type = player->deck[input - 1].type;

		if (selected.color != player->deck[input - 1].color) // Check if the color is same, if not - ask the player to choose a card again
		{
			Print_Invalid_Card();
			Special_Open_Taki(player, game, TURN_TYPE_TAKI);
		}

		else // The color is the same. Assign the last card as this card, remove it from the deck and add it to the statistics of the game. Call this function again until getting 0
		{
			game->last_card.color = player->deck[input - 1].color;
			game->last_card.type = player->deck[input - 1].type;
			Remove_Card_From_Deck(player, input - 1);
			Add_To_Stats(game->last_card.type, game->statistics);
			last = Special_Open_Taki(player, game, TURN_TYPE_TAKI);

			if (last == ZERO) // if the last is 0, check the card's type
			{
				switch (game->last_card.type)
				{
				case PLUS: // If it's a plus card, add a card from the deck
					Player_Turn(player, game, TURN_TYPE_REGULAR);
					return PLUS;
					break;
				case STOP: // If it's a stop card, stop the next player's turn
					Special_Stop_Next(player, game);
					return STOP;
					break;
				case COLOR_CHANGE: // If it's a color change card, ask the user to choose a new color
					Special_Change_Color(game);
					return COLOR_CHANGE;
					break;
				case DIRECTION_CHANGE: // If it's a direction change card, change the game play direction
					Special_Change_Direction(game);
					return DIRECTION_CHANGE;
					break;
				default: // A regular card, don't do anything special
					return game->last_card.type;
					break;
				}
			}
		}
	}
}
// Stop
void Special_Stop_Next(Player* player, Game* game) // Stops the next player's turn
{
	if (game->direction == PLAY_REVERSE) // Check if the direction is reversed. If so - the next player (-1) is being stopped
	{
		game->last_player--;
		
		if (game->last_player < ZERO) // If the index is below 0, assign it by a mod and multiply in (-1)
		{
			game->last_player %= 3;
			game->last_player *= PLAY_REVERSE;
		}
	}
	else // The direction is 1, so add 1 to the next player
	{
		game->last_player++;

		if (game->last_player >= game->players) // If the last player is bigger than the players amount, assign it by mod
		{
			game->last_player %= 3;
		}
	}
}
// 6 - Print the winner's name
void Print_Winner(char* winner_name) // Print the winner's name
{
	printf("The winner is... %s! Congratulations!\n", winner_name);
}
// 7 - Print statistics table
void Add_To_Stats(int type, Statistics statistics[CARDS]) // Add a card type to the statistics array
{
	statistics[type - 1].count++;
}
void Print_Stats(Game* game) // Prints the statistics array, and sorting it before
{
	int i;

	// Sort:
	Merge_Sort(game->statistics, CARDS);

	// Print:
	Print_Char_X_Times('*', 12);
	printf(" Game Statistics ");
	Print_Char_X_Times('*', 12);
	printf("\n");

	printf("Card # | Frequency\n");
	Print_Char_X_Times('_', 18);
	printf("\n");
	for (i = 0; i < CARDS; i++)
	{
		switch (game->statistics[i].card_type) // Checks the card type and printing the right line
		{
		case COLOR_CHANGE:
			printf("% *s", strlen(COLOR_CHANGE_T) + 1, COLOR_CHANGE_T);
			Print_Char_X_Times(SPACE, LINE_TYPE - strlen(COLOR_CHANGE_T) - 1);
			Print_Char_X_Times('|', 1);
			break;
		case TAKI:
			printf("% *s", strlen(TAKI_T) + 1, TAKI_T);
			Print_Char_X_Times(SPACE, LINE_TYPE - strlen(TAKI_T) - 1);
			Print_Char_X_Times('|', 1);
			break;
		case STOP:
			printf("% *s", strlen(STOP_T) + 1, STOP_T);
			Print_Char_X_Times(SPACE, LINE_TYPE - strlen(TAKI_T) - 1);
			Print_Char_X_Times('|', 1);
			break;
		case DIRECTION_CHANGE:
			printf("% *s", strlen(DIRECTION_CHANGE_T) + 2, DIRECTION_CHANGE_T);
			Print_Char_X_Times(SPACE, LINE_TYPE - strlen(DIRECTION_CHANGE_T) - 2);
			Print_Char_X_Times('|', 1);
			break;
		case PLUS:
			printf("% *s", strlen(PLUS_T) + 3, PLUS_T);
			Print_Char_X_Times(SPACE, LINE_TYPE - strlen(PLUS_T) - 3);
			Print_Char_X_Times('|', 1);
			break;
		default:
			printf("% *d", LINE_TYPE - 3, game->statistics[i].card_type);
			Print_Char_X_Times(SPACE, 3);
			Print_Char_X_Times('|', 1);
			break;
		}

		printf("% *d\n", 5, game->statistics[i].count); // Prints the count of this card type
	}
}
void Merge_Sort(Statistics arr[], int size) // A merge sort like we had in class - but in a descending order
{
	Statistics* tmp_arr = NULL;

	if (size <= 1)
	{
		return;
	}

	Merge_Sort(arr, size / 2);
	Merge_Sort(arr + size / 2, size - size / 2);

	tmp_arr = (Statistics*)malloc(size * sizeof(Statistics));

	Merge(arr, size / 2, arr + size / 2, size - size / 2, tmp_arr);
	Copy_Array(arr, tmp_arr, size);
	free(tmp_arr);
}
void Merge(Statistics a1[], int n1, Statistics a2[], int n2, Statistics res[]) // Merge the "arrays" that we've got from the Merge_Sort method - like we had in class
{
	int ind1, ind2, res_ind;
	ind1 = ind2 = res_ind = 0;

	while ((ind1 < n1) && (ind2 < n2))
	{
		if (a1[ind1].count >= a2[ind2].count)
		{
			res[res_ind].card_type = a1[ind1].card_type;
			res[res_ind].count = a1[ind1].count;
			ind1++;
		}
		else
		{
			res[res_ind].card_type = a2[ind2].card_type;
			res[res_ind].count = a2[ind2].count;
			ind2++;
		}
		res_ind++;
	}

	while (ind1 < n1)
	{
		res[res_ind].card_type = a1[ind1].card_type;
		res[res_ind].count = a1[ind1].count;
		ind1++;
		res_ind++;
	}
	while (ind2 < n2)
	{
		res[res_ind].card_type = a2[ind2].card_type;
		res[res_ind].count = a2[ind2].count;
		ind2++;
		res_ind++;
	}
}
void Copy_Array(Statistics dest[], Statistics src[], int size) // Copies an array values from source array to destination array
{
	int i;

	for (i = 0; i < size; i++)
	{
		dest[i].card_type = src[i].card_type;
		dest[i].count = src[i].count;
	}
}