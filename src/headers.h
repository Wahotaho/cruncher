// Data structure for position
struct position {
  char position_string[FEN_STRING_LEN];
  char side_move[SIDE_TO_MOVE_LEN];
  _Bool white_can_castle_kingside;
  _Bool white_can_castle_queenside;
  _Bool black_can_castle_kingside;
  _Bool black_can_castle_queenside;
  char en_passant_square[SQUARE_LEN];
  double half_move_clock;
  int full_move_number;
  char board[WID][LEN];
};
//

// Function definitions for "board.h"
void load_board (struct position * pos);
void parse_rank (char * rank, char * pieces);
//

// Function definitions "position.h"
struct position position_init (char * str, char * side, char * castling_options, char * en_passant, double half_moves, int full_moves);
void print_position (struct position pos) ;
//

// Function definitions for "pieces.h"
int is_piece (char p);
//

// Function definitions for "misc.h"
void split (char * array[], char * buf, char * splitter);
_Bool string_contains (char * str, char s);
//
