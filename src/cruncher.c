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

// Global Variables
char * initial_position_string;
char initial_side_move;
char * initial_castling_options;
char * initial_en_passant_square;
double initial_half_move_clock;
int initial_full_move_number;

// Header files

#include "headers.h"
#include "misc.h"
#include "board.h"
#include "position.h"
#include "pieces.h"
#include "move.h"
#include "evaluate.h"
#include "game_nodes.h"

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

  // Create initial position struct
  struct position initial_position = position_init(initial_position_string, initial_side_move, initial_castling_options, initial_en_passant_square, initial_half_move_clock, initial_full_move_number);
  //

  printf("\nKing checked? : %d\n", white_king_checked(&initial_position, 4, 4));


  srand(time(NULL));
  int chosen_move = 256;
  struct move m;

  while (!initial_position.game_ended) {
    chosen_move = rand () % initial_position.num_moves;
    printf("\nNumber of move legal moves before: %d\n", initial_position.num_moves);
    m = initial_position.moves[chosen_move];
    printf("%s\n", "Chosen Move: ");
    print_move (&m,&initial_position);
    printf("Fifty Move Clock: %f\n", initial_position.half_move_clock);
    printf("Full Move Clock: %d\n", initial_position.full_move_number);
    print_position (&initial_position);
    printf("%s\n", "");
    make_move(&initial_position, &m);
    printf("%s\n", "");
  }

  return 0;
}
