// Data structure for position
struct position {
  // Basic position elements
  char position_string[FEN_STRING_LEN];
  char side_move;
  _Bool white_can_castle_kingside;
  _Bool white_can_castle_queenside;
  _Bool black_can_castle_kingside;
  _Bool black_can_castle_queenside;
  double half_move_clock;
  int full_move_number;
  char board[WID][LEN];

    // King locations
    int white_king_rank;
    int white_king_file;
    int black_king_rank;
    int black_king_file;

  // Advanced position elements
  _Bool game_ended;
  double position_value;
  double material_value;
  double mobility_value;
  int en_passant_rank;
  int en_passant_file;

  struct move * moves;
  int num_moves;
  int previous_num_moves;
};
//

// Data structure for move
struct move {
  int start_rank;
  int start_file;
  int end_rank;
  int end_file;
  char promotion_piece;
  _Bool is_en_passant;
  _Bool is_castling;
};
//

// Function definitions for "calculate.h"
void go_infinite (struct position p);
void create_child_nodes (struct node  * n);
double max (struct node * n);
double min (struct node * n);
void calculate_next_ply (int i);
void calculate_depth_minimax_val (struct node * n);
//

// Function definitions for "board.h"
void load_board (struct position * pos);
void parse_rank (char * rank, char * pieces);
//

// Function definitions "position.h"
struct position position_init (char * str, char side, char * castling_options, char * en_passant, double half_moves, int full_moves);
int white_king_checked (struct position * pos, char rank, char file);
int black_king_checked (struct position * pos, char rank, char file);
void print_position (struct position * pos) ;
//

// Function definitions for "pieces.h"
int is_piece (char p);
int is_white (char p);
int is_black (char p);
//

// Function definitions for "move.h"
struct move move_init_from_string (char * move_str);
struct move move_init_f (int f1, int r1, int f2, int r2, char p, _Bool ep);
void make_move (struct position * pos, struct move * mov);
void place_move (struct position * pos, struct move * mov);
void print_move (struct move * m, struct position * p);
void legal_moves (struct position * p);
void white_rook_moves (struct position * p, char rank, char file);
void black_rook_moves (struct position * p, char rank, char file);
void white_bishop_moves (struct position * p, char rank, char file);
void black_bishop_moves (struct position * p, char rank, char file);
void white_knight_moves (struct position * p, char rank, char file);
void black_knight_moves (struct position * p, char rank, char file);
void white_pawn_moves (struct position * p, char rank, char file);
void black_pawn_moves (struct position * p, char rank, char file);
void white_king_moves (struct position * p, char rank, char file);
void black_king_moves (struct position * p, char rank, char file);
void white_kingside_castle (struct position * p);
void black_kingside_castle (struct position * p);
void white_queenside_castle (struct position * p);
void black_queenside_castle (struct position * p);
//

// Function definitions for "misc.h"
void split (char * array[], char * buf, char * splitter);
_Bool string_contains (char * str, char s);
int abs (int a);
double abs_double (double a);
char toUpper (char c);
char toLower (char c);
//

// Function definitions for "evaluate.h"
void evaluate_position (struct position * p);
//

// Function definitions for "game_nodes.h"
struct node * create_node (struct position p);
//
