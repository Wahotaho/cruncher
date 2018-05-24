// Libraries included
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global Constants
#define len 8
#define wid 8

// Global Variables
char * position_string;
char * side_move;
char * castling_options;
char * en_passant_square;
double half_move_clock;
int full_move_number;
char board[wid][len];

// Header files

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
  position_string = argv[1];
  side_move = argv[2];
  castling_options = argv[3];
  en_passant_square = argv[4];
  sscanf(argv[5], "%lf", &half_move_clock);
  sscanf(argv[6], "%d", &full_move_number);
  //

  // Load position into board
  load_board();
  //

  print_position();

  return 0;
}
