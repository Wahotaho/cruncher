// Libraries included
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h> // may not be needed in final version

// Global Constants
#define LEN 8
#define WID 8
#define POSSIBLE_MOVES 256
#define FEN_STRING_LEN 128
#define SQUARE_LEN 2
#define TRUE 1
#define FALSE 0
#define CHECKMATE_VAL 90000.0
#define MAX_GAME_LEN 2048

// Global Variables
char * initial_position_string;
char initial_side_move;
char * initial_castling_options;
char * initial_en_passant_square;
double initial_half_move_clock;
int initial_full_move_number;
int levels_deep = 0;

struct node * root;
size_t node_pointer_size = sizeof (root);
struct node ** heads;
struct node ** tails;
int initial_calculating_depth;

// Header files

#include "headers.h"
#include "misc.h"
#include "board.h"
#include "position.h"
#include "pieces.h"
#include "move.h"
#include "evaluate.h"
#include "game_nodes.h"
#include "calculate.h"

struct move chosen_move;

int main (int argc, char * argv[]) {

  // Test if arguments exists
  if (argc != 7) {
      printf("%s\n", "Usage: ./cruncher <fen_String>");
      exit(1);
  }
  //

  // Initialize position components
  initial_position_string = argv[1];
  initial_side_move = argv[2][0];
  initial_castling_options = argv[3];
  initial_en_passant_square = argv[4];
  sscanf(argv[5], "%lf", &initial_half_move_clock);
  sscanf(argv[6], "%d", &initial_full_move_number);
  //

  // Initialize head a tail lists

  heads = malloc(sizeof(&root) * MAX_GAME_LEN);
  tails = malloc(sizeof(&root) * MAX_GAME_LEN);

  for (int i = 0; i < MAX_GAME_LEN; i++) {
    heads[i] = NULL;
    tails[i] = NULL;
  }

  // Create initial position struct
  struct position initial_position = position_init(initial_position_string, initial_side_move, initial_castling_options, initial_en_passant_square, initial_half_move_clock, initial_full_move_number);
  //

  go_infinite(initial_position);

  /*srand(time(NULL));
  struct move m;

  while (initial_position.game_ended == FALSE) {
    printf("\n\nPosition Value: %f\n", initial_position.position_value);
    printf("Full move clock: %d\n", initial_position.full_move_number);
    print_position(&initial_position);
    printf("%s\n", "");

    m = initial_position.moves[rand () % initial_position.num_moves];
    make_move(&initial_position, &m);
    printf("\nLast Move: %s", "");
    print_move(&m, &initial_position);
    printf("%s\n", "");
  }*/

  return 0;
}
