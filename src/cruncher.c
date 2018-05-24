// Libraries included
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global Constants
#define LEN 8
#define WID 8
#define POSSIBLE_MOVES 256
#define FEN_STRING_LEN 128
#define SIDE_TO_MOVE_LEN 1
#define SQUARE_LEN 2

// Global Variables
char * current_position_string;
char * current_side_move;
char * current_castling_options;
char * current_en_passant_square;
double current_half_move_clock;
int current_full_move_number;
char current_board[WID][LEN];

// Header files

#include "misc.h"
#include "headers.h"
#include "board.h"
#include "position.h"
#include "pieces.h"

int main (int argc, char * argv[]) {

  // Test if arguments exists
  if (argc != 7) {
      printf("%s\n", "Usage: ./cruncher <fen_String>");
      exit(1);
  }
  //

  // Initialize position components
  current_position_string = argv[1];
  current_side_move = argv[2];
  current_castling_options = argv[3];
  current_en_passant_square = argv[4];
  sscanf(argv[5], "%lf", &current_half_move_clock);
  sscanf(argv[6], "%d", &current_full_move_number);
  //

  // Create current position struct
  struct position current_position = position_init(current_position_string, current_side_move, current_castling_options, current_en_passant_square, current_half_move_clock, current_full_move_number);
  //

  // Load position into board
  load_board_from_struct(&current_position);
  //

  print_position_from_struct (current_position);

  return 0;
}
